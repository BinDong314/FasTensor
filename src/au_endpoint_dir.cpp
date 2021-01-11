/**
 *
 * Email questions to {dbin}@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#include "au_endpoint_dir.h"
#include "au_array_view_access.h"

extern int au_mpi_size_global;
extern int au_mpi_rank_global;
extern int au_size;
extern int au_rank;

int EndpointDIR::ExtractMeta()
{
    std::vector<std::string> temp_dir_file_list = GetDirFileList(dir_str);
    //dir_file_list = GetDirFileList(dir_str);
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

    AU_VERBOSE("The [" + std::to_string(dir_file_list.size()) + "] files used by DIR (ordered): ", 0);
    //std::vector<unsigned long long> endpoint_dim_size;
    //int endpoint_ranks;
    std::vector<unsigned long long> temp_endpoint_dim_size;
    for (int i = 0; i < dir_file_list.size(); i++)
    {
        AU_VERBOSE(dir_file_list[i], 0);
        sub_endpoint->SetEndpointInfo(dir_str + "/" + dir_file_list[i] + ":" + append_sub_endpoint_info);
        sub_endpoint->ExtractMeta();
        temp_endpoint_dim_size = sub_endpoint->GetDimensions();

        if (i == 0)
        {
            endpoint_size = temp_endpoint_dim_size;
        }
        else
        {
            if (endpoint_size != temp_endpoint_dim_size)
            {
                AU_EXIT("Data under directory must have same size");
            }
        }
    }

    //Set chunk size to be the size of a end_point
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

    int data_rank = start.size();

    sub_endpoint->SetDataElementType(data_element_type);

    //get the size of data to read
    std::vector<unsigned long long> count(data_rank);
    COUNT_RANGES(start, end, count);

    int sub_endpoint_index = 0, sub_endpoint_index_end = 0;
    sub_endpoint_index = start[dir_data_merge_index] / dir_chunk_size[dir_data_merge_index];
    sub_endpoint_index_end = (end[dir_data_merge_index] + 1) / dir_chunk_size[dir_data_merge_index];

    std::vector<unsigned long long> start_sub_endpoint(start.begin(), start.end());
    std::vector<unsigned long long> end_sub_endpoint(end.begin(), end.end());

    start_sub_endpoint[dir_data_merge_index] = 0;
    end_sub_endpoint[dir_data_merge_index] = dir_chunk_size[dir_data_merge_index] - 1;

    //PrintVector("R: start : ", start);
    //PrintVector("R: end : ", end);
    //PrintVector("R: start_sub_endpoint : ", start_sub_endpoint);
    //PrintVector("R: end_sub_endpoint : ", end_sub_endpoint);

    size_t total_element;
    COUNT_CELLS(start_sub_endpoint, end_sub_endpoint, total_element);
    size_t sub_endpoint_element_size = sub_endpoint->GetDataElementTypeSize();

    void *data_temp = malloc(total_element * sub_endpoint_element_size);

    //Insert data_temp into data
    std::vector<unsigned long long> view_start(start_sub_endpoint.begin(), start_sub_endpoint.end());
    std::vector<unsigned long long> view_end(end_sub_endpoint.begin(), end_sub_endpoint.end());
    for (int i = sub_endpoint_index; i < sub_endpoint_index_end; i++)
    {
        sub_endpoint->SetEndpointInfo(dir_str + "/" + dir_file_list[i] + ":" + append_sub_endpoint_info);
        sub_endpoint->Open();
        sub_endpoint->Read(start_sub_endpoint, end_sub_endpoint, data_temp);
        sub_endpoint->Close();
        view_start[dir_data_merge_index] = (i - sub_endpoint_index) * (end_sub_endpoint[dir_data_merge_index] + 1);
        view_end[dir_data_merge_index] = (i - sub_endpoint_index + 1) * (end_sub_endpoint[dir_data_merge_index] + 1) - 1;

        //PrintVector("R: count : ", count);
        //PrintVector("R: view_start : ", view_start);
        //PrintVector("R: view_end : ", view_end);

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
    PrintVector("EndpointDIR::Write start :", start);
    PrintVector("EndpointDIR::Write end :", end);
    std::cout << " dir_data_merge_index = " << dir_data_merge_index << " \n";
    int sub_endpoint_index = 0;
    sub_endpoint_index = start[dir_data_merge_index] / dir_chunk_size[dir_data_merge_index];
    //start[dir_data_merge_index] = 0;
    //end[dir_data_merge_index] = dir_chunk_size[dir_data_merge_index] - 1;
    for (int i = 0; i < endpoint_ranks; i++)
    {
        endpoint_size[i] = end[i] - start[i] + 1;
        start[i] = 0;
        end[i] = endpoint_size[i] - 1;
    }
    PrintVector("EndpointDIR::Write endpoint_size:", endpoint_size);

    std::cout << "call write :  " << dir_str + "/" + dir_file_list[sub_endpoint_index] + ": " + append_sub_endpoint_info << " \n";

    PrintVector("EndpointDIR::Write start (after):", start);
    PrintVector("EndpointDIR::Write end (after) :", end);

    std::string new_file_name_after_regex;
    //regex re("^(.*)\\.tdms$");

    if (output_replace_regex_flag)
    {
        new_file_name_after_regex = std::regex_replace(dir_file_list[sub_endpoint_index], *output_replace_regex, output_replace_regex_aug);
    }
    else
    {
        new_file_name_after_regex = dir_file_list[sub_endpoint_index];
    }

    std::cout << "call write :  " << dir_str + "/" + new_file_name_after_regex + ": " + append_sub_endpoint_info << " \n";

    sub_endpoint->SetDataElementType(data_element_type);
    sub_endpoint->SetDimensions(endpoint_size);
    //sub_endpoint->SetEndpointInfo(dir_str + "/" + dir_file_list[sub_endpoint_index] + ":" + append_sub_endpoint_info);
    sub_endpoint->SetEndpointInfo(dir_str + "/" + new_file_name_after_regex + ":" + append_sub_endpoint_info);
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
        if (sub_endpoint != nullptr)
            sub_cmd = sub_endpoint->MapOpStr2Int(parameter_v[0]);

        if (parameter_v.size() > 1)
        {
            sub_cmd_arg.push_back(parameter_v[1]);
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
        std::cout << "regex string: " << parameter_v[0] << "\n";
        input_replace_regex_flag = true;
        input_filter_regex = new std::regex(parameter_v[0]);
        break;
    case DIR_OUPUT_REPLACE_RGX:
        if (parameter_v.size() < 2)
        {
            AU_EXIT("DIR_OUPUT_REPLACE_RGX  needs two parameters: regex pattern and replace string pattern \n");
        }
        output_replace_regex_flag = true;
        output_replace_regex = new std::regex(parameter_v[0]);
        output_replace_regex_aug = parameter_v[1];
        break;
    case DIR_FILE_SORT_INDEXES:
        has_ordering_on_file_list = true;
        if (parameter_v.size() < 1)
        {
            AU_EXIT("DIR_OUPUT_REPLACE_RGX  needs one parameters: order vector (as string) \n");
        }
        String2Vector(parameter_v[0], order_on_file_list);
        break;
        //  case DIR_OUTPUT_SPLIT_FILE:
        //      break;
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