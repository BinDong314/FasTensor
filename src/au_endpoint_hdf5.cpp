

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
    //cout << "Print Out : ExtractMeta\n";
    //PrintInfo();
    Open();
    //cout << "Print Out : ExtractMeta after open\n";
    hid_t datatype = H5Dget_type(did); /* datatype handle */
    H5T_class_t type_class = H5Tget_class(datatype);
    dataspace_id = H5Dget_space(did);
    endpoint_ranks = H5Sget_simple_extent_ndims(dataspace_id);
    endpoint_size.resize(endpoint_ranks);
    H5Sget_simple_extent_dims(dataspace_id, &endpoint_size[0], NULL);
    return 0;
}

int EndpointHDF5::Create()
{
    //std::cout << "EndpointHDF5::Create :: " << fn_str << "\n";

    Map2MyType();
    std::string root_dir = "/";
    //PrintInfo();
    //plist_id = H5Pcreate(H5P_FILE_ACCESS);
    //H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

    if (file_exist(fn_str.c_str()) == 0)
    {
        //std::cout << "Call H5Fcreate 1 : " << fn_str << "\n"<< std::flush;

        fid = H5Fcreate(fn_str.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
        //std::cout << "After Call H5Fcreate : af " << fn_str << "\n"<< std::flush;
    }
    else
    {
        fid = H5Fopen(fn_str.c_str(), H5F_ACC_RDWR, plist_id);
        if (fid < 0)
        {
            //std::cout << "Call H5Fcreate 2\n";
            fid = H5Fcreate(fn_str.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
        }
        if (fid < 0)
        {
            AU_EXIT("Can not create/open file !");
        }
    }

    if (gn_str != root_dir)
    {
        // printf("Debug: %s:%d\n", __FILE__, __LINE__);
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
    dims_out = endpoint_size;
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
        //printf("Debug: %s:%d\n", __FILE__, __LINE__);
        did = H5Dcreate(fid, dn_str.c_str(), disk_type, ts_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        assert(did >= 0);
    }
    //printf("Debug: %s:%d\n", __FILE__, __LINE__);

    dataspace_id = H5Dget_space(did);
    H5Sclose(ts_id);
    SetRwFlag(H5F_ACC_RDWR);
    SetOpenFlag(true);

    Close(); //Close for data consistency during writing
    return 0;
}

int EndpointHDF5::Open()
{
    //std::cout << "EndpointHDF5::Open :: fn_str = " << fn_str << ", gn_str = " << gn_str << ", dn_str = " << dn_str << "\n";
    //PrintInfo();
    if (file_exist(fn_str.c_str()) == 0)
    {
        //Since the create close it , we may reopen it again should we return ?
        //std::cout << "Call create in Open again !\n";
        Create();
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

    //std::cout << "Write HDF5 \n";
    if (!GetOpenFlag())
    {
        //std::cout << "Write HDF5 before open \n";
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
    //PrintVector("offset =", offset);
    //PrintVector("count =", count);

    std::vector<unsigned long long> endpoint_dim_size_temp(endpoint_ranks);
    H5Sget_simple_extent_dims(dataspace_id, &endpoint_dim_size_temp[0], NULL);

    //PrintVector("endpoint_dim_size = ", endpoint_dim_size_temp);

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
    //cout << "H5P_DEFAULT =" << H5P_DEFAULT << ", DisableCollectiveIO \n";
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
    //cout << "DisableMPIIO \n";
    if (plist_id > 0)
        H5Pclose(plist_id);
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

void EndpointHDF5::Map2MyTypeParameters(FTDataType ft_type, hid_t &mem_type_p, hid_t &disk_type_p)
{
    switch (ft_type)
    {
    case AU_SHORT:
        mem_type_p = H5T_NATIVE_SHORT;
        disk_type_p = H5T_STD_I16LE;
        return;
    case AU_INT:
        mem_type_p = H5T_NATIVE_INT;
        disk_type_p = H5T_STD_I32LE;
        return;
    case AU_LONG:
        mem_type_p = H5T_NATIVE_LONG;
        disk_type_p = H5T_STD_I64LE;
        return;
    case AU_LONG_LONG:
        mem_type_p = H5T_NATIVE_LLONG;
        disk_type_p = H5T_STD_I64LE;
        return;
    case AU_USHORT:
        mem_type_p = H5T_NATIVE_UINT;
        disk_type_p = H5T_STD_U16LE;
        return;
    case AU_UINT:
        mem_type_p = H5T_NATIVE_USHORT;
        disk_type_p = H5T_STD_U32LE;
        return;
    case AU_ULONG:
        mem_type_p = H5T_NATIVE_ULONG;
        disk_type_p = H5T_STD_U64LE;
        return;
    case AU_ULLONG:
        mem_type_p = H5T_NATIVE_ULLONG;
        disk_type_p = H5T_STD_U64LE;
        return;
    case AU_FLOAT:
        mem_type_p = H5T_NATIVE_FLOAT;
        disk_type_p = H5T_IEEE_F32LE;
        return;
    case AU_DOUBLE:
        mem_type_p = H5T_NATIVE_DOUBLE;
        disk_type_p = H5T_IEEE_F64LE;
        return;
    default:
        std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
        std::flush(std::cout);
        std::exit(EXIT_FAILURE);
    }
}

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
/** comment out 
int EndpointHDF5::SpecialOperator(int opt_code, std::string parameter)
{
    switch (OP_ENABLE_MPI_IO)
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
}*/

/**
     * @brief call a special operator on endpoint
     *        such as, enable collective I/O for HDF5
     *                 dump file from MEMORY to HDF5
     * @param opt_code, specially defined code 
     */
int EndpointHDF5::Control(int opt_code, std::vector<std::string> &parameter_v)
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
    case OP_CREATE_VIS_SCRIPT:
        CreateXDMF();
        break;
    case OP_LIST_TAG:
        ReadAllAttributeName(parameter_v);
        break;
    default:
        break;
    }

    return 0;
}

int EndpointHDF5::ReadAllAttributeName(std::vector<std::string> &attr_names)
{
    int ret = 0;
    //std::cout << "Write HDF5 \n";
    if (!GetOpenFlag())
    {
        //std::cout << "Write HDF5 before open \n";
        SetRwFlag(H5F_ACC_RDWR);
        ExtractMeta(); //Will call open
    }

    int na;
    hid_t aid;
    int i;

    na = H5Aget_num_attrs(did);
    ssize_t len;
    char buf[1024];
    attr_names.clear();
    for (i = 0; i < na; i++)
    {
        aid = H5Aopen_idx(did, (unsigned int)i);
        memset(buf, '\0', 1024);
        //do_attr(aid);
        len = H5Aget_name(aid, 1024, buf);
        attr_names.push_back(std::string(buf));
        H5Aclose(aid);
    }
}

/**
     * @brief Set the Attribute object
     * 
     * @param name 
     * @param data 
     * @return int 
     */
int EndpointHDF5::WriteAttribute(const std::string &name, const void *data, FTDataType data_type_p, const size_t &data_length_p)
{
    int ret = 0;
    //std::cout << "Write HDF5 \n";
    if (!GetOpenFlag())
    {
        //std::cout << "Write HDF5 before open \n";
        SetRwFlag(H5F_ACC_RDWR);
        ExtractMeta(); //Will call open
    }

    if (data_type_p != AU_STRING)
    {
        hid_t mem_type_l, disk_type_l;
        Map2MyTypeParameters(data_type_p, mem_type_l, disk_type_l);
        hsize_t attribute_dims = data_length_p;
        hid_t attribute_dataspace_id = H5Screate_simple(1, &attribute_dims, NULL);
        hid_t attribute_id = H5Acreate(did, name.c_str(), disk_type_l, attribute_dataspace_id, H5P_DEFAULT, H5P_DEFAULT);
        ret = H5Awrite(attribute_id, mem_type_l, data);
        H5Sclose(attribute_dataspace_id);
        H5Aclose(attribute_id);
    }
    else
    {
        hid_t attribute_space_scalar = H5Screate(H5S_SCALAR);
        hid_t attribute_str_type = H5Tcopy(H5T_C_S1);
        H5Tset_size(attribute_str_type, data_length_p);
        //H5Tset_strpad(attribute_str_type, H5T_STR_NULLTERM);
        hid_t attribute_id = H5Acreate(did, name.c_str(), attribute_str_type, attribute_space_scalar, H5P_DEFAULT, H5P_DEFAULT);
        ret = H5Awrite(attribute_id, attribute_str_type, data);

        H5Aclose(attribute_id);
        H5Sclose(attribute_space_scalar);
        H5Tclose(attribute_str_type);
    }
    return ret;
}

/**
     * @brief Get the Attribute object
     * 
     * @param name 
     * @param data 
     * @return int 
     */
int EndpointHDF5::ReadAttribute(const std::string &name, void *data, FTDataType data_type_p, const size_t &data_length_p)
{
    int ret = 0;
    //std::cout << "Write HDF5 \n";
    if (!GetOpenFlag())
    {
        //std::cout << "Write HDF5 before open \n";
        SetRwFlag(H5F_ACC_RDWR);
        ExtractMeta(); //Will call open
    }

    if (data_type_p != AU_STRING)
    {
        hid_t mem_type_l, disk_type_l;
        Map2MyTypeParameters(data_type_p, mem_type_l, disk_type_l);
        //hsize_t attribute_dims = data_length_p;
        //hid_t attribute_dataspace_id = H5Screate_simple(1, &attribute_dims, NULL);
        //hid_t attribute_id = H5Acreate(did, name.c_str(), disk_type_l, attribute_dataspace_id, H5P_DEFAULT, H5P_DEFAULT);
        //hsize_t attribute_dims[1];
        hid_t attribute_id = H5Aopen(did, name.c_str(), H5P_DEFAULT);
        //hid_t attribute_space = H5Aget_space(attribute_id);
        //int attribute_ndims = H5Sget_simple_extent_dims(attribute_space, attribute_dims, NULL);
        ret = H5Aread(attribute_id, mem_type_l, data);
        //H5Sclose(attribute_dataspace_id);
        H5Aclose(attribute_id);
    }
    else
    {
        hid_t attribute_id = H5Aopen(did, name.c_str(), H5P_DEFAULT);
        hid_t attribute_datatype = H5Aget_type(attribute_id);
        size_t attribute_sdim = H5Tget_size(attribute_datatype);
        hid_t attribute_memtype = H5Tcopy(H5T_C_S1);
        H5Tset_size(attribute_memtype, attribute_sdim);
        ret = H5Aread(attribute_id, attribute_memtype, data);
        H5Aclose(attribute_id);
        //H5Sclose(attribute_space_scalar);
        //H5Tclose(attribute_str_type);
    }
    return ret;
}

size_t EndpointHDF5::GetAttributeSize(const std::string &name, FTDataType data_type_p)
{
    if (data_type_p != AU_STRING)
    {
        hsize_t attribute_dims[1];
        hid_t attribute_id = H5Aopen(did, name.c_str(), H5P_DEFAULT);
        hid_t attribute_space = H5Aget_space(attribute_id);
        int attribute_ndims = H5Sget_simple_extent_dims(attribute_space, attribute_dims, NULL);
        H5Aclose(attribute_id);
        H5Sclose(attribute_space);
        return attribute_dims[0];
    }
    else
    {
        hid_t attribute_id = H5Aopen(did, name.c_str(), H5P_DEFAULT);
        hid_t attribute_datatype = H5Aget_type(attribute_id);
        size_t attribute_sdim = H5Tget_size(attribute_datatype);
        H5Aclose(attribute_id);
        return attribute_sdim;
    }
}

int EndpointHDF5::CreateXDMF()
{
    int ret = 0;
    //std::cout << "Write HDF5 \n";
    if (!GetOpenFlag())
    {
        //std::cout << "Write HDF5 before open \n";
        SetRwFlag(H5F_ACC_RDWR);
        ExtractMeta(); //Will call open
    }
    //int create_xdmf(std::string file_name, std::string dset_name, std::vector<unsigned long long> dimensions, FTType data_element_type)
    create_xdmf(fn_str, gn_str + "/" + dn_str, endpoint_size, data_element_type);
}