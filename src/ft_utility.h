

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

#ifndef ARRAY_UDF_UTILITY_H
#define ARRAY_UDF_UTILITY_H

#include <algorithm>
#include <cmath>
#include <cstring>
#include <dirent.h>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <tuple>
#include <type_traits>
#include <unistd.h>
#include <utility>
#include <variant>
#include <vector>

#include "ft_type.h"
#include "ft_utility_macro.h"

#if __cplusplus > 201402L
#include "cista.h"
#endif

#define V2VOIDP(vv) static_cast<void *>(vv.data())

extern int ft_rank;

/**
 * @brief expand the path to full directory
 *    https://www.dreamincode.net/forums/topic/218601-realpath-and-tilde/
 * @param path
 * @param buff
 * @return char*
 */
std::string realpathEx(std::string path);

/**
 * @brief
 *
 * @param dir_name
 * @return int 1: exists, others does not
 */
int dir_exist(const char *dir_name);

/**
 * @brief Get file list of a direction
 *
 * @param dir_str_p
 * @return std::vector<std::string>
 */
std::vector<std::string> GetDirFileList(std::string dir_str_p);

/**
 * @brief Get the Dir File List Recursive object
 *
 * @param dir_str_p : the directory
 * @return std::vector<std::string>
 */
std::vector<std::string> GetDirFileListRecursive(std::string dir_str_p);

/**
 * @brief Split endpoint_type_info string to type and information
 *
 * @param endpoint_type_info , information string for the endpoint
 * @param endpoint_type , output value containing the type
 * @param endpoint_info , output value containging all other parts
 * @return int
 */
int ExtractEndpointTypeInfo(std::string endpoint_type_info,
                            AuEndpointType &endpoint_type,
                            std::string &endpoint_info);

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

template <typename T> inline bool IsVectorEq(const std::vector<T> &v, T value) {
  for (int i = 0; i < v.size(); i++) {
    if (v[i] != value) {
      return false;
    }
  }
  return true;
}

template <typename T1, typename T2>
inline bool VectorEqVector(const std::vector<T1> &v1,
                           const std::vector<T2> &v2) {
  if (v1.size() != v2.size()) {
    return false;
  }

  for (size_t i = 0; i < v1.size(); i++) {
    if (v1[i] != v2[i]) {
      return false;
    }
  }
  return true;
}

template <typename T>
inline void PrintVector(std::string name, std::vector<T> v) {
  int n = v.size();
  if (name != "")
    std::cout << "Rank=" << ft_rank << ", size=" << v.size() << ", " << name
              << ": ";

  if (!n) {
    std::cout << std::endl;
    return;
  }

  if (n > 8) {
    for (int i = 0; i < 4; i++) {
      std::cout << v[i] << ",";
    }
    std::cout << " ... ";
    for (int i = n - 4; i < n - 1; i++) {
      std::cout << v[i] << ",";
    }
    std::cout << v[n - 1];
  } else {
    for (int i = 0; i < n - 1; i++) {
      std::cout << v[i] << ",";
    }
    std::cout << v[n - 1];
  }
  std::cout << std::endl;
}

template <typename T>
inline void PrintVV(std::string name, std::vector<std::vector<T>> v) {
  int n = v.size();
  if (v.size() > 1) {
    if (name != "")
      std::cout << "Rank " << ft_rank << ", "
                << ", size= (" << v.size() << "," << v[0].size() << "), "
                << name << ": \n";
  } else {
    if (name != "")
      std::cout << "Rank " << ft_rank << ", "
                << ", size= (" << v.size() << "," << 0 << "), " << name
                << ": \n";
  }

  if (!n) {
    std::cout << std::endl;
    return;
  }

  if (n > 8) {
    for (int i = 0; i < 5; i++) {
      PrintVector("", v[i]);
    }
    std::cout << " ... \n";
    for (int i = n - 5; i < n; i++) {
      PrintVector("", v[i]);
    }
  } else {
    for (int i = 0; i < n; i++) {
      PrintVector("", v[i]);
    }
  }
  std::cout << std::endl;
}

template <typename T> inline void PrintScalar(std::string name, T v) {
  if (name != "")
    std::cout << "Rank " << ft_rank << ", " << name << ": "
              << ": " << v << std::endl;
}

template <typename T> inline void PrintString(std::string name) {
  if (name != "")
    std::cout << "Rank " << ft_rank << ", " << name << std::endl;
}

/**
 * @brief convert coordinate to linearized one
 *
 * @param dsize : size of all dimensions of the data
 * @param coordinate : multidimensional coordinate
 * @return unsigned long long : linearized one
 */
inline unsigned long long
RowMajorOrder(std::vector<unsigned long long> dsize,
              std::vector<unsigned long long> coordinate) {
  unsigned long long offset = coordinate[0];
  int n = dsize.size();
  for (int i = 1; i < n; i++) {
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
inline std::vector<unsigned long long>
RowMajorOrderReverse(unsigned long long offset,
                     std::vector<unsigned long long> dsize) {
  int n = dsize.size();
  std::vector<unsigned long long> original_coordinate;
  original_coordinate.resize(n);
  // unsigned long long reminder;
  for (unsigned long long i = n - 1; i >= 1; i--) {
    original_coordinate[i] = offset % dsize[i];
    offset = offset / dsize[i];
  }
  // Last dimenstion
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
inline void InsertAttribute2VirtualArrayVector(
    const std::vector<T1> &attribute_vector, AuEndpointDataType union_index,
    std::vector<T2> &virtual_array_vector, int attribute_index) {
  assert(attribute_vector.size() == virtual_array_vector.size());
  size_t n = attribute_vector.size();
  for (size_t i = 0; i < n; i++) {
    T1 attribute_vector_value = attribute_vector[i];
    int m_index = 0;
    cista::for_each_field(virtual_array_vector[i], [&m_index, attribute_index,
                                                    attribute_vector_value,
                                                    union_index](auto &&m) {
      if (m_index == attribute_index) {
        switch (union_index) {
        case AU_SHORT: {
          m = std::get<AU_SHORT>(attribute_vector_value);
          break;
        }
        case AU_INT: {
          m = std::get<AU_INT>(attribute_vector_value);
          break;
        }
        case AU_LONG: {
          m = std::get<AU_LONG>(attribute_vector_value);
          break;
        }
        case AU_LONG_LONG: {
          m = std::get<AU_LONG_LONG>(attribute_vector_value);
          break;
        }
        case AU_USHORT: {
          m = std::get<AU_USHORT>(attribute_vector_value);
          break;
        }
        case AU_UINT: {
          m = std::get<AU_UINT>(attribute_vector_value);
          break;
        }
        case AU_ULONG: {
          m = std::get<AU_ULONG>(attribute_vector_value);
          break;
        }
        case AU_ULLONG: {
          m = std::get<AU_ULLONG>(attribute_vector_value);
          break;
        }
        case AU_FLOAT: {
          m = std::get<AU_FLOAT>(attribute_vector_value);
          break;
        }
        case AU_DOUBLE: {
          m = std::get<AU_DOUBLE>(attribute_vector_value);
          break;
        }
        default:
          std::cout << "Unsupported datatype in " << __FILE__ << " : "
                    << __LINE__ << std::endl;
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
inline void InsertAttribute2VirtualArrayVector<AuEndpointDataTypeUnion,
                                               std::complex<double>>(
    const std::vector<AuEndpointDataTypeUnion> &attribute_vector,
    AuEndpointDataType union_index,
    std::vector<std::complex<double>> &virtual_array_vector,
    int attribute_index) {
  AU_EXIT("std::complex does not work with cista::for_each_field now");
}

#define ExtractAttributeFromVirtualArrayVector_HELPER(ELEMENT_TYPE)            \
  {                                                                            \
    ELEMENT_TYPE *attribute_data_typed =                                       \
        (ELEMENT_TYPE *)attribute_data_void_pointer;                           \
    for (size_t i = 0; i < n; i++) {                                           \
      int m_index = 0;                                                         \
      ELEMENT_TYPE temp_attribute_value;                                       \
      cista::for_each_field(                                                   \
          virtual_array_vector[i],                                             \
          [&m_index, attribute_index, &temp_attribute_value](auto &&m) {       \
            if (m_index == attribute_index) {                                  \
              temp_attribute_value = m;                                        \
            }                                                                  \
            m_index++;                                                         \
          });                                                                  \
      attribute_data_typed[i] = temp_attribute_value;                          \
    }                                                                          \
  }

template <class T2>
inline void *ExtractAttributeFromVirtualArrayVector(
    std::vector<T2> &virtual_array_vector, int attribute_index,
    AuEndpointDataType element_type, int element_type_size) {

  size_t n = virtual_array_vector.size();
  void *attribute_data_void_pointer = malloc(n * element_type_size);

  switch (element_type) {
  case AU_SHORT: {
    ExtractAttributeFromVirtualArrayVector_HELPER(short);
    break;
  }
  case AU_INT: {
    ExtractAttributeFromVirtualArrayVector_HELPER(int);
    break;
  }
  case AU_LONG: {
    ExtractAttributeFromVirtualArrayVector_HELPER(long);
    break;
  }
  case AU_LONG_LONG: {
    ExtractAttributeFromVirtualArrayVector_HELPER(long long);
    break;
  }
  case AU_USHORT: {
    ExtractAttributeFromVirtualArrayVector_HELPER(unsigned short);
    break;
  }
  case AU_UINT: {
    ExtractAttributeFromVirtualArrayVector_HELPER(unsigned int);
    break;
  }
  case AU_ULONG: {
    ExtractAttributeFromVirtualArrayVector_HELPER(unsigned long);
    break;
  }
  case AU_ULLONG: {
    ExtractAttributeFromVirtualArrayVector_HELPER(unsigned long long);
    break;
  }
  case AU_FLOAT: {
    ExtractAttributeFromVirtualArrayVector_HELPER(float);
    break;
  }
  case AU_DOUBLE: {
    ExtractAttributeFromVirtualArrayVector_HELPER(double);
    break;
  }
  default:
    std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__
              << std::endl;
    std::flush(std::cout);
    std::exit(EXIT_FAILURE);
  }
  return attribute_data_void_pointer;
}

inline void *ExtractAttributeFromVirtualArrayVector(
    std::vector<std::string> &virtual_array_vector, int attribute_index,
    AuEndpointDataType element_type, int element_type_size) {
  std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__
            << std::endl;
  std::flush(std::cout);
  std::exit(EXIT_FAILURE);
}

template <>
inline void *ExtractAttributeFromVirtualArrayVector<std::complex<double>>(
    std::vector<std::complex<double>> &virtual_array_vector,
    int attribute_index, AuEndpointDataType element_type,
    int element_type_size) {
  AU_EXIT("std::complex<double> does work with cista::to_tuple<");
}

#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
template <typename T> std::string Vector2String(const std::vector<T> &vec) {
  std::ostringstream vts;
  vts << std::setprecision(17); // force high prevision for float
  if (!vec.empty()) {
    // Convert all but the last element to avoid a trailing ","
    std::copy(vec.begin(), std::prev(vec.end(), 1),
              std::ostream_iterator<T>(vts, ","));

    // Now add the last element with no delimiter
    vts << vec.back();
  }
  return vts.str();
}

template <typename T>
void String2Vector(const std::string &str, std::vector<T> &vec_new) {
  std::stringstream ss(str);
  vec_new.clear();
  for (T i; ss >> i;) {
    vec_new.push_back(i);
    if (ss.peek() == ',')
      ss.ignore();
  }
}

template <class T> inline void clear_vector(std::vector<T> &v) {
  v.clear();
  std::vector<T>().swap(v);
}

#endif
