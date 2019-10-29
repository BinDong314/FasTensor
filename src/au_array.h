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

#ifndef ARRAY_UDF_ARRAY_H
#define ARRAY_UDF_ARRAY_H

#include <assert.h>
#include "mpi.h"
#include "au_endpoint.h"
#include "au_stencil.h"
#include "au_utility.h"
#include "au_attribute.h"
#include "au_endpoint_factory.h"


namespace AU
{
template <class T, class AttributeType = T>
class Array
{
private:
  /**
   * @brief array_data_endpoint_info has the format
   *        AuDataEndpointType : [other information]
   *        [other information] -- AU_HDF5 --> FileName:GroupName:DatasetName
   *                            -- AU_NETCDF --> Todo
   *                            -- AU_AUDIOS --> Todo
   *                            -- AU_BINARY --> Todo
   *                            -- AU_VIRTUAL --> NULL
   *                            -- AU_IARRAY  --> NULL
   *                            -- AU_CXXVECTOR --> NULL
   */
  std::string array_data_endpoint_info; 
  std::vector<int> data_chunk_size;     //size of each chunk (user provide)
  std::vector<int> data_overlap_size;   //size of overlapping  (user provide)
  int data_auto_chunk_dim_index;

public:
  /**
   * @brief Construct a new Array object for Write
   * The data can be cached or dumped later
   */
  Array(){

  };

  //Below are three constructors for file based constructor

  /**
   * @brief Construct a new Array object for either Input or Output
    *       For Input, data_endpoint is opened before Apply
    *       For Ouput, data_endpoint is created during Apply
    * @param data_endpoint file information, ("AuDataEndpointType + [file name]")
    *        data_endpoint  get chunk infro from Apply
    *          
    */
  Array(std::string data_endpoint)
  {
    array_data_endpoint_info = data_endpoint;
  }

  /**
   * @brief Construct a new Array object for read, as Input of Apply 
   * 
   * @param data_endpoint file information ("AuDataEndpointType + file name") 
   * @param auto_chunk_dim_index  fileinfo is chunked on auto_chunk_dim_index
   */
  Array(std::string data_endpoint, int auto_chunk_dim_index)
  {
    array_data_endpoint_info = data_endpoint;
    data_auto_chunk_dim_index = auto_chunk_dim_index;
  }

  /**
   * @brief Construct a new Array object for read, as Input of Apply
   * 
   * @param data_endpoint contains file info, ("AuDataEndpointType + file name")
   * @param cs , chunk size
   * @param os , ghost size
   */
  Array(std::string data_endpoint, std::vector<int> cs, std::vector<int> os)
  {
    array_data_endpoint_info = data_endpoint;
    data_chunk_size = cs;
    data_overlap_size = os;
  }

  /**
   * @brief Construct a new Array object from in-memory vector
   *        The data are assumed to be 1D too here
   * @param data_vector_endpoint the data to intialize 
   */
  Array(std::vector<T> &data_vector_endpoint)
  {
  }

  /**
   * @brief Construct a new Array object
   * 
   * @param data_vector_endpoint 
   * @param cs 
   * @param os 
   */
  Array(std::vector<T> &data_vector_endpoint, std::vector<int> cs, std::vector<int> os)
  {
  }

  void CalculateChunkSize(std::vector<unsigned long long> &data_size, std::vector<unsigned long long> &chunk_size)
  {
  }

  void FinalizeApplyOutput()
  {
  }

  void InitializeApplyInput()
  {
  }
}; // namespace AU

#endif
}
