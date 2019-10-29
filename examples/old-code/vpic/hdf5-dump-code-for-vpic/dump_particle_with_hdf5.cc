{
    char fname[256];
    char group_name[256];
    char particle_scratch[128];
    char subparticle_scratch[128];

    int np_local;
    species_t *sp;

    float *Pf;
    int *Pi;

    // get the total number of particles. in this example, output only electrons
    sp = species_list;
    sprintf(particle_scratch, DUMP_DIR_FORMAT, "particle_hdf5");
    dump_mkdir(particle_scratch);
    sprintf(subparticle_scratch, "%s/T.%d/", particle_scratch, step());
    dump_mkdir(subparticle_scratch);

    while (sp)
    {

        np_local = (sp->np + global->stride_particle_dump - 1) / global->stride_particle_dump;

        // make a copy of the part of particle data to be dumped
        double ec1 = uptime();

        int sp_np = sp->np;
        int sp_max_np = sp->max_np;
        particle_t *ALIGNED(128) p_buf = NULL;
        if (!p_buf)
            MALLOC_ALIGNED(p_buf, np_local, 128);
        particle_t *sp_p = sp->p;
        sp->p = p_buf;
        sp->np = np_local;
        sp->max_np = np_local;

        for (long long iptl = 0, i = 0; iptl < sp_np; iptl += global->stride_particle_dump, ++i)
        {
            COPY(&sp->p[i], &sp_p[iptl], 1);
        }
        center_p(sp, interpolator_array);
        ec1 = uptime() - ec1;
        int mpi_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

        sim_log("On mpi_rank: " << mpi_rank << ", Time in copying particle data: " << ec1 << " s"
                                << ", np_local = " << np_local);

        Pf = (float *)sp->p;
        Pi = (int *)sp->p;

        // open HDF5 file in "particle/T.<step>/" subdirectory
        // filename: eparticle.h5p
        sprintf(fname, "%s/%s_%d.h5", subparticle_scratch, sp->name, step());
        sprintf(group_name, "/Timestep_%d", step());
        double el1 = uptime();

        hid_t plist_id = H5Pcreate(H5P_FILE_ACCESS);
        H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);
        hid_t file_id = H5Fcreate(fname, H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
        hid_t group_id = H5Gcreate(file_id, group_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

        H5Pclose(plist_id);

        long long total_particles, offset;
        long long numparticles = np_local;
        MPI_Allreduce(&numparticles, &total_particles, 1, MPI_LONG_LONG, MPI_SUM, MPI_COMM_WORLD);
        MPI_Scan(&numparticles, &offset, 1, MPI_LONG_LONG, MPI_SUM, MPI_COMM_WORLD);
        offset -= numparticles;

        hid_t filespace = H5Screate_simple(1, (hsize_t *)&total_particles, NULL);

        hsize_t memspace_count_temp = numparticles * 8;
        hid_t memspace = H5Screate_simple(1, &memspace_count_temp, NULL);
        plist_id = H5Pcreate(H5P_DATASET_XFER);

        //Comment out for test only
        H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
        H5Sselect_hyperslab(filespace, H5S_SELECT_SET, (hsize_t *)&offset, NULL, (hsize_t *)&numparticles, NULL);

        hsize_t memspace_start = 0, memspace_stride = 8, memspace_count = np_local;
        H5Sselect_hyperslab(memspace, H5S_SELECT_SET, &memspace_start, &memspace_stride, &memspace_count, NULL);

        el1 = uptime() - el1;
        sim_log("Particle TimeHDF5Open): " << el1 << " s"); //Easy to handle results for scripts

        double el2 = uptime();

        hid_t dset_id = H5Dcreate(group_id, "dX", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        int ierr = H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, filespace, plist_id, Pf);
        H5Dclose(dset_id);
        //if (rank == 0) printf ("Written variable dX \n");

        dset_id = H5Dcreate(group_id, "dY", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, filespace, plist_id, Pf + 1);
        H5Dclose(dset_id);
        //if (rank == 0) printf ("Written variable dY \n");

        dset_id = H5Dcreate(group_id, "dZ", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, filespace, plist_id, Pf + 2);
        H5Dclose(dset_id);
        //if (rank == 0) printf ("Written variable dZ \n");

        dset_id = H5Dcreate(group_id, "i", H5T_NATIVE_INT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(dset_id, H5T_NATIVE_INT, memspace, filespace, plist_id, Pi + 3);
        H5Dclose(dset_id);
        //if (rank == 0) printf ("Written variable i \n");

        dset_id = H5Dcreate(group_id, "Ux", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, filespace, plist_id, Pf + 4);
        H5Dclose(dset_id);
        //if (rank == 0) printf ("Written variable  Ux \n");

        dset_id = H5Dcreate(group_id, "Uy", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, filespace, plist_id, Pf + 5);
        H5Dclose(dset_id);
        //if (rank == 0) printf ("Written variable Uy \n");

        dset_id = H5Dcreate(group_id, "Uz", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, filespace, plist_id, Pf + 6);
        H5Dclose(dset_id);
        //if (rank == 0) printf ("Written variable Uz \n");

        dset_id = H5Dcreate(group_id, "q", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, filespace, plist_id, Pf + 7);
        H5Dclose(dset_id);
        //if (rank == 0) printf ("Written variable q \n");

        el2 = uptime() - el2;
        sim_log("Particle TimeHDF5Write: " << el2 << " s");

        double el3 = uptime();
        H5Sclose(memspace);
        H5Sclose(filespace);
        H5Pclose(plist_id);
        H5Gclose(group_id);
        H5Fclose(file_id);
        el3 = uptime() - el3;
        sim_log("Particle TimeHDF5Close: " << el3 << " s");

        sp->p = sp_p;
        sp->np = sp_np;
        sp->max_np = sp_max_np;
        FREE_ALIGNED(p_buf);

        // Write metadata if step() == 0
        char meta_fname[256];

        sprintf(meta_fname, "%s/grid_metadata_%s_%d.h5", subparticle_scratch,
                sp->name, step());

        double meta_el1 = uptime();

        hid_t meta_plist_id = H5Pcreate(H5P_FILE_ACCESS);
        H5Pset_fapl_mpio(meta_plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);
        hid_t meta_file_id = H5Fcreate(meta_fname, H5F_ACC_TRUNC, H5P_DEFAULT, meta_plist_id);
        hid_t meta_group_id = H5Gcreate(meta_file_id, group_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Pclose(meta_plist_id);

        long long meta_total_particles, meta_offset;
        long long meta_numparticles = 1;
        MPI_Allreduce(&meta_numparticles, &meta_total_particles, 1, MPI_LONG_LONG, MPI_SUM, MPI_COMM_WORLD);
        MPI_Scan(&meta_numparticles, &meta_offset, 1, MPI_LONG_LONG, MPI_SUM, MPI_COMM_WORLD);
        meta_offset -= meta_numparticles;

        hid_t meta_filespace = H5Screate_simple(1, (hsize_t *)&meta_total_particles, NULL);
        hid_t meta_memspace = H5Screate_simple(1, (hsize_t *)&meta_numparticles, NULL);
        meta_plist_id = H5Pcreate(H5P_DATASET_XFER);
        H5Pset_dxpl_mpio(meta_plist_id, H5FD_MPIO_COLLECTIVE);
        H5Sselect_hyperslab(meta_filespace, H5S_SELECT_SET, (hsize_t *)&meta_offset, NULL, (hsize_t *)&meta_numparticles, NULL);
        meta_el1 = uptime() - meta_el1;
        sim_log("Metafile TimeHDF5Open): " << meta_el1 << " s"); //Easy to handle results for scripts

        double meta_el2 = uptime();

        hid_t meta_dset_id = H5Dcreate(meta_group_id, "np_local", H5T_NATIVE_INT, meta_filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(meta_dset_id, H5T_NATIVE_INT, meta_memspace, meta_filespace, meta_plist_id, (int32_t *)&np_local);
        H5Dclose(meta_dset_id);
        //if (rank == 0) printf ("Written variable dX \n");

        meta_dset_id = H5Dcreate(meta_group_id, "nx", H5T_NATIVE_INT, meta_filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(meta_dset_id, H5T_NATIVE_INT, meta_memspace, meta_filespace, meta_plist_id, &grid->nx);
        H5Dclose(meta_dset_id);
        //if (rank == 0) printf ("Written variable dY \n");

        meta_dset_id = H5Dcreate(meta_group_id, "ny", H5T_NATIVE_INT, meta_filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(meta_dset_id, H5T_NATIVE_INT, meta_memspace, meta_filespace, meta_plist_id, &grid->ny);
        H5Dclose(meta_dset_id);
        //if (rank == 0) printf ("Written variable dZ \n");

        meta_dset_id = H5Dcreate(meta_group_id, "nz", H5T_NATIVE_INT, meta_filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(meta_dset_id, H5T_NATIVE_INT, meta_memspace, meta_filespace, meta_plist_id, &grid->nz);
        H5Dclose(meta_dset_id);
        //if (rank == 0) printf ("Written variable i \n");

        meta_dset_id = H5Dcreate(meta_group_id, "x0", H5T_NATIVE_FLOAT, meta_filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(meta_dset_id, H5T_NATIVE_FLOAT, meta_memspace, meta_filespace, meta_plist_id, &grid->x0);
        H5Dclose(meta_dset_id);

        meta_dset_id = H5Dcreate(meta_group_id, "y0", H5T_NATIVE_FLOAT, meta_filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(meta_dset_id, H5T_NATIVE_FLOAT, meta_memspace, meta_filespace, meta_plist_id, &grid->y0);
        H5Dclose(meta_dset_id);

        meta_dset_id = H5Dcreate(meta_group_id, "z0", H5T_NATIVE_FLOAT, meta_filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(meta_dset_id, H5T_NATIVE_FLOAT, meta_memspace, meta_filespace, meta_plist_id, &grid->z0);
        H5Dclose(meta_dset_id);

        meta_dset_id = H5Dcreate(meta_group_id, "dx", H5T_NATIVE_FLOAT, meta_filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(meta_dset_id, H5T_NATIVE_FLOAT, meta_memspace, meta_filespace, meta_plist_id, &grid->dx);
        H5Dclose(meta_dset_id);

        meta_dset_id = H5Dcreate(meta_group_id, "dy", H5T_NATIVE_FLOAT, meta_filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(meta_dset_id, H5T_NATIVE_FLOAT, meta_memspace, meta_filespace, meta_plist_id, &grid->dy);
        H5Dclose(meta_dset_id);

        meta_dset_id = H5Dcreate(meta_group_id, "dz", H5T_NATIVE_FLOAT, meta_filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(meta_dset_id, H5T_NATIVE_FLOAT, meta_memspace, meta_filespace, meta_plist_id, &grid->dz);
        H5Dclose(meta_dset_id);

        meta_el2 = uptime() - meta_el2;
        sim_log("Metafile TimeHDF5Write: " << meta_el2 << " s");
        double meta_el3 = uptime();
        H5Sclose(meta_memspace);
        H5Sclose(meta_filespace);
        H5Pclose(meta_plist_id);
        H5Gclose(meta_group_id);
        H5Fclose(meta_file_id);
        meta_el3 = uptime() - meta_el3;
        sim_log("Metafile TimeHDF5Close: " << meta_el3 << " s");

        sp = sp->next;
    }
}
