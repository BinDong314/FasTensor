
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
#ifndef END_POINT_HDF5_H
#define END_POINT_HDF5_H

#include "ft_type.h"
#include "ft_endpoint.h"
#include "ft_xdmf.h"

#include <string>
#include <iostream>
#include <vector>
#include <math.h>

#include "hdf5.h" //right now, we only have code for HDF5

#define HDF5_ENABLE_MPI_IO (OP_USER_DEFINED_START + 0)
#define HDF5_DISABLE_MPI_IO (OP_USER_DEFINED_START + 1)
#define HDF5_ENABLE_COLLECTIVE_IO (OP_USER_DEFINED_START + 2)
#define HDF5_DISABLE_COLLECTIVE_IO (OP_USER_DEFINED_START + 3)
#define HDF5_ENABLE_FILTER (OP_USER_DEFINED_START + 4)
#define HDF5_ENABLE_FILTER_PREPROCESSING (OP_USER_DEFINED_START + 5)
#define HDF5_LIST_RECURSIVE (OP_USER_DEFINED_START + 6)

//For some old code
#define OP_ENABLE_MPI_IO HDF5_ENABLE_MPI_IO
#define OP_DISABLE_MPI_IO HDF5_DISABLE_MPI_IO
#define OP_ENABLE_COLLECTIVE_IO HDF5_ENABLE_COLLECTIVE_IO
#define OP_DISABLE_COLLECTIVE_IO HDF5_DISABLE_COLLECTIVE_IO

//
//I/O layer
class EndpointHDF5 : public Endpoint
{
private:
    hid_t fid = -1, gid = -1, did = -1;
    hid_t dataspace_id = -1;
    std::string fn_str, gn_str, dn_str;
    hid_t plist_id = H5P_DEFAULT, plist_cio_id = H5P_DEFAULT, create_dcpl_id = H5P_DEFAULT;
    hid_t mem_type, disk_type;
    bool is_mpi_enabled = false;

    bool is_filter_set_before = false;
    H5Z_filter_t filter_id;
    unsigned int filter_flags = H5Z_FLAG_MANDATORY;
    size_t filter_cd_nelmts;
    std::vector<unsigned int> filter_cd_values;
    std::vector<hsize_t> filter_chunk_size;

    H5Z_filter_t filter_preprocessing_id;
    bool is_filter_preprocessing = false;

public:
    /**
     * @brief Construct a new EndpointHDF5 object
     * 
     * @param data_endpoint contains the info of the endpoint, e.g., file type + file info
     */
    EndpointHDF5(std::string endpoint_info_p)
    {
        endpoint_info = endpoint_info_p;
        ParseEndpointInfo();
        SetOpenFlag(false);
        SetRwFlag(H5F_ACC_RDONLY);
        SetEndpointType(EP_HDF5);
        EnableMPIIO();
        EnableCollectiveIO();
        is_mpi_enabled = true;
    }
    /**
     * @brief Construct a new Endpoint in HDF5 
     *         Nothing to do there, can be used as sub-endpoint of directory
     */
    EndpointHDF5()
    {
        SetOpenFlag(false);
        SetRwFlag(H5F_ACC_RDONLY);
        SetEndpointType(EP_HDF5);
        EnableMPIIO();
        EnableCollectiveIO();
        is_mpi_enabled = true;
    }

    /**
     * @brief Construct a new EndpointHDF5 object without MPI
     * 
     * @param no_mpi  any number should work
     */
    EndpointHDF5(int no_mpi)
    {
        //endpoint_info = endpoint_info_p;
        //ParseEndpointInfo();
        SetOpenFlag(false);
        SetRwFlag(H5F_ACC_RDONLY);
        SetEndpointType(EP_HDF5);
        //EnableMPIIO();
        //EnableCollectiveIO();
    }

    ~EndpointHDF5()
    {
        Close();
        if (is_mpi_enabled)
        {
            DisableMPIIO();
            DisableCollectiveIO();
        }
    }
    /**
     * @brief extracts metadata, possbile endpoint_ranks/endpoint_dim_size/data_element_type
     * 
     * @return int < 0 error, >= 0 works 
     */
    int ExtractMeta() override;
    /**
     * @brief print information about the endpoint
     * 
     * @return < 0 error, >= 0 works 
     */
    int PrintInfo() override;

    /**
     * @brief create the endpoint
     * 
     * @return  < 0 error, >= 0 works 
     */
    int Create() override;

    /**
     * @brief open the endpoint
     * 
     * @return < 0 error, >= 0 works 
     */
    int Open() override;

    /**
     * @brief read the data from end-point
     * 
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data 
     * @return int < 0 error, >= 0 works
     */
    int Read(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data) override;

    /**
     * @brief write the data to the end-point
     * 
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data 
     * @return int < 0 error, >= 0 works
     */
    int Write(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data) override;

    /**
     * @brief close the end-point
     * 
     * @return int int < 0 error, >= 0 works
     */
    int Close() override;

    void Map2MyType() override;

    void Map2MyTypeParameters(FTDataType ft_type, hid_t &mem_type, hid_t &disk_type);

    void EnableCollectiveIO() override;

    void DisableCollectiveIO() override;

    void EnableMPIIO();

    void DisableMPIIO();

    /**
     * @brief parse endpoint_info to my own info
     *        In HDF5, it map endpoint_info to filename, group name and datasetname
     * @return int: 0 works,  < 0 error,
     */
    int ParseEndpointInfo() override;

    /**
     * @brief call a special operator on endpoint
     *        such as, enable collective I/O for HDF5
     *                 dump file from MEMORY to HDF5
     * @param opt_code, specially defined code 
     */
    //int SpecialOperator(int opt_code, std::string parameter) override;

    /**
     * @brief call a special operator on endpoint
     *        such as, enable collective I/O for HDF5
     *                 dump file from MEMORY to HDF5
     * @param opt_code, specially defined code 
     */
    int Control(int opt_code, std::vector<std::string> &parameter_v) override;

    /**
     * @brief Set the Attribute object
     * 
     * @param name 
     * @param data 
     * @return int 
     */
    int WriteAttribute(const std::string &name, const void *data, FTDataType data_type_p, const size_t &data_length_p = 0) override;

    /**
     * @brief Read all attribute name
     * 
     * @param attri_name 
     * @return int 
     */
    int ReadAllAttributeName(std::vector<std::string> &attr_name);

    /**
     * @brief Get the Attribute object
     * 
     * @param name 
     * @param data 
     * @return int 
     */
    int ReadAttribute(const std::string &name, void *data, FTDataType data_type_p, const size_t &data_length_p = 0) override;

    int GetAttributeSize(const std::string &name, FTDataType data_type_p) override;

    int CreateXDMF();

    void EnableFilter(std::vector<std::string> &parameter_v);

    void EnableFilterPreprocessing(std::vector<std::string> &parameter_v);

    /**
     * @brief List datasets of a HDF5 
     * 
     * @param dataset_list, each item contain path from root to HDF5 
     */
    void ListDatasetsRecursive(std::vector<std::string> &dataset_list);

    /**
     * @brief an operator function for ListDatasetsRecursive
     * 
     * @param loc_id 
     * @param name 
     * @param info 
     * @param operator_data 
     * @return herr_t 
     */
    //herr_t static ListDatasetsRecursiveOpFunc(hid_t loc_id, const char *name, const H5O_info_t *info,
    //                                          void *operator_data);
};
#endif
