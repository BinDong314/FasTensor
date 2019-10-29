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

#ifndef END_POINT_HDF5_H
#define END_POINT_HDF5_H

#include "array_udf_attribute.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>

#include "hdf5.h" //right now, we only have code for HDF5

using namespace std;

//
//I/O layer
template <class T>
class EndpointHDF5 : public Endpoint
{
private:
    hid_t fid = -1, gid = -1, did = -1;
    hid_t dataspace_id = -1;
    string fn_str, gn_str, dn_str;
    hid_t plist_id = -1, plist_cio_id = H5P_DEFAULT;
    hid_t mem_type, disk_type;

public:
    /**
     * @brief Construct a new EndpointHDF5 object
     * 
     * @param data_endpoint contains the info of the endpoint, e.g., file type + file info
     */
    EndpointHDF5(string data_endpoint) : Endpoint(data_endpoint)
    {
        if (endpoint_info.size() == 3)
        {
            fn_str = endpoint_info[0];
            gn_str = endpoint_info[1];
            dn_str = endpoint_info[2];
        }
        else if (endpoint_info.size() == 2)
        {
            fn_str = endpoint_info[0];
            gn_str = "/";
            dn_str = endpoint_info[2];
        }
        else
        {
            std::cout << " data endpoint to HDF5 is not correct" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    /**
     * @brief extracts metadata, possbile endpoint_ranks/endpoint_dim_size/data_element_type
     * 
     * @return int < 0 error, >= 0 works 
     */
    virtual int ExtractMeta() override
    {
        Open();
        hid_t datatype = H5Dget_type(did); /* datatype handle */
        H5T_class_t type_class = H5Tget_class(datatype);
        dataspace_id = H5Dget_space(did);
        endpoint_ranks = H5Sget_simple_extent_ndims(dataspace_id);
        endpoint_dim_size.resize(endpoint_ranks);
        H5Sget_simple_extent_dims(dataspace_id, &endpoint_dim_size[0], NULL);
    }
    /**
     * @brief print information about the endpoint
     * 
     * @return < 0 error, >= 0 works 
     */
    virtual int PrintInfo() override
    {
    }

    /**
     * @brief create the endpoint
     * 
     * @return  < 0 error, >= 0 works 
     */
    virtual int Create() override
    {
        Map2MyType();
        std::string root_dir = "/";
        plist_id = H5Pcreate(H5P_FILE_ACCESS);
        H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

        if (file_exist(fn_str.c_str()) == 0)
        {
            fid = H5Fcreate(fn_str.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
        }
        else
        {
            fid = H5Fopen(fn_str.c_str(), H5F_ACC_RDWR, plist_id);
            if (fid < 0)
                fid = H5Fcreate(fn_str.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
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
    }

    /**
     * @brief open the endpoint
     * 
     * @return < 0 error, >= 0 works 
     */
    virtual int Open() override
    {
        plist_id = H5Pcreate(H5P_FILE_ACCESS);
        //Comment out for paralle VDS test on sigle node
        H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

        fid = H5Fopen(fn_str.c_str(), H5F_ACC_RDONLY, plist_id);
        if (fid < 0)
        {
            std::cout << "Error happens in open file " << fn << std::endl;
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
    virtual int Read(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data) override
    {
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
    virtual int Write(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data) override
    {
        std::vector<unsigned long long> offset, count;
        offset.resize(endpoint_ranks);
        count.resize(endpoint_ranks);
        for (int i = 0; i < endpoint_ranks; i++)
        {
            offset[i] = start[i];
            count[i] = end[i] - start[i] + 1; //Starting from zero
        }

        memspace_id = H5Screate_simple(endpoint_ranks, &count[0], NULL);
        H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, &offset[0], NULL, &count[0], NULL);
        int ret = H5Dwrite(did, h5_mem_type, memspace_id, dataspace_id, plist_cio_id, data);
        assert(ret >= 0);
        H5Sclose(memspace_id);
        return ret;
    }

    /**
     * @brief close the end-point
     * 
     * @return int int < 0 error, >= 0 works
     */
    virtual int Close()
    {
        if (fid > 0)
            H5Fflush(fid, H5F_SCOPE_GLOBAL);
        if (plist_id > 0)
            H5Pclose(plist_id);
        if (plist_cio_id > 0)
            H5Pclose(plist_cio_id);
        if (dataspace_id > 0)
            H5Sclose(dataspace_id);
        if (did > 0)
            H5Dclose(did);
        if (gid > 0)
            H5Gclose(gid);
        if (fid > 0)
            H5Fclose(fid);
        plist_id = -1;
        plist_cio_id = -1;
        dataspace_id = -1;
        did = -1;
        gid = -1;
        fid = -1;
    };

    void Map2MyType()
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
        case AU_DOUBLE
            mem_type = H5T_NATIVE_DOUBLE;
            disk_type = H5T_IEEE_F64LE;
            default:
            std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
            exit(-1);
        }
    };

    void EnableCollectiveIO()
    {
        if (plist_cio_id > 0)
            H5Pclose(plist_cio_id);
        plist_cio_id = H5Pcreate(H5P_DATASET_XFER);
        H5Pset_dxpl_mpio(plist_cio_id, H5FD_MPIO_COLLECTIVE);
    }

    void DisableCollectiveIO()
    {
        if (plist_cio_id > 0)
            H5Pclose(plist_cio_id);
        plist_cio_id = H5P_DEFAULT;
    }
}
#endif
