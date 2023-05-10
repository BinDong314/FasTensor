/*
****************************

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy).
All rights reserved.

If you have questions about your rights to use or distribute this software,
please contact Berkeley Lab's Intellectual Property Office at
IPO@lbl.gov.

NOTICE.  This Software was developed under funding from the U.S. Department
of Energy and the U.S. Government consequently retains certain rights.  As
such, the U.S. Government has been granted for itself and others acting on
its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the
Software to reproduce, distribute copies to the public, prepare derivative
works, and perform publicly and display publicly, and to permit others to do so.


****************************


*** License Agreement ***

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy).
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

(1) Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

(2) Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

(3) Neither the name of the University of California, Lawrence Berkeley
National Laboratory, U.S. Dept. of Energy nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

You are under no obligation whatsoever to provide any bug fixes, patches,
or upgrades to the features, functionality or performance of the source
code ("Enhancements") to anyone; however, if you choose to make your
Enhancements available either publicly, or directly to Lawrence Berkeley
National Laboratory, without imposing a separate written license agreement
for such Enhancements, then you hereby grant the following license: a
non-exclusive, royalty-free perpetual license to install, use, modify,
prepare derivative works, incorporate into other computer software,
distribute, and sublicense such enhancements or derivative works thereof,
in binary and source code form.
*/

/**
 *
 * Author: Bin Dong dbin@lbl.gov
 * Web: https://crd.lbl.gov/bin-dong
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#include "ft_endpoint_hdf5.h"

int EndpointHDF5::ExtractMeta()
{
    // cout << "Print Out : ExtractMeta\n";
    // PrintInfo();
    Open();
    // cout << "Print Out : ExtractMeta after open\n";
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
#ifdef FT_DEBUG
    std::cout << "EndpointHDF5::Create :: " << fn_str << "\n";
#endif
    Map2MyType();
    std::string root_dir = "/";
    // PrintInfo();
    // plist_id = H5Pcreate(H5P_FILE_ACCESS);
    // H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

    if (file_exist(fn_str.c_str()) == 0)
    {
        // char *ts1 = strdup(fn_str.c_str());
        // char *dir = dirname(ts1);
        // std::string dir_str(dir);
        // mkpath(dir_str, 0755);
        //  std::cout << "Call H5Fcreate 1 : " << fn_str << "\n"<< std::flush;
        // free(ts1);
        fid = H5Fcreate(fn_str.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
        // std::cout << "After Call H5Fcreate : af " << fn_str << "\n"<< std::flush;
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
    // PrintVector("endpoint_size = ", endpoint_size);
    // printf("endpoint_ranks = %d \n", endpoint_ranks);
    std::vector<hsize_t> dims_out;
    dims_out = endpoint_size;
    hid_t ts_id;
    ts_id = H5Screate_simple(endpoint_ranks, &dims_out[0], NULL);

    if (gn_str != root_dir)
    {
        if (H5Lexists(gid, dn_str.c_str(), H5P_DEFAULT) > 0)
        {
            H5Ldelete(gid, dn_str.c_str(), H5P_DEFAULT); // we delete
        }

        did = H5Dcreate(gid, dn_str.c_str(), disk_type, ts_id, H5P_DEFAULT, create_dcpl_id, H5P_DEFAULT);
    }
    else
    {
        if (H5Lexists(fid, dn_str.c_str(), H5P_DEFAULT) > 0)
        {
            H5Ldelete(fid, dn_str.c_str(), H5P_DEFAULT); // we delete
        }
        // printf("Debug: %s:%d\n", __FILE__, __LINE__);
        did = H5Dcreate(fid, dn_str.c_str(), disk_type, ts_id, H5P_DEFAULT, create_dcpl_id, H5P_DEFAULT);
        assert(did >= 0);
    }
    // printf("Debug: %s:%d\n", __FILE__, __LINE__);

    dataspace_id = H5Dget_space(did);
    H5Sclose(ts_id);
    SetRwFlag(H5F_ACC_RDWR);
    SetOpenFlag(true);

    Close(); // Close for data consistency during writing
    return 0;
}

int EndpointHDF5::Open()
{
#ifdef FT_DEBUG
    std::cout << "Debug EndpointHDF5::Open :: fn_str = " << fn_str << ", gn_str = " << gn_str << ", dn_str = " << dn_str << "\n";
#endif
    // PrintInfo();
    if (file_exist(fn_str.c_str()) == 0)
    {
        // Since the create close it , we may reopen it again should we return ?
        // std::cout << "Call create in Open again !\n";
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
        // std::cout << "Open Group : " << gn << std::endl;
        gid = H5Gopen(fid, gn_str.c_str(), H5P_DEFAULT);
        did = H5Dopen(gid, dn_str.c_str(), H5P_DEFAULT);
    }
    else
    {
        gid = -1;
        did = H5Dopen(fid, dn_str.c_str(), H5P_DEFAULT);
    }
    assert(did >= 0);

    dataspace_id = H5Dget_space(did);

    SetOpenFlag(true);

    // plist_cio_id = H5P_DEFAULT;
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
        ExtractMeta(); // It also opens
    }

    Map2MyType();

    std::vector<unsigned long long> offset, count;
    offset.resize(endpoint_ranks);
    count.resize(endpoint_ranks);
    for (int i = 0; i < endpoint_ranks; i++)
    {
        offset[i] = start[i];
        count[i] = end[i] - start[i] + 1; // Starting from zero
    }

    hid_t memspace_id = H5Screate_simple(endpoint_ranks, &count[0], NULL);
    H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, &offset[0], NULL, &count[0], NULL);

    hid_t dset_type_id = H5Dget_type(did);
    hid_t typeclass = H5Tget_class(dset_type_id);
    int ret;
    if (typeclass != H5T_COMPOUND)
    {

        ret = H5Dread(did, mem_type, memspace_id, dataspace_id, plist_cio_id, data);
    }
    else
    {
        hid_t mem_typeclass = H5Tget_native_type(dset_type_id, H5T_DIR_ASCEND);
        ret = H5Dread(did, mem_typeclass, memspace_id, dataspace_id, plist_cio_id, data);
    }
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

    // std::cout << "Write HDF5 \n";
    if (!GetOpenFlag())
    {
        // std::cout << "Write HDF5 before open \n";
        SetRwFlag(H5F_ACC_RDWR);
        ExtractMeta(); // Will call open
    }
    else if (GetRwFlag() != H5F_ACC_RDWR)
    {
        SetRwFlag(H5F_ACC_RDWR);
        Close();       // Close
        ExtractMeta(); // Re-open it
    }
    // Close();
    // SetRwFlag(H5F_ACC_RDWR);
    // Open(); //Re-open it

    bool is_empty_write = false;
    std::vector<unsigned long long> offset, count;
    offset.resize(endpoint_ranks);
    count.resize(endpoint_ranks);
    for (int i = 0; i < endpoint_ranks; i++)
    {
        offset[i] = start[i];
        count[i] = end[i] - start[i] + 1; // Starting from zero
    }

    bool is_zero_start = std::all_of(start.begin(), start.end(), [](int i)
                                     { return i == 0; });
    bool is_zero_end = std::all_of(start.begin(), start.end(), [](int i)
                                   { return i == 0; });

    if (data == nullptr && is_zero_start && is_zero_end)
    {
        is_empty_write = true;
    }
    // PrintVector("offset =", offset);
    // PrintVector("count =", count);

    std::vector<unsigned long long> endpoint_dim_size_temp(endpoint_ranks);
    H5Sget_simple_extent_dims(dataspace_id, &endpoint_dim_size_temp[0], NULL);

    // PrintVector("endpoint_dim_size = ", endpoint_dim_size_temp);

    hid_t memspace_id = H5Screate_simple(endpoint_ranks, &count[0], NULL);
    if (is_empty_write)
    {
        H5Sselect_none(dataspace_id);
        H5Sselect_none(memspace_id);
    }
    else
    {
        H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, &offset[0], NULL, &count[0], NULL);
    }
    int ret = H5Dwrite(did, mem_type, memspace_id, dataspace_id, plist_cio_id, data);
    assert(ret >= 0);
    H5Sclose(memspace_id);
    Close(); // Close
    return ret;
}

/**
 * @brief close the end-point
 *
 * @return int int < 0 error, >= 0 works
 */
int EndpointHDF5::Close()
{
    // std::cout << "Rank " << ft_rank << ", Enter Close \n";
    // std::cout << "Rank " << ft_rank << ", fid= " << fid << ", H5Fflush \n";
    if (fid > 0)
        H5Fflush(fid, H5F_SCOPE_GLOBAL);

    // std::cout << "Rank " << ft_rank << ", dataspace_id= " << dataspace_id << ", H5Sclose \n";
    if (dataspace_id > 0)
        H5Sclose(dataspace_id);

    // std::cout << "Rank " << ft_rank << ", did= " << did << ", H5Dclose \n";
    if (did > 0)
        H5Dclose(did);

    // std::cout << "Rank " << ft_rank << ", gid= " << gid << ", H5Gclose \n";
    if (gid > 0)
        H5Gclose(gid);

    if (fid > 0)
        assert(H5Fclose(fid) >= 0);

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

/**
 * @brief EnableFilterPreprocessing
 *        It has the same ID as the EnableFilter below
 * @param parameter_v
 */
void EndpointHDF5::EnableFilterPreprocessing(std::vector<std::string> &parameter_v)
{
    filter_preprocessing_id = std::stoi(parameter_v[0]);
    is_filter_preprocessing = true;
}

/**
 * @brief Enable Fil
 *
 * @param parameter_v
 *  parameter_v[0]: filter_id
 *  parameter_v[2]: filter_cd_values, (filter_cd_nelmts = filter_cd_values.size())
 *  parameter_v[1]: chunk_size
 *   Both filter_cd_values and chunk_size are two comma seperated string
 */
void EndpointHDF5::EnableFilter(std::vector<std::string> &parameter_v)
{
    if (!is_filter_set_before)
    {
        create_dcpl_id = H5Pcreate(H5P_DATASET_CREATE);
        is_filter_set_before = true;
    }
    filter_id = std::stoi(parameter_v[0]);
    // std::stringstream sstream(parameter_v[1]);
    // sstream >> filter_cd_nelmts;
    if (is_filter_preprocessing)
    {
        switch (filter_preprocessing_id)
        {
        case H5Z_FILTER_SHUFFLE:
            H5Pset_shuffle(create_dcpl_id);
            break;
        default:
            AU_EXIT("filter_preprocessing_id is not supported " + std::to_string(filter_preprocessing_id));
        }
    }

    // AU_VERBOSE("EnableFilter: filter_id = " + std::to_string(filter_id), 0)
    if (!ft_rank)
        std::cout << "EnableFilter: filter_id = " << filter_id << "\n";
    String2Vector(parameter_v[1], filter_cd_values);
    filter_cd_nelmts = filter_cd_values.size();
    if (!ft_rank)
        PrintVector("EnableFilter:filter_cd_values =", filter_cd_values);
    // herr_t H5Pset_filter(hid_t plist_id, H5Z_filter_t filter_id, unsigned int flags, size_t cd_nelmts, const unsigned int cd_values[] )
    H5Pset_filter(create_dcpl_id, filter_id, filter_flags, filter_cd_nelmts, filter_cd_values.data());

    String2Vector(parameter_v[2], filter_chunk_size);
    endpoint_ranks = filter_chunk_size.size();
    H5Pset_chunk(create_dcpl_id, endpoint_ranks, filter_chunk_size.data());
    if (!ft_rank)
        PrintVector("EnableFilter:filter_chunk_size =", filter_chunk_size);

    // printf("Set fill never !\n");
    // H5Pset_fill_time(create_dcpl_id, H5D_FILL_TIME_NEVER);

    // Double check
    htri_t avail = H5Zfilter_avail(filter_id);
    if (avail)
    {
        unsigned filter_config;
        herr_t status = H5Zget_filter_info(filter_id, &filter_config);
        if (filter_config & H5Z_FILTER_CONFIG_ENCODE_ENABLED)
        {
            if (!ft_rank)
                AU_INFO("Filter " + std::to_string(filter_id) + " is  ENABLED for HDF5 Endpoint.\n");
        }
        else
        {
            if (!ft_rank)
                AU_INFO("Filter " + std::to_string(filter_id) + " is  NOT ENABLED for HDF5 Endpoint.\n");
        }
    }
    else
    {
        if (!ft_rank)
            AU_EXIT("Filter " + std::to_string(filter_id) + " is  NOT available for HDF5 Endpoint.\n");
    }
}

void EndpointHDF5::DisableCollectiveIO()
{
    // cout << "H5P_DEFAULT =" << H5P_DEFAULT << ", DisableCollectiveIO \n";
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
    // cout << "DisableMPIIO \n";
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
        printf(" EndpointHDF5::Map2MyTypeParameters: USHOR !\n");
        mem_type = H5T_NATIVE_USHORT;
        disk_type = H5T_STD_U16LE;
        return;
    case AU_UINT:
        mem_type = H5T_NATIVE_UINT;
        disk_type = H5T_STD_U32LE;
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
    case AU_NO_TYPE:
        mem_type = -1;
        disk_type = -1;
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
        printf(" EndpointHDF5::Map2MyTypeParameters: USHOR !\n");
        mem_type_p = H5T_NATIVE_USHORT;
        disk_type_p = H5T_STD_U32LE;
        return;
    case AU_UINT:
        mem_type_p = H5T_NATIVE_UINT;
        disk_type_p = H5T_STD_U16LE;
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
    std::cout << endpoint_info << "\n";
    //std::stringstream ss(endpoint_info);
    std::stringstream ss;
    ss<<endpoint_info;
    if (!std::getline(ss, fn_str, ':'))
    {
        AU_EXIT("Invalid endpoint_info");
    }

    std::string group_dataset_name_str;

    if (!std::getline(ss, group_dataset_name_str, ':'))
    {
        return 0; // Ingore the one without dataset but just name
        AU_EXIT("Invalid endpoint_info");
    }

    gn_str = ExtractPath(group_dataset_name_str);
    if (gn_str == "")
    {
        gn_str = "/";
    }
    dn_str = ExtractFileName(group_dataset_name_str);

//#ifdef FT_DEBUG
    std::cout << "EndpointHDF5::ParseEndpointInfo: fn_str =" << fn_str << ", gn_str = " << gn_str << ", dn_str =" << dn_str << std::endl;
<<<<<<< HEAD
#endif
    return 0;
=======
    return 0;
//#endif
>>>>>>> 27a314d063ede07af4bf4961aff7caf5c3bc367b
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
    case HDF5_ENABLE_FILTER:
        EnableFilter(parameter_v);
        break;
    case HDF5_ENABLE_FILTER_PREPROCESSING:
        EnableFilterPreprocessing(parameter_v);
        break;
    case HDF5_LIST_RECURSIVE:
        ListDatasetsRecursive(parameter_v);
        break;
    default:
        break;
    }

    return 0;
}

int EndpointHDF5::ReadAllAttributeName(std::vector<std::string> &attr_names)
{

    int ret = 0;
    // std::cout << "Write HDF5 \n";
    if (!GetOpenFlag())
    {
        // std::cout << "Write HDF5 before open \n";
        SetRwFlag(H5F_ACC_RDWR);
        ExtractMeta(); // Will call open
    }

    int na;
    hid_t aid;

    na = H5Aget_num_attrs(did);
    if (na < 0)
    {
        AU_EXIT("Error in H5Aget_num_attrs, na = " + std::to_string(na));
    }

    // printf("ReadAllAttributeName, na = %d \n", na);
    ssize_t len;
    char buf[1024];
    attr_names.clear();

    for (unsigned int iiiii = 0; iiiii < na; iiiii++)
    {
        // printf("Open attribute = %u , na = %d \n", iiiii, na);
        aid = H5Aopen_idx(did, iiiii);
        if (aid < 0)
        {
            AU_EXIT("Error in H5Aopen_idx = " + std::to_string(iiiii));
        }
        // continue;
        memset(buf, '\0', 1024);
        len = H5Aget_name(aid, 1024, buf);
        if (len < 0)
        {
            AU_EXIT("Error in H5Aget_name \n");
        }
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
    // std::cout << "Write HDF5 \n";
    if (!GetOpenFlag())
    {
        // std::cout << "Write HDF5 before open \n";
        SetRwFlag(H5F_ACC_RDWR);
        ExtractMeta(); // Will call open
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
        if (data_length_p > 0)
        {
            H5Tset_size(attribute_str_type, data_length_p);
        }
        else
        {
            // std::cout << "WriteAttribute: name " << name << ", data_length_p = " << data_length_p << "\n";
            H5Tset_size(attribute_str_type, 1);
        }
        // H5Tset_strpad(attribute_str_type, H5T_STR_NULLTERM);
        hid_t attribute_id = H5Acreate(did, name.c_str(), attribute_str_type, attribute_space_scalar, H5P_DEFAULT, H5P_DEFAULT);
        ret = H5Awrite(attribute_id, attribute_str_type, data);

        H5Aclose(attribute_id);
        H5Sclose(attribute_space_scalar);
        H5Tclose(attribute_str_type);
    }
    return ret;
}

// https://support.hdfgroup.org/ftp/HDF5/examples/misc-examples/attrvstr.c
// https://support.hdfgroup.org/ftp/HDF5/examples/examples-by-api/hdf5-examples/1_10/C/H5T/h5ex_t_vlstringatt.c
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
    // std::cout << "Write HDF5 \n";
    if (!GetOpenFlag())
    {
        // std::cout << "Write HDF5 before open \n";
        SetRwFlag(H5F_ACC_RDWR);
        ExtractMeta(); // Will call open
    }

    if (data_type_p != AU_STRING)
    {
        hid_t mem_type_l, disk_type_l;
        Map2MyTypeParameters(data_type_p, mem_type_l, disk_type_l);
        if (!H5Aexists(did, name.c_str()))
        {
            return -1;
        }
        hid_t attribute_id = H5Aopen(did, name.c_str(), H5P_DEFAULT);
        assert(attribute_id >= 0);
        ret = H5Aread(attribute_id, mem_type_l, data);
        H5Aclose(attribute_id);
    }
    else
    {
        if (!H5Aexists(did, name.c_str()))
        {
            return -1;
        }
        hid_t attribute_id = H5Aopen(did, name.c_str(), H5P_DEFAULT);
        assert(attribute_id >= 0);
        hid_t attribute_datatype = H5Aget_type(attribute_id);
        size_t attribute_sdim = H5Tget_size(attribute_datatype);
        hid_t attribute_memtype = H5Tcopy(H5T_C_S1);

        if (H5Tis_variable_str(attribute_datatype))
        {
            char *string_attr;
            H5Tset_size(attribute_memtype, H5T_VARIABLE);
            ret = H5Aread(attribute_id, attribute_memtype, &string_attr);
            assert(ret >= 0);
            std::string temp_str(string_attr);
            // std::cout << "ReadAttribute: name " << name << ", value = " << std::string(string_attr) << "\n";
            memcpy(data, temp_str.data(), temp_str.size());
        }
        else
        {
            H5Tset_size(attribute_memtype, attribute_sdim);
            ret = H5Aread(attribute_id, attribute_memtype, data);
            assert(ret >= 0);
        }

        H5Aclose(attribute_id);
        // H5Sclose(attribute_space_scalar);
        // H5Tclose(attribute_str_type);
    }
    return ret;
}

int EndpointHDF5::GetAttributeSize(const std::string &name, FTDataType data_type_p)
{
    // std::cout << "Write HDF5 \n";
    if (!GetOpenFlag())
    {
        // std::cout << "Write HDF5 before open \n";
        SetRwFlag(H5F_ACC_RDWR);
        ExtractMeta(); // Will call open
    }

    if (data_type_p != AU_STRING)
    {
        hsize_t attribute_dims[1];
        if (!H5Aexists(did, name.c_str()))
        {
            return -1;
        }
        hid_t attribute_id = H5Aopen(did, name.c_str(), H5P_DEFAULT);
        hid_t attribute_space = H5Aget_space(attribute_id);
        int attribute_ndims = H5Sget_simple_extent_dims(attribute_space, attribute_dims, NULL);
        H5Aclose(attribute_id);
        H5Sclose(attribute_space);
        // printf("attribute_dims[0] GetAttributeSize = %d \n\n", attribute_dims[0]);
        return attribute_dims[0];
    }
    else
    {
        if (!H5Aexists(did, name.c_str()))
        {
            return -1;
        }
        hid_t attribute_id = H5Aopen(did, name.c_str(), H5P_DEFAULT);
        size_t attribute_size;
        hid_t attribute_space = H5Aget_space(attribute_id);
        hid_t attribute_datatype = H5Aget_type(attribute_id);
        if (H5Tis_variable_str(attribute_datatype))
        {
            // printf("It is H5Tis_variable_str!\n");
            hid_t attribute_memtype = H5Tcopy(H5T_C_S1);
            // char **string_attr;
            char *string_attr = 0;
            H5Tset_size(attribute_memtype, H5T_VARIABLE);
            // string_attr = (char **)malloc(1 * sizeof(char *));
            assert(H5Aread(attribute_id, attribute_memtype, &string_attr) >= 0);
            if (string_attr == NULL)
            {
                attribute_size = 0;
                goto read_null;
            }
            // std::string temp_str(string_attr[0]);
            attribute_size = strlen(string_attr);
            // if (attribute_size == 0)
            //{
            //     std::cout << "H5Aread name = " << name << " , size = " << attribute_size << " fname = " << fn_str << "\n";
            // }
            // std::string temp_str(string_attr);
            // attribute_size = temp_str.size();
            // H5Dvlen_reclaim(attribute_memtype, attribute_space, H5P_DEFAULT, string_attr);
            free(string_attr);
            // std::cout << "It is H5Tis_variable_str, temp_str = " << temp_str << "\n";
        read_null:
            H5Tclose(attribute_memtype);
        }
        else
        {
            attribute_size = H5Tget_size(attribute_datatype);
            // printf("It is NOT NOT H5Tis_variable_str!\n");
        }
        H5Tclose(attribute_datatype);
        H5Sclose(attribute_space);
        H5Aclose(attribute_id);
        return attribute_size;
    }
}

int EndpointHDF5::CreateXDMF()
{
    int ret = 0;
    // std::cout << "Write HDF5 \n";
    if (!GetOpenFlag())
    {
        // std::cout << "Write HDF5 before open \n";
        SetRwFlag(H5F_ACC_RDWR);
        ExtractMeta(); // Will call open
    }
    // int create_xdmf(std::string file_name, std::string dset_name, std::vector<unsigned long long> dimensions, FTType data_element_type)
    std::string gd_str = gn_str + "/" + dn_str;
    create_xdmf(fn_str, gd_str, endpoint_size, data_element_type);
}

std::vector<std::string> ListDatasetsRecursiveOpFuncList;
/************************************************************

  Operator function for H5Ovisit.  This function prints the
  name and type of the object passed to it.

 ************************************************************/
herr_t
ListDatasetsRecursiveOpFunc(hid_t loc_id, const char *name, const H5O_info_t *info,
                            void *operator_data)
{

    if (info->type == H5O_TYPE_DATASET)
    {
        // std::vector<std::string> *p = static_cast<std::vector<std::string> *>(operator_data);
        // list_recursive_objects.push_back(name);
        ListDatasetsRecursiveOpFuncList.push_back(name);
    }
    return 0;
}

/**
 * @brief List datasets of a HDF5
 *
 * @param dataset_list, each item contain path from root to HDF5
 */
void EndpointHDF5::ListDatasetsRecursive(std::vector<std::string> &dataset_list)
{

    hid_t file; /* Handle */
    herr_t status;
    dataset_list.clear();
    ListDatasetsRecursiveOpFuncList.clear();
    // list_recursive_objects.clear();
    file = H5Fopen(fn_str.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    status = H5Ovisit(file, H5_INDEX_NAME, H5_ITER_NATIVE, ListDatasetsRecursiveOpFunc, NULL, H5O_INFO_ALL);
    status = H5Fclose(file);
    dataset_list = ListDatasetsRecursiveOpFuncList;
    ListDatasetsRecursiveOpFuncList.clear();
}
