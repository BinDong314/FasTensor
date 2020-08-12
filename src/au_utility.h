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

#ifndef ARRAY_UDF_UTILITY_H
#define ARRAY_UDF_UTILITY_H

#include <vector>
#include <type_traits>
#include <cstring>
#include <cmath>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <tuple>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "au_type.h"
#include <utility>
#include <variant>
#include <dirent.h>

#if __cplusplus > 201402L
#include "cista.h"
#endif

template <class T>
inline void ClearVector(std::vector<T> &v)
{
    v.clear();
    std::vector<T>().swap(v);
}

#define V2VOIDP(vv) static_cast<void *>(vv.data())

/**
 * @brief expand the path to full directory 
 *    https://www.dreamincode.net/forums/topic/218601-realpath-and-tilde/
 * @param path 
 * @param buff 
 * @return char* 
 */
std::string realpathEx(std::string path);

/**
 * @brief Get file list of a direction
 * 
 * @param dir_str_p 
 * @return std::vector<std::string> 
 */
std::vector<std::string> GetDirFileList(std::string dir_str_p);

/**
 * @brief Split endpoint_type_info string to type and information
 * 
 * @param endpoint_type_info , information string for the endpoint
 * @param endpoint_type , output value containing the type
 * @param endpoint_info , output value containging all other parts
 * @return int 
 */
int ExtractEndpointTypeInfo(std::string endpoint_type_info, AuEndpointType &endpoint_type, std::string &endpoint_info);

/**
 * @brief Check wether the file exists
 * 
 * @param filename 
 * @return int 
 */
int file_exist(const char *filename);

/**
 * @brief help function to counts cells between start/end
 * 
 */
#define COUNT_CELLS(start_address_p, end_address_p, cells_count_p)                       \
    {                                                                                    \
        assert(start_address_p.size() == end_address_p.size());                          \
        cells_count_p = 1;                                                               \
        for (int i = 0; i < start_address_p.size(); i++)                                 \
        {                                                                                \
            cells_count_p = cells_count_p * (end_address_p[i] - start_address_p[i] + 1); \
        }                                                                                \
    }

template <typename T>
inline void PrintVector(std::string name, std::vector<T> v)
{
    int n = v.size();
    if (name != "")
        std::cout << name << ": ";
    if (n > 8)
    {
        for (int i = 0; i < 4; i++)
        {
            std::cout << v[i] << ",";
        }
        std::cout << " ... ";
        for (int i = n - 4; i < n - 1; i++)
        {
            std::cout << v[i] << ",";
        }
        std::cout << v[n - 1];
    }
    else
    {
        for (int i = 0; i < n - 1; i++)
        {
            std::cout << v[i] << ",";
        }
        std::cout << v[n - 1];
    }
    std::cout << std::endl
              << std::flush;
}

template <typename T>
inline void PrintVV(std::string name, std::vector<std::vector<T>> v)
{
    int n = v.size();
    std::cout << name << ": ";

    if (n > 8)
    {
        for (int i = 0; i < 4; i++)
        {
            PrintVector("", v[i]);
        }
        std::cout << " ... \n";
        for (int i = n - 4; i < n; i++)
        {
            PrintVector("", v[i]);
        }
    }
    else
    {
        for (int i = 0; i < n - 1; i++)
        {
            PrintVector("", v[i]);
        }
    }
    std::cout << std::endl;
}

template <typename T>
inline void PrintScalar(std::string name, T v)
{
    std::cout << name << ": " << v << std::endl;
}

/**
 * @brief convert coordinate to linearized one
 * 
 * @param dsize : size of all dimensions of the data
 * @param coordinate : multidimensional coordinate
 * @return unsigned long long : linearized one
 */
inline unsigned long long RowMajorOrder(std::vector<unsigned long long> dsize, std::vector<unsigned long long> coordinate)
{
    unsigned long long offset = coordinate[0];
    int n = dsize.size();
    for (int i = 1; i < n; i++)
    {
        offset = offset * dsize[i] + coordinate[i];
    }
    return offset;
}

/**
 * @brief convert linearized coordinate to multidimensional one
 * 
 * @param offset : linearized coordinate
 * @param dsize : data size 
 * @return std::vector<unsigned long long> :multidimensional coordinate
 */
inline std::vector<unsigned long long> RowMajorOrderReverse(unsigned long long offset, std::vector<unsigned long long> dsize)
{
    int n = dsize.size();
    std::vector<unsigned long long> original_coordinate;
    original_coordinate.resize(n);
    //unsigned long long reminder;
    for (unsigned long long i = n - 1; i >= 1; i--)
    {
        original_coordinate[i] = offset % dsize[i];
        offset = offset / dsize[i];
    }
    //Last dimenstion
    original_coordinate[0] = offset;

    return original_coordinate;
}

/**
 * @brief macro version of above two functions for speed
 * 
 */
#ifndef ROW_MAJOR_ORDER_MACRO
#define ROW_MAJOR_ORDER_MACRO(dsize, dsize_len, coordinate, offset) \
    {                                                               \
        offset = coordinate[0];                                     \
        for (int iii = 1; iii < dsize_len; iii++)                   \
        {                                                           \
            offset = offset * dsize[iii] + coordinate[iii];         \
        }                                                           \
    }
#endif

#ifndef ROW_MAJOR_ORDER_REVERSE_MACRO
#define ROW_MAJOR_ORDER_REVERSE_MACRO(offset, dsize, dsize_len, result_coord_v) \
    {                                                                           \
        unsigned long long temp_offset = offset;                                \
        for (int iii = dsize_len - 1; iii >= 1; iii--)                          \
        {                                                                       \
            result_coord_v[iii] = temp_offset % dsize[iii];                     \
            temp_offset = temp_offset / dsize[iii];                             \
        }                                                                       \
        result_coord_v[0] = temp_offset;                                        \
    }
#endif

#define AU_EXIT(info)                                                                                     \
    {                                                                                                     \
        std::cout << "Exit happens at " << __FILE__ << ", " << __func__ << ", " << __LINE__ << std::endl; \
        std::cout << "Log : " << info << std::endl;                                                       \
        std::exit(EXIT_FAILURE);                                                                          \
    }

#define AU_INFO(info)                                                                             \
    {                                                                                             \
        std::cout << "Info at " << __FILE__ << ", " << __func__ << ", " << __LINE__ << std::endl; \
        std::cout << "Info : " << info << std::endl;                                              \
    }

/**
 * @brief 
 *flat vector of vector to 1D vector
 * direction specify row-major or colum major
 *  AU_FLAT_OUTPUT_ROW (0): row major, e.g., for a 2 by 2 vector
 *     v[0][0] v[0][1] v[1][0] v[1][1]
 * AU_FLAT_OUTPUT_COL (1): column major, e.g., for a 2 by 2 vector
 *    v[0][0] v[1][0] v[0][1] v[1][1]
 * 
 * Since this is the support function to WriteEndpoint,
 * It also convert the address associated with it. 
 * 
 * @tparam T type of data element
 * @param v the vector of vector
 * @param direction how to flat the data
 * @param start_address the start address before/after flat, it may chage the # of dimensions
 * @param end_address  the end address before/after flat, it may chage the # of dimensions
 * @param v_size, the size of each element vector
 * @return void* the flat data
 */
template <typename T>
void *FlatVector(std::vector<std::vector<T>> &v, OutputVectorFlatDirection direction, std::vector<unsigned long long> &start_address, std::vector<unsigned long long> &end_address, size_t &v_size)
{
    if (v.size() < 1)
    {
        return NULL;
    }

    if (v[0].size() < 1)
    {
        return NULL;
    }

    //Check v's elemtnt has same size
    for (unsigned j = 0; j < v.size() - 1; j++)
    {
        if (v[j].size() != v[j + 1].size())
        {
            AU_EXIT("Size of each element of vector must be queal.");
        }
    }
    v_size = v[0].size();

    if (direction == AU_FLAT_OUTPUT_NEW)
    {
        start_address.push_back(0);
        end_address.push_back(v_size - 1);
        return &v[0];
    }

    T *rv = (T *)malloc(v.size() * v_size * sizeof(T)); //Assuming all rows have the same size
    if (rv == NULL)
    {
        AU_EXIT("Not enough memory");
    }

    if (direction == AU_FLAT_OUTPUT_COL)
    {
        for (unsigned i = 0; i < v_size; i++)
        {
            for (unsigned j = 0; j < v.size(); j++)
            {
                memcpy(rv + v.size() * i + j, &(v[j][i]), sizeof(T));
            }
        }
    }
    else if (direction == AU_FLAT_OUTPUT_ROW)
    {

        for (unsigned i = 0; i < v.size(); i++)
        {
            memcpy(rv + v_size * i, &(v[i][0]), v_size * sizeof(T));
        }
    }
    else
    {
        AU_EXIT("Not supported option yet to flat vector");
    }

    end_address[direction] = start_address[direction] + (end_address[direction] - start_address[direction] + 1) * v_size - 1;

    return (void *)rv;
}

template <typename T>
void *FlatVector(std::vector<T> &v, OutputVectorFlatDirection direction, std::vector<unsigned long long> &start_address, std::vector<unsigned long long> &end_address, size_t &v_size)
{
    AU_EXIT("Should not be here");
}

template <typename T>
void *FlatVector(T &v, OutputVectorFlatDirection direction, std::vector<unsigned long long> &start_address, std::vector<unsigned long long> &end_address, size_t &v_size)
{
    AU_EXIT("Should not be here");
}

std::string ExtractFileName(const std::string &fullPath);

std::string ExtractPath(const std::string &fullPath);

/**
 * @brief 
 * 
 * @tparam T1 
 * @tparam T2 
 * @param attribute_vector it is type of AuEndpointDataTypeUnion
 * @param virtual_array_vector 
 * @param index 
 */
template <class T1, class T2>
inline void InsertAttribute2VirtualArrayVector(const std::vector<T1> &attribute_vector, AuEndpointDataType union_index, std::vector<T2> &virtual_array_vector, int attribute_index)
{
    assert(attribute_vector.size() == virtual_array_vector.size());
    size_t n = attribute_vector.size();
    for (size_t i = 0; i < n; i++)
    {
        T1 attribute_vector_value = attribute_vector[i];
        int m_index = 0;
        cista::for_each_field(virtual_array_vector[i], [&m_index, attribute_index, attribute_vector_value, union_index](auto &&m) {
            if (m_index == attribute_index)
            {
                switch (union_index)
                {
                case AU_SHORT:
                {
                    m = std::get<AU_SHORT>(attribute_vector_value);
                    break;
                }
                case AU_INT:
                {
                    m = std::get<AU_INT>(attribute_vector_value);
                    break;
                }
                case AU_LONG:
                {
                    m = std::get<AU_LONG>(attribute_vector_value);
                    break;
                }
                case AU_LONG_LONG:
                {
                    m = std::get<AU_LONG_LONG>(attribute_vector_value);
                    break;
                }
                case AU_USHORT:
                {
                    m = std::get<AU_USHORT>(attribute_vector_value);
                    break;
                }
                case AU_UINT:
                {
                    m = std::get<AU_UINT>(attribute_vector_value);
                    break;
                }
                case AU_ULONG:
                {
                    m = std::get<AU_ULONG>(attribute_vector_value);
                    break;
                }
                case AU_ULLONG:
                {
                    m = std::get<AU_ULLONG>(attribute_vector_value);
                    break;
                }
                case AU_FLOAT:
                {
                    m = std::get<AU_FLOAT>(attribute_vector_value);
                    break;
                }
                case AU_DOUBLE:
                {
                    m = std::get<AU_DOUBLE>(attribute_vector_value);
                    break;
                }
                default:
                    std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
                    std::flush(std::cout);
                    std::exit(EXIT_FAILURE);
                }
                return;
            }
            m_index++;
        });
    }
}

template <>
inline void InsertAttribute2VirtualArrayVector<AuEndpointDataTypeUnion, std::complex<double>>(const std::vector<AuEndpointDataTypeUnion> &attribute_vector, AuEndpointDataType union_index, std::vector<std::complex<double>> &virtual_array_vector, int attribute_index)
{
    AU_EXIT("std::complex does not work with cista::for_each_field now");
}

#define ExtractAttributeFromVirtualArrayVector_HELPER(ELEMENT_TYPE)                                                       \
    {                                                                                                                     \
        ELEMENT_TYPE *attribute_data_typed = (ELEMENT_TYPE *)attribute_data_void_pointer;                                 \
        for (size_t i = 0; i < n; i++)                                                                                    \
        {                                                                                                                 \
            int m_index = 0;                                                                                              \
            ELEMENT_TYPE temp_attribute_value;                                                                            \
            cista::for_each_field(virtual_array_vector[i], [&m_index, attribute_index, &temp_attribute_value](auto &&m) { \
                if (m_index == attribute_index)                                                                           \
                {                                                                                                         \
                    temp_attribute_value = m;                                                                             \
                }                                                                                                         \
                m_index++;                                                                                                \
            });                                                                                                           \
            attribute_data_typed[i] = temp_attribute_value;                                                               \
        }                                                                                                                 \
    }

template <class T2>
inline void *ExtractAttributeFromVirtualArrayVector(std::vector<T2> &virtual_array_vector, int attribute_index, AuEndpointDataType element_type, int element_type_size)
{

    size_t n = virtual_array_vector.size();
    void *attribute_data_void_pointer = malloc(n * element_type_size);

    switch (element_type)
    {
    case AU_SHORT:
    {
        ExtractAttributeFromVirtualArrayVector_HELPER(short);
        break;
    }
    case AU_INT:
    {
        ExtractAttributeFromVirtualArrayVector_HELPER(int);
        break;
    }
    case AU_LONG:
    {
        ExtractAttributeFromVirtualArrayVector_HELPER(long);
        break;
    }
    case AU_LONG_LONG:
    {
        ExtractAttributeFromVirtualArrayVector_HELPER(long long);
        break;
    }
    case AU_USHORT:
    {
        ExtractAttributeFromVirtualArrayVector_HELPER(unsigned short);
        break;
    }
    case AU_UINT:
    {
        ExtractAttributeFromVirtualArrayVector_HELPER(unsigned int);
        break;
    }
    case AU_ULONG:
    {
        ExtractAttributeFromVirtualArrayVector_HELPER(unsigned long);
        break;
    }
    case AU_ULLONG:
    {
        ExtractAttributeFromVirtualArrayVector_HELPER(unsigned long long);
        break;
    }
    case AU_FLOAT:
    {
        ExtractAttributeFromVirtualArrayVector_HELPER(float);
        break;
    }
    case AU_DOUBLE:
    {
        ExtractAttributeFromVirtualArrayVector_HELPER(double);
        break;
    }
    default:
        std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
        std::flush(std::cout);
        std::exit(EXIT_FAILURE);
    }
    return attribute_data_void_pointer;
}

template <>
inline void *ExtractAttributeFromVirtualArrayVector<std::complex<double>>(std::vector<std::complex<double>> &virtual_array_vector, int attribute_index, AuEndpointDataType element_type, int element_type_size)
{
    AU_EXIT("std::complex<double> does work with cista::to_tuple<");
}
#endif
