

/**
 *
 * Email questions to dbin@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#include "au_endpoint_binary.h"

int EndpointBinary::ExtractMeta()
{
    return 0;
}

int EndpointBinary::Create()
{

    return 0;
}

int EndpointBinary::Open()
{
    std::cout << "call open in binary, fn_str = " << fn_str << ", model_str= " << model_str << "  \n";
    PrintInfo();
    //Opne input file
    fp = fopen(fn_str.c_str(), model_str.c_str());
    if (fp == NULL)
    {
        AU_EXIT("Can not open file" + fn_str);
    }
    //fseek(fp, seek_offset, SEEK_SET);
    SetOpenFlag(true);

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
int EndpointBinary::Read(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{
    int ret;
    if (GetOpenFlag() == false)
    {
        Open();
    }

    Map2MyType();

    size_t count = 1;
    int data_rank = start.size();
    for (int i = 0; i < data_rank; i++)
    {
        count = count * (end[i] - start[i] + 1);
    }

    std::cout << "rw_type_size = " << rw_type_size << " , count = " << count << "\n";

    ret = fread(data, rw_type_size, count, fp);

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
int EndpointBinary::Write(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{
    int ret;

    size_t count = 1;
    int data_rank = start.size();
    for (int i = 0; i < data_rank; i++)
    {
        count = count * (end[i] - start[i] + 1);
    }

    ret = fwrite(data, rw_type_size, count, fp);

    return ret;
}

/**
     * @brief close the end-point
     * 
     * @return int int < 0 error, >= 0 works
     */
int EndpointBinary::Close()
{
    fclose(fp);
    SetOpenFlag(false);
    return 0;
};

int EndpointBinary::PrintInfo()
{
    std::cout << data_endpoint_orig << std::endl;
    return 0;
}

void EndpointBinary::Map2MyType()
{
    switch (data_element_type)
    {
    case AU_SHORT:
        rw_type_size = sizeof(short);
        return;
    case AU_INT:
        rw_type_size = sizeof(int);
        return;
    case AU_LONG:
        rw_type_size = sizeof(long);
        return;
    case AU_LONG_LONG:
        rw_type_size = sizeof(long long);
        return;
    case AU_USHORT:
        rw_type_size = sizeof(unsigned short);
        return;
    case AU_UINT:
        rw_type_size = sizeof(unsigned int);
        return;
    case AU_ULONG:
        rw_type_size = sizeof(unsigned long);
        return;
    case AU_ULLONG:
        rw_type_size = sizeof(unsigned long long);
        return;
    case AU_FLOAT:
        rw_type_size = sizeof(float);
        return;
    case AU_DOUBLE:
        rw_type_size = sizeof(double);

        return;
    default:
        std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
        std::flush(std::cout);
        std::exit(EXIT_FAILURE);
    }
};

/**
     * @brief parse endpoint_info to my own info
     *        In TMDS, it map endpoint_info to filename
     * @return int: 0 works,  < 0 error,
     */
int EndpointBinary::ParseEndpointInfo()
{
    //std::cout << endpoint_info << "\n";
    std::stringstream ss(endpoint_info);
    if (!std::getline(ss, fn_str, ':'))
    {
        AU_EXIT("Invalued endpoint_info");
    }

    return 0;
}

/**
    * @brief update the seek_offset
    * 
    */
void EndpointBinary::UpdateSeekOffset()
{
    seek_offset = 0;
}

int EndpointBinary::SpecialOperator(int opt_code, std::string parameter)
{
    std::vector<unsigned long long> binary_array_size;
    unsigned long long temp_size;
    stringstream s_stream(parameter); //parameter has the format: "dim0_size, dim2_size,...""
    string substr;
    switch (opt_code)
    {
    case BINARY_SET_SIZE:
        while (s_stream.good())
        {
            getline(s_stream, substr, ','); //get first string delimited by comma
            //sscanf(substr.c_str(), "%zu", &temp_size);
            temp_size = std::stoull(substr, nullptr, 0);
            binary_array_size.push_back(temp_size);
        }
        //SetArraySize(binary_array_size);
        SetDimensions(binary_array_size);
        break;
    default:
        AU_EXIT("Not supported opt_code in SpecialOperator of EndpointBinary!");
        break;
    }

    return 0;
}

FILE *EndpointBinary::GetFP()
{
    return fp;
}

void EndpointBinary::SetSeekOffset(long int seek_offset_p)
{
    seek_offset = seek_offset_p;
}

/**
     * @brief map a op_cmd_str to int as input of SpecialOperator
     * 
     * @param op_cmd_str cmd string 
     * @return int 
     */
int EndpointBinary::MapOpStr2Int(std::string op_cmd_str)
{
    if (op_cmd_str == "BINARY_SET_SIZE")
        return BINARY_SET_SIZE;

    return -1;
}

/**
     * @brief map op_int to string 
     * 
     * @param op_int 
     * @return std::string 
     */
std::string EndpointBinary::MapOpInt2Str(int op_int)
{
    if (op_int == BINARY_SET_SIZE)
        return "BINARY_SET_SIZE";

    return "";
}