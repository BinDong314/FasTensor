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

// see au.h for its definations
// extern int ft_size;
// extern int ft_rank;
extern int ft_size;
extern int ft_rank;

#ifndef ARRAY_UDF_ARRAY_H
#define ARRAY_UDF_ARRAY_H

#include "ft_array_transpose.h"
#include "ft_chunk_scheduling.h"
#include "ft_endpoint.h"
#include "ft_endpoint_factory.h"
#include "ft_merge.h"
#include "ft_mpi.h"
#include "ft_output_vector.h"
#include "ft_stencil.h"
#include "ft_type.h"
#include "ft_utility.h"
#include "ft_vis.h"
#include "mpi.h"
#include <assert.h>
#include <limits>
#include <regex>
#include <stdarg.h>

#if defined(_OPENMP)
#include <omp.h>
#endif

// std::vector<Endpoint *> endpoint_clean_vector;
// extern std::map<Endpoint *, bool> endpoint_clean_vector;

// #define AU_DEFAULT_CHUNK_SIZE_PER_DIM 1000
//  extern std::map<Endpoint *, bool> endpoint_clean_vector;

extern std::vector<Endpoint *> endpoint_to_clean_vector;

namespace FT {

/**
 * @brief transform the ArrayBase object to one with the type A_TYPE
 *        can call the transform function
 *
 */
#define TRANSFORM(A_BASE_OBJECT, UDF, B, A_TYPE, UDF_OUT_TYPE)                 \
  {                                                                            \
    switch (A_TYPE) {                                                          \
    case AuEndpointDataType::AU_SHORT:                                         \
      static_cast<FT::Array<short> *>(A_BASE_OBJECT)                           \
          ->Transform<UDF_OUT_TYPE>(UDF, B);                                   \
      break;                                                                   \
    case AuEndpointDataType::AU_DOUBLE:                                        \
      static_cast<FT::Array<double> *>(A_BASE_OBJECT)                          \
          ->Transform<UDF_OUT_TYPE>(UDF, B);                                   \
      break;                                                                   \
    case AuEndpointDataType::AU_FLOAT:                                         \
      static_cast<FT::Array<float> *>(A_BASE_OBJECT)                           \
          ->Transform<UDF_OUT_TYPE>(UDF, B);                                   \
      break;                                                                   \
    case AuEndpointDataType::AU_INT:                                           \
      static_cast<FT::Array<int> *>(A_BASE_OBJECT)                             \
          ->Transform<UDF_OUT_TYPE>(UDF, B);                                   \
      break;                                                                   \
    default:                                                                   \
      AU_EXIT("Not supported type yet in TRANSFORM macro !");                  \
    }                                                                          \
  }

#define TRANSFORM_NO_MP(A_BASE_OBJECT, UDF, B, A_TYPE, UDF_OUT_TYPE)           \
  {                                                                            \
    switch (A_TYPE) {                                                          \
    case AuEndpointDataType::AU_SHORT:                                         \
      static_cast<FT::Array<short> *>(A_BASE_OBJECT)                           \
          ->TransformNoMP<UDF_OUT_TYPE>(UDF, B);                               \
      break;                                                                   \
    case AuEndpointDataType::AU_DOUBLE:                                        \
      static_cast<FT::Array<double> *>(A_BASE_OBJECT)                          \
          ->TransformNoMP<UDF_OUT_TYPE>(UDF, B);                               \
      break;                                                                   \
    case AuEndpointDataType::AU_FLOAT:                                         \
      static_cast<FT::Array<float> *>(A_BASE_OBJECT)                           \
          ->TransformNoMP<UDF_OUT_TYPE>(UDF, B);                               \
      break;                                                                   \
    case AuEndpointDataType::AU_INT:                                           \
      static_cast<FT::Array<int> *>(A_BASE_OBJECT)                             \
          ->TransformNoMP<UDF_OUT_TYPE>(UDF, B);                               \
      break;                                                                   \
    default:                                                                   \
      AU_EXIT("Not supported type yet in TRANSFORM macro !");                  \
    }                                                                          \
  }

/**
 * @brief the object to the ArrayBase
 *
 */
class ArrayBase {
public:
  virtual int EnableApplyStride(const std::vector<int> &skip_size_p) = 0;
  virtual void
  SetVectorDirection(OutputVectorFlatDirection flat_direction_index) = 0;
  virtual void ReportCost() = 0;
  virtual int ControlEndpoint(int cmd_p) = 0;
  virtual int ControlEndpoint(int cmd_p, std::vector<std::string> &arg_v_p) = 0;
  virtual int SetChunkSize(std::vector<int> data_chunk_size_p) = 0;
  virtual int SetOverlapSize(const vector<int> os_p) = 0;
  virtual int GetArraySize(std::vector<unsigned long long> &size_p) = 0;
  virtual int GetStencilTag() = 0;
  virtual int GetTag(const std::string &name, std::string &value) = 0;
  virtual void SetView(unsigned long long start, unsigned long long count,
                       int rank) = 0;
  virtual void SetView(std::vector<unsigned long long> start,
                       std::vector<unsigned long long> count) = 0;
  virtual void DisableOverlapLower() = 0;
  virtual void DisableOverlapUpper() = 0;
  virtual void SkipFileTail() = 0;
  virtual void ExecuteUDFOnce() = 0;
  virtual void GetMyChunkStartEnd(unsigned long long &start,
                                  unsigned long long &end) = 0;
  virtual void EnableCollectiveIO() = 0;
  virtual void DisableCollectiveIO() = 0;
  virtual void EnableWriteRootOnly() = 0;
  virtual void DisableWriteRootOnly() = 0;
  virtual ~ArrayBase() = default;
};

template <class T> class Array : public ArrayBase {
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
  Endpoint *endpoint = NULL;

  // Below are info about endpooint
  int data_dims; // The number of dimensioins of data in endpoint
  std::vector<unsigned long long>
      data_size; // The size of each dimension (global, extracted from data of
                 // endpoint
  std::vector<int> data_chunk_size;   // size of each chunk (user provide)
  std::vector<int> data_overlap_size; // size of overlapping  (user provide)
  std::vector<unsigned long long>
      data_chunked_size; // The number of chunks per dimenstion
  int data_auto_chunk_dim_index;
  unsigned long long data_total_chunks; // The total number of chunks (global)

  std::vector<unsigned long long>
      current_chunk_start_offset; // Start offset on disk
  std::vector<unsigned long long>
      current_chunk_end_offset; // End offset on disk
  std::vector<unsigned long long>
      current_chunk_size; // Size of the chunk, euqal to end_offset -
                          // start_offset
  unsigned long long
      current_chunk_cells; // The number of cells in current chunk

  std::vector<unsigned long long>
      current_result_chunk_start_offset; // Start offset on disk
  std::vector<unsigned long long>
      current_result_chunk_end_offset; // End offset on disk
  unsigned long long
      current_result_chunk_cells; // The number of cells in current chunk

  std::vector<unsigned long long>
      current_chunk_ol_start_offset; // Start offset on disk with overlapping
  std::vector<unsigned long long>
      current_chunk_ol_end_offset; // End offset on disk with overlapping
  std::vector<unsigned long long>
      current_chunk_ol_size; // Size of the chunk, euqal to end_offset -
                             // start_offset
  unsigned long long
      current_chunk_ol_cells; // The number of cells in current chunk

  unsigned long long
      current_chunk_id; // Id of the current chunk (in memory) to apply UDF
  unsigned long long prev_chunk_id; // for SetChunkID of CellTarget
  std::vector<int> skip_size;       // Size to ship after each operation
  std::vector<unsigned long long>
      skiped_dims_size;                          // Size of the data after skip
  std::vector<unsigned long long> skiped_chunks; // # of chunks after skip
  std::vector<int> skiped_chunk_size; // Size of each chunk after skip

  std::vector<T> current_chunk_data; // Pointer to data of current chunk

  std::vector<long long>
      ol_origin_offset; // Size of the chunk, euqal to end_offset - start_offset

  std::vector<unsigned long long> view_start_offset;
  std::vector<unsigned long long> view_size;
  std::vector<unsigned long long> view_orginal_data_dims_size;
  int view_rank;

  std::vector<T> mirror_values;

  OutputVectorFlatDirection output_vector_flat_direction_index;

  std::vector<size_t> output_vector_flat_shape;

  std::vector<std::string> attribute_array_data_endpoint_info;

  std::vector<Endpoint *>
      attribute_endpoint_vector; // vector of endpoints for a virtual array

  // For directory
  std::regex dir_input_regex, dir_output_regex;
  std::string dir_output_replace_str;

  // For chunk operations
  size_t set_chunk_size_by_mem_max_mem_size = 1073741824; // Byte, 1GB

  // Flag variable for different purposes
  bool skip_flag = false;
  bool view_flag = false;
  bool is_view_rank = false;
  bool cpp_vec_flag = false;
  bool virtual_array_flag = false;
  bool save_result_flag = true;
  bool reverse_apply_direction_flag = false;
  bool mirror_value_flag = false;
  bool apply_replace_flag = false;
  bool vector_type_flag = false;
  bool endpoint_memory_flag = false;
  bool has_padding_value_flag = false;
  bool skip_not_aligned_w_array_flag = false;
  bool is_the_last_chunk = false;
  bool is_endpoint_created_flag = false;

  bool chunk_size_by_user_flag = false;
  bool chunk_size_by_user_by_dimension_flag = false;
  bool set_chunk_size_by_mem_flag = false;
  bool set_overlap_size_by_auto_detection_flag = false;
  bool variable_chunking_from_file_flag =
      false; // mostly used for streaming data with binary input

  bool is_disable_overlap_upper_set = false;
  bool is_disable_overlap_lower_set = false;

  bool set_direct_output_flag = false;
  bool is_skip_tail_chunk = false;
  bool is_execute_udf_once = false;
  bool is_write_root_only = false;
  // is init function called in ReadNextChunk function
  bool is_init_called_in_rnc = false;

  // bool is_set_chunk_scheduling = false;
  ChunkSchedulingMethod scheduling_method = CHUNK_SCHEDULING_RR;

  bool get_stencil_tag_flag = false;
  std::map<std::string, std::string> stencil_metadata_map;

  bool has_output_stencil_tag_flag = false;
  std::map<std::string, std::string> output_stencil_metadata_map;

  int skip_not_aligned_w_array_index;
  // The shape of output_vector when vector_type_flag = true
  std::vector<size_t> output_vector_shape;
  std::vector<size_t> previous_output_vector_shape;
  // float input_output_ratio; // the ratio = size of input / size of output,
  // only valid along output_vector_shape padding_value_p
  T padding_value;

  // help variable
  AU_WTIME_TYPE t_start, time_read = 0, time_udf = 0, time_write = 0,
                         time_create = 0, time_sync = 0, time_nonvolatile = 0;

public:
  /**
   * @brief Construct a new Array object for Write
   * The data can be cached or dumped later
   */
  Array() {

  };

  // Below are three constructors for file based constructor

  /**
   * @brief Construct a new Array object for either Input or Output
   *       For Input, data_endpoint is opened before Apply
   *       For Ouput, data_endpoint is created during Apply
   * @param data_endpoint file information, ("AuDataEndpointType + [file name]")
   *        data_endpoint  get chunk infro from Apply
   *
   */
  Array(std::string data_endpoint) {
    // printf("Debug: Here at INIT\n");
    array_data_endpoint_info = data_endpoint;
    endpoint = EndpointFactory::NewEndpoint(data_endpoint);
    AuEndpointDataType data_element_type = InferDataType<T>();
    endpoint->SetDataElementType(data_element_type);

    if (endpoint->GetEndpointType() == EP_MEMORY)
      endpoint_memory_flag = true;
    // endpoint_clean_vector[endpoint] = true;
    endpoint_to_clean_vector.push_back(endpoint);
  }

  /**
   * @brief Construct a new Array object for read, as Input of Apply
   *
   * @param data_endpoint contains file info, ("AuDataEndpointType + file name")
   * @param cs , chunk size
   * @param os , ghost size
   */
  Array(std::string data_endpoint, std::vector<int> cs) {
    array_data_endpoint_info = data_endpoint;
    data_chunk_size = cs;
    data_overlap_size.resize(cs.size());
    std::fill(data_overlap_size.begin(), data_overlap_size.end(), 0);

    endpoint = EndpointFactory::NewEndpoint(data_endpoint);

    AuEndpointDataType data_element_type = InferDataType<T>();
    endpoint->SetDataElementType(data_element_type);

    chunk_size_by_user_flag = true;
    if (endpoint->GetEndpointType() == EP_MEMORY)
      endpoint_memory_flag = true;

    // endpoint_clean_vector.push_back(endpoint);
    // endpoint_clean_vector[endpoint] = true;
    endpoint_to_clean_vector.push_back(endpoint);
  }

  /**
   * @brief Construct a new Array object for read, as Input of Apply
   *
   * @param data_endpoint contains file info, ("AuDataEndpointType + file name")
   * @param cs , chunk size
   * @param os , ghost size
   */
  Array(std::string data_endpoint, std::vector<int> cs, std::vector<int> os) {
    array_data_endpoint_info = data_endpoint;
    data_chunk_size = cs;
    data_overlap_size = os;
    endpoint = EndpointFactory::NewEndpoint(data_endpoint);

    AuEndpointDataType data_element_type = InferDataType<T>();
    endpoint->SetDataElementType(data_element_type);

    chunk_size_by_user_flag = true;
    if (endpoint->GetEndpointType() == EP_MEMORY)
      endpoint_memory_flag = true;

    // endpoint_clean_vector.push_back(endpoint);
    // endpoint_clean_vector[endpoint] = true;
    endpoint_to_clean_vector.push_back(endpoint);
  }

  /**
   * @brief Construct a new Array object for read, as Input of Apply
   *
   * @param data_endpoint file information ("AuDataEndpointType + file name")
   * @param auto_chunk_dim_index  fileinfo is chunked on auto_chunk_dim_index
   */
  Array(std::string data_endpoint, int auto_chunk_dim_index) {
    array_data_endpoint_info = data_endpoint;
    data_auto_chunk_dim_index = auto_chunk_dim_index;
    endpoint = EndpointFactory::NewEndpoint(data_endpoint);
    // std::cout << data_endpoint << "\n";
    AuEndpointDataType data_element_type = InferDataType<T>();
    endpoint->SetDataElementType(data_element_type);
    chunk_size_by_user_by_dimension_flag = true;
    if (endpoint->GetEndpointType() == EP_MEMORY)
      endpoint_memory_flag = true;

    // endpoint_clean_vector[endpoint] = true;
    endpoint_to_clean_vector.push_back(endpoint);
  }

  /**
   * @brief Construct a new Array object for read, as Input of Apply
   *
   * @param data_endpoint contains file info, ("AuDataEndpointType + file name")
   * @param cs , chunk size
   * @param os , ghost size
   */
  Array(std::string data_endpoint, std::vector<unsigned long long> size_p) {
    array_data_endpoint_info = data_endpoint;
    endpoint = EndpointFactory::NewEndpoint(data_endpoint);
    AuEndpointDataType data_element_type = InferDataType<T>();
    endpoint->SetDataElementType(data_element_type);
    endpoint->SetDimensions(size_p);

    if (endpoint->GetEndpointType() == EP_MEMORY) {
      endpoint_memory_flag = true;
      endpoint->Create();
    }

    data_size = size_p;
    // endpoint_clean_vector[endpoint] = true;
    endpoint_to_clean_vector.push_back(endpoint);
  }

  /**
   * @brief Construct a new Array object with only chunk size and overlap size
   *         Mostly, this is used for virtual array which has uniform chunk size
   * and overlap size
   * @param cs chunk size
   * @param os overlap size
   */
  Array(std::vector<int> cs, std::vector<int> os) {
    data_chunk_size = cs;
    data_overlap_size = os;
  }

  Array(std::vector<int> cs) {
    data_chunk_size = cs;
    data_overlap_size.resize(cs.size());
    std::fill(data_overlap_size.begin(), data_overlap_size.end(), 0);
  }
  /**
   * @brief Construct a new Array object from in-memory vector
   *        The data are assumed to be 1D too here
   * @param data_vector_endpoint the data to intialize
   */
  Array(std::vector<T> &data_vector_endpoint) {}

  /**
   * @brief Construct a new Array object
   *
   * @param data_vector_endpoint
   * @param cs
   * @param os
   */
  Array(std::vector<T> &data_vector_endpoint, std::vector<int> cs,
        std::vector<int> os) {}

  /**
   * @brief Construct a new Array object from in-memory vector
   *        The data are assumed to be 1D too here
   * @param data_vector_endpoint the data to intialize
   */
  ~Array() {
    // endpoint_clean_vector[endpoint] = false;
    // if (endpoint != NULL)
    //   delete endpoint;
    // endpoint = NULL;
  }

  /**
   * @brief Update the chunk size, given
   * chunk_size_by_user_flag/chunk_size_by_user_by_dimension_flag
   *
   * @param chunk_size_p
   * @param overlap_size_p
   * @param data_size
   */
  void UpdateChunkSize() {
    // partition by dimensions
    if (chunk_size_by_user_by_dimension_flag) {
      // data_chunked_size.resize(data_dims);
      data_chunk_size.resize(data_dims);
      // overlap_size_p.resize(data_dims);
      for (int i = 0; i < data_dims; i++) {
        if (data_auto_chunk_dim_index == i) {
          if (data_size[i] % ft_size == 0) {
            data_chunk_size[i] = data_size[i] / ft_size;
          } else {
            data_chunk_size[i] = data_size[i] / ft_size + 1;
          }
        } else {
          data_chunk_size[i] = data_size[i];
        }
        // data_overlap_size[i] = 0;
      }

      return;
    }

    if (set_chunk_size_by_mem_flag) {
      unsigned long long total_elements = 1;
      for (int i = 0; i < data_dims; i++) {
        total_elements = data_size[i] * total_elements;
      }
      total_elements = total_elements / ft_size;
      if (total_elements > (set_chunk_size_by_mem_max_mem_size / sizeof(T))) {
        total_elements = set_chunk_size_by_mem_max_mem_size / sizeof(T);
      }
      std::vector<unsigned long long> chunk_size_temp =
          RowMajorOrderReverse(total_elements, data_size);
      int replace_flag = 1;
      for (int i = data_dims - 1; i > 0; i--) {
        if (chunk_size_temp[i] != data_size[i]) {
          chunk_size_temp[i] = data_size[i];
          if (chunk_size_temp[i - 1] != 0) {
            chunk_size_temp[i - 1] = chunk_size_temp[i - 1] - 1;
          } else {
            replace_flag = 0;
            break;
          }
        }
      }

      for (int i = data_dims - 1; i >= 0; i--) {
        if (chunk_size_temp[i] == 0)
          replace_flag = 0;
      }

      if (replace_flag) {
        // unsigned long long to int here
        for (int i = 0; i < data_dims; i++)
          data_chunk_size[i] = chunk_size_temp[i];
      } else {
        AU_EXIT("SetChunkSizeByMem failed ! Please try specify the chunk size "
                "via SetChunkSize !");
      }
      return;
    }

    if (endpoint != NULL && variable_chunking_from_file_flag) {
      // chunk_size_p = endpoint->GetChunkSize();
      std::vector<std::string> file_size_vector;
      endpoint->Control(DIR_GET_FILE_SIZE, file_size_vector);
      String2Vector(file_size_vector[0], data_chunk_size);
      data_overlap_size.resize(data_chunk_size.size(), 0);
    }
    // optimal chunk_size
  }

  inline int SetOverlapSize(const vector<int> os_p) {
    data_overlap_size = os_p;
    return 0;
  }
  inline int SetOverlapSizeByDetection() {
    set_overlap_size_by_auto_detection_flag = true;
    return 0;
  }
  inline int GetOverlapSize(vector<int> &os_p) {
    os_p = data_overlap_size;
    return 0;
  }
  inline int SetOverlapPadding(const T &padding_value_p) {
    has_padding_value_flag = true;
    padding_value = padding_value_p;
    return 0;
  }

  inline int SyncOverlap() {
    if (endpoint != NULL) {
      if (endpoint->GetEndpointType() == EP_MEMORY) {
        std::vector<std::string> arg_v;
        return endpoint->Control(MEMORY_SYNC_OVERLAP, arg_v);
      }
      return 0;
    }

    return 0;
  }

  template <class UDFType> int UpdateOverlapSize(UDFType UDF) {
    if (endpoint != NULL) {
      //        if (endpoint->GetEndpointType() == EP_DIR ||
      //        chunk_size_by_user_by_dimension_flag)
      if (chunk_size_by_user_by_dimension_flag) {
        data_overlap_size.resize(data_dims);
        for (int i = 0; i < data_dims; i++) {
          data_overlap_size[i] = 0;
        }
      }
      return 0;
    }

    if (set_overlap_size_by_auto_detection_flag) {
      std::vector<T> trail_data;
      trail_data.resize(1);
      Stencil<T> trail_cell(data_dims, &trail_data[0]);
      UDF(trail_cell);
      trail_cell.GetTrailRunResult(data_overlap_size);
    }
    // optimal overlap size
    return 0;
  }

  template <class UDFType> void InitializeApplyInput(UDFType UDF) {
    // Read the metadata (rank, dimension size) from endpoint
    if (virtual_array_flag && attribute_endpoint_vector.size() >= 1) {
      // Get the data_size from first attribute
      attribute_endpoint_vector[0]->ExtractMeta();
      data_size = attribute_endpoint_vector[0]->GetDimensions();
      data_dims = data_size.size();
      for (int i = 1; i < attribute_endpoint_vector.size(); i++) {
        attribute_endpoint_vector[i]->ExtractMeta();
        std::vector<unsigned long long> data_size_p =
            attribute_endpoint_vector[i]->GetDimensions();

        if (data_size != data_size_p) {
          AU_EXIT("All attributes must have same size");
        }
      }
    } else {
      // endpoint->ExtractMeta();
      // data_size = endpoint->GetDimensions();
      // data_dims = data_size.size();
      std::cout << "GetEndpointType() = " << GetEndpointType() << "\n";
      if (GetEndpointType() == EP_DIR_STREAM) {
        endpoint->ExtractMeta();
        data_size = endpoint->GetDimensions();
        data_dims = data_size.size();
        std::vector<std::string> para;
        endpoint->Control(DIR_GET_MERGE_INDEX, para);
        int merge_dim_index = std::stoi(para[0]);
        data_size[merge_dim_index] =
            std::numeric_limits<unsigned long long>::max();
        std::cout << "max_per_dim = "
                  << std::numeric_limits<unsigned long long>::max() << "\n";
      } else if (GetEndpointType() == EP_RabbitMQ ||
                 GetEndpointType() == EP_RabbitMQ_RESTAPI) {
        data_dims = data_chunk_size.size();
        if (data_dims == 0) {
          throw std::runtime_error("Error: data_dims cannot be zero.");
        }
        unsigned long long max_value =
            std::numeric_limits<unsigned long long>::max();
        double result = pow(max_value, 1.0 / data_dims);
        unsigned long long max_per_dim = std::floor(result);
        data_size = std::vector<unsigned long long>(data_dims, max_per_dim);
        std::cout << "max_per_dim = " << max_per_dim << "\n";
      } else {
        endpoint->ExtractMeta();
        data_size = endpoint->GetDimensions();
        data_dims = data_size.size();
      }
    }

    if (view_flag) {
      if (view_rank) {
        data_size[view_rank] = view_size[0];
      } else {
        data_size = view_size;
      }
      view_orginal_data_dims_size = data_size;
    }

    UpdateChunkSize();
    UpdateOverlapSize(UDF);

    // PrintVector("data_chunk_size: ", data_chunk_size);
    // PrintVector("data_size: ", data_size);

    current_chunk_start_offset.resize(data_dims);
    current_chunk_end_offset.resize(data_dims);
    current_chunk_size.resize(data_dims);

    current_result_chunk_start_offset.resize(data_dims);
    current_result_chunk_end_offset.resize(data_dims);

    current_chunk_ol_start_offset.resize(data_dims);
    current_chunk_ol_end_offset.resize(data_dims);
    current_chunk_ol_size.resize(data_dims);

    data_chunked_size.resize(data_dims);
    ol_origin_offset.resize(data_dims);

    data_total_chunks = 1;

    for (int i = 0; i < data_dims; i++) {
      if (data_size[i] % data_chunk_size[i] == 0 || is_skip_tail_chunk) {
        data_chunked_size[i] = data_size[i] / data_chunk_size[i];
      } else {
        data_chunked_size[i] = data_size[i] / data_chunk_size[i] + 1;
      }
      data_total_chunks = data_total_chunks * data_chunked_size[i];
    }

    if (skip_flag) {
      skiped_dims_size.resize(data_dims);
      skiped_chunks.resize(data_dims);
      skiped_chunk_size.resize(data_dims);

      for (int i = 0; i < data_dims; i++) {

        /*if (data_size[i] % skip_size[i] != 0 || data_chunk_size[i] %
      skip_size[i] != 0)
      {
        AU_EXIT("Strip size must be aligned with size of both array and chunk !
      \n");
      }*/

        if (data_chunk_size[i] % skip_size[i] != 0) {
          AU_EXIT("Strip size must be aligned with the size of chunk ! \n");
        }

        // Fix me: here we assume skip_size needs to be aligned
        //         with data_size when is_skip_tail_chunk is set
        if ((data_size[i] % skip_size[i] != 0) &&
            (is_skip_tail_chunk == false)) {
          skip_not_aligned_w_array_flag = true;
          skip_not_aligned_w_array_index = i;
          skiped_dims_size[i] = data_size[i] / skip_size[i] + 1;
        } else {
          skiped_dims_size[i] = data_size[i] / skip_size[i];
        }

        skiped_chunk_size[i] = data_chunk_size[i] / skip_size[i];
        if (skiped_dims_size[i] % skiped_chunk_size[i] != 0) {
          skiped_chunks[i] = skiped_dims_size[i] / skiped_chunk_size[i] + 1;
        } else {
          skiped_chunks[i] = skiped_dims_size[i] / skiped_chunk_size[i];
        }
      }
    }

    // current_chunk_id = ft_rank; // Each process deal with one chunk one time,
    // starting from its rank
    current_chunk_id = InitFirstChunk();

    // #ifdef DEBUG
    if (ft_rank == 0) {
      if (!virtual_array_flag)
        endpoint->PrintInfo();
      PrintVector("       data size", data_size);
      PrintVector("      chunk size", data_chunk_size);
      PrintVector("    overlap size", data_overlap_size);
      PrintScalar("current chunk id:", current_chunk_id);
      PrintScalar("    total chunks", data_total_chunks);
    }
  }

  template <class UDFOutputType>
  void InitializeApplyInputTest(int (*UDF)(const Stencil<T> &,
                                           Stencil<UDFOutputType> &)) {
    // Read the metadata (rank, dimension size) from endpoint
    if (virtual_array_flag && attribute_endpoint_vector.size() >= 1) {
      // Get the data_size from first attribute
      attribute_endpoint_vector[0]->ExtractMeta();
      data_size = attribute_endpoint_vector[0]->GetDimensions();
      data_dims = data_size.size();
      for (int i = 1; i < attribute_endpoint_vector.size(); i++) {
        attribute_endpoint_vector[i]->ExtractMeta();
        std::vector<unsigned long long> data_size_p =
            attribute_endpoint_vector[i]->GetDimensions();

        if (data_size != data_size_p) {
          AU_EXIT("All attributes must have same size");
        }
      }
    } else {
      if (GetEndpointType() == EP_RabbitMQ ||
          GetEndpointType() == EP_RabbitMQ_RESTAPI ||
          GetEndpointType() == EP_DIR_STREAM) {
        data_dims = data_chunk_size.size();
        if (data_dims == 0) {
          throw std::runtime_error("Error: data_dims cannot be zero.");
        }
        unsigned long long max_value =
            std::numeric_limits<unsigned long long>::max();
        double result = pow(max_value, 1.0 / data_dims);
        unsigned long long max_per_dim = std::floor(result);
        data_size = std::vector<unsigned long long>(data_dims, max_per_dim);
        std::cout << "max_per_dim = " << max_per_dim << "\n";
      } else {
        endpoint->ExtractMeta();
        data_size = endpoint->GetDimensions();
        data_dims = data_size.size();
      }
    }

    UpdateChunkSize();
    // UpdateOverlapSize(UDF);

    PrintVector("data_chunk_size: ", data_chunk_size);
    PrintVector("data_size: ", data_size);

    current_chunk_start_offset.resize(data_dims);
    current_chunk_end_offset.resize(data_dims);
    current_chunk_size.resize(data_dims);

    current_result_chunk_start_offset.resize(data_dims);
    current_result_chunk_end_offset.resize(data_dims);

    current_chunk_ol_start_offset.resize(data_dims);
    current_chunk_ol_end_offset.resize(data_dims);
    current_chunk_ol_size.resize(data_dims);

    data_chunked_size.resize(data_dims);
    ol_origin_offset.resize(data_dims);

    data_total_chunks = 1;

    for (int i = 0; i < data_dims; i++) {
      if (data_size[i] % data_chunk_size[i] == 0) {
        data_chunked_size[i] = data_size[i] / data_chunk_size[i];
      } else {
        data_chunked_size[i] = data_size[i] / data_chunk_size[i] + 1;
      }
      data_total_chunks = data_total_chunks * data_chunked_size[i];
    }

    if (skip_flag) {
      skiped_dims_size.resize(data_dims);
      skiped_chunks.resize(data_dims);
      skiped_chunk_size.resize(data_dims);

      for (int i = 0; i < data_dims; i++) {

        /*if (data_size[i] % skip_size[i] != 0 || data_chunk_size[i] %
      skip_size[i] != 0)
      {
        AU_EXIT("Strip size must be aligned with size of both array and chunk !
      \n");
      }*/

        if (data_chunk_size[i] % skip_size[i] != 0) {
          AU_EXIT("Strip size must be aligned with the size of chunk ! \n");
        }

        if (data_size[i] % skip_size[i] != 0) {
          skip_not_aligned_w_array_flag = true;
          skip_not_aligned_w_array_index = i;
          skiped_dims_size[i] = data_size[i] / skip_size[i] + 1;
        } else {
          skiped_dims_size[i] = data_size[i] / skip_size[i];
        }

        skiped_chunk_size[i] = data_chunk_size[i] / skip_size[i];
        if (skiped_dims_size[i] % skiped_chunk_size[i] != 0) {
          skiped_chunks[i] = skiped_dims_size[i] / skiped_chunk_size[i] + 1;
        } else {
          skiped_chunks[i] = skiped_dims_size[i] / skiped_chunk_size[i];
        }
      }
    }

    current_chunk_id = ft_rank; // Each process deal with one chunk one time,
                                // starting from its rank

    // #ifdef DEBUG
    if (ft_rank == 0) {
      if (!virtual_array_flag)
        endpoint->PrintInfo();
      PrintVector("       data size", data_size);
      PrintVector("      chunk size", data_chunk_size);
      PrintVector("    overlap size", data_overlap_size);
      PrintScalar("current chunk id:", current_chunk_id);
      PrintScalar("    total chunks", data_total_chunks);
    }
  }
  /**
   * @brief print endpoint info of this array
   *
   */
  void PrintEndpointInfo() {
    if (!virtual_array_flag) {
      endpoint->PrintInfo();
    } else {
      for (auto aep : attribute_endpoint_vector)
        aep->PrintInfo();
    }
  }

  /**
   * @brief Run a UDF on the data pointed by the array
   *
   * @tparam UDFOutputType : the output type of UDF
   * @tparam BType : The element type of output Array B
   * @param UDF: pointer to user-defined function
   * @param B : Output Array B
   */
  template <class UDFOutputType, class BType = UDFOutputType>
  void Transform2(Stencil<UDFOutputType> (*UDF)(const Stencil<T> &),
                  ArrayBase *B = nullptr) {
    Transform(UDF, static_cast<FT::Array<BType> *>(B));
  }

  /**
   * @brief Run a UDF on the data pointed by the array
   *
   * @tparam UDFOutputType : the output type of UDF
   * @tparam BType : The element type of output Array B
   * @param UDF: pointer to user-defined function
   * @param B : Output Array B
   */
  template <class UDFOutputType, class BType = UDFOutputType>
  void Apply(Stencil<UDFOutputType> (*UDF)(const Stencil<T> &),
             Array<BType> *B = nullptr) {
    Transform(UDF, B);
  }

  template <class UDFOutputType, class BType = UDFOutputType>
  void Transform(Stencil<UDFOutputType> (*UDF)(const Stencil<T> &),
                 Array<BType> &B) {
    Transform(UDF, &B);
  }
  /*
  template <class UDFOutputType>
  void Transform(Stencil<UDFOutputType> (*UDF)(const Stencil<T> &))
  {
    Transform(UDF, nullptr);
  }
*/

  /**
   * @brief Run a UDF on the data pointed by the array
   *        This version returns the UDFOutputType type
   *        The next Transform which returns int is the latest.
   * @tparam UDFOutputType : the output type of UDF
   * @tparam BType : The element type of output Array B
   * @param UDF: pointer to user-defined function
   * @param B : Output Array B
   */
  template <class UDFOutputType, class BType = UDFOutputType>
  void Transform(Stencil<UDFOutputType> (*UDF)(const Stencil<T> &),
                 Array<BType> *B = nullptr) {
    // Set up the input data for LoadNextChunk
    InitializeApplyInput(UDF);

    // reset timer to zero
    time_read = 0;
    time_write = 0;
    time_udf = 0;

    std::vector<UDFOutputType> current_result_chunk_data;
    unsigned long long current_result_chunk_data_size = 1;

    vector_type_flag = InferVectorType<UDFOutputType>();

    t_start = AU_WTIME;
    int load_ret = LoadNextChunk(current_result_chunk_data_size);
    current_result_chunk_data.resize(current_result_chunk_data_size);
    time_read = time_read + (AU_WTIME - t_start);

    unsigned long long result_vector_index = 0;

    bool has_no_udf_return_result = false;

    while (load_ret == 1) {
      unsigned long long cell_target_g_location_rm;
      result_vector_index = 0;

      // unsigned long long lrm;
      t_start = AU_WTIME;

#if defined(_OPENMP)
      size_t *prefix;
#endif

#if defined(_OPENMP)
#pragma omp parallel private(has_no_udf_return_result)
#endif
      {
        std::vector<unsigned long long> cell_coordinate(data_dims, 0),
            cell_coordinate_ol(data_dims, 0),
            global_cell_coordinate(data_dims, 0);
        unsigned long long offset_ol;
        Stencil<T> cell_target(0, &current_chunk_data[0], cell_coordinate_ol,
                               current_chunk_ol_size, data_size);
        if (has_padding_value_flag) {
          cell_target.SetPadding(padding_value);
        }
        cell_target.SetChunkID(prev_chunk_id);

        if (get_stencil_tag_flag) {
          cell_target.SetTagMap(stencil_metadata_map);
        }
        Stencil<UDFOutputType> cell_return_stencil;
        UDFOutputType cell_return_value;
        unsigned long long cell_target_g_location_rm;
        int is_mirror_value = 0;
        std::vector<unsigned long long> skip_chunk_coordinate(data_dims, 0),
            skip_chunk_coordinate_start(data_dims, 0);
        int skip_flag_on_cell = 0;

#if defined(_OPENMP)
        int ithread = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
#pragma omp single
        {
          prefix = new size_t[nthreads + 1];
          prefix[0] = 0;
        }
        std::vector<UDFOutputType> vec_private;
        // std::cout << "nthreads = " << nthreads << "\n";
#endif

#if defined(_OPENMP)
#pragma omp for schedule(static) nowait
#endif
        for (unsigned long long i = 0; i < current_chunk_cells; i++) {
          ROW_MAJOR_ORDER_REVERSE_MACRO(
              i, current_chunk_size, current_chunk_size.size(), cell_coordinate)

          if (skip_flag == 1) {
            skip_flag_on_cell = 0;
            for (int id = 0; id < data_dims; id++) {
              // The coordinate of the skip chunk this coordinate belong to
              skip_chunk_coordinate[id] =
                  std::floor(cell_coordinate[id] / skip_size[id]);
              skip_chunk_coordinate_start[id] =
                  skip_chunk_coordinate[id] *
                  skip_size[id]; // first cell's coodinate of this skip chunk
              if (skip_chunk_coordinate_start[id] !=
                  cell_coordinate[id]) { // we only run on the first  element of
                                         // this skip chunk
                skip_flag_on_cell = 1;
                break;
              }
            }

            if (skip_flag_on_cell == 1)
              continue; //          for (unsigned long long i = 0; i <
                        //          current_chunk_cells; i++)
            assert(i < current_chunk_cells);
          }

          // Get the coodinate with overlapping
          // Also, get the global coodinate of the cell in original array
          for (int ii = 0; ii < data_dims; ii++) {
            if (cell_coordinate[ii] + ol_origin_offset[ii] <
                current_chunk_ol_size[ii]) {
              cell_coordinate_ol[ii] =
                  cell_coordinate[ii] + ol_origin_offset[ii];
            } else {
              cell_coordinate_ol[ii] = current_chunk_ol_size[ii] - 1;
            }
            // get the global coordinate
            global_cell_coordinate[ii] =
                current_chunk_start_offset[ii] + cell_coordinate[ii];
          }

          // Update the offset with overlapping
          ROW_MAJOR_ORDER_MACRO(current_chunk_ol_size,
                                current_chunk_ol_size.size(),
                                cell_coordinate_ol, offset_ol);
          if (endpoint != NULL && endpoint->GetEndpointType() != EP_DIR) {
            ROW_MAJOR_ORDER_MACRO(data_size, data_size.size(),
                                  global_cell_coordinate,
                                  cell_target_g_location_rm)
            cell_target.SetLocation(
                offset_ol, cell_coordinate_ol, cell_coordinate,
                current_chunk_size, ol_origin_offset, current_chunk_ol_size,
                global_cell_coordinate, cell_target_g_location_rm);
            // cell_target.SetLocation(cell_coordinate_ol,
            // global_cell_coordinate);
          } else {
            cell_target.SetLocation(offset_ol, cell_coordinate_ol,
                                    cell_coordinate, current_chunk_size,
                                    ol_origin_offset, current_chunk_ol_size,
                                    cell_coordinate_ol, offset_ol);
          }
          // Set the global coodinate of the cell as the ID of the cell
          // Disable it for performance.
          // RowMajorOrder(data_dims_size, global_cell_coordinate)
          // ROW_MAJOR_ORDER_MACRO(data_dims_size, data_dims_size.size(),
          // global_cell_coordinate, cell_target_g_location_rm)
          // cell_target.set_my_g_location_rm(cell_target_g_location_rm);

          cell_return_stencil = UDF(cell_target); // Called by C++

          if (!cell_return_stencil.IsEmpty()) {
            // std::cout << "Got value ! \n";
            cell_return_value = cell_return_stencil.get_value();
            has_no_udf_return_result = false;
          } else {
            std::cout << "IsEmpty = True ! \n";
            // std::cout << cell_return_stencil.get_value() <
            // std::cout << "Disable the [goto to] let openMP work(test)  !" <<
            // cell_return_stencil.get_value() << ", IsEmpty =" <<
            // cell_return_stencil.IsEmpty() << " \n"; goto end_of_process;
            has_no_udf_return_result = true;
            //  break;
          }

          if (vector_type_flag == true) {
            cell_return_stencil.GetShape(output_vector_shape);
          }

          if (cell_return_stencil.HasTagMap()) {
            has_output_stencil_tag_flag = true;
            cell_return_stencil.GetTagMap(output_stencil_metadata_map);
          }
          // std::cout << "save_result_flag = " << save_result_flag << ",
          // has_no_udf_return_result =" << has_no_udf_return_result << "\n";
          if (save_result_flag && (has_no_udf_return_result == false)) {
            // std::cout << "save_result_flag && (has_no_udf_return_result ==
            // false) \n";
            if (skip_flag) {
#if defined(_OPENMP)
              vec_private.push_back(cell_return_value);
#else
              current_result_chunk_data[result_vector_index] =
                  cell_return_value;
              result_vector_index = result_vector_index + 1;
#endif
              // When skip_flag is set, there is no need to have
              // apply_replace_flag Todo: in future if(apply_replace_flag == 1){
              //   current_chunk_data[i] = cell_return_value; //Replace the
              //   orginal data
              // }
            } else {
#if defined(_OPENMP)
              vec_private.push_back(cell_return_value);
              // std::cout << "vec_private.push_back(cell_return_value) is
              // called \n";
#else
              current_result_chunk_data[i] =
                  cell_return_value; // cell_return =  cell_return.
#endif
              if (apply_replace_flag == 1) {
                std::memcpy(&current_chunk_data[offset_ol], &cell_return_value,
                            sizeof(T));
              }
            }
          }
        } // end for loop, finish the processing on a single chunk in row-major
          // direction
#if defined(_OPENMP)
        prefix[ithread + 1] = vec_private.size();
#pragma omp barrier
#pragma omp single
        {
          for (int i = 1; i < (nthreads + 1); i++) {
            prefix[i] += prefix[i - 1];
          }
          if (current_result_chunk_data.size() != prefix[nthreads]) {
            std::cout << "DD Wrong output size ! prefix[nthreads] ="
                      << prefix[nthreads] << ", current.size() = "
                      << current_result_chunk_data.size() << " \n ";
          }
        } // end of omp for
        std::copy(vec_private.begin(), vec_private.end(),
                  current_result_chunk_data.begin() + prefix[ithread]);
        clear_vector(vec_private);
#endif
      } // end of omp parallel

#if defined(_OPENMP)
      delete[] prefix;
#endif
      time_udf = AU_WTIME - t_start + time_udf;

      //////////////////////////////////////
      // end of processing on a single chunk
      /////////////////////////////////////

      t_start = AU_WTIME;

      if (B != nullptr && (has_no_udf_return_result == false)) {

        std::vector<unsigned long long> B_data_size;
        std::vector<int> B_data_chunk_size, B_data_overlap_size;

        if (B->GetEndpointType() == EP_DIR && GetEndpointType() == EP_DIR) {
          std::vector<std::string> dir_file_list = GetDirFile();
          std::vector<int> dir_chunk_size = GetDirChunkSize();
          B->SetDirChunkSize(dir_chunk_size);
          // Here we update the filename list for the output endpoint
          // Basically, if the input has merge
          // We only keep the filename of the first merged as output
          // PrintVector("Debug:  data_chunk_size = ", data_chunk_size);
          // PrintVector("Debug:  dir_chunk_size = ", dir_chunk_size);
          int n_merge = 1;
          for (int i = 0; i < data_dims; i++) {
            n_merge = n_merge * data_chunk_size[i] / dir_chunk_size[i];
          }
          std::vector<std::string> dir_file_list_new;
          for (int i = 0; i < dir_file_list.size(); i++) {
            if (i % n_merge == 0)
              dir_file_list_new.push_back(dir_file_list[i]);
          }
          // Debug
          // for (int i = 0; i < dir_file_list_new.size(); i++)
          //{
          //   std::cout << "i: " << dir_file_list_new[i] << "\n";
          // }
          B->SetDirFile(dir_file_list_new);
        }

        // PrintVector("Debug: write current_chunk_start_offset_v = ",
        // current_chunk_start_offset_v); PrintVector("Debug: write
        // current_chunk_end_offset_v = ", current_chunk_end_offset_v);
        if (GetEndpointType() == EP_DIR_STREAM &&
            (B->GetEndpointType() == EP_RabbitMQ ||
             B->GetEndpointType() == EP_RabbitMQ_RESTAPI)) {
          std::vector<std::string> para;
          ControlEndpoint(DIR_STREAM_GET_CURRENT_SUB_INFO, para);
          para.insert(para.begin(), "fileinfo");
          para.push_back("chunksize");
          para.push_back(Vector2String(data_chunk_size));
          B->ControlEndpoint(RABBITMQ_SET_HEADER, para);
        }

        if ((GetEndpointType() == EP_RabbitMQ ||
             GetEndpointType() == EP_RabbitMQ_RESTAPI) &&
            B->GetEndpointType() == EP_DIR_STREAM) {
          std::vector<std::string> para;
          ControlEndpoint(RABBITMQ_GET_HEADER, para);
          PrintVector("RABBITMQ_GET_HEADER = ", para);
          B->ControlEndpoint(DIR_STREAM_SET_CURRENT_SUB_INFO, para);
          para.clear();
          para.push_back(Vector2String(data_chunk_size));
          B->ControlEndpoint(DIR_SET_CHUNK_SIZE, para);
          // para.insert(para.begin(), "fileinfo");
          // para.push_back("chunksize");
          // para.push_back(Vector2String(data_chunk_size));
          // B->ControlEndpoint(RABBITMQ_SET_HEADER, para);
        }

        if (vector_type_flag) {
          // output_vector_shape
          size_t vector_size;
          std::vector<unsigned long long> current_chunk_start_offset_v =
                                              current_result_chunk_start_offset,
                                          current_chunk_end_offset_v =
                                              current_result_chunk_end_offset;
          void *data_point;
          // data_point = FlatVector(current_result_chunk_data,
          // output_vector_flat_direction_index, current_chunk_start_offset_v,
          // current_chunk_end_offset_v, vector_size);
          // InferOutputSize(B_data_size, B_data_chunk_size,
          // B_data_overlap_size, vector_size);

          if (is_the_last_chunk && (previous_output_vector_shape.size() == 0)) {
            // int normal_chunk_output_size = data_chunk_size[i] /
            // (current_chunk_ol_size[i] / output_vector_shape[i]);
            previous_output_vector_shape = output_vector_shape;
            for (int i = 0; i < output_vector_shape.size(); i++) {
              if (skip_not_aligned_w_array_flag &&
                  skip_not_aligned_w_array_index == i) {
                previous_output_vector_shape[i] =
                    data_chunk_size[i] /
                    (current_chunk_ol_size[i] / output_vector_shape[i]);
              }
            }
          }
          // PrintVector("Debug:  output_vector_shape = ", output_vector_shape);
          // PrintVector("Debug:  current_chunk_start_offset_v = ",
          // current_chunk_start_offset_v); PrintVector("Debug:
          // current_chunk_end_offset_v = ", current_chunk_end_offset_v);
          // PrintVector("Debug:  previous_output_vector_shape = ",
          // previous_output_vector_shape);

          data_point = InsertOutputVV2WriteV(
              current_result_chunk_data, output_vector_shape,
              current_chunk_start_offset_v, current_chunk_end_offset_v,
              is_the_last_chunk, previous_output_vector_shape);
          CalculateOutputSize(B_data_size, B_data_chunk_size,
                              B_data_overlap_size);
          // PrintVector("Debug: create B_data_size = ", B_data_size);
          if (B->GetEndpointType() == EP_DIR && GetEndpointType() == EP_DIR) {
            B->SetDirChunkSize(B_data_chunk_size);
          }

          B->CreateEndpoint(B_data_size, B_data_chunk_size,
                            B_data_overlap_size);
          // PrintVector("Debug: write B_data_chunk_size = ",
          // B_data_chunk_size); PrintVector("Debug: write B_data_size = ",
          // B_data_size); PrintVector("Debug: write
          // current_chunk_start_offset_v = ", current_chunk_start_offset_v);
          // PrintVector("Debug: write current_chunk_end_offset_v = ",
          // current_chunk_end_offset_v);

          B->WriteEndpoint(current_chunk_start_offset_v,
                           current_chunk_end_offset_v, data_point);
          free(data_point);
        } else {
          InferOutputSize(B_data_size, B_data_chunk_size, B_data_overlap_size,
                          0);
          B->CreateEndpoint(B_data_size, B_data_chunk_size,
                            B_data_overlap_size);
          // B->WriteEndpoint(current_chunk_start_offset,
          // current_chunk_end_offset, &current_result_chunk_data[0]);
          if (!skip_flag) {
            // PrintVector("current_chunk_start_offset = ",
            // current_chunk_start_offset);
            // PrintVector("current_chunk_end_offset = ",
            // current_chunk_end_offset);
            B->WriteArray(current_result_chunk_start_offset,
                          current_chunk_end_offset, current_result_chunk_data);
          } else {
            // PrintVector("current_chunk_start_offset = ",
            // current_result_chunk_start_offset);
            // PrintVector("current_chunk_end_offset = ",
            // current_result_chunk_end_offset);
            B->WriteArray(current_result_chunk_start_offset,
                          current_result_chunk_end_offset,
                          current_result_chunk_data);
          }
        }
      }

      if (has_output_stencil_tag_flag) {
        for (std::map<std::string, std::string>::iterator it =
                 output_stencil_metadata_map.begin();
             it != output_stencil_metadata_map.end(); ++it) {

          B->SetTag(it->first, it->second);
          // std::cout << "Key: " << it->first << std::endl();
          // std::cout << "Value: " << it->second << std::endl();
        }
        has_output_stencil_tag_flag = false;
      }

      time_write = time_write + AU_WTIME - t_start;

      if (vector_type_flag == true) {
        previous_output_vector_shape = output_vector_shape;
      }

      t_start = AU_WTIME;
      load_ret = LoadNextChunk(current_result_chunk_data_size);
      current_result_chunk_data.resize(current_result_chunk_data_size);
      time_read = time_read + AU_WTIME - t_start;

    } // end of while:: no more chunks to process

    // end_of_process:
    //  May start a empty write for collective I/O
    if ((has_no_udf_return_result == true) &&
        (data_total_chunks % ft_size != 0) &&
        (current_chunk_id >= data_total_chunks) &&
        (current_chunk_id <
         (data_total_chunks + ft_size - (data_total_chunks % ft_size))) &&
        B != nullptr) {
      // std::cout << "current_chunk_id = " << current_chunk_id << std::endl;
      // std::cout << "leftover_chunks  = " << data_total_chunks % ft_size <<
      // std::endl; std::cout << "data_total_chunks  = " << data_total_chunks <<
      // std::endl;
      std::vector<unsigned long long> null_start; // Start offset on disk
      std::vector<unsigned long long> null_end;   // End offset on disk
      void *data_point = nullptr;
      null_start.resize(data_dims, 0);
      null_end.resize(data_dims, 0);
      B->WriteEndpoint(null_start, null_end, data_point);
    }

    return;
  }
  bool IsVectorZeros(const std::vector<size_t> &vec) {
    for (const size_t &element : vec) {
      if (element != 0) {
        return false; // Found a non-zero element
      }
    }
    return true; // All elements are zero
  }

  /**
   * @brief Run a UDF on the data pointed by the array
   *
   * @tparam UDFOutputType : the output type of UDF
   * @tparam BType : The element type of output Array B
   * @param UDF: pointer to user-defined function
   * @param B : Output Array B
   */
  template <class UDFOutputType, class BType = UDFOutputType>
  void TransformNoMP(Stencil<UDFOutputType> (*UDF)(const Stencil<T> &),
                     Array<BType> *B = nullptr) {
    // Set up the input data for LoadNextChunk
    InitializeApplyInput(UDF);

    // reset timer to zero
    time_read = 0;
    time_write = 0;
    time_udf = 0;

    std::vector<UDFOutputType> current_result_chunk_data;
    unsigned long long current_result_chunk_data_size = 1;

    vector_type_flag = InferVectorType<UDFOutputType>();

    t_start = AU_WTIME;
    int load_ret = LoadNextChunk(current_result_chunk_data_size);
    current_result_chunk_data.resize(current_result_chunk_data_size);
    time_read = time_read + (AU_WTIME - t_start);

    unsigned long long result_vector_index = 0;

    while (load_ret == 1) {
      unsigned long long cell_target_g_location_rm;
      result_vector_index = 0;

      // unsigned long long lrm;
      t_start = AU_WTIME;

      {
        std::vector<unsigned long long> cell_coordinate(data_dims, 0),
            cell_coordinate_ol(data_dims, 0),
            global_cell_coordinate(data_dims, 0);
        unsigned long long offset_ol;
        Stencil<T> cell_target(0, &current_chunk_data[0], cell_coordinate_ol,
                               current_chunk_ol_size, data_size);
        if (has_padding_value_flag) {
          cell_target.SetPadding(padding_value);
        }
        cell_target.SetChunkID(prev_chunk_id);

        if (get_stencil_tag_flag) {
          cell_target.SetTagMap(stencil_metadata_map);
        }
        Stencil<UDFOutputType> cell_return_stencil;
        UDFOutputType cell_return_value;
        unsigned long long cell_target_g_location_rm;
        int is_mirror_value = 0;
        std::vector<unsigned long long> skip_chunk_coordinate(data_dims, 0),
            skip_chunk_coordinate_start(data_dims, 0);
        int skip_flag_on_cell = 0;

        for (unsigned long long i = 0; i < current_chunk_cells; i++) {
          ROW_MAJOR_ORDER_REVERSE_MACRO(
              i, current_chunk_size, current_chunk_size.size(), cell_coordinate)

          if (skip_flag == 1) {
            skip_flag_on_cell = 0;
            for (int id = 0; id < data_dims; id++) {
              // The coordinate of the skip chunk this coordinate belong to
              skip_chunk_coordinate[id] =
                  std::floor(cell_coordinate[id] / skip_size[id]);
              skip_chunk_coordinate_start[id] =
                  skip_chunk_coordinate[id] *
                  skip_size[id]; // first cell's coodinate of this skip chunk
              if (skip_chunk_coordinate_start[id] !=
                  cell_coordinate[id]) { // we only run on the first  element of
                                         // this skip chunk
                skip_flag_on_cell = 1;
                break;
              }
            }

            if (skip_flag_on_cell == 1)
              continue; //          for (unsigned long long i = 0; i <
                        //          current_chunk_cells; i++)
            assert(i < current_chunk_cells);
          }

          // Get the coodinate with overlapping
          // Also, get the global coodinate of the cell in original array
          for (int ii = 0; ii < data_dims; ii++) {
            if (cell_coordinate[ii] + ol_origin_offset[ii] <
                current_chunk_ol_size[ii]) {
              cell_coordinate_ol[ii] =
                  cell_coordinate[ii] + ol_origin_offset[ii];
            } else {
              cell_coordinate_ol[ii] = current_chunk_ol_size[ii] - 1;
            }
            // get the global coordinate
            global_cell_coordinate[ii] =
                current_chunk_start_offset[ii] + cell_coordinate[ii];
          }

          // Update the offset with overlapping
          ROW_MAJOR_ORDER_MACRO(current_chunk_ol_size,
                                current_chunk_ol_size.size(),
                                cell_coordinate_ol, offset_ol);
          if (endpoint != NULL && endpoint->GetEndpointType() != EP_DIR) {
            ROW_MAJOR_ORDER_MACRO(data_size, data_size.size(),
                                  global_cell_coordinate,
                                  cell_target_g_location_rm)
            cell_target.SetLocation(
                offset_ol, cell_coordinate_ol, cell_coordinate,
                current_chunk_size, ol_origin_offset, current_chunk_ol_size,
                global_cell_coordinate, cell_target_g_location_rm);
            // cell_target.SetLocation(cell_coordinate_ol,
            // global_cell_coordinate);
          } else {
            cell_target.SetLocation(offset_ol, cell_coordinate_ol,
                                    cell_coordinate, current_chunk_size,
                                    ol_origin_offset, current_chunk_ol_size,
                                    cell_coordinate_ol, offset_ol);
          }
          // Set the global coodinate of the cell as the ID of the cell
          // Disable it for performance.
          // RowMajorOrder(data_dims_size, global_cell_coordinate)
          // ROW_MAJOR_ORDER_MACRO(data_dims_size, data_dims_size.size(),
          // global_cell_coordinate, cell_target_g_location_rm)
          // cell_target.set_my_g_location_rm(cell_target_g_location_rm);

          cell_return_stencil = UDF(cell_target); // Called by C++

          if (!cell_return_stencil.IsEmpty()) {
            // std::cout << "Got value ! \n";
            cell_return_value = cell_return_stencil.get_value();
          } else {
            // std::cout << "Disable the [goto to] let openMP work(test)  ! \n";
            goto end_of_process;
            //  break;
          }

          if (vector_type_flag == true) {
            cell_return_stencil.GetShape(output_vector_shape);
            if (IsVectorZeros(output_vector_shape))
              goto end_of_process;
          }

          if (cell_return_stencil.HasTagMap()) {
            has_output_stencil_tag_flag = true;
            cell_return_stencil.GetTagMap(output_stencil_metadata_map);
          }

          if (save_result_flag) {
            if (skip_flag) {

              current_result_chunk_data[result_vector_index] =
                  cell_return_value;
              result_vector_index = result_vector_index + 1;
              // When skip_flag is set, there is no need to have
              // apply_replace_flag Todo: in future if(apply_replace_flag == 1){
              //   current_chunk_data[i] = cell_return_value; //Replace the
              //   orginal data
              // }
            } else {
              current_result_chunk_data[i] =
                  cell_return_value; // cell_return =  cell_return.
              if (apply_replace_flag == 1) {
                std::memcpy(&current_chunk_data[offset_ol], &cell_return_value,
                            sizeof(T));
              }
            }
          }
          if (is_execute_udf_once)
            break;
        } // end for loop, finish the processing on a single chunk in row-major
          // direction
      } // end of omp parallel

      time_udf = AU_WTIME - t_start + time_udf;

      //////////////////////////////////////
      // end of processing on a single chunk
      /////////////////////////////////////

      t_start = AU_WTIME;

      if (B != nullptr) {

        std::vector<unsigned long long> B_data_size;
        std::vector<int> B_data_chunk_size, B_data_overlap_size;

        if (B->GetEndpointType() == EP_DIR && GetEndpointType() == EP_DIR) {
          std::vector<std::string> dir_file_list = GetDirFile();
          std::vector<int> dir_chunk_size = GetDirChunkSize();
          B->SetDirChunkSize(dir_chunk_size);
          // Here we update the filename list for the output endpoint
          // Basically, if the input has merge
          // We only keep the filename of the first merged as output
          PrintVector("SetDirChunkSize Debug:  data_chunk_size = ",
                      data_chunk_size);
          PrintVector("SetDirChunkSize Debug:  dir_chunk_size = ",
                      dir_chunk_size);
          int n_merge = 1;
          for (int i = 0; i < data_dims; i++) {
            n_merge = n_merge * data_chunk_size[i] / dir_chunk_size[i];
          }
          std::vector<std::string> dir_file_list_new;
          for (int i = 0; i < dir_file_list.size(); i++) {
            if (i % n_merge == 0)
              dir_file_list_new.push_back(dir_file_list[i]);
          }
          // Debug
          // for (int i = 0; i < dir_file_list_new.size(); i++)
          //{
          //   std::cout << "i: " << dir_file_list_new[i] << "\n";
          // }
          B->SetDirFile(dir_file_list_new);
        }

        // PrintVector("Debug: write current_chunk_start_offset_v = ",
        // current_chunk_start_offset_v); PrintVector("Debug: write
        // current_chunk_end_offset_v = ", current_chunk_end_offset_v);

        if (vector_type_flag) {
          // output_vector_shape
          size_t vector_size;
          std::vector<unsigned long long> current_chunk_start_offset_v =
                                              current_result_chunk_start_offset,
                                          current_chunk_end_offset_v =
                                              current_result_chunk_end_offset;
          void *data_point;
          // data_point = FlatVector(current_result_chunk_data,
          // output_vector_flat_direction_index, current_chunk_start_offset_v,
          // current_chunk_end_offset_v, vector_size);
          // InferOutputSize(B_data_size, B_data_chunk_size,
          // B_data_overlap_size, vector_size);

          if (is_the_last_chunk && (previous_output_vector_shape.size() == 0)) {
            // int normal_chunk_output_size = data_chunk_size[i] /
            // (current_chunk_ol_size[i] / output_vector_shape[i]);
            previous_output_vector_shape = output_vector_shape;
            for (int i = 0; i < output_vector_shape.size(); i++) {
              if (skip_not_aligned_w_array_flag &&
                  skip_not_aligned_w_array_index == i) {
                previous_output_vector_shape[i] =
                    data_chunk_size[i] /
                    (current_chunk_ol_size[i] / output_vector_shape[i]);
              }
            }
          }
          PrintVector("Debug:  output_vector_shape = ", output_vector_shape);
          PrintVector("Debug:  current_chunk_start_offset_v = ",
                      current_chunk_start_offset_v);
          PrintVector("Debug:  current_chunk_end_offset_v = ",
                      current_chunk_end_offset_v);
          PrintVector("Debug:  previous_output_vector_shape = ",
                      previous_output_vector_shape);

          data_point = InsertOutputVV2WriteV(
              current_result_chunk_data, output_vector_shape,
              current_chunk_start_offset_v, current_chunk_end_offset_v,
              is_the_last_chunk, previous_output_vector_shape);
          CalculateOutputSize(B_data_size, B_data_chunk_size,
                              B_data_overlap_size);
          PrintVector("Debug: create B_data_size = ", B_data_size);
          if (B->GetEndpointType() == EP_DIR && GetEndpointType() == EP_DIR) {
            B->SetDirChunkSize(B_data_chunk_size);
          }
          B->CreateEndpoint(B_data_size, B_data_chunk_size,
                            B_data_overlap_size);
          PrintVector("Debug: write B_data_chunk_size = ", B_data_chunk_size);
          PrintVector("Debug: write B_data_size = ", B_data_size);
          PrintVector("Debug: write current_chunk_start_offset_v = ",
                      current_chunk_start_offset_v);
          PrintVector("Debug: write current_chunk_end_offset_v = ",
                      current_chunk_end_offset_v);

          B->WriteEndpoint(current_chunk_start_offset_v,
                           current_chunk_end_offset_v, data_point);
          free(data_point);
        } else {
          InferOutputSize(B_data_size, B_data_chunk_size, B_data_overlap_size,
                          0);
          B->CreateEndpoint(B_data_size, B_data_chunk_size,
                            B_data_overlap_size);
          // B->WriteEndpoint(current_chunk_start_offset,
          // current_chunk_end_offset, &current_result_chunk_data[0]);
          if (!skip_flag) {
            // PrintVector("current_chunk_start_offset = ",
            // current_chunk_start_offset);
            // PrintVector("current_chunk_end_offset = ",
            // current_chunk_end_offset);
            B->WriteArray(current_result_chunk_start_offset,
                          current_chunk_end_offset, current_result_chunk_data);
          } else {
            // PrintVector("current_chunk_start_offset = ",
            // current_result_chunk_start_offset);
            // PrintVector("current_chunk_end_offset = ",
            // current_result_chunk_end_offset);
            B->WriteArray(current_result_chunk_start_offset,
                          current_result_chunk_end_offset,
                          current_result_chunk_data);
          }
        }
      }

      if (has_output_stencil_tag_flag) {
        for (std::map<std::string, std::string>::iterator it =
                 output_stencil_metadata_map.begin();
             it != output_stencil_metadata_map.end(); ++it) {

          B->SetTag(it->first, it->second);
          // std::cout << "Key: " << it->first << std::endl();
          // std::cout << "Value: " << it->second << std::endl();
        }
        has_output_stencil_tag_flag = false;
      }

      time_write = time_write + AU_WTIME - t_start;

      if (vector_type_flag == true) {
        previous_output_vector_shape = output_vector_shape;
      }

      t_start = AU_WTIME;
      load_ret = LoadNextChunk(current_result_chunk_data_size);
      current_result_chunk_data.resize(current_result_chunk_data_size);
      time_read = time_read + AU_WTIME - t_start;

    } // end of while:: no more chunks to process

  end_of_process:
    // May start a empty write for collective I/O
    if ((data_total_chunks % ft_size != 0) &&
        (current_chunk_id >= data_total_chunks) &&
        (current_chunk_id <
         (data_total_chunks + ft_size - (data_total_chunks % ft_size))) &&
        B != nullptr) {
      // std::cout << "current_chunk_id = " << current_chunk_id << std::endl;
      // std::cout << "leftover_chunks  = " << data_total_chunks % ft_size <<
      // std::endl; std::cout << "data_total_chunks  = " << data_total_chunks <<
      // std::endl;
      std::vector<unsigned long long> null_start; // Start offset on disk
      std::vector<unsigned long long> null_end;   // End offset on disk
      void *data_point = nullptr;
      null_start.resize(data_dims, 0);
      null_end.resize(data_dims, 0);
      B->WriteEndpoint(null_start, null_end, data_point);
    }

    return;
  }

  /**
   * @brief Run a UDF on the data pointed by the array
   *
   * @tparam UDFOutputType : the output type of UDF
   * @tparam BType : The element type of output Array B
   * @param UDF: pointer to user-defined function
   * @param B : Output Array B
   */
  template <class UDFOutputType, class BType = UDFOutputType>
  void Transform(int (*UDF)(const Stencil<T> &, Stencil<UDFOutputType> &),
                 Array<BType> *B = nullptr) {
    // Set up the input data for LoadNextChunk
    // InitializeApplyInput(UDF);
    InitializeApplyInputTest(UDF);
    // reset timer to zero
    time_read = 0;
    time_write = 0;
    time_udf = 0;

    std::vector<UDFOutputType> current_result_chunk_data;
    unsigned long long current_result_chunk_data_size = 1;

    vector_type_flag = InferVectorType<UDFOutputType>();

    t_start = AU_WTIME;
    int load_ret = LoadNextChunk(current_result_chunk_data_size);
    current_result_chunk_data.resize(current_result_chunk_data_size);
    time_read = time_read + (AU_WTIME - t_start);

    unsigned long long result_vector_index = 0;

    while (load_ret == 1) {
      unsigned long long cell_target_g_location_rm;
      result_vector_index = 0;

      // unsigned long long lrm;
      t_start = AU_WTIME;

#if defined(_OPENMP)
      size_t *prefix;
#endif

#if defined(_OPENMP)
#pragma omp parallel
#endif
      {
        std::vector<unsigned long long> cell_coordinate(data_dims, 0),
            cell_coordinate_ol(data_dims, 0),
            global_cell_coordinate(data_dims, 0);
        unsigned long long offset_ol;
        Stencil<T> cell_target(0, &current_chunk_data[0], cell_coordinate_ol,
                               current_chunk_ol_size, data_size);
        if (has_padding_value_flag) {
          cell_target.SetPadding(padding_value);
        }
        cell_target.SetChunkID(prev_chunk_id);

        if (get_stencil_tag_flag) {
          cell_target.SetTagMap(stencil_metadata_map);
        }
        Stencil<UDFOutputType> cell_return_stencil;
        UDFOutputType cell_return_value;
        unsigned long long cell_target_g_location_rm;
        int is_mirror_value = 0;
        std::vector<unsigned long long> skip_chunk_coordinate(data_dims, 0),
            skip_chunk_coordinate_start(data_dims, 0);
        int skip_flag_on_cell = 0;

#if defined(_OPENMP)
        int ithread = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
#pragma omp single
        {
          prefix = new size_t[nthreads + 1];
          prefix[0] = 0;
        }
        std::vector<UDFOutputType> vec_private;
#endif

#if defined(_OPENMP)
#pragma omp for schedule(static) nowait
#endif
        for (unsigned long long i = 0; i < current_chunk_cells; i++) {
          ROW_MAJOR_ORDER_REVERSE_MACRO(
              i, current_chunk_size, current_chunk_size.size(), cell_coordinate)

          if (skip_flag == 1) {
            skip_flag_on_cell = 0;
            for (int id = 0; id < data_dims; id++) {
              // The coordinate of the skip chunk this coordinate belong to
              skip_chunk_coordinate[id] =
                  std::floor(cell_coordinate[id] / skip_size[id]);
              skip_chunk_coordinate_start[id] =
                  skip_chunk_coordinate[id] *
                  skip_size[id]; // first cell's coodinate of this skip chunk
              if (skip_chunk_coordinate_start[id] !=
                  cell_coordinate[id]) { // we only run on the first  element of
                                         // this skip chunk
                skip_flag_on_cell = 1;
                break;
              }
            }

            if (skip_flag_on_cell == 1)
              continue; //          for (unsigned long long i = 0; i <
                        //          current_chunk_cells; i++)
            assert(i < current_chunk_cells);
          }

          // Get the coodinate with overlapping
          // Also, get the global coodinate of the cell in original array
          for (int ii = 0; ii < data_dims; ii++) {
            if (cell_coordinate[ii] + ol_origin_offset[ii] <
                current_chunk_ol_size[ii]) {
              cell_coordinate_ol[ii] =
                  cell_coordinate[ii] + ol_origin_offset[ii];
            } else {
              cell_coordinate_ol[ii] = current_chunk_ol_size[ii] - 1;
            }
            // get the global coordinate
            global_cell_coordinate[ii] =
                current_chunk_start_offset[ii] + cell_coordinate[ii];
          }

          // Update the offset with overlapping
          ROW_MAJOR_ORDER_MACRO(current_chunk_ol_size,
                                current_chunk_ol_size.size(),
                                cell_coordinate_ol, offset_ol);
          if (endpoint != NULL && endpoint->GetEndpointType() != EP_DIR) {
            ROW_MAJOR_ORDER_MACRO(data_size, data_size.size(),
                                  global_cell_coordinate,
                                  cell_target_g_location_rm)
            cell_target.SetLocation(
                offset_ol, cell_coordinate_ol, cell_coordinate,
                current_chunk_size, ol_origin_offset, current_chunk_ol_size,
                global_cell_coordinate, cell_target_g_location_rm);
            // cell_target.SetLocation(cell_coordinate_ol,
            // global_cell_coordinate);
          } else {
            cell_target.SetLocation(offset_ol, cell_coordinate_ol,
                                    cell_coordinate, current_chunk_size,
                                    ol_origin_offset, current_chunk_ol_size,
                                    cell_coordinate_ol, offset_ol);
          }
          // Set the global coodinate of the cell as the ID of the cell
          // Disable it for performance.
          // RowMajorOrder(data_dims_size, global_cell_coordinate)
          // ROW_MAJOR_ORDER_MACRO(data_dims_size, data_dims_size.size(),
          // global_cell_coordinate, cell_target_g_location_rm)
          // cell_target.set_my_g_location_rm(cell_target_g_location_rm);

          UDF(cell_target, cell_return_stencil); // Called by C++
          cell_return_value = cell_return_stencil.get_value();

          if (vector_type_flag == true) {
            cell_return_stencil.GetShape(output_vector_shape);
          }

          if (cell_return_stencil.HasTagMap()) {
            has_output_stencil_tag_flag = true;
            cell_return_stencil.GetTagMap(output_stencil_metadata_map);
          }

          if (save_result_flag) {
            if (skip_flag) {
#if defined(_OPENMP)
              vec_private.push_back(cell_return_value);
#else
              current_result_chunk_data[result_vector_index] =
                  cell_return_value;
              result_vector_index = result_vector_index + 1;
#endif
              // When skip_flag is set, there is no need to have
              // apply_replace_flag Todo: in future if(apply_replace_flag == 1){
              //   current_chunk_data[i] = cell_return_value; //Replace the
              //   orginal data
              // }
            } else {
              current_result_chunk_data[i] =
                  cell_return_value; // cell_return =  cell_return.
              if (apply_replace_flag == 1) {
                std::memcpy(&current_chunk_data[offset_ol], &cell_return_value,
                            sizeof(T));
              }
            }
          }
          // This code is like execute the UDF once
          // To do, we need to test its effect.
          //  if (skip_flag == 1 && VectorEqVector(current_chunk_size,
          //  skip_size))
          //{
          //    break;
          //  }
        } // end for loop, finish the processing on a single chunk in row-major
          // direction
#if defined(_OPENMP)
        prefix[ithread + 1] = vec_private.size();
#pragma omp barrier
#pragma omp single
        {
          for (int i = 1; i < (nthreads + 1); i++) {
            prefix[i] += prefix[i - 1];
          }
          if (current_result_chunk_data.size() != prefix[nthreads]) {
            std::cout << "Wrong output size ! prefix[nthreads] ="
                      << prefix[nthreads] << ", current.size() = "
                      << current_result_chunk_data.size() << " \n ";
          }
        } // end of omp for
        std::copy(vec_private.begin(), vec_private.end(),
                  current_result_chunk_data.begin() + prefix[ithread]);
        clear_vector(vec_private);
#endif
      } // end of omp parallel

#if defined(_OPENMP)
      delete[] prefix;
#endif
      time_udf = AU_WTIME - t_start + time_udf;

      //////////////////////////////////////
      // end of processing on a single chunk
      /////////////////////////////////////

      t_start = AU_WTIME;

      if (B != nullptr) {

        std::vector<unsigned long long> B_data_size;
        std::vector<int> B_data_chunk_size, B_data_overlap_size;

        if (B->GetEndpointType() == EP_DIR && GetEndpointType() == EP_DIR) {
          std::vector<std::string> dir_file_list = GetDirFile();
          std::vector<int> dir_chunk_size = GetDirChunkSize();
          // B->SetDirChunkSize(dir_chunk_size);
          // Here we update the filename list for the output endpoint
          // Basically, if the input has merge
          // We only keep the filename of the first merged as output
          int n_merge = 1;
          for (int i = 0; i < data_dims; i++) {

            n_merge = n_merge * data_chunk_size[i] / dir_chunk_size[i];
          }
          std::vector<std::string> dir_file_list_new;
          for (int i = 0; i < dir_file_list.size(); i++) {
            if (i % n_merge == 0)
              dir_file_list_new.push_back(dir_file_list[i]);
          }
          // Debug
          // for (int i = 0; i < dir_file_list_new.size(); i++)
          //{
          //   std::cout << "i: " << dir_file_list_new[i] << "\n";
          // }
          B->SetDirFile(dir_file_list_new);
        }

        // PrintVector("Debug: write current_chunk_start_offset_v = ",
        // current_chunk_start_offset_v); PrintVector("Debug: write
        // current_chunk_end_offset_v = ", current_chunk_end_offset_v);

        if (vector_type_flag) {
          // output_vector_shape
          size_t vector_size;
          std::vector<unsigned long long> current_chunk_start_offset_v =
                                              current_result_chunk_start_offset,
                                          current_chunk_end_offset_v =
                                              current_result_chunk_end_offset;
          void *data_point;
          // data_point = FlatVector(current_result_chunk_data,
          // output_vector_flat_direction_index, current_chunk_start_offset_v,
          // current_chunk_end_offset_v, vector_size);
          // InferOutputSize(B_data_size, B_data_chunk_size,
          // B_data_overlap_size, vector_size);

          if (is_the_last_chunk && (previous_output_vector_shape.size() == 0)) {
            // int normal_chunk_output_size = data_chunk_size[i] /
            // (current_chunk_ol_size[i] / output_vector_shape[i]);
            previous_output_vector_shape = output_vector_shape;
            for (int i = 0; i < output_vector_shape.size(); i++) {
              if (skip_not_aligned_w_array_flag &&
                  skip_not_aligned_w_array_index == i) {
                previous_output_vector_shape[i] =
                    data_chunk_size[i] /
                    (current_chunk_ol_size[i] / output_vector_shape[i]);
              }
            }
          }
          // PrintVector("Debug:  output_vector_shape = ", output_vector_shape);
          // PrintVector("Debug:  current_chunk_start_offset_v = ",
          // current_chunk_start_offset_v); PrintVector("Debug:
          // current_chunk_end_offset_v = ", current_chunk_end_offset_v);
          // PrintVector("Debug:  previous_output_vector_shape = ",
          // previous_output_vector_shape);

          data_point = InsertOutputVV2WriteV(
              current_result_chunk_data, output_vector_shape,
              current_chunk_start_offset_v, current_chunk_end_offset_v,
              is_the_last_chunk, previous_output_vector_shape);
          CalculateOutputSize(B_data_size, B_data_chunk_size,
                              B_data_overlap_size);
          // PrintVector("Debug: create B_data_size = ", B_data_size);
          if (B->GetEndpointType() == EP_DIR && GetEndpointType() == EP_DIR) {
            B->SetDirChunkSize(B_data_chunk_size);
          }
          B->CreateEndpoint(B_data_size, B_data_chunk_size,
                            B_data_overlap_size);
          // PrintVector("Debug: write B_data_chunk_size = ",
          // B_data_chunk_size); PrintVector("Debug: write B_data_size = ",
          // B_data_size); PrintVector("Debug: write
          // current_chunk_start_offset_v = ", current_chunk_start_offset_v);
          // PrintVector("Debug: write current_chunk_end_offset_v = ",
          // current_chunk_end_offset_v);

          B->WriteEndpoint(current_chunk_start_offset_v,
                           current_chunk_end_offset_v, data_point);
          free(data_point);
        } else {
          InferOutputSize(B_data_size, B_data_chunk_size, B_data_overlap_size,
                          0);
          B->CreateEndpoint(B_data_size, B_data_chunk_size,
                            B_data_overlap_size);
          // B->WriteEndpoint(current_chunk_start_offset,
          // current_chunk_end_offset, &current_result_chunk_data[0]);
          if (!skip_flag) {
            // PrintVector("current_chunk_start_offset = ",
            // current_chunk_start_offset);
            // PrintVector("current_chunk_end_offset = ",
            // current_chunk_end_offset);
            B->WriteArray(current_result_chunk_start_offset,
                          current_chunk_end_offset, current_result_chunk_data);
          } else {
            // PrintVector("current_chunk_start_offset = ",
            // current_result_chunk_start_offset);
            // PrintVector("current_chunk_end_offset = ",
            // current_result_chunk_end_offset);
            B->WriteArray(current_result_chunk_start_offset,
                          current_result_chunk_end_offset,
                          current_result_chunk_data);
          }
        }
      }

      if (has_output_stencil_tag_flag) {
        for (std::map<std::string, std::string>::iterator it =
                 output_stencil_metadata_map.begin();
             it != output_stencil_metadata_map.end(); ++it) {

          B->SetTag(it->first, it->second);
          // std::cout << "Key: " << it->first << std::endl();
          // std::cout << "Value: " << it->second << std::endl();
        }
        has_output_stencil_tag_flag = false;
      }

      time_write = time_write + AU_WTIME - t_start;

      if (vector_type_flag == true) {
        previous_output_vector_shape = output_vector_shape;
      }

      t_start = AU_WTIME;
      load_ret = LoadNextChunk(current_result_chunk_data_size);
      current_result_chunk_data.resize(current_result_chunk_data_size);
      time_read = time_read + AU_WTIME - t_start;

    } // end of while:: no more chunks to process

    // May start a empty write for collective I/O
    if ((data_total_chunks % ft_size != 0) &&
        (current_chunk_id >= data_total_chunks) &&
        (current_chunk_id <
         (data_total_chunks + ft_size - (data_total_chunks % ft_size))) &&
        B != nullptr) {
      // std::cout << "current_chunk_id = " << current_chunk_id << std::endl;
      // std::cout << "leftover_chunks  = " << data_total_chunks % ft_size <<
      // std::endl; std::cout << "data_total_chunks  = " << data_total_chunks <<
      // std::endl;
      std::vector<unsigned long long> null_start; // Start offset on disk
      std::vector<unsigned long long> null_end;   // End offset on disk
      void *data_point = nullptr;
      null_start.resize(data_dims, 0);
      null_end.resize(data_dims, 0);
      B->WriteEndpoint(null_start, null_end, data_point);
    }

    return;
  }

  /**
   * @brief Run a UDF on the data pointed by the array
   *
   * @tparam UDFOutputType : the output type of UDF
   * @tparam BType : The element type of output Array B
   * @param UDF: pointer to user-defined function
   * @param B : Output Array B
   */
  template <class UDFOutputType, class BType = UDFOutputType>
  void TransformTest(int (*UDF)(const Stencil<T> &, Stencil<UDFOutputType> &),
                     Array<BType> *B = nullptr) {
    // Set up the input data for LoadNextChunk
    InitializeApplyInputTest(UDF);

    // reset timer to zero
    time_read = 0;
    time_write = 0;
    time_udf = 0;

    std::vector<UDFOutputType> current_result_chunk_data;
    unsigned long long current_result_chunk_data_size = 1;

    vector_type_flag = InferVectorType<UDFOutputType>();

    t_start = AU_WTIME;
    int load_ret = LoadNextChunk(current_result_chunk_data_size);
    current_result_chunk_data.resize(current_result_chunk_data_size);
    time_read = time_read + (AU_WTIME - t_start);

    unsigned long long result_vector_index = 0;

    while (load_ret == 1) {
      unsigned long long cell_target_g_location_rm;
      result_vector_index = 0;

      // unsigned long long lrm;
      t_start = AU_WTIME;

#if defined(_OPENMP)
      size_t *prefix;
#endif

#if defined(_OPENMP)
#pragma omp parallel
#endif
      {
        std::vector<unsigned long long> cell_coordinate(data_dims, 0),
            cell_coordinate_ol(data_dims, 0),
            global_cell_coordinate(data_dims, 0);
        unsigned long long offset_ol;
        Stencil<T> cell_target(0, &current_chunk_data[0], cell_coordinate_ol,
                               current_chunk_ol_size, data_size);
        if (has_padding_value_flag) {
          cell_target.SetPadding(padding_value);
        }
        cell_target.SetChunkID(prev_chunk_id);

        if (get_stencil_tag_flag) {
          cell_target.SetTagMap(stencil_metadata_map);
        }
        Stencil<UDFOutputType> cell_return_stencil;
        UDFOutputType cell_return_value;
        unsigned long long cell_target_g_location_rm;
        int is_mirror_value = 0;
        std::vector<unsigned long long> skip_chunk_coordinate(data_dims, 0),
            skip_chunk_coordinate_start(data_dims, 0);
        int skip_flag_on_cell = 0;

        std::vector<unsigned long long> begin_coo(data_dims, 0);
#if defined(_OPENMP)
        int ithread = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
#pragma omp single
        {
          prefix = new size_t[nthreads + 1];
          prefix[0] = 0;
        }
        std::vector<UDFOutputType> vec_private;
#endif

#if defined(_OPENMP)
#pragma omp for schedule(static) nowait
#endif
        for (unsigned long long i = 0; i < current_chunk_cells; i++) {
          // ROW_MAJOR_ORDER_REVERSE_MACRO_3D(i, current_chunk_size,
          // current_chunk_size.size(), cell_coordinate)
          if (i != 0)
            INCREASE_COORDINATE(begin_coo, current_chunk_size, cell_coordinate,
                                data_dims);

          if (skip_flag == 1) {
            skip_flag_on_cell = 0;
            for (int id = 0; id < data_dims; id++) {
              // The coordinate of the skip chunk this coordinate belong to
              skip_chunk_coordinate[id] =
                  std::floor(cell_coordinate[id] / skip_size[id]);
              skip_chunk_coordinate_start[id] =
                  skip_chunk_coordinate[id] *
                  skip_size[id]; // first cell's coodinate of this skip chunk
              if (skip_chunk_coordinate_start[id] !=
                  cell_coordinate[id]) { // we only run on the first  element of
                                         // this skip chunk
                skip_flag_on_cell = 1;
                break;
              }
            }

            if (skip_flag_on_cell == 1)
              continue; //          for (unsigned long long i = 0; i <
                        //          current_chunk_cells; i++)
            assert(i < current_chunk_cells);
          }

          // Get the coodinate with overlapping
          // Also, get the global coodinate of the cell in original array
          for (int ii = 0; ii < data_dims; ii++) {
            if (cell_coordinate[ii] + ol_origin_offset[ii] <
                current_chunk_ol_size[ii]) {
              cell_coordinate_ol[ii] =
                  cell_coordinate[ii] + ol_origin_offset[ii];
            } else {
              cell_coordinate_ol[ii] = current_chunk_ol_size[ii] - 1;
            }
            // get the global coordinate
            global_cell_coordinate[ii] =
                current_chunk_start_offset[ii] + cell_coordinate[ii];
          }

          // Update the offset with overlapping
          // ROW_MAJOR_ORDER_MACRO(current_chunk_ol_size,
          // current_chunk_ol_size.size(), cell_coordinate_ol, offset_ol);
          if (endpoint != NULL && endpoint->GetEndpointType() != EP_DIR) {
            // ROW_MAJOR_ORDER_MACRO(data_size, data_size.size(),
            // global_cell_coordinate, cell_target_g_location_rm)
            // cell_target.SetLocation(offset_ol, cell_coordinate_ol,
            // cell_coordinate, current_chunk_size, ol_origin_offset,
            // current_chunk_ol_size, global_cell_coordinate,
            // cell_target_g_location_rm); cout << "U\n";
            // cell_target.SetLocation2(cell_coordinate_ol,
            // global_cell_coordinate);
            cell_target.my_location = cell_coordinate_ol;
            cell_target.global_coordinate = global_cell_coordinate;
          } else {
            cell_target.SetLocation(offset_ol, cell_coordinate_ol,
                                    cell_coordinate, current_chunk_size,
                                    ol_origin_offset, current_chunk_ol_size,
                                    cell_coordinate_ol, offset_ol);
          }
          // Set the global coodinate of the cell as the ID of the cell
          // Disable it for performance.
          // RowMajorOrder(data_dims_size, global_cell_coordinate)
          // ROW_MAJOR_ORDER_MACRO(data_dims_size, data_dims_size.size(),
          // global_cell_coordinate, cell_target_g_location_rm)
          // cell_target.set_my_g_location_rm(cell_target_g_location_rm);

          // cell_return_stencil = UDF(cell_target); // Called by C++
          UDF(cell_target, cell_return_stencil); // Called by C++
          cell_return_value = cell_return_stencil.get_value();

          if (vector_type_flag == true) {
            cell_return_stencil.GetShape(output_vector_shape);
          }

          if (cell_return_stencil.HasTagMap()) {
            has_output_stencil_tag_flag = true;
            cell_return_stencil.GetTagMap(output_stencil_metadata_map);
          }

          if (save_result_flag) {
            if (skip_flag) {
#if defined(_OPENMP)
              vec_private.push_back(cell_return_value);
#else
              current_result_chunk_data[result_vector_index] =
                  cell_return_value;
              result_vector_index = result_vector_index + 1;
#endif
              // When skip_flag is set, there is no need to have
              // apply_replace_flag Todo: in future if(apply_replace_flag == 1){
              //   current_chunk_data[i] = cell_return_value; //Replace the
              //   orginal data
              // }
            } else {
              current_result_chunk_data[i] =
                  cell_return_value; // cell_return =  cell_return.
              if (apply_replace_flag == 1) {
                std::memcpy(&current_chunk_data[offset_ol], &cell_return_value,
                            sizeof(T));
              }
            }
          }
        } // end for loop, finish the processing on a single chunk in row-major
          // direction
#if defined(_OPENMP)
        prefix[ithread + 1] = vec_private.size();
#pragma omp barrier
#pragma omp single
        {
          for (int i = 1; i < (nthreads + 1); i++) {
            prefix[i] += prefix[i - 1];
          }
          if (current_result_chunk_data.size() != prefix[nthreads]) {
            std::cout << "Wrong output size ! prefix[nthreads] ="
                      << prefix[nthreads] << ", current.size() = "
                      << current_result_chunk_data.size() << " \n ";
          }
        } // end of omp for
        std::copy(vec_private.begin(), vec_private.end(),
                  current_result_chunk_data.begin() + prefix[ithread]);
        clear_vector(vec_private);
#endif
      } // end of omp parallel

#if defined(_OPENMP)
      delete[] prefix;
#endif
      time_udf = AU_WTIME - t_start + time_udf;

      //////////////////////////////////////
      // end of processing on a single chunk
      /////////////////////////////////////

      t_start = AU_WTIME;

      if (B != nullptr) {

        std::vector<unsigned long long> B_data_size;
        std::vector<int> B_data_chunk_size, B_data_overlap_size;

        if (B->GetEndpointType() == EP_DIR && GetEndpointType() == EP_DIR) {
          std::vector<std::string> dir_file_list = GetDirFile();
          B->SetDirFile(dir_file_list);
          std::vector<int> dir_chunk_size = GetDirChunkSize();
          B->SetDirChunkSize(dir_chunk_size);
        }

        // PrintVector("Debug: create B_data_size = ", B_data_size);
        // PrintVector("Debug: create B_data_chunk_size = ", B_data_chunk_size);

        if (vector_type_flag) {
          // output_vector_shape
          size_t vector_size;
          std::vector<unsigned long long> current_chunk_start_offset_v =
                                              current_result_chunk_start_offset,
                                          current_chunk_end_offset_v =
                                              current_result_chunk_end_offset;
          void *data_point;
          // data_point = FlatVector(current_result_chunk_data,
          // output_vector_flat_direction_index, current_chunk_start_offset_v,
          // current_chunk_end_offset_v, vector_size);
          // InferOutputSize(B_data_size, B_data_chunk_size,
          // B_data_overlap_size, vector_size);

          if (is_the_last_chunk && (previous_output_vector_shape.size() == 0)) {
            // int normal_chunk_output_size = data_chunk_size[i] /
            // (current_chunk_ol_size[i] / output_vector_shape[i]);
            previous_output_vector_shape = output_vector_shape;
            for (int i = 0; i < output_vector_shape.size(); i++) {
              if (skip_not_aligned_w_array_flag &&
                  skip_not_aligned_w_array_index == i) {
                previous_output_vector_shape[i] =
                    data_chunk_size[i] /
                    (current_chunk_ol_size[i] / output_vector_shape[i]);
              }
            }
          }
          // PrintVector("Debug:  output_vector_shape = ", output_vector_shape);
          // PrintVector("Debug:  current_chunk_start_offset_v = ",
          // current_chunk_start_offset_v); PrintVector("Debug:
          // current_chunk_end_offset_v = ", current_chunk_end_offset_v);
          // PrintVector("Debug:  previous_output_vector_shape = ",
          // previous_output_vector_shape);

          data_point = InsertOutputVV2WriteV(
              current_result_chunk_data, output_vector_shape,
              current_chunk_start_offset_v, current_chunk_end_offset_v,
              is_the_last_chunk, previous_output_vector_shape);
          CalculateOutputSize(B_data_size, B_data_chunk_size,
                              B_data_overlap_size);
          PrintVector("Debug: create B_data_size = ", B_data_size);
          PrintVector("Debug: create B_data_chunk_size = ", B_data_chunk_size);
          B->CreateEndpoint(B_data_size, B_data_chunk_size,
                            B_data_overlap_size);
          // PrintVector("Debug: write current_chunk_start_offset_v = ",
          // current_chunk_start_offset_v); PrintVector("Debug: write
          // current_chunk_end_offset_v = ", current_chunk_end_offset_v);
          B->WriteEndpoint(current_chunk_start_offset_v,
                           current_chunk_end_offset_v, data_point);
          free(data_point);
        } else {
          InferOutputSize(B_data_size, B_data_chunk_size, B_data_overlap_size,
                          0);
          B->CreateEndpoint(B_data_size, B_data_chunk_size,
                            B_data_overlap_size);

          PrintVector("Debug: create B_data_size = ", B_data_size);
          PrintVector("Debug: create B_data_chunk_size = ", B_data_chunk_size);

          // B->WriteEndpoint(current_chunk_start_offset,
          // current_chunk_end_offset, &current_result_chunk_data[0]);
          if (!skip_flag) {
            // PrintVector("current_chunk_start_offset = ",
            // current_chunk_start_offset);
            // PrintVector("current_chunk_end_offset = ",
            // current_chunk_end_offset);
            B->WriteArray(current_result_chunk_start_offset,
                          current_chunk_end_offset, current_result_chunk_data);
          } else {
            // PrintVector("current_chunk_start_offset = ",
            // current_result_chunk_start_offset);
            // PrintVector("current_chunk_end_offset = ",
            // current_result_chunk_end_offset);
            B->WriteArray(current_result_chunk_start_offset,
                          current_result_chunk_end_offset,
                          current_result_chunk_data);
          }
        }
      }

      if (has_output_stencil_tag_flag) {
        for (std::map<std::string, std::string>::iterator it =
                 output_stencil_metadata_map.begin();
             it != output_stencil_metadata_map.end(); ++it) {

          B->SetTag(it->first, it->second);
          // std::cout << "Key: " << it->first << std::endl();
          // std::cout << "Value: " << it->second << std::endl();
        }
        has_output_stencil_tag_flag = false;
      }

      time_write = time_write + AU_WTIME - t_start;

      if (vector_type_flag == true) {
        previous_output_vector_shape = output_vector_shape;
      }

      t_start = AU_WTIME;
      load_ret = LoadNextChunk(current_result_chunk_data_size);
      current_result_chunk_data.resize(current_result_chunk_data_size);
      time_read = time_read + AU_WTIME - t_start;

    } // end of while:: no more chunks to process

    // May start a empty write for collective I/O
    if ((data_total_chunks % ft_size != 0) &&
        (current_chunk_id >= data_total_chunks) &&
        (current_chunk_id <
         (data_total_chunks + ft_size - (data_total_chunks % ft_size))) &&
        B != nullptr) {
      // std::cout << "current_chunk_id = " << current_chunk_id << std::endl;
      // std::cout << "leftover_chunks  = " << data_total_chunks % ft_size <<
      // std::endl; std::cout << "data_total_chunks  = " << data_total_chunks <<
      // std::endl;
      std::vector<unsigned long long> null_start; // Start offset on disk
      std::vector<unsigned long long> null_end;   // End offset on disk
      void *data_point = nullptr;
      null_start.resize(data_dims, 0);
      null_end.resize(data_dims, 0);
      B->WriteEndpoint(null_start, null_end, data_point);
    }

    return;
  }

  void EnableWriteRootOnly() { is_write_root_only = true; }

  void DisableWriteRootOnly() { is_write_root_only = false; }

  int WriteArray(const std::vector<unsigned long long> &start_p,
                 const std::vector<unsigned long long> &end_p,
                 std::vector<T> &data_p) {
    if (!is_endpoint_created_flag) {
      CreateEndpoint(data_size, data_chunk_size, data_overlap_size);
    }
    // InitializeApplyInput();
    if (!virtual_array_flag) {
      return endpoint->Write(start_p, end_p,
                             static_cast<void *>(data_p.data()));
    } else {

      int n = attribute_endpoint_vector.size();
      for (int i = 0; i < n; i++) {
        // std::cout << "write " << i << " \n" ;
        void *current_chunk_data_void_p =
            ExtractAttributeFromVirtualArrayVector(
                data_p, i, attribute_endpoint_vector[i]->GetDataElementType(),
                attribute_endpoint_vector[i]->GetDataElementTypeSize());
        attribute_endpoint_vector[i]->Write(start_p, end_p,
                                            current_chunk_data_void_p);
        free(current_chunk_data_void_p);
      }
      return 1;
    }
  }

  /**
   * @brief Test a new API to read modfy and write a array.
   *        It is useful for cases that you have a incremently updated array
   *        //https://en.wikipedia.org/wiki/Read%E2%80%93modify%E2%80%93write
   * @param start_p
   * @param end_p
   * @param data_p
   * @param op
   * @return int
   */
  int ReadModifyWriteArray(const std::vector<unsigned long long> &start_p,
                           const std::vector<unsigned long long> &end_p,
                           std::vector<T> &data_p, AU_Op op) {
    if (!is_endpoint_created_flag) {
      CreateEndpoint(data_size, data_chunk_size, data_overlap_size);
    }
    // InitializeApplyInput();
    if (!virtual_array_flag) {
      std::vector<T> data_p_read;
      data_p_read.resize(data_p.size());
      endpoint->Read(start_p, end_p, static_cast<void *>(data_p.data()));
      switch (op) {
      case AU_SUM:
        std::transform(data_p_read.begin(), data_p_read.end(), data_p.begin(),
                       data_p.begin(), std::plus<T>());
        break;
      default:
        AU_EXIT("Not supported op type in ReadModifyWriteArray now\n");
        break;
      }
      return endpoint->Write(start_p, end_p,
                             static_cast<void *>(data_p.data()));
    } else {

      AU_EXIT("Not supported ReadModifyWriteArray on virtual array yet !\n");
      return -1;
    }
  }

  int WriteArray(std::vector<unsigned long long> &start_p,
                 std::vector<unsigned long long> &end_p,
                 std::vector<std::vector<T>> data_p) {
    AU_EXIT("You should not be here !");
    return 0;
  }

  unsigned long long CountCells(const std::vector<unsigned long long> &start_p,
                                const std::vector<unsigned long long> &end_p) {
    // Check if the dimensions of start_p and end_p are the same
    if (start_p.size() != end_p.size()) {
      // Handle error: Dimensions mismatch
      return 0; // You can also throw an exception or handle it according to
                // your needs
    }

    // Calculate the number of cells in each dimension
    unsigned long long total_cells = 1;
    for (size_t i = 0; i < start_p.size(); ++i) {
      // Ensure that the end point is greater than or equal to the start point
      // in each dimension
      if (end_p[i] < start_p[i]) {
        // Handle error: End point is less than start point
        return 0; // You can also throw an exception or handle it according to
                  // your needs
      }

      // Calculate the number of cells in the current dimension
      total_cells *= (end_p[i] - start_p[i] + 1);
    }

    return total_cells;
  }

  int WriteEndpoint(std::vector<unsigned long long> &start_p,
                    std::vector<unsigned long long> &end_p, void *data) {
    // PrintVector("start_p =", start_p);
    // PrintVector("end_p = ", end_p);

    if (!virtual_array_flag) {
      return endpoint->Write(start_p, end_p, data);
    } else {
      int n = attribute_endpoint_vector.size();
      size_t total_cells = CountCells(start_p, end_p);
      // std::vector<T> vt(data, data + total_cells);
      std::vector<T> vt;
      vt.reserve(total_cells); // Reserve space for n elements in the vector

      // Copy the elements from the allocated memory to the vector
      T *arr = static_cast<T *>(data);
      std::copy(arr, arr + total_cells, std::back_inserter(vt));

      for (int i = 0; i < n; i++) {
        std::cout << "WriteArray write attribute [ " << i << " ] "
                  << "total_cells = " << total_cells << " \n";
        void *current_chunk_data_void_p =
            ExtractAttributeFromVirtualArrayVector(
                vt, i, attribute_endpoint_vector[i]->GetDataElementType(),
                attribute_endpoint_vector[i]->GetDataElementTypeSize());
        attribute_endpoint_vector[i]->Write(start_p, end_p,
                                            current_chunk_data_void_p);
        free(current_chunk_data_void_p);
      }
      return 1;

      // AU_EXIT("Not supported yet !\n");
    }

    return 0;
  }

  int ReadEndpoint(std::vector<unsigned long long> &start_p,
                   std::vector<unsigned long long> &end_p, void *data) {
    return endpoint->Read(start_p, end_p, data);
  }

  /**
   * @brief Calculate the Size of Output array (B)
   *
   * @param data_size_p, the size of the output array
   * @param data_chunk_size_p , the chunk size of the output array
   * @param data_overlap_size_p , the overlap size of the output array
   */
  void inline CalculateOutputSize(std::vector<unsigned long long> &data_size_p,
                                  std::vector<int> &data_chunk_size_p,
                                  std::vector<int> &data_overlap_size_p) {
    // Add this to support vector search on 1D case
    if (is_write_root_only && is_execute_udf_once) {
      data_size_p.resize(output_vector_shape.size());
      for (int i = 0; i < output_vector_shape.size(); i++)
        data_size_p[i] = output_vector_shape[i];
      return;
    }

    if (skip_flag) {
      data_size_p = skiped_dims_size;
      data_chunk_size_p = skiped_chunk_size;
      data_overlap_size_p =
          data_overlap_size; // Todo:  need to consider data_overlap size
    } else {
      data_size_p = data_size;
      data_chunk_size_p = data_chunk_size;
      data_overlap_size_p = data_overlap_size;
    }

    // we may update the data_size_p/data_chunk_size_p/data_overlap_size_p if
    // vector used output_vector_shape is the parameter
    if (vector_type_flag) {
      int rank = data_size_p.size();
      for (int i = 0; i < output_vector_shape.size(); i++) {
        if (i >= rank) {
          data_size_p.push_back(output_vector_shape[i]);
          data_chunk_size_p.push_back(output_vector_shape[i]);
          data_overlap_size_p.push_back(0);
        } else {
          if (skip_not_aligned_w_array_flag &&
              skip_not_aligned_w_array_index == i) {
            if (!is_the_last_chunk) {
              data_size_p[i] = (data_size_p[i] - 1) * output_vector_shape[i];
              data_size_p[i] = data_size_p[i] + ((data_size[i] % skip_size[i]) *
                                                 output_vector_shape[i]) /
                                                    data_chunk_size[i];
            } else {
              // we need to update the output_vector_shape,  data_chunk_size,
              // current_chunk_ol_size PrintVector("data_chunk_size");
              //     600000 / (630000 / 62219)
              int normal_chunk_output_size =
                  data_chunk_size[i] /
                  (current_chunk_ol_size[i] / output_vector_shape[i]);
              PrintScalar("data_chunk_size[i] =", data_chunk_size[i]);
              PrintScalar("current_chunk_ol_size[i] =",
                          current_chunk_ol_size[i]);
              PrintScalar("output_vector_shape[i] =", output_vector_shape[i]);
              PrintScalar("normal_chunk_output_size =",
                          normal_chunk_output_size);
              PrintScalar("skip_size[i] = ", skip_size[i]);
              PrintScalar("data_size[i] = ", data_size[i]);
              PrintScalar("data_size[i] % skip_size[i] = ",
                          data_size[i] % skip_size[i]);

              data_size_p[i] = (data_size_p[i] - 1) * normal_chunk_output_size;
              data_size_p[i] = data_size_p[i] + ((data_size[i] % skip_size[i]) *
                                                 normal_chunk_output_size) /
                                                    data_chunk_size[i];
            }
          } else {
            data_size_p[i] = data_size_p[i] * output_vector_shape[i];
            data_chunk_size_p[i] = output_vector_shape[i];
          }
        }
      }
    }
  }
  /**
   * @brief infer the size for output array
   *
   * @param data_size_p
   * @param data_chunk_size_p
   * @param data_overlap_size_p
   * @param output_vector_size
   */
  void inline InferOutputSize(std::vector<unsigned long long> &data_size_p,
                              std::vector<int> &data_chunk_size_p,
                              std::vector<int> &data_overlap_size_p,
                              size_t output_vector_size) {
    if (skip_flag) {
      if (!vector_type_flag) {
        data_size_p = skiped_dims_size;
        data_chunk_size_p = skiped_chunk_size;
        data_overlap_size_p =
            data_overlap_size; // Todo:  need to consider data_overlap size
      } else                   // skip_flag == true and vector_type_flag == true
      {
        int data_dims_t;
        std::vector<unsigned long long> skiped_dims_size_t;
        std::vector<int> skiped_chunk_size_t;
        std::vector<int> data_overlap_size_t;

        if (output_vector_flat_direction_index >
            (data_dims - 1)) { // We save the output vector as new dimensions
          data_dims_t = data_dims + 1;
          skiped_dims_size_t.resize(data_dims_t);
          skiped_chunk_size_t.resize(data_dims_t);
          data_overlap_size_t.resize(data_dims_t);
          for (int k = 0; k < data_dims; k++) {
            skiped_dims_size_t[k] = skiped_dims_size[k];
            data_overlap_size_t[k] = data_overlap_size[k];
            skiped_chunk_size_t[k] = skiped_chunk_size[k];
          }
          skiped_dims_size_t[data_dims] = output_vector_size;
          data_overlap_size_t[data_dims] = 0;
          skiped_chunk_size_t[data_dims] = output_vector_size;
        } else { // We save the output vector within existing dimensions
          data_dims_t = data_dims;
          skiped_dims_size_t.resize(data_dims_t);
          skiped_chunk_size_t.resize(data_dims_t);
          data_overlap_size_t.resize(data_dims_t);
          for (int k = 0; k < data_dims; k++) {
            skiped_dims_size_t[k] = skiped_dims_size[k];
            if (k == output_vector_flat_direction_index)
              skiped_dims_size_t[k] =
                  skiped_dims_size_t[k] * output_vector_size;
            data_overlap_size_t[k] = data_overlap_size[k];
            skiped_chunk_size_t[k] = skiped_chunk_size[k];
          }
        }

        data_size_p = skiped_dims_size_t;
        data_chunk_size_p = skiped_chunk_size_t;
        data_overlap_size_p = data_overlap_size_t;
      }
    } else { // skip_flag == false
      if (vector_type_flag == 0) {
        data_size_p = data_size;
        data_chunk_size_p = data_chunk_size;
        data_overlap_size_p = data_overlap_size;
      } else // skip_flag == false and vector_type_flag == true
      {
        int data_dims_t;
        std::vector<unsigned long long> dims_size_t;
        std::vector<int> chunk_size_t;
        std::vector<int> data_overlap_size_t;

        if (output_vector_flat_direction_index > (data_dims - 1)) {
          data_dims_t = data_dims + 1;
          dims_size_t.resize(data_dims_t);
          chunk_size_t.resize(data_dims_t);
          data_overlap_size_t.resize(data_dims_t);
          for (int k = 0; k < data_dims; k++) {
            dims_size_t[k] = data_size[k];
            chunk_size_t[k] = data_chunk_size[k];
            data_overlap_size_t[k] = data_overlap_size[k];
          }
          dims_size_t[data_dims] = output_vector_size;
          data_overlap_size_t[data_dims] = 0;
          chunk_size_t[data_dims] = output_vector_size;
        } else {
          data_dims_t = data_dims;
          dims_size_t.resize(data_dims_t);
          chunk_size_t.resize(data_dims_t);
          data_overlap_size_t.resize(data_dims_t);
          for (int k = 0; k < data_dims; k++) {
            dims_size_t[k] = data_size[k];
            if (k == output_vector_flat_direction_index)
              dims_size_t[k] = dims_size_t[k] * output_vector_size;
            // dims_size_t[k] = data_dims_size[k] * output_vector_size;
            chunk_size_t[k] = data_chunk_size[k];
            data_overlap_size_t[k] = data_overlap_size[k];
          }
        }

        data_size_p = dims_size_t;
        data_chunk_size_p = chunk_size_t;
        data_overlap_size_p = data_overlap_size_t;
      }
    }
  }

  int inline CreateEndpoint(std::vector<unsigned long long> data_size_p,
                            std::vector<int> data_chunk_size_p,
                            std::vector<int> data_overlap_size_p) {
    if (!virtual_array_flag && endpoint->GetOpenFlag())
      return 0;

    if (is_endpoint_created_flag)
      return 0;

    // cout << "CreateEndpoint: is_endpoint_created_flag " <<
    // is_endpoint_created_flag << "\n";
    is_endpoint_created_flag = true;

    data_size = data_size_p;
    data_chunk_size = data_chunk_size_p;
    data_overlap_size = data_overlap_size_p;

    if (virtual_array_flag) {
      for (int i = 0; i < attribute_endpoint_vector.size(); i++) {
        if (attribute_endpoint_vector[i]->GetOpenFlag())
          return 0;
        attribute_endpoint_vector[i]->SetDimensions(data_size);
        attribute_endpoint_vector[i]->Create();
        attribute_endpoint_vector[i]
            ->Close(); // call it to make sure it is consistency
      }
      return 0;
    } else {
      AuEndpointDataType type_p = InferDataType<T>();
      endpoint->SetDimensions(data_size);
      endpoint->SetDataElementType(type_p);
      return endpoint->Create();
    }
  }

  /**
   * @brief read the current chunk once the chunk_size and overlap_size are
   * specified at Constructor
   *
   * @param data_vector
   * @return int
   */
  int ReadChunk(std::vector<T> &data_vector) {
    Stencil<T> (*UDF)(const Stencil<T>);
    InitializeApplyInput(UDF);
    int ret;
    unsigned long long current_result_chunk_data_size = 1;
    ret = LoadNextChunk(current_result_chunk_data_size);
    data_vector = current_chunk_data;
    return ret;
  }

  /**
   * @brief read the current chunk once the chunk_size and overlap_size are
   * specified at Constructor
   *
   * @param data_vector
   * @return int
   *   1, data read into   local_chunk_data
   *     0, end of file (no data left to handle)
   *    -1: error happen
   */
  int ReadNextChunk(std::vector<T> &data_vector) {
    // We need to set chunk size first
    if (!is_init_called_in_rnc) {
      Stencil<T> (*UDF)(const Stencil<T>);
      InitializeApplyInput(UDF);
      is_init_called_in_rnc = true;
    }
    int ret;
    unsigned long long current_result_chunk_data_size = 1;
    ret = LoadNextChunk(current_result_chunk_data_size);
    data_vector = current_chunk_data;
    return ret;
  }

  /**
   * @brief Read a array cells from start to the end
   *
   * @param start
   * @param end
   * @param data_vector
   * @return int
   */
  int ReadArrayAll(std::vector<T> &data_vector) {
    // InitializeApplyInput();
    std::vector<unsigned long long> start, end;
    if (!virtual_array_flag) {
      endpoint->ExtractMeta();
      end = endpoint->GetDimensions();
    } else {
      assert(attribute_endpoint_vector.size() >= 1);
      attribute_endpoint_vector[0]->ExtractMeta();
      end = attribute_endpoint_vector[0]->GetDimensions();
    }

    start.resize(end.size());
    for (int i = 0; i < end.size(); i++) {
      start[i] = 0;
      end[i] = end[i] - 1;
    }

    unsigned long long data_vector_size;
    COUNT_CELLS(start, end, data_vector_size);

    // std::vector<T> data_vector;
    data_vector.resize(data_vector_size);
    if (!virtual_array_flag) {
      endpoint->Read(start, end, &data_vector[0]);
    } else {
      int n = attribute_endpoint_vector.size();
      std::vector<AuEndpointDataTypeUnion> current_chunk_data_union_vector;
      for (int i = 0; i < n; i++) {
        int element_type_size =
            attribute_endpoint_vector[i]->GetDataElementTypeSize();
        void *current_chunk_data_temp =
            (void *)malloc(data_vector_size * element_type_size);
        if (!current_chunk_data_temp) {
          AU_EXIT("Not enough memory");
        }
        attribute_endpoint_vector[i]->Read(start, end, current_chunk_data_temp);
        current_chunk_data_union_vector =
            attribute_endpoint_vector[i]->Void2Union(current_chunk_data_temp,
                                                     data_vector_size);
        InsertAttribute2VirtualArrayVector(
            current_chunk_data_union_vector,
            attribute_endpoint_vector[i]->GetDataElementType(), data_vector, i);
        free(current_chunk_data_temp);
      }
    }
    return 0;
  }

  /**
   * @brief Read a array cells from start to the end
   *
   * @param start
   * @param end
   * @param data_vector
   * @return int
   */
  int ReadArray(const std::vector<unsigned long long> &start,
                const std::vector<unsigned long long> &end,
                std::vector<T> &data_vector) {
    // InitializeApplyInput();
    unsigned long long data_vector_size;
    COUNT_CELLS(start, end, data_vector_size);

    // std::vector<T> data_vector;
    data_vector.resize(data_vector_size);
    if (!virtual_array_flag) {
      endpoint->Read(start, end, &data_vector[0]);
    } else {
      int n = attribute_endpoint_vector.size();
      std::vector<AuEndpointDataTypeUnion> current_chunk_data_union_vector;
      for (int i = 0; i < n; i++) {
        int element_type_size =
            attribute_endpoint_vector[i]->GetDataElementTypeSize();
        void *current_chunk_data_temp =
            (void *)malloc(data_vector_size * element_type_size);
        if (!current_chunk_data_temp) {
          AU_EXIT("Not enough memory");
        }
        attribute_endpoint_vector[i]->Read(start, end, current_chunk_data_temp);
        current_chunk_data_union_vector =
            attribute_endpoint_vector[i]->Void2Union(current_chunk_data_temp,
                                                     data_vector_size);
        InsertAttribute2VirtualArrayVector(
            current_chunk_data_union_vector,
            attribute_endpoint_vector[i]->GetDataElementType(), data_vector, i);
        free(current_chunk_data_temp);
      }
    }
    return 0;
  }
  // used by the old code
  inline std::vector<T> ReadArray(const std::vector<unsigned long long> start,
                                  const std::vector<unsigned long long> end) {
    std::vector<T> data_vector;
    ReadArray(start, end, data_vector);
    return data_vector;
  }

  inline bool HasNextChunk() {
    // Only read will call this function, so it's OK to return it
    if (endpoint->GetEndpointType() == EP_RabbitMQ ||
        GetEndpointType() == EP_RabbitMQ_RESTAPI)
      return true;

    // Only read will call this function, so it's OK to return it
    if (endpoint->GetEndpointType() == EP_DIR_STREAM)
      return true;

    switch (scheduling_method) {
    case CHUNK_SCHEDULING_RR:
      if (current_chunk_id >= data_total_chunks || current_chunk_id < 0) {
        return false;
      }
      // current_chunk_id = current_chunk_id + ft_size;
      break;
    case CHUNK_SCHEDULING_CR:
      if (current_chunk_id >=
          CRMyLastChunks(data_total_chunks, ft_rank, ft_size)) {
        return false;
      }
      // current_chunk_id = current_chunk_id + 1;
      break;
    default:
      AU_EXIT("Unsupported scheduling methods !\n");
      break;
    }

    return true;
  }

  inline unsigned long long InitFirstChunk() {
    switch (scheduling_method) {
    case CHUNK_SCHEDULING_RR:
      return ft_rank;
      break;
    case CHUNK_SCHEDULING_CR:
      return CRMyStartChunk(data_total_chunks, ft_rank, ft_size);
      break;
    default:
      AU_EXIT("Unsupported scheduling methods !\n");
      break;
    }
  }
  /**
   * @brief update current_chunk_id
   *
   * @return int
   */
  inline void ScheduleChunkNext() {
    prev_chunk_id = current_chunk_id;
    // Next chunk id
    // if (!reverse_apply_direction_flag)
    //{
    // current_chunk_id = current_chunk_id + ft_size;
    //}
    // else
    //{
    // current_chunk_id = current_chunk_id - ft_size;
    //}
    switch (scheduling_method) {
    case CHUNK_SCHEDULING_RR:
      current_chunk_id = current_chunk_id + ft_size;
      break;
    case CHUNK_SCHEDULING_CR:
      current_chunk_id = current_chunk_id + 1;
      break;
    default:
      AU_EXIT("Unsupported scheduling methods !\n");
      break;
    }

    // std::cout << "Rank [" << ft_rank << "]: current_chunk_id = " <<
    // prev_chunk_id << " , next chunk = " << current_chunk_id << "\n";
  }
  /**
   * @brief Load the next chunk
   *
   * @param result_vector_size
   * @return
   *     1, data read into   local_chunk_data
   *     0, end of file (no data left to handle)
   *    -1: error happen
   */
  int LoadNextChunk(unsigned long long &result_vector_size) {
    if (!HasNextChunk()) {
      return 0;
    }

    if (GetEndpointType() == EP_DIR_STREAM &&
        variable_chunking_from_file_flag) {
      std::vector<std::string> para;
      endpoint->Control(DIR_STREAM_GET_NEXT_SUB_SIZE, para);
      String2Vector(para[0], data_chunk_size);
      data_overlap_size.resize(data_chunk_size.size(), 0);
    }

    // We only handle last chunk when skip is not even with the array size
    if (current_chunk_id == (data_total_chunks - 1) &&
        skip_not_aligned_w_array_flag)
      is_the_last_chunk = true;

    result_vector_size = 0;

    current_chunk_cells = 1;
    current_result_chunk_cells = 1;
    current_chunk_ol_cells = 1;

    // PrintVector("LoadNextChunk.data_chunked_size = ", data_chunked_size);
    std::vector<unsigned long long> chunk_coordinate =
        RowMajorOrderReverse(current_chunk_id, data_chunked_size);
    std::vector<unsigned long long> skiped_chunk_coordinate;
    if (skip_flag)
      skiped_chunk_coordinate =
          RowMajorOrderReverse(current_chunk_id, skiped_chunks);

    // calculate the start and end of a chunk
    for (int i = 0; i < data_dims; i++) {
      if (data_chunk_size[i] * chunk_coordinate[i] < data_size[i]) {
        current_chunk_start_offset[i] =
            data_chunk_size[i] * chunk_coordinate[i];
      } else {
        current_chunk_start_offset[i] = data_size[i];
      }

      if (current_chunk_start_offset[i] + data_chunk_size[i] - 1 <
          data_size[i]) {
        current_chunk_end_offset[i] =
            current_chunk_start_offset[i] + data_chunk_size[i] - 1;
      } else {
        current_chunk_end_offset[i] = data_size[i] - 1;
      }

      assert((current_chunk_end_offset[i] - current_chunk_start_offset[i] + 1 >=
              0));
      current_chunk_size[i] =
          current_chunk_end_offset[i] - current_chunk_start_offset[i] + 1;
      current_chunk_cells = current_chunk_cells * current_chunk_size[i];

      // Deal with the result chunks size
      if (!skip_flag) {
        current_result_chunk_start_offset[i] = current_chunk_start_offset[i];
        current_result_chunk_end_offset[i] = current_chunk_end_offset[i];
        current_result_chunk_cells = current_chunk_cells;
      } else {
        if (skiped_chunk_coordinate[i] * skiped_chunk_size[i] <
            skiped_dims_size[i]) {
          current_result_chunk_start_offset[i] =
              skiped_chunk_coordinate[i] * skiped_chunk_size[i];
        } else {
          current_result_chunk_start_offset[i] = skiped_dims_size[i];
        }

        if (current_result_chunk_start_offset[i] + skiped_chunk_size[i] - 1 <
            skiped_dims_size[i]) {
          current_result_chunk_end_offset[i] =
              current_result_chunk_start_offset[i] + skiped_chunk_size[i] - 1;
        } else {
          current_result_chunk_end_offset[i] = skiped_dims_size[i] - 1;
        }
        assert((current_result_chunk_end_offset[i] -
                    current_result_chunk_start_offset[i] + 1 >=
                0));
        current_result_chunk_cells = current_result_chunk_cells *
                                     (current_result_chunk_end_offset[i] -
                                      current_result_chunk_start_offset[i] + 1);
      } // end of result chunk

      // Deal with overlapping
      // Starting coordinate for the data chunk with overlapping
      if (!is_disable_overlap_lower_set) {
        if (current_chunk_start_offset[i] <= data_overlap_size[i]) {
          current_chunk_ol_start_offset[i] = 0;
        } else {
          current_chunk_ol_start_offset[i] =
              current_chunk_start_offset[i] - data_overlap_size[i];
        }
      } else {
        current_chunk_ol_start_offset[i] = current_chunk_start_offset[i];
      }

      // Original coordinate offset for each, used to get gloabl coordinate in
      // Apply
      ol_origin_offset[i] =
          current_chunk_start_offset[i] - current_chunk_ol_start_offset[i];

      // Ending coordinate for the data chunk with overlapping
      if (!is_disable_overlap_upper_set) {
        if (current_chunk_end_offset[i] + data_overlap_size[i] < data_size[i]) {
          current_chunk_ol_end_offset[i] =
              current_chunk_end_offset[i] + data_overlap_size[i];
        } else {
          current_chunk_ol_end_offset[i] = data_size[i] - 1;
        }
      } else {
        current_chunk_ol_end_offset[i] = current_chunk_end_offset[i];
      }
      assert((current_chunk_ol_end_offset[i] -
                  current_chunk_ol_start_offset[i] + 1 >=
              0));
      current_chunk_ol_size[i] =
          current_chunk_ol_end_offset[i] - current_chunk_ol_start_offset[i] + 1;
      current_chunk_ol_cells =
          current_chunk_ol_cells * current_chunk_ol_size[i];
    }

    current_chunk_data.resize(current_chunk_ol_cells);
    if (save_result_flag == 1) {
      if (!skip_flag) {
        result_vector_size = current_chunk_cells;
      } else {
        result_vector_size = current_result_chunk_cells;
      }
    }

    if (view_flag) {
      if (view_rank) {
        current_chunk_ol_start_offset[view_rank] =
            current_chunk_ol_start_offset[view_rank] + view_start_offset[0];
        current_chunk_ol_end_offset[view_rank] =
            current_chunk_ol_end_offset[view_rank] + view_start_offset[0];
      } else {
        for (int i = 0; i < data_dims; i++) {
          current_chunk_ol_start_offset[i] =
              current_chunk_ol_start_offset[i] + view_start_offset[i];
          current_chunk_ol_end_offset[i] =
              current_chunk_ol_end_offset[i] + view_start_offset[i];
        }
      }
    }

    // PrintVector("current_chunk_ol_end_offset = ",
    // current_chunk_ol_end_offset); PrintVector("current_chunk_ol_start_offset
    // = ", current_chunk_ol_start_offset);

    // update current_chunk_id
    ScheduleChunkNext();

    // Return  1, data read into   local_chunk_data
    // Return  0, end of file (no data left to handle)
    // Return -1: error happen
    // Read data between local_chunk_start_offset and local_chunk_end_offset
    // current_chunk_data.resize(current_chunk_cells);
    // return data_on_disk->ReadData(current_chunk_start_offset,
    // current_chunk_end_offset, current_chunk_data);
    if (!virtual_array_flag) {
      endpoint->Read(current_chunk_ol_start_offset, current_chunk_ol_end_offset,
                     &current_chunk_data[0]);
      // Get the tag if needed
      if (get_stencil_tag_flag) {
        std::vector<std::string> stencil_tag_names;
        GetAllTagName(stencil_tag_names);
        // Here we assume all value are string
        std::string tag_value;
        for (int i = 0; i < stencil_tag_names.size(); i++) {
          GetTag(stencil_tag_names[i], tag_value);
          stencil_metadata_map[stencil_tag_names[i]] = tag_value;
          // std::cout << " tag name " << stencil_tag_names[i] << " , tag value
          // = " << tag_value << "\n";
        }
        // endpoint->Control(stencil_metadata_map);
      }
      return 1;
    } else {
      int n = attribute_endpoint_vector.size();
      std::vector<AuEndpointDataTypeUnion> current_chunk_data_union_vector;
      for (int i = 0; i < n; i++) {
        int element_type_size =
            attribute_endpoint_vector[i]->GetDataElementTypeSize();
        void *current_chunk_data_temp =
            (void *)malloc(current_chunk_ol_cells * element_type_size);
        if (!current_chunk_data_temp) {
          AU_EXIT("Not enough memory");
        }
        attribute_endpoint_vector[i]->Read(current_chunk_ol_start_offset,
                                           current_chunk_ol_end_offset,
                                           current_chunk_data_temp);
        current_chunk_data_union_vector =
            attribute_endpoint_vector[i]->Void2Union(current_chunk_data_temp,
                                                     current_chunk_ol_cells);
        InsertAttribute2VirtualArrayVector(
            current_chunk_data_union_vector,
            attribute_endpoint_vector[i]->GetDataElementType(),
            current_chunk_data, i);
        free(current_chunk_data_temp);
      }
      return 1;
    }
  }

  inline int EnableVariableChunkingFromFile() {
    variable_chunking_from_file_flag = true;
    return 0;
  }
  inline int EnableApplyStride(const std::vector<int> &skip_size_p) {
    return SetStride(skip_size_p);
  }

  inline int SetStride(const std::vector<int> &skip_size_p) {
    skip_size = skip_size_p;
    skip_flag = true;
    return 0;
  }

  /**
   * @brief Set the View object
   *
   * @param start: the index of the start (including)
   * @param count: the number of elements after the start
   * @param rank : the rank on which dimension to subset
   * @return int
   */
  void SetView(unsigned long long start, unsigned long long count, int rank) {
    view_flag = true;
    is_view_rank = true;

    view_start_offset.resize(1);
    view_size.resize(1);

    view_start_offset[0] = start;
    view_size[0] = count;

    view_rank = rank;
    std::vector<unsigned long long> view_par;
    view_par.push_back(0);    // type 0
    view_par.push_back(rank); // rank to create view
    view_par.push_back(start);
    view_par.push_back(count);
    std::string view_par_str = Vector2String(view_par);
    std::vector<std::string> view_par_str_str;
    view_par_str_str.push_back(view_par_str);
    if (GetEndpointType() == EP_DIR) {
      endpoint->Control(DIR_SET_VIEW, view_par_str_str);
    }
  }

  void SetView(std::vector<unsigned long long> start,
               std::vector<unsigned long long> count) {
    view_flag = true;
    view_start_offset = start;
    view_size = count;

    /*
    view_orginal_data_dims_size = data_dims_size; // for backup
    data_dims_size = count;

    for (int i = 0; i < data_dims; i++)
    {
      if (chunk_dimension_index == i)
      {
        if (view_size[i] % mpi_size == 0)
        {
          data_chunk_size[i] = view_size[i] / mpi_size;
        }
        else
        {
          data_chunk_size[i] = view_size[i] / mpi_size + 1;
        }
      }
      else
      {
        data_chunk_size[i] = view_size[i];
      }
      data_overlap_size[i] = os[i];
    }

    data_total_chunks = 1;

    for (int i = 0; i < data_dims; i++)
    {
      if (view_size[i] % data_chunk_size[i] == 0)
      {
        data_chunked_dims_size[i] = view_size[i] / data_chunk_size[i];
      }
      else
      {
        data_chunked_dims_size[i] = view_size[i] / data_chunk_size[i] + 1;
      }
      data_total_chunks = data_total_chunks * data_chunked_dims_size[i];
    }

    //#ifdef DEBUG
    if (mpi_rank == 0)
    {
      std::cout << "View start  = ";
      for (int i = 0; i < data_dims; i++)
      {
        std::cout << ", " << view_start_offset[i];
      }
      std::cout << std::endl;

      std::cout << "View size  = ";
      for (int i = 0; i < data_dims; i++)
      {
        std::cout << ", " << view_size[i];
      }
      std::cout << std::endl;

      std::cout << "chunk size in View  = ";
      for (int i = 0; i < data_dims; i++)
      {
        std::cout << ", " << data_chunk_size[i];
      }
      std::cout << std::endl;

      std::cout << "overlap size in View = ";
      for (int i = 0; i < data_dims; i++)
      {
        std::cout << ", " << data_overlap_size[i];
      }
      std::cout << std::endl;
      std::cout << "Total chunks in View =  " << data_total_chunks << std::endl;
    }
    //#endif

    if (data_total_chunks % mpi_size != 0)
    {
      data->DisableCollectivIO();
    }

    current_chunk_id = mpi_rank; // Each process deal with one chunk one time,
    starting from its rank
    */
  }

  void SetVectorDirection(OutputVectorFlatDirection flat_direction_index) {
    output_vector_flat_direction_index = flat_direction_index;
  }

  /**
   * @brief A geneic verion of function to control how to deal with output
   * vector during the run of Apply on Array. The output of Apply in this way is
   * vector of vector (2D). We need to convert it into vector (1D). Also we need
   * to control the shape of the 1D vector to write into output Array.
   *
   * @param flat_direction,  the direction to flat the 2D vector to 1D vector
   * @param flat_shape, the shape of the data after the flat from 2D to the 2D
   */
  void ControlOutputVector(OutputVectorFlatDirection flat_direction,
                           std::vector<size_t> flat_shape) {
    output_vector_flat_direction_index = flat_direction;
    output_vector_flat_shape = flat_shape;
  }

  template <typename... Is> inline T operator()(Is... indexs) const {
    std::vector<int> iv{{indexs...}};
    std::vector<unsigned long long> start;
    std::vector<unsigned long long> end;
    std::vector<T> data_v;

    start.resize(iv.size());
    end.resize(iv.size());
    data_v.resize(iv.size());

    for (int i = 0; i < iv.size(); i++) {
      start[i] = iv[i];
      end[i] = iv[i];
    }

    endpoint->Read(start, end, static_cast<void *>(data_v.data()));
    return data_v[0];
  }

  /**
   * @brief Get the Value at indexs
   *
   * @tparam Is : index type
   * @param indexs : the index of data
   * @return T : the value at indexs
   */
  template <typename... Is> inline T GetValue(Is... indexs) {
    std::vector<int> iv{{indexs...}};
    std::vector<unsigned long long> start;
    std::vector<unsigned long long> end;

    start.resize(iv.size());
    end.resize(iv.size());

    for (int i = 0; i < iv.size(); i++) {
      start[i] = iv[i];
      end[i] = iv[i];
    }

    if (virtual_array_flag == 0) {
      std::vector<T> data_v;
      data_v.resize(1);
      endpoint->Read(start, end, static_cast<void *>(data_v.data()));
      return data_v[0];
    } else {
      std::vector<T> data_v;
      ReadArray(start, end, data_v);
      return data_v[0];
    }
  }

  template <typename... Is> inline void SetValue(T data_p, Is... indexs) {
    std::vector<int> iv{{indexs...}};
    std::vector<unsigned long long> start;
    std::vector<unsigned long long> end;
    std::vector<T> data_v;

    start.resize(iv.size());
    end.resize(iv.size());
    data_v.resize(1);

    data_v[0] = data_p;
    for (int i = 0; i < iv.size(); i++) {
      start[i] = iv[i];
      end[i] = iv[i];
    }
    if (!is_endpoint_created_flag) {
      CreateEndpoint(data_size, data_chunk_size, data_overlap_size);
    }

    endpoint->Write(start, end, static_cast<void *>(data_v.data()));
  }

  template <class AttributeType>
  void PushBackAttribute(std::string data_endpoint) {
    if (attribute_endpoint_vector.size() == 0)
      virtual_array_flag = true;
    Endpoint *attribute_endpoint = EndpointFactory::NewEndpoint(data_endpoint);
    AuEndpointDataType data_element_type = InferDataType<AttributeType>();
    attribute_endpoint->SetDataElementType(data_element_type);
    if (attribute_endpoint->GetEndpointType() == EP_MEMORY)
      endpoint_memory_flag = true;

    // Fixme: here it assign first attribute's endpoint to the object's endpoint
    if (attribute_endpoint_vector.size() == 0) {
      endpoint = attribute_endpoint;
    }
    attribute_endpoint_vector.push_back(attribute_endpoint);
  }

  template <class AttributeType>
  int AppendAttribute(const std::string &data_endpoint) {
    if (attribute_endpoint_vector.size() == 0)
      virtual_array_flag = true;
    Endpoint *attribute_endpoint = EndpointFactory::NewEndpoint(data_endpoint);
    AuEndpointDataType data_element_type = InferDataType<AttributeType>();
    attribute_endpoint->SetDataElementType(data_element_type);
    if (attribute_endpoint->GetEndpointType() == EP_MEMORY)
      endpoint_memory_flag = true;
    attribute_endpoint_vector.push_back(attribute_endpoint);
    attribute_array_data_endpoint_info.push_back(data_endpoint);
    // add to clean
    // endpoint_clean_vector[attribute_endpoint] = true;
    endpoint_to_clean_vector.push_back(attribute_endpoint);

    return 0;
  }

  template <class AttributeType>
  int InsertAttribute(const std::string &data_endpoint, const int index) {
    if (attribute_endpoint_vector.size() == 0)
      virtual_array_flag = true;
    Endpoint *attribute_endpoint = EndpointFactory::NewEndpoint(data_endpoint);
    AuEndpointDataType data_element_type = InferDataType<AttributeType>();
    attribute_endpoint->SetDataElementType(data_element_type);
    attribute_endpoint_vector.insert(attribute_endpoint_vector.begin() + index,
                                     attribute_endpoint);
    attribute_array_data_endpoint_info.insert(
        attribute_array_data_endpoint_info.begin() + index, data_endpoint);
    if (attribute_endpoint->GetEndpointType() == EP_MEMORY)
      endpoint_memory_flag = true;
    // add to clean
    // endpoint_clean_vector[attribute_endpoint] = true;
    endpoint_to_clean_vector.push_back(attribute_endpoint);
    return 0;
  }

  // index is zero based
  int EraseAttribute(const int &index) {
    if (attribute_endpoint_vector[index] != NULL) {
      delete attribute_endpoint_vector[index];
    }
    attribute_endpoint_vector.erase(attribute_endpoint_vector.begin() + index);
    attribute_array_data_endpoint_info.erase(
        attribute_array_data_endpoint_info.begin() + index);
    return 0;
  }

  int GetAttribute(const int &index, std::string &endpoint_id) {
    if (index < attribute_array_data_endpoint_info.size()) {
      endpoint_id = attribute_array_data_endpoint_info[index];
      return 0;
    }
    return -1;
  }

  std::vector<Endpoint *> GetAttributeEndpoint() {
    return attribute_endpoint_vector;
  }

  bool GetVirtualArrayFlag() { return virtual_array_flag; }

  void SetVirtualArrayFlag(bool flag_p) { virtual_array_flag = true; }

  void SetDirOutputRegexReplace(std::regex &regex_p,
                                std::string &replace_str_p) {
    dir_output_regex = regex_p;
    dir_output_replace_str = replace_str_p;
  }

  void SetDirInputRegexSearch(std::regex &regex_p) {
    dir_input_regex = regex_p;
  }

  AuEndpointType GetEndpointType() {
    if (!virtual_array_flag) {
      return endpoint->GetEndpointType();
    } else {
      return attribute_endpoint_vector[0]->GetEndpointType();
    }
  }

  std::vector<std::string> GetDirFile() { return endpoint->GetDirFileVector(); }

  void SetDirFile(std::vector<std::string> &file_list) {
    if (!virtual_array_flag) {
      endpoint->SetDirFileVector(file_list);
    } else {
      int n = attribute_endpoint_vector.size();
      for (int i = 0; i < n; i++)
        attribute_endpoint_vector[i]->SetDirFileVector(file_list);
    }
  }

  std::vector<int> GetDirChunkSize() { return endpoint->GetDirChunkSize(); }

  inline void SetDirChunkSize(std::vector<int> &dir_chunk_size_p) {
    if (!virtual_array_flag) {
      endpoint->SetDirChunkSize(dir_chunk_size_p);
    } else {
      int n = attribute_endpoint_vector.size();
      for (int i = 0; i < n; i++)
        attribute_endpoint_vector[i]->SetDirChunkSize(dir_chunk_size_p);
    }
  }

  inline int SetChunkSize(std::vector<int> data_chunk_size_p) {
    data_chunk_size = data_chunk_size_p;
    return 0;
  }

  inline int GetChunkSize(std::vector<int> &data_chunk_size_p) {

    if (GetEndpointType() == EP_DIR_STREAM &&
        variable_chunking_from_file_flag) {
      std::vector<std::string> para;
      endpoint->Control(DIR_STREAM_GET_NEXT_SUB_SIZE, para);
      String2Vector(para[0], data_chunk_size);
      data_overlap_size.resize(data_chunk_size.size(), 0);
    }
    data_chunk_size_p = data_chunk_size;
    return 0;
  }
  inline std::vector<int> GetChunkSize() {
    std::vector<int> data_chunk_size_p;
    GetChunkSize(data_chunk_size_p);
    return data_chunk_size_p;
  }

  inline int SetChunkSizeByMem(size_t max_mem_size) {
    set_chunk_size_by_mem_flag = true;
    set_chunk_size_by_mem_max_mem_size = max_mem_size;
    return 0;
  }

  inline int SetChunkSizeByDim(int dim_rank) {
    chunk_size_by_user_by_dimension_flag = true;
    data_auto_chunk_dim_index = dim_rank;
    return 0;
  }

  inline int GetArraySize(std::vector<unsigned long long> &size_p) {
    if (endpoint != NULL) {
      endpoint->ExtractMeta();
      data_size = endpoint->GetDimensions();
      size_p = data_size;
      return 0;
    }
    return -1;
  }

  inline int SetArraySize(const std::vector<unsigned long long> &size_p) {
    data_size = size_p;
    return 0;
  }

  inline int GetArrayRank(int &rank) {
    if (endpoint != NULL) {
      endpoint->ExtractMeta();
      data_size = endpoint->GetDimensions();
      rank = data_size.size();
      return 0;
    }
    return -1;
  }

  /*
void SetSize(std::vector<unsigned long long> size_p)
{
  data_size = size_p;
}
std::vector<unsigned long long> GetSize()
{
  if (endpoint != NULL)
  {
    endpoint->ExtractMeta();
    data_size = endpoint->GetDimensions();
    data_dims = data_size.size();
    return data_size;
  }
 }
 */

  /**
   * @brief write data to another endpoint type, e.g., HDF5 in disk
   *
   * @param data_endpoint
   * @return int
   */
  int Backup(std::string data_endpoint_p) {
    // std::string target_array_data_endpoint_info = data_endpoint_p;
    // Endpoint *target_endpoint =
    // EndpointFactory::NewEndpoint(data_endpoint_p);

    AuEndpointType target_endpoint_type;
    std::string endpoint_info;
    ExtractEndpointTypeInfo(data_endpoint_p, target_endpoint_type,
                            endpoint_info);

    int ret;
    if (endpoint_memory_flag == true && target_endpoint_type == EP_HDF5) {
      std::vector<std::string> arg_v;
      arg_v.push_back(endpoint_info);
      ret = endpoint->Control(0, arg_v);
    } else {
      AU_EXIT("Nonvolatile: only support MEMORY to HDF5 now!");
    }

    return ret;
  }
  inline int Nonvolatile(std::string data_endpoint_p) // used by the old code
  {
    return Backup(data_endpoint_p);
  }

  /**
   * @brief load HDF5 array to IN_MEMORY array
   *
   * @param data_endpoint_p
   * @return int
   */
  int Restore(std::string data_endpoint_p) {
    std::string target_array_data_endpoint_info = data_endpoint_p;
    Endpoint *target_endpoint = EndpointFactory::NewEndpoint(data_endpoint_p);
    int ret;
    if (endpoint_memory_flag == true &&
        target_endpoint->GetEndpointType() == EP_HDF5) {
      std::vector<std::string> arg_v;
      arg_v.push_back(target_endpoint->GetEndpointInfo());
      ret = endpoint->Control(1, arg_v);
    } else {
      AU_EXIT("Volatile: only support HDF5 to MEMORY now!");
    }

    return ret;
  }
  inline int Volatile(std::string data_endpoint_p) // used by the old code
  {
    Restore(data_endpoint_p);
  }

  /**
   * @brief create a local mirror (clone) of array
   *        with the inital value
   * @param intial_value
   * @return int
   */
  int Clone(T intial_value) {
    std::vector<std::string> intial_value_str;
    intial_value_str.push_back(std::to_string(intial_value));

    endpoint->Control(DASH_ENABLE_LOCAL_MIRROR_CODE, intial_value_str);
    return 0;
  }

  /**
   * @brief create a local miroor (clone of array) without intial value
   *        so, it needs to copy the whole array
   * @return int
   */
  int Clone() {
    // std::string intial_value_str = ""; //Empty string
    std::vector<std::string> op_str;

    endpoint->Control(DASH_ENABLE_LOCAL_MIRROR_CODE, op_str);
    return 0;
  }

  int Merge(int Op) {
    std::vector<std::string> op_str;
    op_str.push_back(std::to_string(Op));
    endpoint->Control(DASH_MERGE_MIRRORS_CODE, op_str);
    return 0;
  }

  /**
   * @brief Fill the array with value (only on rank 0)
   *
   * @param value
   * @return int
   */
  int Fill(T fill_value) {
    if (!ft_rank) {
      unsigned long long total_size = 1;
      std::vector<unsigned long long> start_p(data_size.size());
      std::vector<unsigned long long> end_p(data_size.size());

      for (int i = 0; i < data_size.size(); i++) {
        start_p[i] = 0;
        end_p[i] = data_size[i] - 1;
        total_size = total_size * data_size[i];
      }
      std::vector<T> default_value_v(total_size, fill_value);
      endpoint->Write(start_p, end_p,
                      static_cast<void *>(default_value_v.data()));
    }

    return 0;
  }

  inline int SetEndpoint(const string &endpoint_id) {
    array_data_endpoint_info = endpoint_id;
    endpoint = EndpointFactory::NewEndpoint(endpoint_id);
    AuEndpointDataType data_element_type = InferDataType<T>();
    endpoint->SetDataElementType(data_element_type);

    if (endpoint->GetEndpointType() == EP_MEMORY) {
      endpoint_memory_flag = true;
    }
    // endpoint_clean_vector[endpoint] = true;
    endpoint_to_clean_vector.push_back(endpoint);

    return 0;
  }
  inline int GetEndpoint(string &endpoint_id) {
    endpoint_id = array_data_endpoint_info;
    return 0;
  }

  /**
   * @brief pass command cmd to Endpoint of Array
   *
   * @param cmd_p : int cmd (specific to Endpoint)
   * @param arg_v_p : a vector of arg. It is typed as string and specific to
   * cmd_p
   */
  int ControlEndpoint(int cmd_p, std::string arg_v_p) {
    std::vector<std::string> arg_v;
    arg_v.push_back("");

    return endpoint->Control(cmd_p, arg_v);
  }

  /**
   * @brief pass command cmd to Endpoint of Array
   *
   * @param cmd_p : int cmd (specific to Endpoint)
   * @param arg_v_p : a vector of arg. It is typed as string and specific to
   * cmd_p
   */
  int ControlEndpoint(int cmd_p, std::vector<std::string> &arg_v_p) {
    return endpoint->Control(cmd_p, arg_v_p);
  }

  int ControlEndpoint(int cmd_p) {
    std::vector<std::string> arg_v_p;
    return endpoint->Control(cmd_p, arg_v_p);
  }

  // For old code
  inline int EndpointControl(int cmd_p, std::vector<std::string> &arg_v_p) {
    return ControlEndpoint(cmd_p, arg_v_p);
  }

  void inline ReportCost() {
    std::vector<double> mpi_stats_read, mpi_stats_udf, mpi_stats_write;
    MPIReduceStats(time_read, mpi_stats_read);
    MPIReduceStats(time_udf, mpi_stats_udf);
    MPIReduceStats(time_write, mpi_stats_write);

    if (ft_rank == 0) {
      std::cout << "Timing Results of All " << std::endl;
      std::cout << "Read      time (s) : max = " << mpi_stats_read[0]
                << ", min = " << mpi_stats_read[1]
                << ", ave = " << mpi_stats_read[2] / ft_size << std::endl;
      std::cout << "UDF       time (s) : max = " << mpi_stats_udf[0]
                << ", min = " << mpi_stats_udf[1]
                << ", ave = " << mpi_stats_udf[2] / ft_size << std::endl;
      std::cout << "Write     time (s) : max = " << mpi_stats_write[0]
                << ", min = " << mpi_stats_write[1]
                << ", ave = " << mpi_stats_write[2] / ft_size << std::endl;
      fflush(stdout);
    }
  }
  // Old-code
  inline void ReportTime() { ReportCost(); }

  inline int GetReadCost(vector<double> &cost_stats) {
    MPIReduceStats(time_read, cost_stats);
    cost_stats[2] = cost_stats[2] / ft_size;
    return 0;
  }

  inline int GetWriteCost(vector<double> &cost_stats) {
    MPIReduceStats(time_write, cost_stats);
    cost_stats[2] = cost_stats[2] / ft_size;
    return 0;
  }
  inline int GetComputingCost(vector<double> &cost_stats) {
    MPIReduceStats(time_udf, cost_stats);
    cost_stats[2] = cost_stats[2] / ft_size;
    return 0;
  }

  /**
   * @brief Set the Attribute object
   *
   * @tparam AType, the type of the value (can not be vector)
   * @param name
   * @param value
   * @return int
   */
  template <class PType>
  int SetTag(const std::string &name, const PType value) {
    AuEndpointDataType data_element_type = InferDataType<PType>();
    // std::cout << AU_DOUBLE << ", " << data_element_type << "\n";
    return endpoint->WriteAttribute(name, &value, data_element_type, 1);
  }

  int SetTag(const std::string &name, const std::string value) {
    return endpoint->WriteAttribute(name, &value[0], AU_STRING, value.length());
  }

  template <class PType>
  int SetTag(const std::string &name, const std::vector<PType> value) {
    AuEndpointDataType data_element_type = InferDataType<PType>();
    return endpoint->WriteAttribute(name, &value[0], data_element_type,
                                    value.size());
  }
  /**
   * @brief Get the Attribute object
   *
   * @tparam AType
   * @param name
   * @param value , return
   * @return int
   */
  template <class PType> int GetTag(const std::string &name, PType &value) {
    AuEndpointDataType data_element_type = InferDataType<PType>();
    return endpoint->ReadAttribute(name, &value, data_element_type);
  }

  int GetTag(const std::string &name, std::string &value) {
    int str_len = endpoint->GetAttributeSize(name, AU_STRING);
    if (str_len < 0) {
      return -1;
    }

    if (str_len == 0) {
      value = "";
      return 0;
    }
    value.resize(str_len);
    // std::cout << "GetTag : name = " << name << ", str_len = " << str_len <<
    // "\n";
    return endpoint->ReadAttribute(name, &value[0], AU_STRING, value.length());
  }
  template <class PType>
  int GetTag(const std::string &name, std::vector<PType> &value) {
    AuEndpointDataType data_element_type = InferDataType<PType>();
    int vec_len = endpoint->GetAttributeSize(name, data_element_type);
    value.resize(vec_len);
    return endpoint->ReadAttribute(name, &value[0], data_element_type,
                                   value.size());
  }

  int GetAllTagName(std::vector<string> &tag_name) {
    return endpoint->Control(OP_LIST_TAG, tag_name);
  }

  //
  // Below code are not used and just kept in case
  ///
  /**
   * @brief merge below to EndpointControl
   *
   */
  void DisableCollectiveIO() {

    if (!virtual_array_flag) {
      if (endpoint->GetEndpointType() == EP_HDF5)
        endpoint->DisableCollectiveIO();
    } else {
      int n = attribute_endpoint_vector.size();
      for (int i = 0; i < n; i++) {
        if (attribute_endpoint_vector[i]->GetEndpointType() == EP_HDF5)
          attribute_endpoint_vector[i]->DisableCollectiveIO();
      }
    }
  }

  void EnableCollectiveIO() {
    if (!virtual_array_flag) {
      if (endpoint->GetEndpointType() == EP_HDF5) {
        endpoint->EnableCollectiveIO();
      }
    } else {
      int n = attribute_endpoint_vector.size();
      for (int i = 0; i < n; i++) {
        if (attribute_endpoint_vector[i]->GetEndpointType() == EP_HDF5)
          attribute_endpoint_vector[i]->EnableCollectiveIO();
      }
    }
  }

  inline void DisableMPIIO() {

    if (!virtual_array_flag) {
      if (endpoint->GetEndpointType() == EP_HDF5)
        endpoint->DisableMPIIO();
    } else {
      int n = attribute_endpoint_vector.size();
      for (int i = 0; i < n; i++) {
        if (attribute_endpoint_vector[i]->GetEndpointType() == EP_HDF5)
          attribute_endpoint_vector[i]->DisableMPIIO();
      }
    }
  }

  inline void EnableMPIIO() {

    if (!virtual_array_flag) {
      if (endpoint->GetEndpointType() == EP_HDF5)
        endpoint->EnableMPIIO();
    } else {
      int n = attribute_endpoint_vector.size();
      for (int i = 0; i < n; i++) {
        if (attribute_endpoint_vector[i]->GetEndpointType() == EP_HDF5)
          attribute_endpoint_vector[i]->EnableMPIIO();
      }
    }
  }

  inline int CreateVisFile(FTVisType vis_type) {
    if (endpoint->GetEndpointType() == EP_HDF5 && vis_type == FT_VIS_XDMF) {
      std::vector<std::string> arg_v_p;

      return endpoint->Control(OP_CREATE_VIS_SCRIPT, arg_v_p);
    } else {
      AU_EXIT("FT only supports Create XDMF on HDF5 now !\n");
    }
  }

  inline int CreateVisFile() {
    // && vis_type == FT_VIS_XDMF
    if (endpoint->GetEndpointType() == EP_HDF5) {
      std::vector<std::string> arg_v_p;

      return endpoint->Control(OP_CREATE_VIS_SCRIPT, arg_v_p);
    } else {
      AU_EXIT("FT only supports Create XDMF on HDF5 now !\n");
    }
  }

  inline int UpdateOverlap() {}

  inline int SetDirectOutput() { set_direct_output_flag = true; }

  inline int GetStencilTag() {
    get_stencil_tag_flag = true;
    return 0;
  }

  inline void DisableOverlapUpper() { is_disable_overlap_upper_set = true; }

  inline void DisableOverlapLower() { is_disable_overlap_lower_set = true; }

  inline void SkipFileTail() { is_skip_tail_chunk = true; }

  inline void ExecuteUDFOnce() { is_execute_udf_once = true; }

  inline void SetChunkSchedulingMethod(const ChunkSchedulingMethod &m_p) {
    // is_set_chunk_scheduling = true;
    scheduling_method = m_p;
  }

  /**
   * @brief Get the My Chunk Start End object
   *
   * @param start inclusive
   * @param end inclusive
   */
  inline void GetMyChunkStartEnd(unsigned long long &start,
                                 unsigned long long &end) {
    if (scheduling_method == CHUNK_SCHEDULING_CR) {
      // We need to set chunk size first
      if (!is_init_called_in_rnc) {
        Stencil<T> (*UDF)(const Stencil<T>);
        InitializeApplyInput(UDF);
        is_init_called_in_rnc = true;
      }
      start = current_chunk_id;
      end = CRMyLastChunks(data_total_chunks, ft_rank, ft_size);
    } else {
      AU_EXIT("Unsupported GetMyChunkStartEnd for the scheduling methods !\n");
    }
  }
}; // end of class of array
} // end of namespace FT

namespace AU = FT;
#endif
