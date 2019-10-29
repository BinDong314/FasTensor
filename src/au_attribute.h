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

#ifndef ARRAY_UDF_ATTRIBUTE
#define ARRAY_UDF_ATTRIBUTE

typedef enum AuEndpointType
{
  EP_HDF5 = 0,    //from/to HDF5
  EP_NETCDF = 1,  //from/to NETCDF
  EP_AUDIOS = 2,  //from/to AUDIOS
  EP_BINARY = 3,  //from/to binary file
  EP_VIRTUAL = 4, //from/to Virtual array of above types
  EP_IARRAY = 5,  //from/to in-memory cache
  EP_IVECTOR = 6, //from/to from CXX vector
  EP_H5VDS = 7,   //from/to from CXX vector
} AuEndpointType;

//See https://support.hdfgroup.org/ftp/HDF5/current/src/unpacked/src/H5Tpublic.h
//for reference
typedef enum AuEndpointDataType
{
  AU_NO_TYPE = -1, /*error                                      */
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
  AU_NCLASSES /*this must be last                          */
} AuEndpointDataType;

//See https://support.hdfgroup.org/ftp/HDF5/current/src/unpacked/src/H5Tpublic.h
//for reference
typedef enum AType
{
  AU_NO_TYPE = -1,  /*error                                      */
  AU_INTEGER = 0,   /*integer types                              */
  AU_FLOAT = 1,     /*floating-point types                       */
  AU_TIME = 2,      /*date and time types                        */
  AU_STRING = 3,    /*character string types                     */
  AU_BITFIELD = 4,  /*bit field types                            */
  AU_OPAQUE = 5,    /*opaque types                               */
  AU_COMPOUND = 6,  /*compound types                             */
  AU_REFERENCE = 7, /*reference types                            */
  AU_ENUM = 8,      /*enumeration types                          */
  AU_VLEN = 9,      /*Variable-Length types                      */
  AU_NCLASSES       /*this must be last                          */
} AUType;

typedef enum NVSFile
{
  AU_HDF5 = 0,   //Data comes HDF5
  AU_NETCDF = 1, //Data comes NETCDF
  AU_AUDIOS = 2, //Data comes AUDIOS
  AU_BINARY = 3, //Data comes from binary file
} NVSFile;

typedef enum DataOrigin
{
  AU_NVS = 0,       //Data comes from/to disk
  AU_COMPUTED = 1,  //Data comes from UDF
  AU_NV = 2,        //Data comes from memory
  AU_VIRTUAL = 3,   //It is a virtual array with attributes as array.
  AU_NV_VECTOR = 4, //It is C++ vector (in memory)
} DataOrigin;

typedef enum CacheFlag
{
  AU_NOCACHE = 0,
  AU_CACHED = 1, //All data are stored in memory as global array.
} CacheFlag;

typedef enum GlobalArrayOperation
{
  AU_PRELOAD = 0,
} GlobalArrayOperation;

typedef struct UDFAttribute
{
  void *gs_data_r; //global shared data for reading. It's better to be an array in Global Array Toolkit;
  void *gs_data_w; //global shared data for writing. It's better to be an array in Global Array Toolkit;
  void *local_reduce_var;
  MPI_Op local_reduce_op;
  int *skip_before;
  int *skip_after;
} UDFAttribute;

/*
//NVS: non volatile storage
typedef struct NVSInfo{
  DataFileType type; 
  char *h5_file;
  char *h5_group;
  char *h5_dataset;
}NVSInfo;


#define INIT_NVSINFO {AU_HDF5, NULL, NULL, NULL}



typedef enum CacheFlag{
  AU_NOCACHE=0,
  AU_CACHED=1, //All data are stored in memory as global array. 
  //CACHED_GA_DISK=2, //Not used
}CacheFlag;

//VS: volatile storage 
typedef struct VSInfo{
  int ga_handle;  //The handle to array stored in Global Array Toolkit 
}VSInfo;

#define INIT_VSINFO {-1}

typedef struct ArrayAttribute{
  DataSourceType      data_src_t;  //where the data comes from
  NVSInfo            *nvs_i;       //nonvolatile storage information for the array
  CacheFlag           cache_flag;  //Data is cached in volatile storage 
  VSInfo             *vs_i;        //valid when cache_flag != 0
  int                 rank;
  unsigned long long *dims;
  int                *chunk_size;
  int                *overlap_size;
  void               *operation;   //record the operation to create this file; For further 
}ArrayAttribute;

#define INIT_ARRAYATTRIBUTE {AU_DISK, NULL, AU_NOCACHE, NULL, 0, NULL, NULL, NULL, NULL}
*/

#endif
