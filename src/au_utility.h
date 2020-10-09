

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
#include <utility>
#include <variant>
#include <dirent.h>

#include "au_type.h"
#include "au_utility_macro.h"

#if __cplusplus > 201402L
#include "cista.h"
#endif

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
 * @brief 
 * 
 * @param fullPath 
 * @return std::string 
 */
std::string ExtractFileName(const std::string &fullPath);

/**
 * @brief 
 * 
 * @param fullPath 
 * @return std::string 
 */
std::string ExtractPath(const std::string &fullPath);

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
    std::cout << std::endl;
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

#include <vector>
#include <iterator>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
template <typename T>
std::string Vector2String(const std::vector<T> &vec)
{
    std::ostringstream vts;
    vts << std::setprecision(17); //force high prevision for float
    if (!vec.empty())
    {
        // Convert all but the last element to avoid a trailing ","
        std::copy(vec.begin(), std::prev(vec.end(), 1),
                  std::ostream_iterator<T>(vts, ","));

        // Now add the last element with no delimiter
        vts << vec.back();
    }
    return vts.str();
}

template <typename T>
void String2Vector(const std::string &str, std::vector<T> &vec_new)
{
    std::stringstream ss(str);

    for (T i; ss >> i;)
    {
        vec_new.push_back(i);
        if (ss.peek() == ',')
            ss.ignore();
    }
}

#endif
