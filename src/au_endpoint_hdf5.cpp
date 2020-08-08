/**
 *ArrayUDF Copyright (c) 2017, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy).  All rights reserved.
 *
 *If you have questions about your rights to use or distribute this software, please contact Berkeley Lab's Innovation & Partnerships Office at  IPO@lbl.gov.
 *
 * NOTICE. This Software was developed under funding from the U.S. Department of Energy and the U.S. Government consequently retains certain rights. As such, the U.S. Government has been granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software to reproduce, distribute copies to the public, prepare derivative works, and perform publicly and display publicly, and to permit other to do so. 
 *
 */

/**
 *
 * Email questions to {dbin, kwu, sbyna}@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#include "au_endpoint_hdf5.h"

int EndpointHDF5::ExtractMeta()
{
    //PrintInfo();
    Open();
    hid_t datatype = H5Dget_type(did); /* datatype handle */
    H5T_class_t type_class = H5Tget_class(datatype);
    dataspace_id = H5Dget_space(did);
    endpoint_ranks = H5Sget_simple_extent_ndims(dataspace_id);
    endpoint_dim_size.resize(endpoint_ranks);
    H5Sget_simple_extent_dims(dataspace_id, &endpoint_dim_size[0], NULL);
    return 0;
}

int EndpointHDF5::Create()
{
    Map2MyType();
    std::string root_dir = "/";

    //plist_id = H5Pcreate(H5P_FILE_ACCESS);
    //H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

    if (file_exist(fn_str.c_str()) == 0)
    {
        //std::cout << "Call H5Fcreate 1 : " << fn_str << "\n";

        fid = H5Fcreate(fn_str.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
    }
    else
    {
        fid = H5Fopen(fn_str.c_str(), H5F_ACC_RDWR, plist_id);
        if (fid < 0)
        {
            // std::cout << "Call H5Fcreate 2\n";
            fid = H5Fcreate(fn_str.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
        }
        if (fid < 0)
        {
            AU_EXIT("Can not create/open file !");
        }
    }

    if (gn_str != root_dir)
    {
        //printf("Debug: %s:%d\n", __FILE__, __LINE__);
        if (H5Lexists(fid, gn_str.c_str(), H5P_DEFAULT) == 0)
        {
            gid = H5Gcreate(fid, gn_str.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        }
        else
        {
            gid = H5Gopen(fid, gn_str.c_str(), H5P_DEFAULT);
        }
    }

    std::vector<hsize_t> dims_out;
    dims_out = endpoint_dim_size;
    hid_t ts_id;
    ts_id = H5Screate_simple(endpoint_ranks, &dims_out[0], NULL);

    if (gn_str != root_dir)
    {
        if (H5Lexists(gid, dn_str.c_str(), H5P_DEFAULT) > 0)
        {
            H5Ldelete(gid, dn_str.c_str(), H5P_DEFAULT); //we delete
        }

        did = H5Dcreate(gid, dn_str.c_str(), disk_type, ts_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    }
    else
    {
        if (H5Lexists(fid, dn_str.c_str(), H5P_DEFAULT) > 0)
        {
            H5Ldelete(fid, dn_str.c_str(), H5P_DEFAULT); //we delete
        }
        did = H5Dcreate(fid, dn_str.c_str(), disk_type, ts_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    }

    dataspace_id = H5Dget_space(did);
    H5Sclose(ts_id);
    SetRwFlag(H5F_ACC_RDWR);
    SetOpenFlag(true);

    //Close(); //Close for data consistency during writing
    return 0;
}

int EndpointHDF5::Open()
{
    if (file_exist(fn_str.c_str()) == 0)
    {
        Create();
        //std::cout << "Call create in Open again !\n";
    }

    fid = H5Fopen(fn_str.c_str(), read_write_flag, plist_id);
    if (fid < 0)
    {
        std::cout << "Error happens in open file " << fn_str << std::endl;
        exit(-1);
    }

    std::string root_dir = "/";
    if (gn_str != root_dir)
    {
        //std::cout << "Open Group : " << gn << std::endl;
        gid = H5Gopen(fid, gn_str.c_str(), H5P_DEFAULT);
        did = H5Dopen(gid, dn_str.c_str(), H5P_DEFAULT);
    }
    else
    {
        did = H5Dopen(fid, dn_str.c_str(), H5P_DEFAULT);
    }
    assert(did >= 0);

    dataspace_id = H5Dget_space(did);

    SetOpenFlag(true);

    //plist_cio_id = H5P_DEFAULT;
    return 0;
}

/**
     * @brief read the data from end-point
     * 
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data 
     * @return int < 0 error, >= 0 works
     */
int EndpointHDF5::Read(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{
    if (GetOpenFlag() == false)
    {
        ExtractMeta(); //It also opens
    }

    Map2MyType();

    std::vector<unsigned long long> offset, count;
    offset.resize(endpoint_ranks);
    count.resize(endpoint_ranks);
    for (int i = 0; i < endpoint_ranks; i++)
    {
        offset[i] = start[i];
        count[i] = end[i] - start[i] + 1; //Starting from zero
    }

    hid_t memspace_id = H5Screate_simple(endpoint_ranks, &count[0], NULL);
    H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, &offset[0], NULL, &count[0], NULL);

    int ret = H5Dread(did, mem_type, memspace_id, dataspace_id, plist_cio_id, data);
    assert(ret >= 0);
    H5Sclose(memspace_id);
    return ret;
}

/**
     * @brief write the data to the end-point
     * 
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data 
     * @return int < 0 error, >= 0 works
     */
int EndpointHDF5::Write(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{
    Map2MyType();

    if (!GetOpenFlag())
    {
        SetRwFlag(H5F_ACC_RDWR);
        ExtractMeta(); //Will call open
    }
    else if (GetRwFlag() != H5F_ACC_RDWR)
    {
        SetRwFlag(H5F_ACC_RDWR);
        Close();       //Close
        ExtractMeta(); //Re-open it
    }
    //Close();
    //SetRwFlag(H5F_ACC_RDWR);
    //Open(); //Re-open it

    std::vector<unsigned long long> offset, count;
    offset.resize(endpoint_ranks);
    count.resize(endpoint_ranks);
    for (int i = 0; i < endpoint_ranks; i++)
    {
        offset[i] = start[i];
        count[i] = end[i] - start[i] + 1; //Starting from zero
    }

    hid_t memspace_id = H5Screate_simple(endpoint_ranks, &count[0], NULL);
    H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, &offset[0], NULL, &count[0], NULL);
    int ret = H5Dwrite(did, mem_type, memspace_id, dataspace_id, plist_cio_id, data);
    assert(ret >= 0);
    H5Sclose(memspace_id);
    Close(); //Close
    return ret;
}

/**
     * @brief close the end-point
     * 
     * @return int int < 0 error, >= 0 works
     */
int EndpointHDF5::Close()
{
    if (fid > 0)
        H5Fflush(fid, H5F_SCOPE_GLOBAL);
    /*if (plist_id > 0)
        H5Pclose(plist_id);
    if (plist_cio_id > 0)
        H5Pclose(plist_cio_id);*/
    if (dataspace_id > 0)
        H5Sclose(dataspace_id);
    if (did > 0)
        H5Dclose(did);
    if (gid > 0)
        H5Gclose(gid);
    if (fid > 0)
        H5Fclose(fid);
    //plist_id = H5P_DEFAULT;
    //plist_cio_id = H5P_DEFAULT;
    dataspace_id = -1;
    did = -1;
    gid = -1;
    fid = -1;

    SetOpenFlag(false);
    return 0;
};

void EndpointHDF5::EnableCollectiveIO()
{
    EnableMPIIO();
    if (plist_cio_id > 0)
        H5Pclose(plist_cio_id);
    plist_cio_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist_cio_id, H5FD_MPIO_COLLECTIVE);
}

void EndpointHDF5::DisableCollectiveIO()
{
    std::cout << "DisableCollectiveIO \n";
    if (plist_cio_id > 0)
        H5Pclose(plist_cio_id);
    plist_cio_id = H5P_DEFAULT;
}

void EndpointHDF5::EnableMPIIO()
{
    if (plist_id > 0)
        H5Pclose(plist_id);
    plist_id = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);
}

void EndpointHDF5::DisableMPIIO()
{
    if (plist_id > 0)
        H5Pclose(plist_id);
    std::cout << "DisableMPIIO \n";

    plist_id = H5P_DEFAULT;
}

int EndpointHDF5::PrintInfo()
{
    std::cout << data_endpoint_orig << std::endl;
    return 0;
}

void EndpointHDF5::Map2MyType()
{
    switch (data_element_type)
    {
    case AU_SHORT:
        mem_type = H5T_NATIVE_SHORT;
        disk_type = H5T_STD_I16LE;
        return;
    case AU_INT:
        mem_type = H5T_NATIVE_INT;
        disk_type = H5T_STD_I32LE;
        return;
    case AU_LONG:
        mem_type = H5T_NATIVE_LONG;
        disk_type = H5T_STD_I64LE;
        return;
    case AU_LONG_LONG:
        mem_type = H5T_NATIVE_LLONG;
        disk_type = H5T_STD_I64LE;
        return;
    case AU_USHORT:
        mem_type = H5T_NATIVE_UINT;
        disk_type = H5T_STD_U32LE;
        return;
    case AU_UINT:
        mem_type = H5T_NATIVE_USHORT;
        disk_type = H5T_STD_U16LE;
        return;
    case AU_ULONG:
        mem_type = H5T_NATIVE_ULONG;
        disk_type = H5T_STD_U64LE;
        return;
    case AU_ULLONG:
        mem_type = H5T_NATIVE_ULLONG;
        disk_type = H5T_STD_U64LE;
        return;
    case AU_FLOAT:
        mem_type = H5T_NATIVE_FLOAT;
        disk_type = H5T_IEEE_F32LE;
        return;
    case AU_DOUBLE:
        mem_type = H5T_NATIVE_DOUBLE;
        disk_type = H5T_IEEE_F64LE;
        return;
    default:
        std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
        std::flush(std::cout);
        std::exit(EXIT_FAILURE);
    }
};

/**
     * @brief parse endpoint_info to my own info
     *        In HDF5, it map endpoint_info to filename, group name and datasetname
     * @return int: 0 works,  < 0 error,
     */
int EndpointHDF5::ParseEndpointInfo()
{
    //std::cout << endpoint_info << "\n";
    std::stringstream ss(endpoint_info);
    if (!std::getline(ss, fn_str, ':'))
    {
        AU_EXIT("Invalued endpoint_info");
    }

    std::string group_dataset_name_str;

    if (!std::getline(ss, group_dataset_name_str, ':'))
    {
        AU_EXIT("Invalued endpoint_info");
    }

    gn_str = ExtractPath(group_dataset_name_str);
    if (gn_str == "")
    {
        gn_str = "/";
    }
    dn_str = ExtractFileName(group_dataset_name_str);

    //std::cout << "fn_str =" << fn_str << ", gn_str = " << gn_str << ", dn_str =" << dn_str << std::endl;
    return 0;
}

/**
     * @brief call a special operator on endpoint
     *        such as, enable collective I/O for HDF5
     *                 dump file from MEMORY to HDF5
     * @param opt_code, specially defined code 
     */
int EndpointHDF5::SpecialOperator(int opt_code, std::string parameter)
{
    switch (opt_code)
    {
    case OP_ENABLE_MPI_IO:
        EnableMPIIO();
        break;
    case OP_DISABLE_MPI_IO:
        DisableMPIIO();
        break;
    case OP_ENABLE_COLLECTIVE_IO:
        EnableCollectiveIO();
        break;
    case OP_DISABLE_COLLECTIVE_IO:
        DisableCollectiveIO();
        break;
    default:
        break;
    }

    return 0;
}