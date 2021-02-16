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

#include "au_endpoint.h"

/**
     * @brief Get the Dimensions of the data
     * 
     * @return vector for the size of data  endpoint_dim_size.size() is the rank
     */
std::vector<unsigned long long> Endpoint::GetDimensions()
{
    return endpoint_size;
}

/**
     * @brief Set the Dimensions 
     * 
     * @return < 0 error, works otherwise
     */
void Endpoint::SetDimensions(std::vector<unsigned long long> endpoint_dim_size_p)
{
    endpoint_ranks = endpoint_dim_size_p.size();
    endpoint_size = endpoint_dim_size_p;
    set_endpoint_dim_size_flag = true;
}

void Endpoint::SetDataElementType(AuEndpointDataType data_element_type_p)
{
    data_element_type = data_element_type_p;
}

AuEndpointDataType Endpoint::GetDataElementType()
{
    return data_element_type;
}

/**
     * @brief Get the size of the type for the element
     * 
     * @return int 
     */
int Endpoint::GetDataElementTypeSize()
{
    //std::cout << "data_element_type =" << data_element_type << "\n";
    switch (data_element_type)
    {
    case AU_SHORT:
        return sizeof(short);
    case AU_INT:
        return sizeof(int);
    case AU_LONG:
        return sizeof(long);
    case AU_LONG_LONG:
        return sizeof(long long);
    case AU_USHORT:
        return sizeof(unsigned short);
    case AU_UINT:
        return sizeof(unsigned int);
    case AU_ULONG:
        return sizeof(unsigned long);
    case AU_ULLONG:
        return sizeof(unsigned long long);
    case AU_FLOAT:
        return sizeof(float);
    case AU_DOUBLE:
        return sizeof(double);
    default:
        std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
        std::flush(std::cout);
        std::exit(EXIT_FAILURE);
    }
}

bool Endpoint::GetOpenFlag()
{
    return open_flag;
}

void Endpoint::SetOpenFlag(bool open_flag_p)
{
    open_flag = open_flag_p;
}

bool Endpoint::GetCreateFlag()
{
    return create_flag;
}

void Endpoint::SetCreateFlag(bool create_flag_p)
{
    create_flag = create_flag_p;
}

void Endpoint::SetRwFlag(unsigned read_write_flag_p)
{
    read_write_flag = read_write_flag_p;
}

unsigned Endpoint::GetRwFlag()
{
    return read_write_flag;
}

#define VOID2UNION_HELPER(TARGET_TYPE, EMPLACE_INDEX)                                     \
    {                                                                                     \
        TARGET_TYPE *temp_pointer_of_type = (TARGET_TYPE *)data_vector_in_void;           \
        for (int i = 0; i < n_elements; i++)                                              \
        {                                                                                 \
            data_vector_in_union_type[i].emplace<EMPLACE_INDEX>(temp_pointer_of_type[i]); \
        }                                                                                 \
    }

std::vector<AuEndpointDataTypeUnion> Endpoint::Void2Union(void *data_vector_in_void, size_t n_elements)
{
    std::vector<AuEndpointDataTypeUnion> data_vector_in_union_type;
    data_vector_in_union_type.resize(n_elements);

    switch (data_element_type)
    {
    case AU_SHORT:
    {
        VOID2UNION_HELPER(short, AU_SHORT);
        break;
    }
    case AU_INT:
    {
        VOID2UNION_HELPER(int, AU_INT);
        break;
    }
    case AU_LONG:
    {
        VOID2UNION_HELPER(long, AU_LONG);
        break;
    }
    case AU_LONG_LONG:
    {
        VOID2UNION_HELPER(long long, AU_LONG_LONG);
        break;
    }
    case AU_USHORT:
    {
        VOID2UNION_HELPER(unsigned short, AU_USHORT);
        break;
    }
    case AU_UINT:
    {
        VOID2UNION_HELPER(unsigned int, AU_UINT);
        break;
    }
    case AU_ULONG:
    {
        VOID2UNION_HELPER(unsigned long, AU_ULONG);
        break;
    }
    case AU_ULLONG:
    {
        VOID2UNION_HELPER(unsigned long long, AU_ULLONG);
        break;
    }
    case AU_FLOAT:
    {
        VOID2UNION_HELPER(float, AU_FLOAT);
        break;
    }
    case AU_DOUBLE:
    {
        VOID2UNION_HELPER(double, AU_DOUBLE);
        break;
    }
    default:
        std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
        std::flush(std::cout);
        std::exit(EXIT_FAILURE);
    }

    return data_vector_in_union_type;
}

#define UNION2VOID_HELPER(TARGET_TYPE, EMPLACE_INDEX)                                        \
    {                                                                                        \
        std::vector<TARGET_TYPE> temp_pointer_of_type;                                       \
        temp_pointer_of_type.resize(n_elements);                                             \
        for (int i = 0; i < n_elements; i++)                                                 \
        {                                                                                    \
            temp_pointer_of_type[i] = std::get<EMPLACE_INDEX>(data_vector_in_union_type[i]); \
        }                                                                                    \
        return static_cast<void *>(temp_pointer_of_type.data());                             \
    }

void *Endpoint::Union2Void(std::vector<AuEndpointDataTypeUnion> &data_vector_in_union_type)
{
    size_t n_elements = data_vector_in_union_type.size();
    switch (data_element_type)
    {
    case AU_SHORT:
    {
        UNION2VOID_HELPER(short, AU_SHORT);
        break;
    }
    case AU_INT:
    {
        UNION2VOID_HELPER(int, AU_INT);
        break;
    }
    case AU_LONG:
    {
        UNION2VOID_HELPER(long, AU_LONG);
        break;
    }
    case AU_LONG_LONG:
    {
        UNION2VOID_HELPER(long long, AU_LONG_LONG);
        break;
    }
    case AU_USHORT:
    {
        UNION2VOID_HELPER(unsigned short, AU_USHORT);
        break;
    }
    case AU_UINT:
    {
        UNION2VOID_HELPER(unsigned int, AU_UINT);
        break;
    }
    case AU_ULONG:
    {
        UNION2VOID_HELPER(unsigned long, AU_ULONG);
        break;
    }
    case AU_ULLONG:
    {
        UNION2VOID_HELPER(unsigned long long, AU_ULLONG);
        break;
    }
    case AU_FLOAT:
    {
        UNION2VOID_HELPER(float, AU_FLOAT);
        break;
    }
    case AU_DOUBLE:
    {
        UNION2VOID_HELPER(double, AU_DOUBLE);
        break;
    }
    default:
        std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
        std::flush(std::cout);
        std::exit(EXIT_FAILURE);
    }
}

/**
     * @brief set the endpoint_info string 
     * 
     * @param endpoint_info 
     */
void Endpoint::SetEndpointInfo(std::string endpoint_info_p)
{
    endpoint_info = endpoint_info_p;
    ParseEndpointInfo(); //After set, we need to refresh related other parts
}

/**
    * @brief Get the endpoint_info string 
     * 
     * @return std::string 
     */
std::string Endpoint::GetEndpointInfo()
{
    return endpoint_info;
}

/**
     * @brief Set the Endpoint Type object
     * 
     * @param endpoint_type_p 
     */
void Endpoint::SetEndpointType(AuEndpointType endpoint_type_p)
{
    endpoint_type = endpoint_type_p;
}

/**
     * @brief Get the Endpoint Type object
     * 
     * @return AuEndpointType 
     */
AuEndpointType Endpoint::GetEndpointType()
{
    return endpoint_type;
}

std::vector<std::string> Endpoint::GetDirFileVector()
{
    AU_EXIT("Error: should not be called");
    std::vector<std::string> sv;
    return sv;
}

void Endpoint::SetDirFileVector(std::vector<std::string> &file_list)
{
    AU_EXIT("Error: should not be called");
}

/**
     * @brief Get the Dir Chunk Size object
     * 
     * @return std::vector<int> 
     */
std::vector<int> Endpoint::GetDirChunkSize()
{
    AU_EXIT("Error: should not be called");
}

/**
     * @brief Set the Dir Chunk Size object
     * 
     * @param dir_chunk_size_p 
     */
void Endpoint::SetDirChunkSize(std::vector<int> &dir_chunk_size_p)
{
    AU_EXIT("Error: SetDirChunkSize is not implemented in endpoint");
}

/**
     * @brief call a special operator on endpoint
     *        such as, enable collective I/O for HDF5
     *                 dump file from MEMORY to HDF5
     * @param opt_code, specially defined code 
     */
//int Endpoint::SpecialOperator(int opt_code, std::string parameter)
//{
//    AU_EXIT("Error: SpecialOperator is not implemented in endpoint");
//    return 0;
//}

/**
     * @brief call a special operator on endpoint
     *        such as, enable collective I/O for HDF5
     *                 dump file from MEMORY to HDF5
     * @param opt_code, specially defined code 
     * @param parameter_v, vector of argument
     */
int Endpoint::Control(int opt_code, std::vector<std::string> &parameter_v)
{
    AU_EXIT("Error: Control is not implemented in endpoint");
    return 0;
}

void Endpoint::EnableCollectiveIO()
{
}

void Endpoint::DisableCollectiveIO()
{
}

int Endpoint::MapOpStr2Int(std::string op_cmd_str)
{
    AU_EXIT("Error: MapOpString2Int is not implemented in endpoint");

    return 0;
}

/**
     * @brief map op_int to string 
     * 
     * @param op_int 
     * @return std::string 
     */
std::string Endpoint::MapOpInt2Str(int op_int)
{
    AU_EXIT("Error: MapOpInt2String is not implemented in endpoint");
    return 0;
}

/**
     * @brief Set the Attribute object
     * 
     * @param name 
     * @param data 
     * @return int 
     */
int Endpoint::WriteAttribute(const std::string &name, const void *data, FTDataType data_type_p, const size_t &data_length_p)
{
    AU_EXIT("Error: SetAttribute is not implemented in endpoint");
    return 0;
}

/**
     * @brief Get the Attribute object
     * 
     * @param name 
     * @param data 
     * @return int 
     */
int Endpoint::ReadAttribute(const std::string &name, void *data, FTDataType data_type_p, const size_t &data_length_p)
{
    AU_EXIT("Error: GetAttribute is not implemented in endpoint");
    return 0;
}

size_t Endpoint::GetAttributeSize(const std::string &name, FTDataType data_type_p)
{
    AU_EXIT("Error: GetAttributeSize is not implemented in endpoint");
    return 0;
}
