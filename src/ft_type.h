

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

#ifndef ARRAY_UDF_TYPE_H
#define ARRAY_UDF_TYPE_H

#include <type_traits>
#include <iostream>
#include <variant>
#include <complex>

#if __cplusplus > 201402L
#include "cista.h"
#endif

typedef enum AuEndpointType
{
  EP_HDF5 = 0,    // from/to HDF5
  EP_PNETCDF = 1, // from/to NETCDF; todo
  EP_ADIOS = 2,   // from/to AUDIOS; todo
  EP_BINARY = 3,  // from/to binary file; todo
  EP_VIRTUAL = 4, // from/to Virtual array of above types; todo
  EP_MEMORY = 5,  // from/to in-memory cache: todo
  EP_H5VDS = 6,   // H5 VDS: todo
  EP_DIR = 7,     // Directory to data to process. file format to be detected
  EP_DASSA = 8,   // customized DASSA IO
  EP_TDMS = 9,    // tdms file format (to binary file)
  EP_CSV = 10,    // CSV format, text format with , or user-specified del
} AuEndpointType;
/**
 * @brief map string typed name of type to AuEndpointType
 *
 * @param endpoint_type_str input of string name
 * @return AuEndpointType output of type
 */
AuEndpointType MapString2EndpointType(std::string endpoint_type_str);

std::string getEndpointExtension(const std::string& format);

// See https://support.hdfgroup.org/ftp/HDF5/current/src/unpacked/src/H5Tpublic.h
// for reference
typedef enum AuEndpointDataType
{
  AU_NO_TYPE = -1,
  AU_SHORT = 0,
  AU_INT = 1,
  AU_LONG = 2,
  AU_LONG_LONG = 3,
  AU_USHORT = 4,
  AU_UINT = 5,
  AU_ULONG = 6,
  AU_ULLONG = 7,
  AU_FLOAT = 8,
  AU_DOUBLE = 9,
  AU_DOUBLE_COMPLEX = 10, // it is std::complex<double>
  AU_STRING = 11,         // mostly used to store attribute
  AU_NCLASSES             /*this must be last                          */
} AuEndpointDataType;

typedef AuEndpointDataType FTDataType;
typedef AuEndpointDataType FTType;

/**
 * @brief It should follow the order of above AuEndpointDataType
 *
 */
using AuEndpointDataTypeUnion = std::variant<short, int, long, long long, unsigned short, unsigned int, unsigned long, unsigned long long, float, double, std::complex<double>, std::string>;

template <typename T>
AuEndpointDataType InferDataType()
{
  // printf("InferDataType : enter\n");

  if (std::is_same<T, int>::value)
  {
    return AU_INT;
  }
  else if (std::is_same<T, short>::value)
  {
    return AU_SHORT;
  }
  else if (std::is_same<T, long>::value)
  {
    return AU_LONG;
  }
  else if (std::is_same<T, long long>::value)
  {
    return AU_LONG_LONG;
  }
  else if (std::is_same<T, unsigned int>::value)
  {
    return AU_UINT;
  }
  else if (std::is_same<T, unsigned short>::value)
  {
    // printf("InferDataType : AU_USHORT\n");
    return AU_USHORT;
  }
  else if (std::is_same<T, unsigned long>::value)
  {
    return AU_ULONG;
  }
  else if (std::is_same<T, unsigned long long>::value)
  {
    return AU_ULLONG;
  }
  else if (std::is_same<T, float>::value)
  {
    return AU_FLOAT;
  }
  else if (std::is_same<T, double>::value)
  {
    return AU_DOUBLE;
  }
  else if (std::is_same<T, std::complex<double>>::value)
  {
    return AU_DOUBLE_COMPLEX;
  }
  else if (std::is_same<T, std::string>::value)
  {
    return AU_STRING;
  }
  else
  {
    return AU_NO_TYPE; // Here it might be User-defined class
  }
}

// The "direction" specify how to flat output vector
// For example, when running a UDF on a cell (i, j) of a 2D array
//  flat_direction_index = 0: store result vector in (i : i + vsize, j)
//  flat_direction_index = 1: store result vector in (i,             j: j + vsize)
//  flat_direction_index = 2: store result vector as new dimension of a 3D array (i, j, 0:vsize)
//
//   vsize = -1,  variable size of output vector (determined at runtime)
//   vsize >= 0,  size of output vector (set by the users)
// Todo: we can infer "vsize" in futher version
//      we can also support multiple dimensional vector
typedef enum OutputVectorFlatDirection
{
  AU_FLAT_OUTPUT_COL = 0,
  AU_FLAT_OUTPUT_ROW = 1,
  AU_FLAT_OUTPUT_NEW = 2,
} OutputVectorFlatDirection;

template <typename T>
struct is_vector : public std::false_type
{
};

template <typename T, typename A>
struct is_vector<std::vector<T, A>> : public std::true_type
{
};

// 1: vector type
// 0: other types
template <typename T>
bool InferVectorType()
{
  return is_vector<T>{};
}

template <typename T>
struct is_vector_vector : public std::false_type
{
};

template <typename T, typename A>
struct is_vector_vector<std::vector<std::vector<T, A>>> : public std::true_type
{
};

// 1: vector type
// 0: other types
template <typename T>
bool InferVectorVectorType()
{
  return is_vector_vector<T>{};
}

// see more detail in third_party/cista.h
#define AU_UDT_INIT(A) FT_UDT_INIT(A)

// see more detail in third_party/cista.h
#define FT_UDT_INIT(A) \
  CISTA_PRINTABLE(A)   \
  CISTA_COMPARABLE()
#endif
