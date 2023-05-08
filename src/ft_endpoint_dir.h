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

#ifndef END_POINT_DIR_H
#define END_POINT_DIR_H

#define DIR_MERGE_INDEX (OP_USER_DEFINED_START + 0)
#define DIR_SUB_CMD_ARG (OP_USER_DEFINED_START + 1)
#define DIR_INPUT_SEARCH_RGX (OP_USER_DEFINED_START + 2)
#define DIR_OUPUT_REPLACE_RGX (OP_USER_DEFINED_START + 3)
#define DIR_OUPUT_REPLACE_RGX_ARG (OP_USER_DEFINED_START + 4)
#define DIR_FILE_SORT_INDEXES (OP_USER_DEFINED_START + 15)
#define DIR_N_FILES (OP_USER_DEFINED_START + 6)
#define DIR_GET_FILE_SIZE (OP_USER_DEFINED_START + 7)
#define DIR_SET_CHUNK_SIZE (OP_USER_DEFINED_START + 8)
#define DIR_GET_ALL_CHUNK_TAGS (OP_USER_DEFINED_START + 9)
#define DIR_SET_ALL_CHUNK_TAGS (OP_USER_DEFINED_START + 10)

// Todo
#define DIR_INPUT_ELASTIC_SIZE (OP_USER_DEFINED_START + 11)
#define DIR_OUTPUT_ELASTIC_SIZE (OP_USER_DEFINED_START + 12)
#define DIR_SET_OUTPUT_FILE_NAMES (OP_USER_DEFINED_START + 13)

#define DIR_LIST_RECURSIVE (OP_USER_DEFINED_START + 14)
#define DIR_FILE_LIST_RECURSIVE (OP_USER_DEFINED_START + 17)

#define DIR_SKIP_SIZE_CHECK (OP_USER_DEFINED_START + 16)
#define DIR_INPUT_VARIABLE_SEARCH_RGX (OP_USER_DEFINED_START + 18)
#define DIR_SET_VIEW (OP_USER_DEFINED_START + 19)

#define DIR_SAVE_FINAL_FILE_LIST (OP_USER_DEFINED_START + 20)
#define DIR_SET_INPUT_FILE_LIST (OP_USER_DEFINED_START + 21)

#include "ft_utility.h"
#include "ft_type.h"
#include "ft_endpoint.h"
#include "ft_endpoint_hdf5.h"
#include "ft_endpoint_tdms.h"
#include "ft_endpoint_csv.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <regex>
#include <string.h>
#include <fstream>

//
// I/O layer
class EndpointDIR : public Endpoint
{
private:
    std::string endpoint_info;
    AuEndpointType sub_endpoint_type;
    std::string sub_endpoint_info; // Directory of files
    Endpoint *sub_endpoint = nullptr;

    std::string dir_str;
    std::vector<std::string> dir_file_list;
    int dir_file_list_current_index = 0; // index of the current one in read/write

    std::string append_sub_endpoint_info;              // dir_file_list[i] + append_sub_endpoint_info is the finale sub_endpoint_info
    std::vector<int> dir_chunk_size, dir_overlap_size; // set chunk size to be each sub_endpoint
    std::vector<int> dir_file_size;                    // It records the size of each "merged file" from small files.
    int dir_data_merge_index = 0;

    // http://www.cplusplus.com/reference/regex/ECMAScript/
    bool input_replace_regex_flag = false;
    std::regex *input_filter_regex; // The regex to "search" on list of file as input

    bool input_variable_filter_regex_flag = false;
    std::regex *input_variable_filter_regex; // The regex to "search" on list of file as input

    bool output_replace_regex_flag = false;
    std::regex *output_replace_regex; // The regex to "replace" on list of file as output (input)
    std::string output_replace_regex_aug;
    std::string output_replace_regex_match_str;

    bool has_ordering_on_file_list = false; // User may apply ordering on list, based on sorted ordring
    std::vector<size_t> order_on_file_list;

    bool is_list_dir_recursive = false;      // List file in a directory
    bool is_dir_file_list_recursive = false; // List dataset inside a file within a directory.
    bool skip_size_check = false;

    bool is_view_set = false;
    bool is_view_on_a_rank = false;
    std::vector<unsigned long long> set_view_start, set_view_count;
    int index_view_on_a_rank;

    bool is_ExtractMeta_called = false;
    // DIR_SET_INPUT_FILE_LIST
    bool is_set_input_file_list = false;
    std::vector<std::string> input_file_list;

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
        else if (sub_endpoint_type == EP_CSV)
        {
            sub_endpoint = new EndpointCSV();
        }
        else
        {
            AU_EXIT("Not supported sub endpoint in EndpointDIR \n");
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

    /**
     * @brief Set the Attribute object
     *   Do not need to be pure virtual method
     * @param name
     * @param data
     * @return int
     */
    int WriteAttribute(const std::string &name, const void *data, FTDataType data_type_p, const size_t &data_length_p = 0) override;

    /**
     * @brief Get the Attribute object
     *  Do not need to be pure virtual method
     * @param name
     * @param data
     * @return int
     */
    int ReadAttribute(const std::string &name, void *data, FTDataType data_type_p, const size_t &data_length_p = 0) override;

    int GetAttributeSize(const std::string &name, FTDataType data_type_p) override;

    /**
     * @brief Read all attribute name
     *
     * @param attri_name
     * @return int
     */
    int ReadAllAttributeName(std::vector<std::string> &attr_name);

    /**
     * @brief Set the View object
     *
     * @param view_par: is a string, ',' seperated, containing the parameter for the setview
     *              It has two types: 0 and 1, as the first value and then it has below pattern
     *              0,rank_to_set_view,view_start,view_count
     *              1,ranks,view_start[0],.. view_start[ranks], view_count[0],.. view_count[ranks]
     *              The type 0 is called by  SetView(unsigned long long start, unsigned long long count, int rank)
     *              The type 1 is called by  SetView(std::vector<unsigned long long> start, std::vector<unsigned long long> count)
     *
     * Note: this function now is only required by endpoint_dir to update its chunk size inside
     * @return int
     */
    int SetView(const std::string &view_par);
};
#endif
