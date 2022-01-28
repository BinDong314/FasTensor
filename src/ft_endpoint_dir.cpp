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

#include "ft_endpoint_dir.h"
#include "ft_array_view_access.h"

extern int ft_mpi_size_global;
extern int ft_mpi_rank_global;
extern int ft_size;
extern int ft_rank;

int EndpointDIR::ExtractMeta()
{
    if (is_ExtractMeta_called)
        return 0;
    std::vector<std::string> temp_dir_file_list;
    if (!is_list_dir_recursive)
    {
        // This only get the file name
        temp_dir_file_list = GetDirFileList(dir_str);
        // To be the same as GetDirFileListRecursive
        // add directory path to the file list
        for (int i = 0; i < temp_dir_file_list.size(); i++)
        {
            temp_dir_file_list[i] = dir_str + "/" + temp_dir_file_list[i];
        }
    }
    else
    {
        // This get the full path name
        temp_dir_file_list = GetDirFileListRecursive(dir_str);
    }

    // dir_file_list = GetDirFileList(dir_str);
    if (temp_dir_file_list.size() <= 0)
        AU_EXIT("No file under directory" + dir_str);

    if (has_ordering_on_file_list)
    {
        std::vector<std::string> temp_dir_file_list_ordered;
        for (auto order : order_on_file_list)
        {
            if (order > temp_dir_file_list.size())
            {
                AU_EXIT("The sorted index [" + std::to_string(order) + "] is larger than the number of files listed [" + std::to_string(temp_dir_file_list.size()) + "] !\n");
            }
            temp_dir_file_list_ordered.push_back(temp_dir_file_list[order]);
        }
        temp_dir_file_list = temp_dir_file_list_ordered;
    }

    if (input_replace_regex_flag)
    {
        dir_file_list.clear();
        for (int i = 0; i < temp_dir_file_list.size(); i++)
        {
            if (std::regex_match(temp_dir_file_list[i], *input_filter_regex))
            {
                dir_file_list.push_back(temp_dir_file_list[i]);
            }
        }

        if (dir_file_list.size() <= 0)
            AU_EXIT("No file pass regex_match, reconsider your regex string? \n");
    }
    else
    {
        dir_file_list = temp_dir_file_list;
    }

    // std::vector<unsigned long long> endpoint_dim_size;
    // int endpoint_ranks;
    std::vector<unsigned long long> temp_endpoint_dim_size;
    std::vector<std::string> dir_file_list_for_file_list_recursive;
    std::vector<std::string> sub_endpoint_list_for_file_list_recursive, sub_endpoint_list_for_file_list_recursive_temp;

    for (int i = 0; i < dir_file_list.size(); i++)
    {
        // if (!is_list_dir_recursive)
        //{

        // sub_endpoint->SetEndpointInfo(dir_str + "/" + dir_file_list[i] + ":" + append_sub_endpoint_info);
        //   dir_file_list[i] = dir_str + "/" + dir_file_list[i] + ":" + append_sub_endpoint_info;
        //   std::cout << "dir_str =" << dir_str << ", dir_file_list[i] = " << dir_file_list[i] << std::endl;
        //   sub_endpoint->SetEndpointInfo(dir_file_list[i]);
        //}
        // else
        //{
        if (!is_dir_file_list_recursive)
        {
            sub_endpoint->SetEndpointInfo(dir_file_list[i] + ":" + append_sub_endpoint_info);
        }
        else
        {
            sub_endpoint->SetEndpointInfo(dir_file_list[i]);
            sub_endpoint_list_for_file_list_recursive_temp.clear();
            sub_endpoint->Control(HDF5_LIST_RECURSIVE, sub_endpoint_list_for_file_list_recursive_temp);

            // input_variable_filter_regex_flag = true;
            // input_variable_filter_regex
            if (input_variable_filter_regex_flag)
            {
                sub_endpoint_list_for_file_list_recursive.clear();
                for (int i = 0; i < sub_endpoint_list_for_file_list_recursive_temp.size(); i++)
                {
                    // std::cout << "Before: " << sub_endpoint_list_for_file_list_recursive_temp[i] << "\n";
                    if (std::regex_match(sub_endpoint_list_for_file_list_recursive_temp[i], *input_variable_filter_regex))
                    {
                        // std::cout << "After: " << sub_endpoint_list_for_file_list_recursive_temp[i] << "\n";
                        sub_endpoint_list_for_file_list_recursive.push_back(sub_endpoint_list_for_file_list_recursive_temp[i]);
                    }
                }
            }
            else
            {
                sub_endpoint_list_for_file_list_recursive = sub_endpoint_list_for_file_list_recursive_temp;
            }

            for (int j = 0; j < sub_endpoint_list_for_file_list_recursive.size(); j++)
            {
                // std::cout << dir_file_list[i] + ":" + sub_endpoint_list_for_file_list_recursive[j] << "  \n";
                dir_file_list_for_file_list_recursive.push_back(dir_file_list[i] + ":/" + sub_endpoint_list_for_file_list_recursive[j]);
                // std::cout << dir_file_list_for_file_list_recursive[0];
            }

            if (dir_file_list_for_file_list_recursive.size() >= 1)
            {
                sub_endpoint->SetEndpointInfo(dir_file_list_for_file_list_recursive[0]);
            }
            else
            {
                return -1;
            }
        }
        //}

        // PrintVector("Before temp_endpoint_dim_size =", temp_endpoint_dim_size);

        sub_endpoint->ExtractMeta();
        temp_endpoint_dim_size = sub_endpoint->GetDimensions();

        // PrintVector("temp_endpoint_dim_size =", temp_endpoint_dim_size);

        if (i == 0)
        {
            endpoint_size = temp_endpoint_dim_size;
            if (skip_size_check)
                break;
        }
        else
        {
            if (endpoint_size != temp_endpoint_dim_size)
            {
                AU_EXIT("Data under directory must have same size: " + dir_str + "/" + dir_file_list[i] + ":" + append_sub_endpoint_info);
            }
            else if (i == (dir_file_list.size() - 1))
            {
                AU_INFO("All files have the same size ! \n");
            }
        }
    }

    if (skip_size_check)
    {
        for (int i = 1; i < dir_file_list.size(); i++)
        {
            if (!is_dir_file_list_recursive)
            {
                dir_file_list[i] = dir_str + "/" + dir_file_list[i] + ":" + append_sub_endpoint_info;
            }
            else
            {
                for (int j = 0; j < sub_endpoint_list_for_file_list_recursive.size(); j++)
                {
                    // std::cout << sub_endpoint_list_for_file_list_recursive[j] << " \n";
                    dir_file_list_for_file_list_recursive.push_back(dir_file_list[i] + ":" + sub_endpoint_list_for_file_list_recursive[j]);
                }
            }
        }
    }

    if (is_dir_file_list_recursive)
    {
        // std::cout << "dir_file_list_for_file_list_recursive.size = " << dir_file_list_for_file_list_recursive.size() << "\n";
        dir_file_list.clear();
        dir_file_list = dir_file_list_for_file_list_recursive;
    }

    if (dir_file_list.size() == 0)
    {
        AU_EXIT("No files found by DIR !\n");
    }

    AU_VERBOSE("The [" + std::to_string(dir_file_list.size()) + "] files used by DIR (ordered): ", 0);
    if (dir_file_list.size() <= 10)
    {
        for (int i = 0; i < dir_file_list.size(); i++)
        {
            AU_VERBOSE(dir_file_list[i], 0);
        }
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            AU_VERBOSE(dir_file_list[i], 0);
        }
        AU_VERBOSE("...", 0);
        AU_VERBOSE("...", 0);
        for (int i = dir_file_list.size() - 5; i < dir_file_list.size(); i++)
        {
            AU_VERBOSE(dir_file_list[i], 0);
        }
    }
//#define FT_DEBUG 1
#ifdef FT_DEBUG
    for (int i = 0; i < dir_file_list.size(); i++)
    {
        AU_VERBOSE(dir_file_list[i], 0);
    }
    PrintVector("endpoint_size =", endpoint_size);
#endif

    // Set chunk size to be the size of a end_point
    dir_chunk_size.resize(endpoint_size.size());
    dir_overlap_size.resize(endpoint_size.size());
    endpoint_ranks = endpoint_size.size();
    for (int i = 0; i < endpoint_ranks; i++)
    {
        dir_chunk_size[i] = endpoint_size[i];
        dir_overlap_size[i] = 0;
        if (i == dir_data_merge_index)
        {
            endpoint_size[i] = endpoint_size[i] * dir_file_list.size();
        }
        else
        {
            endpoint_size[i] = endpoint_size[i];
        }
    }

    dir_file_size = dir_chunk_size; // The dir_chunk_size may be updated latter to have small subset
    if (is_view_set)
    {
#ifdef FT_DEBUG
        std::cout << "EndpointDIR::ExtractMeta index_view_on_a_rank = " << index_view_on_a_rank << "\n";
        std::cout << "EndpointDIR::ExtractMeta set_view_count[0] = " << set_view_count[0] << "\n";
#endif
        if (is_view_on_a_rank)
        {
            dir_chunk_size[index_view_on_a_rank] = set_view_count[0];
        }
        else
        {
            assert(set_view_count.size() == endpoint_ranks);
            for (int i = 0; i < endpoint_ranks; i++)
                dir_chunk_size[i] = set_view_count[i];
        }
#ifdef FT_DEBUG
        PrintVector("EndpointDIR::ExtractMeta Final dir_chunk_size = ", dir_chunk_size);
#endif
    }

    is_ExtractMeta_called = true;
    return 0;
}

int EndpointDIR::Create()
{
    // sub_endpoint->SetEndpointInfo(dir_str + "/" + dir_file_list[0] + ":" + append_sub_endpoint_info);
    // sub_endpoint->SetDataElementType(data_element_type);
    // return sub_endpoint->Create();
    return 0;
}

int EndpointDIR::Open()
{
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
int EndpointDIR::Read(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{

    int data_rank = start.size();

    sub_endpoint->SetDataElementType(data_element_type);

    // get the size of data to read
    std::vector<unsigned long long> count(data_rank);
    COUNT_RANGES(start, end, count);

    int sub_endpoint_index = 0, sub_endpoint_index_end = 0;
    sub_endpoint_index = start[dir_data_merge_index] / dir_chunk_size[dir_data_merge_index];
    sub_endpoint_index_end = (end[dir_data_merge_index] + 1) / dir_chunk_size[dir_data_merge_index];

    // save for metadata operation
    //  TODO: we only consider the first file here and try to consider more file here
    dir_file_list_current_index = sub_endpoint_index;

    std::vector<unsigned long long> start_sub_endpoint(start.begin(), start.end());
    std::vector<unsigned long long> end_sub_endpoint(end.begin(), end.end());

    start_sub_endpoint[dir_data_merge_index] = 0;
    end_sub_endpoint[dir_data_merge_index] = dir_chunk_size[dir_data_merge_index] - 1;

    size_t total_element;
    COUNT_CELLS(start_sub_endpoint, end_sub_endpoint, total_element);
    size_t sub_endpoint_element_size = sub_endpoint->GetDataElementTypeSize();

    // PrintVector("R: start : ", start);
    // PrintVector("R: end : ", end);
    // PrintVector("R: start_sub_endpoint : ", start_sub_endpoint);
    // PrintVector("R: end_sub_endpoint : ", end_sub_endpoint);
    // PrintScalar("R: total_element :", total_element);

    void *data_temp = malloc(total_element * sub_endpoint_element_size);

    // Insert data_temp into data
    std::vector<unsigned long long> view_start(start_sub_endpoint.begin(), start_sub_endpoint.end());
    std::vector<unsigned long long> view_end(end_sub_endpoint.begin(), end_sub_endpoint.end());
    if (is_view_set && is_view_on_a_rank)
    {
        view_end[index_view_on_a_rank] = view_end[index_view_on_a_rank] - view_start[index_view_on_a_rank];
        view_start[index_view_on_a_rank] = 0;
    }

    for (int i = sub_endpoint_index; i < sub_endpoint_index_end; i++)
    {
        // sub_endpoint->SetEndpointInfo(dir_str + "/" + dir_file_list[i] + ":" + append_sub_endpoint_info);
        // std::cout << "Read = dir_file_list[i] = " << dir_file_list[i] << "\n";
        sub_endpoint->SetEndpointInfo(dir_file_list[i] + ":" + append_sub_endpoint_info);
        sub_endpoint->Open();
        sub_endpoint->Read(start_sub_endpoint, end_sub_endpoint, data_temp);
        sub_endpoint->Close();
        view_start[dir_data_merge_index] = (i - sub_endpoint_index) * (end_sub_endpoint[dir_data_merge_index] + 1);
        view_end[dir_data_merge_index] = (i - sub_endpoint_index + 1) * (end_sub_endpoint[dir_data_merge_index] + 1) - 1;

        // PrintVector("R: count : ", count);
        // PrintVector("R: view_start : ", view_start);
        // PrintVector("R: view_end : ", view_end);

        switch (data_element_type)
        {
        case AU_SHORT:
            ArrayViewAccessP<short>((short *)data_temp, (short *)data, count, view_start, view_end, ARRAY_VIEW_WRITE);
            break;
        case AU_INT:
            ArrayViewAccessP<int>((int *)data_temp, (int *)data, count, view_start, view_end, ARRAY_VIEW_WRITE);
            break;
        case AU_LONG:
            ArrayViewAccessP<long>((long *)data_temp, (long *)data, count, view_start, view_end, ARRAY_VIEW_WRITE);
            break;
        case AU_LONG_LONG:
            ArrayViewAccessP<long long>((long long *)data_temp, (long long *)data, count, view_start, view_end, ARRAY_VIEW_WRITE);
            break;
        case AU_USHORT:
            ArrayViewAccessP<unsigned short>((unsigned short *)data_temp, (unsigned short *)data, count, view_start, view_end, ARRAY_VIEW_WRITE);
            break;
        case AU_UINT:
            ArrayViewAccessP<unsigned int>((unsigned int *)data_temp, (unsigned int *)data, count, view_start, view_end, ARRAY_VIEW_WRITE);
            break;
        case AU_ULONG:
            ArrayViewAccessP<unsigned long>((unsigned long *)data_temp, (unsigned long *)data, count, view_start, view_end, ARRAY_VIEW_WRITE);
            break;
        case AU_ULLONG:
            ArrayViewAccessP<unsigned long long>((unsigned long long *)data_temp, (unsigned long long *)data, count, view_start, view_end, ARRAY_VIEW_WRITE);
            break;
        case AU_FLOAT:
            ArrayViewAccessP<float>((float *)data_temp, (float *)data, count, view_start, view_end, ARRAY_VIEW_WRITE);
            break;
        case AU_DOUBLE:
            ArrayViewAccessP<double>((double *)data_temp, (double *)data, count, view_start, view_end, ARRAY_VIEW_WRITE);
            break;
        default:
            std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
            std::flush(std::cout);
            std::exit(EXIT_FAILURE);
        }
    }

    free(data_temp);
    return 0;
}

/**
 * @brief write the data to the end-point
 *
 * @param start, coordinates of the cell to start (including)
 * @param end , coordinates of the cell to end (including)
 * @param data, store the result data
 * @return int < 0 error, >= 0 works
 */
int EndpointDIR::Write(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{

    // start[dir_data_merge_index] = 0;
    // end[dir_data_merge_index] = dir_chunk_size[dir_data_merge_index] - 1;
    //#define DEBUG 0
#ifdef DEBUG
    PrintVector("EndpointDIR::dir_chunk_size before update :", dir_chunk_size);
#endif
    for (int i = 0; i < endpoint_ranks; i++)
    {
        endpoint_size[i] = end[i] - start[i] + 1;
        if (endpoint_size[i] < dir_chunk_size[i])
        {
            dir_chunk_size[i] = endpoint_size[i];
        }
    }

    unsigned long long vector_con = 1;
    if (IsVectorEq(endpoint_size, vector_con))
    {
        return 0;
    }

#ifdef FT_DEBUG
    PrintVector("EndpointDIR::Write start :", start);
    PrintVector("EndpointDIR::Write end :", end);
    PrintVector("EndpointDIR::dir_chunk_size after update :", dir_chunk_size);
    std::cout << " dir_data_merge_index = " << dir_data_merge_index << " \n";
#endif
    int sub_endpoint_index = 0;
    sub_endpoint_index = start[dir_data_merge_index] / dir_chunk_size[dir_data_merge_index];
    for (int i = 0; i < endpoint_ranks; i++)
    {
        endpoint_size[i] = end[i] - start[i] + 1;
        start[i] = 0;
        end[i] = endpoint_size[i] - 1;
    }

    // save for metadata operation
    dir_file_list_current_index = sub_endpoint_index;

    std::stringstream ss(dir_file_list[sub_endpoint_index]);
    std::string input_path, input_file_str, input_append_sub_endpoint_info;
    // std::cout << "Write: dir_file_list[sub_endpoint_index] =" << dir_file_list[sub_endpoint_index] << "\n";
    if (!std::getline(ss, input_path, ':'))
    {
        AU_EXIT("Wrong sub_endpoint_info");
    }
    if (!std::getline(ss, input_append_sub_endpoint_info, ':'))
    {
        input_append_sub_endpoint_info = append_sub_endpoint_info;
        // std::cout << "input_path = " << input_path << ", input_file_str = " << input_file_str << ", input_append_sub_endpoint_info = " << input_append_sub_endpoint_info << " \n";
        //  AU_EXIT("Wrong sub_endpoint_info"); //Coment out to use the "append_sub_endpoint_info" if "input_path" does not has the appendent info
    }
    input_file_str = ExtractFileName(input_path);

#ifdef FT_DEBUG
    PrintVector("EndpointDIR::Write endpoint_size:", endpoint_size);
    std::cout << "call write (before) :  " << dir_str + "/" + dir_file_list[sub_endpoint_index] + ": " + append_sub_endpoint_info << ", sub_endpoint_index = " << sub_endpoint_index << " \n";
    std::cout << "input_path = " << input_path << ", input_file_str = " << input_file_str << ", input_append_sub_endpoint_info = " << input_append_sub_endpoint_info << " \n";
    PrintVector("EndpointDIR::Write start (after):", start);
    PrintVector("EndpointDIR::Write end (after) :", end);

#endif

    std::string new_file_name_after_regex;
    std::regex re_test(output_replace_regex_match_str, std::regex_constants::ECMAScript);

    // output_replace_regex_match_str
    // std::regex re_test("^(.*)\\.h5$");

    /*if (output_replace_regex_match_str == "^(.*)\\.h5$")
    {
        std::cout << "Yes, they are equal ! \n";
    }
    else
    {
        std::cout << "No, they are not equal ! "
                  << " output_replace_regex_match_str = " << output_replace_regex_match_str << ", "
                  << "^(.*)\\.h5$" << std::endl;
    }*/

    if (output_replace_regex_flag)
    {
        // new_file_name_after_regex = std::regex_replace(dir_file_list[sub_endpoint_index], *output_replace_regex, output_replace_regex_aug);
        // new_file_name_after_regex = std::regex_replace(dir_file_list[sub_endpoint_index], re_test, output_replace_regex_aug);
        new_file_name_after_regex = std::regex_replace(input_file_str, re_test, output_replace_regex_aug);

#ifdef FT_DEBUG
        std::cout << " output_replace_regex_aug = " << output_replace_regex_aug << ", new_file_name_after_regex = " << new_file_name_after_regex << ", dir_file_list[sub_endpoint_index] =" << dir_file_list[sub_endpoint_index] << ", output_replace_regex_match_str =" << output_replace_regex_match_str << "\n";
#endif
    }
    else
    {
        // new_file_name_after_regex = dir_file_list[sub_endpoint_index];
        new_file_name_after_regex = input_file_str;
    }
#ifdef FT_DEBUG
    std::cout << "call write (after) :  " << dir_str + "/" + new_file_name_after_regex + ": " + input_append_sub_endpoint_info << " \n";
#endif
    sub_endpoint->SetDataElementType(data_element_type);
    sub_endpoint->SetDimensions(endpoint_size);
    // sub_endpoint->SetEndpointInfo(dir_str + "/" + dir_file_list[sub_endpoint_index] + ":" + append_sub_endpoint_info);
    // sub_endpoint->SetEndpointInfo(dir_str + "/" + new_file_name_after_regex + ":" + append_sub_endpoint_info);
    sub_endpoint->SetEndpointInfo(dir_str + "/" + new_file_name_after_regex + ":" + input_append_sub_endpoint_info);
    sub_endpoint->Create();
    sub_endpoint->Write(start, end, data);
    return sub_endpoint->Close();
}

/**
 * @brief close the end-point
 *
 * @return int int < 0 error, >= 0 works
 */
int EndpointDIR::Close()
{
    return 0;
};

void EndpointDIR::EnableCollectiveIO()
{
}

void EndpointDIR::DisableCollectiveIO()
{
}

int EndpointDIR::PrintInfo()
{
    std::cout << data_endpoint_orig << std::endl;
    return 0;
}

void EndpointDIR::Map2MyType()
{
}

int EndpointDIR::ParseEndpointInfo()
{
    ExtractEndpointTypeInfo(endpoint_info, sub_endpoint_type, sub_endpoint_info);

    std::stringstream ss(sub_endpoint_info);
    if (!std::getline(ss, dir_str, ':'))
    {
        AU_EXIT("Wrong sub_endpoint_info");
    }

    if (sub_endpoint_type == EP_HDF5)
    {
        if (!std::getline(ss, append_sub_endpoint_info, ':'))
        {
            return 0;
            AU_EXIT("Wrong sub_endpoint_info");
        }
    }

    return 0;
}

/**
 * @brief Get the Chunk Size object
 *
 * @return std::vector<int>
 */
std::vector<int> EndpointDIR::GetDirChunkSize()
{
    return dir_chunk_size;
}

void EndpointDIR::SetDirChunkSize(std::vector<int> &dir_chunk_size_p)
{
    dir_chunk_size = dir_chunk_size_p;
}

std::vector<std::string> EndpointDIR::GetDirFileVector()
{
    return dir_file_list;
}

void EndpointDIR::SetDirFileVector(std::vector<std::string> &file_list)
{
    dir_file_list = file_list;
}

/*
int EndpointDIR::SpecialOperator(int opt_code, std::string parameter)
{
    int sub_cmd;
    std::string sub_cmd_arg, temp_str;
    std::stringstream ss(parameter);

    switch (opt_code)
    {
    case DIR_MERGE_INDEX:
        SetMergeIndex(std::stoi(parameter));
        break;
    case DIR_SUB_CMD_ARG:
        if (!std::getline(ss, temp_str, ':'))
        {
            AU_EXIT("Invalued sub_endpoint_info");
        }
        if (sub_endpoint != nullptr)
            sub_cmd = sub_endpoint->MapOpStr2Int(temp_str);

        if (!std::getline(ss, sub_cmd_arg, ':'))
        {
            sub_cmd_arg = "";
        }

        if (sub_endpoint != nullptr)
            sub_endpoint->SpecialOperator(sub_cmd, sub_cmd_arg);
        break;
    case DIR_INPUT_SEARCH_RGX:
        input_replace_regex_flag = true;
        input_filter_regex = new std::regex(parameter);
        break;
    case DIR_OUPUT_REPLACE_RGX:
        output_replace_regex_flag = true;
        output_replace_regex = new std::regex(parameter);
        break;
    case DIR_OUPUT_REPLACE_RGX_ARG:
        output_replace_regex_aug = parameter;
        break;
    default:
        break;
    }
    return 0;
}*/

int EndpointDIR::Control(int opt_code, std::vector<std::string> &parameter_v)
{
    int sub_cmd;
    std::vector<std::string> sub_cmd_arg;
    switch (opt_code)
    {
    case DIR_MERGE_INDEX:
        if (parameter_v.size() < 1)
        {
            AU_EXIT("DIR_MERGE_INDEX  needs 1 parameter: index of dimension to merge \n");
        }
        SetMergeIndex(std::stoi(parameter_v[0]));
        break;
    case DIR_SUB_CMD_ARG:
        if (parameter_v.size() < 1)
        {
            AU_EXIT("DIR_SUB_CMD_ARG  needs at least 1 parameter: sub command code \n");
        }
        // if (sub_endpoint != nullptr)
        //     sub_cmd = sub_endpoint->MapOpStr2Int(parameter_v[0]);
        sub_cmd = std::stoi(parameter_v[0]);
        if (parameter_v.size() > 1)
        {
            // sub_cmd_arg.push_back(parameter_v[1]);
            sub_cmd_arg.insert(sub_cmd_arg.begin(), parameter_v.begin() + 1, parameter_v.end());
        }
        else
        {
            sub_cmd_arg.push_back("");
        }

        if (sub_endpoint != nullptr)
            sub_endpoint->Control(sub_cmd, sub_cmd_arg);
        break;
    case DIR_INPUT_SEARCH_RGX:
        if (parameter_v.size() < 1)
        {
            AU_EXIT("DIR_INPUT_SEARCH_RGX  needs 1 parameters: regex pattern and replace string pattern \n");
        }
        // std::cout << "regex string: " << parameter_v[0] << "\n";
        AU_VERBOSE("regex string: " + parameter_v[0], 0)
        input_replace_regex_flag = true;
        input_filter_regex = new std::regex(parameter_v[0]);
        break;
    case DIR_OUPUT_REPLACE_RGX:
        if (parameter_v.size() < 2)
        {
            AU_EXIT("DIR_OUPUT_REPLACE_RGX  needs two parameters: regex pattern and replace string pattern \n");
        }
#ifdef FT_DEBUG
        std::cout << " output_replace_regex = " << parameter_v[0] << "\n";
#endif
        output_replace_regex_flag = true;
        output_replace_regex = new std::regex(parameter_v[0]);
        output_replace_regex_match_str = parameter_v[0];
        output_replace_regex_aug = parameter_v[1];
        break;
    case DIR_FILE_SORT_INDEXES:
        has_ordering_on_file_list = true;
        if (parameter_v.size() < 1)
        {
            AU_EXIT("DIR_FILE_SORT_INDEXES needs one parameters: order vector (as string) !! \n");
        }
        String2Vector(parameter_v[0], order_on_file_list);
        break;
    case DIR_N_FILES:
        ExtractMeta();
        parameter_v.clear();
        parameter_v.push_back(std::to_string(dir_file_list.size()));
        break;
    case DIR_GET_FILE_SIZE:
        ExtractMeta();
        // std::string file_size_str = ;
        parameter_v.clear();
        parameter_v.push_back(Vector2String(dir_file_size));
        break;
    case DIR_SET_CHUNK_SIZE:
        String2Vector(parameter_v[0], dir_chunk_size);
        break;
    case OP_LIST_TAG:
        ReadAllAttributeName(parameter_v);
        break;
    case DIR_LIST_RECURSIVE:
        is_list_dir_recursive = true;
        break;
    case DIR_FILE_LIST_RECURSIVE:
        is_dir_file_list_recursive = true;
        break;
    case DIR_SKIP_SIZE_CHECK:
        skip_size_check = true;
        break;
    case DIR_INPUT_VARIABLE_SEARCH_RGX:
        if (parameter_v.size() < 1)
        {
            AU_EXIT("DIR_INPUT_VARIABLE_SEARCH_RGX  needs 1 parameters: regex pattern and replace string pattern \n");
        }
        AU_VERBOSE("regex string: " + parameter_v[0], 0)
        input_variable_filter_regex_flag = true;
        input_variable_filter_regex = new std::regex(parameter_v[0]);
        break;
    case DIR_SET_VIEW:
        SetView(parameter_v[0]);
        break;
    default:
        break;
    }
    return 0;
}

/**
 * @brief Set the Merge Index
 *
 * @param index_p
 */
void EndpointDIR::SetMergeIndex(int index_p)
{
    dir_data_merge_index = index_p;
}

/**
 * @brief Get the Merge Index object
 *
 * @return int
 */
int EndpointDIR::GetMergeIndex()
{
    return dir_data_merge_index;
}

/**
 * @brief Read all attribute name
 *
 * @param attri_name
 * @return int
 */
int EndpointDIR::ReadAllAttributeName(std::vector<std::string> &attr_name)
{
    // sub_endpoint->SetEndpointInfo(dir_str + "/" + dir_file_list[dir_file_list_current_index] + ":" + append_sub_endpoint_info);
    sub_endpoint->SetEndpointInfo(dir_file_list[dir_file_list_current_index] + ":" + append_sub_endpoint_info);

    sub_endpoint->Open();
    // sub_endpoint->Read(start_sub_endpoint, end_sub_endpoint, data_temp);
    sub_endpoint->Control(OP_LIST_TAG, attr_name);
    sub_endpoint->Close();

    return 0;
}

/**
 * @brief Set the Attribute object
 *   Do not need to be pure virtual method
 * @param name
 * @param data
 * @return int
 */
int EndpointDIR::WriteAttribute(const std::string &name, const void *data, FTDataType data_type_p, const size_t &data_length_p)
{
    std::string new_file_name_after_regex;
    // regex re("^(.*)\\.tdms$");

    std::string input_file_str = ExtractFileName(dir_file_list[dir_file_list_current_index]);

    if (output_replace_regex_flag)
    {
        new_file_name_after_regex = std::regex_replace(input_file_str, *output_replace_regex, output_replace_regex_aug);
    }
    else
    {
        new_file_name_after_regex = input_file_str;
    }

#ifdef FT_DEBUG
    std::cout << " write attribute: " << dir_str + "/" + new_file_name_after_regex + ":" + append_sub_endpoint_info << " \n";
#endif
    sub_endpoint->SetEndpointInfo(dir_str + "/" + new_file_name_after_regex + ":" + append_sub_endpoint_info);
    // sub_endpoint->SetEndpointInfo(new_file_name_after_regex + ":" + append_sub_endpoint_info);
    sub_endpoint->Open();
    // sub_endpoint->Read(start_sub_endpoint, end_sub_endpoint, data_temp);
    sub_endpoint->WriteAttribute(name, data, data_type_p, data_length_p);
    sub_endpoint->Close();

    return 0;
}

/**
 * @brief Get the Attribute object
 *  Do not need to be pure virtual method
 * @param name
 * @param data
 * @return int
 */
int EndpointDIR::ReadAttribute(const std::string &name, void *data, FTDataType data_type_p, const size_t &data_length_p)
{
    int ret;
    // sub_endpoint->SetEndpointInfo(dir_str + "/" + dir_file_list[dir_file_list_current_index] + ":" + append_sub_endpoint_info);
    // std::cout << "ReadAttribute: " << dir_file_list[dir_file_list_current_index] + ":" + append_sub_endpoint_info << "\n";
    sub_endpoint->SetEndpointInfo(dir_file_list[dir_file_list_current_index] + ":" + append_sub_endpoint_info);
    sub_endpoint->Open();
    // sub_endpoint->Read(start_sub_endpoint, end_sub_endpoint, data_temp);
    ret = sub_endpoint->ReadAttribute(name, data, data_type_p, data_length_p);
    sub_endpoint->Close();

    return ret;
}

int EndpointDIR::GetAttributeSize(const std::string &name, FTDataType data_type_p)
{
    // std::cout << "ReadAttribute: " << dir_str + "/" + dir_file_list[dir_file_list_current_index] + ":" + append_sub_endpoint_info << "\n";

    sub_endpoint->SetEndpointInfo(dir_file_list[dir_file_list_current_index] + ":" + append_sub_endpoint_info);
    sub_endpoint->Open();
    int a_size = sub_endpoint->GetAttributeSize(name, data_type_p);
    // sub_endpoint->Read(start_sub_endpoint, end_sub_endpoint, data_temp);
    sub_endpoint->Close();

    return a_size;
}

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
int EndpointDIR::SetView(const std::string &view_par)
{
    std::vector<unsigned long long> view_par_vector;
    // std::cout << "EndpointDIR::SetView view_par =" << view_par << "\n";

    String2Vector(view_par, view_par_vector);
    assert(view_par_vector.size() >= 4);
    is_view_set = true;

    if (view_par_vector[0] == 0)
    {
        is_view_on_a_rank = true;
        // view_start, view_end;
        index_view_on_a_rank = view_par_vector[1];
        set_view_start.resize(1);
        set_view_start[0] = view_par_vector[2];
        set_view_count.resize(1);
        set_view_count[0] = view_par_vector[3];
    }
    else if (view_par_vector[0] == 1)
    {
        set_view_start.resize(view_par_vector[1]);
        set_view_count.resize(view_par_vector[1]);
        for (int i = 0; i < view_par_vector[1]; i++)
        {
            set_view_start[i] = view_par_vector[2 + i];
            set_view_count[i] = view_par_vector[2 + i + view_par_vector[1]];
        }
    }
    else
    {
        AU_EXIT("Not supported typed in via_par!");
    }

    return 0;
}