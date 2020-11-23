

/**
 *
 * Email questions to {dbin, kwu, sbyna}@lbl.gov
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
  EP_HDF5 = 0,    //from/to HDF5
  EP_PNETCDF = 1, //from/to NETCDF; todo
  EP_ADIOS = 2,   //from/to AUDIOS; todo
  EP_BINARY = 3,  //from/to binary file; todo
  EP_VIRTUAL = 4, //from/to Virtual array of above types; todo
  EP_MEMORY = 5,  //from/to in-memory cache: todo
  EP_H5VDS = 6,   //H5 VDS: todo
  EP_DIR = 7,     //Directory to data to process. file format to be detected
  EP_DASSA = 8,   //customized DASSA IO
  EP_TDMS = 9,    //tdms file format (to binary file)
} AuEndpointType;
/**
 * @brief map string typed name of type to AuEndpointType
 * 
 * @param endpoint_type_str input of string name
 * @return AuEndpointType output of type 
 */
AuEndpointType MapString2EndpointType(std::string endpoint_type_str);

//See https://support.hdfgroup.org/ftp/HDF5/current/src/unpacked/src/H5Tpublic.h
//for reference
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
  AU_DOUBLE_COMPLEX = 10, //it is std::complex<double>
  AU_STRING = 11,         //mostly used to store attribute
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
    return AU_NO_TYPE; //Here it might be User-defined class
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
//Todo: we can infer "vsize" in futher version
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

//1: vector type
//0: other types
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

//1: vector type
//0: other types
template <typename T>
bool InferVectorVectorType()
{
  return is_vector_vector<T>{};
}

//see more detail in third_party/cista.h
#define AU_UDT_INIT(A) FT_UDT_INIT(A)

//see more detail in third_party/cista.h
#define FT_UDT_INIT(A) \
  CISTA_PRINTABLE(A)   \
  CISTA_COMPARABLE()
#endif
