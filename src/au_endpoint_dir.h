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
 * Email questions to dbin@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#ifndef END_POINT_DIR_H
#define END_POINT_DIR_H

#define DIR_MERGE_INDEX 0
#define DIR_SUB_CMD_ARG 1
#define DIR_INPUT_SEARCH_RGX 2
#define DIR_OUPUT_REPLACE_RGX 3
#define DIR_OUPUT_REPLACE_RGX_ARG 4
#define DIR_FILE_SORT_INDEXES 5
#define DIR_N_FILES 6
#define DIR_GET_FILE_SIZE 7

#include "au_utility.h"
#include "au_type.h"
#include "au_endpoint.h"
#include "au_endpoint_hdf5.h"
#include "au_endpoint_tdms.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <regex>

//
//I/O layer
class EndpointDIR : public Endpoint
{
private:
    std::string endpoint_info;
    AuEndpointType sub_endpoint_type;
    std::string sub_endpoint_info; //Directory of files
    Endpoint *sub_endpoint = nullptr;

    std::string dir_str;
    std::vector<std::string> dir_file_list;
    std::string append_sub_endpoint_info;              //dir_file_list[i] + append_sub_endpoint_info is the finale sub_endpoint_info
    std::vector<int> dir_chunk_size, dir_overlap_size; //set chunk size to be each sub_endpoint

    int dir_data_merge_index = 0;

    //http://www.cplusplus.com/reference/regex/ECMAScript/
    bool input_replace_regex_flag = false;
    std::regex *input_filter_regex; //The regex to "search" on list of file as input

    bool output_replace_regex_flag = false;
    std::regex *output_replace_regex; //The regex to "replace" on list of file as output (input)
    std::string output_replace_regex_aug;

    bool has_ordering_on_file_list = false; //User may apply ordering on list, based on sorted ordring
    std::vector<size_t> order_on_file_list;

public:
    /**
     * @brief Construct a new EndpointDIR object
     * 
     * @param data_endpoint contains the info of the endpoint, e.g., file type + file info
     */
    EndpointDIR(std::string endpoint_info_p)
    {
        endpoint_info = endpoint_info_p;
        ParseEndpointInfo();
        if (sub_endpoint_type == EP_HDF5)
        {
            sub_endpoint = new EndpointHDF5();
            std::vector<std::string> cmd_argv;
            sub_endpoint->Control(OP_DISABLE_MPI_IO, cmd_argv);
            sub_endpoint->Control(OP_DISABLE_COLLECTIVE_IO, cmd_argv);
        }
        else if (sub_endpoint_type == EP_TDMS)
        {
            sub_endpoint = new EndpointTDMS();
        }
        SetEndpointType(EP_DIR);
        sub_endpoint->SetDataElementType(data_element_type);
    }

    EndpointDIR()
    {
    }

    ~EndpointDIR()
    {
        if (sub_endpoint != nullptr)
            delete sub_endpoint;
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

    void EnableCollectiveIO() override;

    void DisableCollectiveIO() override;

    int ParseEndpointInfo() override;

    /**
     * @brief Get the Chunk Size object
     * 
     * @return std::vector<int> 
     */
    std::vector<int> GetDirChunkSize() override;

    void SetDirChunkSize(std::vector<int> &dir_chunk_size_p) override;

    std::vector<std::string> GetDirFileVector() override;

    void SetDirFileVector(std::vector<std::string> &file_list) override;

    /**
     * @brief call a special operator on endpoint
     *        such as, enable collective I/O for HDF5
     *                 dump file from MEMORY to HDF5
     * @param opt_code, specially defined code 
     */
    int Control(int opt_code, std::vector<std::string> &parameter_v) override;

    /**
     * @brief Set the Merge Index
     * 
     * @param index_p 
     */
    void SetMergeIndex(int index_p);

    /**
     * @brief Get the Merge Index object
     * 
     * @return int 
     */
    int GetMergeIndex();
};
#endif
