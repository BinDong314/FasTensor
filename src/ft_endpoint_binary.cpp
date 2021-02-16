

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
#include "au_endpoint_binary.h"
#include "au_array_transpose.h"

int EndpointBinary::ExtractMeta()
{
    return 0;
}

int EndpointBinary::Create()
{
    fp = fopen(fn_str.c_str(), "w");
    if (fp == NULL)
    {
        AU_EXIT("Can not open file : " + fn_str);
    }
    SetOpenFlag(true);
    //UpdateSeekOffset();
    fseek(fp, seek_offset, SEEK_SET);

    return 0;
}

int EndpointBinary::Open()
{

    if (file_exist(fn_str.c_str()) == 0)
    {
        Create();
    }
    else
    {

        //std::cout << "call open in binary, fn_str = " << fn_str << ", model_str= " << model_str << "  \n";
        //PrintInfo();
        //Opne input file
        fp = fopen(fn_str.c_str(), model_str.c_str());
        if (fp == NULL)
        {
            AU_EXIT("Can not open file" + fn_str);
        }
    }
    UpdateSeekOffset();
    fseek(fp, seek_offset, SEEK_SET);
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

    //std::cout << "rw_type_size = " << rw_type_size << " , count = " << count << "\n";

    ret = fread(data, rw_type_size, count, fp);
    assert(ret >= 0);

    if (tranpose_on_read_flag)
    {
        if (data_rank == 2)
        {
            void *temp_data = malloc(count * rw_type_size);
            switch (data_element_type)
            {
            case AU_SHORT:
                transpose_data_2D<short>((short *)data, (short *)temp_data, (end[1] - start[1] + 1), (end[0] - start[0] + 1));
                break;
            case AU_INT:
                transpose_data_2D<int>((int *)data, (int *)temp_data, (end[1] - start[1] + 1), (end[0] - start[0] + 1));
                break;
            case AU_LONG:
                transpose_data_2D<long>((long *)data, (long *)temp_data, (end[1] - start[1] + 1), (end[0] - start[0] + 1));
                break;
            case AU_LONG_LONG:
                transpose_data_2D<long long>((long long *)data, (long long *)temp_data, (end[1] - start[1] + 1), (end[0] - start[0] + 1));
                break;
            case AU_USHORT:
                transpose_data_2D<unsigned short>((unsigned short *)data, (unsigned short *)temp_data, (end[1] - start[1] + 1), (end[0] - start[0] + 1));
                break;
            case AU_UINT:
                transpose_data_2D<unsigned int>((unsigned int *)data, (unsigned int *)temp_data, (end[1] - start[1] + 1), (end[0] - start[0] + 1));
                break;
            case AU_ULONG:
                transpose_data_2D<unsigned long>((unsigned long *)data, (unsigned long *)temp_data, (end[1] - start[1] + 1), (end[0] - start[0] + 1));
                break;
            case AU_ULLONG:
                transpose_data_2D<unsigned long long>((unsigned long long *)data, (unsigned long long *)temp_data, (end[1] - start[1] + 1), (end[0] - start[0] + 1));
                break;
            case AU_FLOAT:
                transpose_data_2D<float>((float *)data, (float *)temp_data, (end[1] - start[1] + 1), (end[0] - start[0] + 1));
                break;
            case AU_DOUBLE:
                transpose_data_2D<double>((double *)data, (double *)temp_data, (end[1] - start[1] + 1), (end[0] - start[0] + 1));
                break;
            default:
                std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
                std::flush(std::cout);
                std::exit(EXIT_FAILURE);
            }
            memcpy(data, temp_data, count * rw_type_size);
            free(temp_data);
        }
        else
        {
            std::cout << "Only 2D transpose supported in " << __FILE__ << " : " << __LINE__ << std::endl;
            std::flush(std::cout);
            std::exit(EXIT_FAILURE);
        }
    }
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
    if (GetOpenFlag() == false)
    {
        Open();
    }

    Map2MyType();

    int ret;
    size_t count = 1;
    int data_rank = start.size();
    for (int i = 0; i < data_rank; i++)
    {
        count = count * (end[i] - start[i] + 1);
    }

    //PrintVector("EndpointBinary::start", start);
    //PrintVector("EndpointBinary::end", end);
    //PrintScalar(" EndpointBinary::Write rw_type_size ", rw_type_size);
    //PrintScalar(" EndpointBinary::Write write count ", count);
    ret = fwrite(data, rw_type_size, count, fp);
    assert(ret >= 0);
    fflush(fp);

    return ret;
}

/**
     * @brief close the end-point
     * 
     * @return int int < 0 error, >= 0 works
     */
int EndpointBinary::Close()
{
    if (fp != NULL)
        fclose(fp);
    fp = NULL;
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

/*
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
    case BINARY_ENABLE_TRANSPOSE_ON_READ:
        EnableTranposeOnRead();
        break;
    case BINARY_ENABLE_TRANSPOSE_ON_WRITE:
        EnableTranposeOnWrite();
        break;
    case BINARY_DISABLE_TRANSPOSE_ON_READ:
        DisableTranposeOnRead();
        break;
    case BINARY_DISABLE_TRANSPOSE_ON_WRITE:
        DisableTranposeOnWrite();
        break;
    default:
        AU_EXIT("Not supported opt_code in SpecialOperator of EndpointBinary!");
        break;
    }

    return 0;
} */

int EndpointBinary::Control(int opt_code, std::vector<std::string> &parameter_v)
{

    switch (opt_code)
    {
    case BINARY_SET_SIZE:
    {
        std::vector<unsigned long long> binary_array_size;
        unsigned long long temp_size;
        string substr;
        stringstream s_stream(parameter_v[0]); //parameter has the format: "dim0_size, dim2_size,...""

        while (s_stream.good())
        {
            getline(s_stream, substr, ','); //get first string delimited by comma
            //sscanf(substr.c_str(), "%zu", &temp_size);
            temp_size = std::stoull(substr, nullptr, 0);
            binary_array_size.push_back(temp_size);
        }
        //SetArraySize(binary_array_size);
        SetDimensions(binary_array_size);
    }
    break;
    case BINARY_ENABLE_TRANSPOSE_ON_READ:
        EnableTranposeOnRead();
        break;
    case BINARY_ENABLE_TRANSPOSE_ON_WRITE:
        EnableTranposeOnWrite();
        break;
    case BINARY_DISABLE_TRANSPOSE_ON_READ:
        DisableTranposeOnRead();
        break;
    case BINARY_DISABLE_TRANSPOSE_ON_WRITE:
        DisableTranposeOnWrite();
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

    if (op_cmd_str == "BINARY_ENABLE_TRANSPOSE_ON_READ")
        return BINARY_ENABLE_TRANSPOSE_ON_READ;

    if (op_cmd_str == "BINARY_ENABLE_TRANSPOSE_ON_WRITE")
        return BINARY_ENABLE_TRANSPOSE_ON_WRITE;

    if (op_cmd_str == "BINARY_DISABLE_TRANSPOSE_ON_READ")
        return BINARY_DISABLE_TRANSPOSE_ON_READ;

    if (op_cmd_str == "BINARY_DISABLE_TRANSPOSE_ON_WRITE")
        return BINARY_DISABLE_TRANSPOSE_ON_WRITE;

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

/**
     * @brief EnableTranposeOnRead
     * 
     */
void EndpointBinary::EnableTranposeOnRead()
{
    //std::cout << "EnableTranposeOnRead \n";
    tranpose_on_read_flag = true;
}
/**
     * @brief EnableTranposeOnWrite
     * 
     */
void EndpointBinary::EnableTranposeOnWrite()
{
    tranpose_on_write_flag = true;
}

/**
     * @brief DisableTranposeOnRead
     * 
     */
void EndpointBinary::DisableTranposeOnRead()
{
    tranpose_on_read_flag = false;
}

/**
     * @brief DisableTranposeOnWrite
     * 
     */
void EndpointBinary::DisableTranposeOnWrite()
{
    tranpose_on_write_flag = false;
}