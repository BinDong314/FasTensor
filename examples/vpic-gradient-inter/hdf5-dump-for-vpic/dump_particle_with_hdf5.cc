{
	char fname[256];
    char particle_scratch[128];
    char subparticle_scratch[128];

	int np_local;
	species_t *sp; 

	float *Pf ;
	int   *Pi ;

	// get the total number of particles. in this example, output only electrons
	sp = species_list;
    sprintf(particle_scratch, DUMP_DIR_FORMAT, "particle");
    /* dump_mkdir(particle_scratch); */
    sprintf(subparticle_scratch, "%s/T.%d/", particle_scratch, step());
    dump_mkdir(subparticle_scratch);

    while (sp) {
        Pf = (float *) sp->p;
        Pi = (int *) sp->p;

	    np_local = (sp->np + global->stride_particle_dump - 1) / global->stride_particle_dump;
            
        // open HDF5 file in "particle/T.<step>/" subdirectory
        // filename: eparticle.h5p
        sprintf (fname, "%s/%s_%d.h5part", subparticle_scratch, sp->name, step());
        double el1 = uptime();

        hid_t plist_id = H5Pcreate(H5P_FILE_ACCESS);
        H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);
        hid_t file_id = H5Fcreate(fname , H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
        H5Pclose(plist_id);

        long long total_particles, offset;
        long long numparticles = np_local;
        MPI_Allreduce(&numparticles, &total_particles, 1, MPI_LONG_LONG, MPI_SUM, MPI_COMM_WORLD);
        MPI_Scan(&numparticles, &offset, 1, MPI_LONG_LONG, MPI_SUM, MPI_COMM_WORLD);	
	    offset -= numparticles;

        hid_t filespace = H5Screate_simple(1, (hsize_t *) &total_particles, NULL);
        hid_t memspace =  H5Screate_simple(1, (hsize_t *) &numparticles, NULL);
        plist_id = H5Pcreate(H5P_DATASET_XFER);

        //Comment out for test only
	    H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
        H5Sselect_hyperslab(filespace, H5S_SELECT_SET, (hsize_t *) &offset, NULL, (hsize_t *) &numparticles, NULL);
        el1 = uptime() - el1;
	    sim_log("TimeHDF5Open): "<< el1 << " s");  //Easy to handle results for scripts

	    double el2 = uptime();

        hid_t dset_id = H5Dcreate(file_id, "dX", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        int ierr = H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, filespace, plist_id, Pf);
        H5Dclose(dset_id);
	    //if (rank == 0) printf ("Written variable dX \n");

	    dset_id = H5Dcreate(file_id, "dY", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, filespace, plist_id, Pf+1);
        H5Dclose(dset_id);
	    //if (rank == 0) printf ("Written variable dY \n");

	    dset_id = H5Dcreate(file_id, "dZ", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, filespace, plist_id, Pf+2);
        H5Dclose(dset_id);
	    //if (rank == 0) printf ("Written variable dZ \n");

	    dset_id = H5Dcreate(file_id, "i", H5T_NATIVE_INT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(dset_id, H5T_NATIVE_INT, memspace, filespace, plist_id, Pi+3);
        H5Dclose(dset_id);
	    //if (rank == 0) printf ("Written variable i \n");

	    dset_id = H5Dcreate(file_id, "Ux", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, filespace, plist_id, Pf+4);
        H5Dclose(dset_id);
        //if (rank == 0) printf ("Written variable  Ux \n");

        dset_id = H5Dcreate(file_id, "Uy", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, filespace, plist_id, Pf+5);
        H5Dclose(dset_id);
        //if (rank == 0) printf ("Written variable Uy \n");

        dset_id = H5Dcreate(file_id, "Uz", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, filespace, plist_id, Pf+6);
        H5Dclose(dset_id);
        //if (rank == 0) printf ("Written variable Uz \n");

        dset_id = H5Dcreate(file_id, "q", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        ierr = H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, filespace, plist_id, Pf+7);
        H5Dclose(dset_id);
        //if (rank == 0) printf ("Written variable q \n");

        el2 = uptime() - el2;
        sim_log("TimeHDF5Write: "<< el2 << " s"); 

        double el3 = uptime();
        H5Sclose(memspace);
        H5Sclose(filespace);
        H5Pclose(plist_id);
        H5Fclose(file_id);
        el3 = uptime() - el3;
        sim_log("TimeHDF5Close: "<< el3 << " s"); 

        sp = sp->next;
    }
}
