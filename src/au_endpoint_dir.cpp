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

#include "au_endpoint_dir.h"

int EndpointDIR::ExtractMeta()
{
    dir_file_list = GetDirFileList(dir_str);

    if (dir_file_list.size() < 0)
        AU_EXIT("No file under directory");

    //std::vector<unsigned long long> endpoint_dim_size;
    //int endpoint_ranks;
    std::vector<unsigned long long> temp_endpoint_dim_size;
    for (int i = 0; i < dir_file_list.size(); i++)
    {
        sub_endpoint->SetEndpointInfo(dir_str + "/" + dir_file_list[i] + ":" + append_sub_endpoint_info);
        sub_endpoint->ExtractMeta();
        temp_endpoint_dim_size = sub_endpoint->GetDimensions();

        if (i == 0)
        {
            endpoint_dim_size = temp_endpoint_dim_size;
        }
        else
        {
            if (endpoint_dim_size != temp_endpoint_dim_size)
            {
                AU_EXIT("Data under directory must have same size");
            }
        }
    }

    //Set chunk size to be the size of a end_point
    dir_chunk_size.resize(endpoint_dim_size.size());
    dir_overlap_size.resize(endpoint_dim_size.size());
    endpoint_ranks = endpoint_dim_size.size();
    for (int i = 0; i < endpoint_ranks; i++)
    {
        dir_chunk_size[i] = endpoint_dim_size[i];
        dir_overlap_size[i] = 0;
        if (i == 0)
        {
            endpoint_dim_size[i] = endpoint_dim_size[i] * dir_file_list.size();
        }
        else
        {
            endpoint_dim_size[i] = endpoint_dim_size[i];
        }
    }

    return 0;
}

int EndpointDIR::Create()
{
    //sub_endpoint->SetEndpointInfo(dir_str + "/" + dir_file_list[0] + ":" + append_sub_endpoint_info);
    //sub_endpoint->SetDataElementType(data_element_type);
    //return sub_endpoint->Create();
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

    int sub_endpoint_index = 0;
    sub_endpoint_index = start[0] / dir_chunk_size[0];
    start[0] = 0;
    end[0] = dir_chunk_size[0] - 1;

    sub_endpoint->SetDataElementType(data_element_type);
    sub_endpoint->Open();
    sub_endpoint->Read(start, end, data);

    return sub_endpoint->Close();
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
    int sub_endpoint_index = 0;
    sub_endpoint_index = start[0] / dir_chunk_size[0];
    start[0] = 0;
    end[0] = dir_chunk_size[0] - 1;
    for (int i = 0; i < endpoint_ranks; i++)
        endpoint_dim_size[i] = dir_chunk_size[i];

    sub_endpoint->SetDataElementType(data_element_type);
    sub_endpoint->SetDimensions(endpoint_dim_size);
    sub_endpoint->SetEndpointInfo(dir_str + "/" + dir_file_list[sub_endpoint_index] + ":" + append_sub_endpoint_info);
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
        AU_EXIT("Invalued sub_endpoint_info");
    }

    if (!std::getline(ss, append_sub_endpoint_info, ':'))
    {
        AU_EXIT("Invalued sub_endpoint_info");
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