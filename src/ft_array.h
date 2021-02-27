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

//see au.h for its definations
//extern int ft_size;
//extern int ft_rank;
extern int ft_size;
extern int ft_rank;

#ifndef ARRAY_UDF_ARRAY_H
#define ARRAY_UDF_ARRAY_H

#include <assert.h>
#include <stdarg.h>
#include <regex>
#include "mpi.h"
#include "ft_endpoint.h"
#include "ft_stencil.h"
#include "ft_utility.h"
#include "ft_type.h"
#include "ft_endpoint_factory.h"
#include "ft_mpi.h"
#include "ft_merge.h"
#include "ft_output_vector.h"
#include "ft_vis.h"
#include "ft_array_transpose.h"

// std::vector<Endpoint *> endpoint_clean_vector;
//extern std::map<Endpoint *, bool> endpoint_clean_vector;

//#define AU_DEFAULT_CHUNK_SIZE_PER_DIM 1000

extern std::map<Endpoint *, bool> endpoint_clean_vector;

namespace FT
{
  template <class T>
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
    Endpoint *endpoint = NULL;

    //Below are info about endpooint
    int data_dims;                                     //The number of dimensioins of data in endpoint
    std::vector<unsigned long long> data_size;         //The size of each dimension (global, extracted from data of endpoint
    std::vector<int> data_chunk_size;                  //size of each chunk (user provide)
    std::vector<int> data_overlap_size;                //size of overlapping  (user provide)
    std::vector<unsigned long long> data_chunked_size; //The number of chunks per dimenstion
    int data_auto_chunk_dim_index;
    unsigned long long data_total_chunks; //The total number of chunks (global)

    std::vector<unsigned long long> current_chunk_start_offset; //Start offset on disk
    std::vector<unsigned long long> current_chunk_end_offset;   //End offset on disk
    std::vector<unsigned long long> current_chunk_size;         //Size of the chunk, euqal to end_offset - start_offset
    unsigned long long current_chunk_cells;                     //The number of cells in current chunk

    std::vector<unsigned long long> current_result_chunk_start_offset; //Start offset on disk
    std::vector<unsigned long long> current_result_chunk_end_offset;   //End offset on disk
    unsigned long long current_result_chunk_cells;                     //The number of cells in current chunk

    std::vector<unsigned long long> current_chunk_ol_start_offset; //Start offset on disk with overlapping
    std::vector<unsigned long long> current_chunk_ol_end_offset;   //End offset on disk with overlapping
    std::vector<unsigned long long> current_chunk_ol_size;         //Size of the chunk, euqal to end_offset - start_offset
    unsigned long long current_chunk_ol_cells;                     //The number of cells in current chunk

    unsigned long long current_chunk_id;              //Id of the current chunk (in memory) to apply UDF
    unsigned long long prev_chunk_id;                 //for SetChunkID of CellTarget
    std::vector<int> skip_size;                       //Size to ship after each operation
    std::vector<unsigned long long> skiped_dims_size; //Size of the data after skip
    std::vector<unsigned long long> skiped_chunks;    //# of chunks after skip
    std::vector<int> skiped_chunk_size;               //Size of each chunk after skip

    std::vector<T> current_chunk_data; //Pointer to data of current chunk

    std::vector<long long> ol_origin_offset; //Size of the chunk, euqal to end_offset - start_offset

    std::vector<unsigned long long> view_start_offset;
    std::vector<unsigned long long> view_size;
    std::vector<unsigned long long> view_orginal_data_dims_size;

    std::vector<T> mirror_values;

    OutputVectorFlatDirection output_vector_flat_direction_index;

    std::vector<size_t> output_vector_flat_shape;

    std::vector<std::string> attribute_array_data_endpoint_info;

    std::vector<Endpoint *> attribute_endpoint_vector; //vector of endpoints for a virtual array

    //For directory
    std::regex dir_input_regex, dir_output_regex;
    std::string dir_output_replace_str;

    //For chunk operations
    size_t set_chunk_size_by_mem_max_mem_size = 1073741824; //Byte, 1GB

    //Flag variable for different purposes
    bool skip_flag = false;
    bool view_flag = false;
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

    bool set_direct_output_flag = false;

    int skip_not_aligned_w_array_index;
    //The shape of output_vector when vector_type_flag = true
    std::vector<size_t> output_vector_shape;
    std::vector<size_t> previous_output_vector_shape;
    //float input_output_ratio; // the ratio = size of input / size of output, only valid along output_vector_shape
    //padding_value_p
    T padding_value;

    //help variable
    AU_WTIME_TYPE t_start, time_read = 0, time_udf = 0, time_write = 0, time_create = 0, time_sync = 0, time_nonvolatile = 0;

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
      endpoint = EndpointFactory::NewEndpoint(data_endpoint);
      AuEndpointDataType data_element_type = InferDataType<T>();
      endpoint->SetDataElementType(data_element_type);

      if (endpoint->GetEndpointType() == EP_MEMORY)
        endpoint_memory_flag = true;
      endpoint_clean_vector[endpoint] = true;
    }

    /**
   * @brief Construct a new Array object for read, as Input of Apply
   * 
   * @param data_endpoint contains file info, ("AuDataEndpointType + file name")
   * @param cs , chunk size
   * @param os , ghost size
   */
    Array(std::string data_endpoint, std::vector<int> cs)
    {
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

      //endpoint_clean_vector.push_back(endpoint);
      endpoint_clean_vector[endpoint] = true;
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
      endpoint = EndpointFactory::NewEndpoint(data_endpoint);

      AuEndpointDataType data_element_type = InferDataType<T>();
      endpoint->SetDataElementType(data_element_type);

      chunk_size_by_user_flag = true;
      if (endpoint->GetEndpointType() == EP_MEMORY)
        endpoint_memory_flag = true;

      //endpoint_clean_vector.push_back(endpoint);
      endpoint_clean_vector[endpoint] = true;
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
      endpoint = EndpointFactory::NewEndpoint(data_endpoint);
      //std::cout << data_endpoint << "\n";
      AuEndpointDataType data_element_type = InferDataType<T>();
      endpoint->SetDataElementType(data_element_type);
      chunk_size_by_user_by_dimension_flag = true;
      if (endpoint->GetEndpointType() == EP_MEMORY)
        endpoint_memory_flag = true;

      endpoint_clean_vector[endpoint] = true;
    }

    /**
   * @brief Construct a new Array object for read, as Input of Apply
   * 
   * @param data_endpoint contains file info, ("AuDataEndpointType + file name")
   * @param cs , chunk size
   * @param os , ghost size
   */
    Array(std::string data_endpoint, std::vector<unsigned long long> size_p)
    {
      array_data_endpoint_info = data_endpoint;
      endpoint = EndpointFactory::NewEndpoint(data_endpoint);
      AuEndpointDataType data_element_type = InferDataType<T>();
      endpoint->SetDataElementType(data_element_type);
      endpoint->SetDimensions(size_p);

      if (endpoint->GetEndpointType() == EP_MEMORY)
      {
        endpoint_memory_flag = true;
        endpoint->Create();
      }

      data_size = size_p;
      endpoint_clean_vector[endpoint] = true;
    }

    /**
   * @brief Construct a new Array object with only chunk size and overlap size
   *         Mostly, this is used for virtual array which has uniform chunk size and overlap size
   * @param cs chunk size 
   * @param os overlap size
   */
    Array(std::vector<int> cs, std::vector<int> os)
    {
      data_chunk_size = cs;
      data_overlap_size = os;
    }

    Array(std::vector<int> cs)
    {
      data_chunk_size = cs;
      data_overlap_size.resize(cs.size());
      std::fill(data_overlap_size.begin(), data_overlap_size.end(), 0);
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

    /**
   * @brief Construct a new Array object from in-memory vector
   *        The data are assumed to be 1D too here
   * @param data_vector_endpoint the data to intialize 
   */
    ~Array()
    {
      //endpoint_clean_vector[endpoint] = false;
      //if (endpoint != NULL)
      //  delete endpoint;
      //endpoint = NULL;
    }

    /**
   * @brief Update the chunk size, given chunk_size_by_user_flag/chunk_size_by_user_by_dimension_flag
   * 
   * @param chunk_size_p 
   * @param overlap_size_p 
   * @param data_size 
   */
    void UpdateChunkSize()
    {
      //partition by dimensions
      if (chunk_size_by_user_by_dimension_flag)
      {
        //data_chunked_size.resize(data_dims);
        data_chunk_size.resize(data_dims);
        //overlap_size_p.resize(data_dims);
        for (int i = 0; i < data_dims; i++)
        {
          if (data_auto_chunk_dim_index == i)
          {
            if (data_size[i] % ft_size == 0)
            {
              data_chunk_size[i] = data_size[i] / ft_size;
            }
            else
            {
              data_chunk_size[i] = data_size[i] / ft_size + 1;
            }
          }
          else
          {
            data_chunk_size[i] = data_size[i];
          }
          //data_overlap_size[i] = 0;
        }

        return;
      }

      if (set_chunk_size_by_mem_flag)
      {
        unsigned long long total_elements = 1;
        for (int i = 0; i < data_dims; i++)
        {
          total_elements = data_size[i] * total_elements;
        }
        total_elements = total_elements / ft_size;
        if (total_elements > (set_chunk_size_by_mem_max_mem_size / sizeof(T)))
        {
          total_elements = set_chunk_size_by_mem_max_mem_size / sizeof(T);
        }
        std::vector<unsigned long long> chunk_size_temp = RowMajorOrderReverse(total_elements, data_size);
        int replace_flag = 1;
        for (int i = data_dims - 1; i > 0; i--)
        {
          if (chunk_size_temp[i] != data_size[i])
          {
            chunk_size_temp[i] = data_size[i];
            if (chunk_size_temp[i - 1] != 0)
            {
              chunk_size_temp[i - 1] = chunk_size_temp[i - 1] - 1;
            }
            else
            {
              replace_flag = 0;
              break;
            }
          }
        }

        for (int i = data_dims - 1; i >= 0; i--)
        {
          if (chunk_size_temp[i] == 0)
            replace_flag = 0;
        }

        if (replace_flag)
        {
          //unsigned long long to int here
          for (int i = 0; i < data_dims; i++)
            data_chunk_size[i] = chunk_size_temp[i];
        }
        else
        {
          AU_EXIT("SetChunkSizeByMem failed ! Please try specify the chunk size via SetChunkSize !");
        }
        return;
      }

      if (endpoint != NULL && endpoint->GetEndpointType() == EP_DIR)
      {
        //chunk_size_p = endpoint->GetChunkSize();
      }
      //optimal chunk_size
    }

    inline int SetOverlapSize(const vector<int> os_p)
    {
      data_overlap_size = os_p;
      return 0;
    }
    inline int SetOverlapSizeByDetection()
    {
      set_overlap_size_by_auto_detection_flag = true;
      return 0;
    }
    inline int GetOverlapSize(vector<int> &os_p)
    {
      os_p = data_overlap_size;
      return 0;
    }
    inline int SetOverlapPadding(const T &padding_value_p)
    {
      has_padding_value_flag = true;
      padding_value = padding_value_p;
      return 0;
    }

    inline int SyncOverlap()
    {
      if (endpoint != NULL)
      {
        if (endpoint->GetEndpointType() == EP_MEMORY)
        {
          std::vector<std::string> arg_v;
          return endpoint->Control(MEMORY_SYNC_OVERLAP, arg_v);
        }
        return 0;
      }

      return 0;
    }

    template <class UDFOutputType>
    int UpdateOverlapSize(Stencil<UDFOutputType> (*UDF)(const Stencil<T> &))
    {
      if (endpoint != NULL)
      {
        if (endpoint->GetEndpointType() == EP_DIR || chunk_size_by_user_by_dimension_flag)
        {
          data_overlap_size.resize(data_dims);
          for (int i = 0; i < data_dims; i++)
          {
            data_overlap_size[i] = 0;
          }
        }
        return 0;
      }

      if (set_overlap_size_by_auto_detection_flag)
      {
        std::vector<T> trail_data;
        trail_data.resize(1);
        Stencil<T> trail_cell(data_dims, &trail_data[0]);
        UDF(trail_cell);
        trail_cell.GetTrailRunResult(data_overlap_size);
      }
      //optimal overlap size
      return 0;
    }

    template <class UDFOutputType>
    void InitializeApplyInput(Stencil<UDFOutputType> (*UDF)(const Stencil<T> &))
    {
      //Read the metadata (rank, dimension size) from endpoint
      if (virtual_array_flag && attribute_endpoint_vector.size() >= 1)
      {
        //Get the data_size from first attribute
        attribute_endpoint_vector[0]->ExtractMeta();
        data_size = attribute_endpoint_vector[0]->GetDimensions();
        data_dims = data_size.size();
        for (int i = 1; i < attribute_endpoint_vector.size(); i++)
        {
          attribute_endpoint_vector[i]->ExtractMeta();
          std::vector<unsigned long long> data_size_p = attribute_endpoint_vector[i]->GetDimensions();

          if (data_size != data_size_p)
          {
            AU_EXIT("All attributes must have same size");
          }
        }
      }
      else
      {
        endpoint->ExtractMeta();
        data_size = endpoint->GetDimensions();
        data_dims = data_size.size();
      }

      UpdateChunkSize();
      UpdateOverlapSize(UDF);

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

      for (int i = 0; i < data_dims; i++)
      {
        if (data_size[i] % data_chunk_size[i] == 0)
        {
          data_chunked_size[i] = data_size[i] / data_chunk_size[i];
        }
        else
        {
          data_chunked_size[i] = data_size[i] / data_chunk_size[i] + 1;
        }
        data_total_chunks = data_total_chunks * data_chunked_size[i];
      }

      if (skip_flag)
      {
        skiped_dims_size.resize(data_dims);
        skiped_chunks.resize(data_dims);
        skiped_chunk_size.resize(data_dims);

        for (int i = 0; i < data_dims; i++)
        {

          /*if (data_size[i] % skip_size[i] != 0 || data_chunk_size[i] % skip_size[i] != 0)
        {
          AU_EXIT("Strip size must be aligned with size of both array and chunk ! \n");
        }*/

          if (data_chunk_size[i] % skip_size[i] != 0)
          {
            AU_EXIT("Strip size must be aligned with the size of chunk ! \n");
          }

          if (data_size[i] % skip_size[i] != 0)
          {
            skip_not_aligned_w_array_flag = true;
            skip_not_aligned_w_array_index = i;
            skiped_dims_size[i] = data_size[i] / skip_size[i] + 1;
          }
          else
          {
            skiped_dims_size[i] = data_size[i] / skip_size[i];
          }

          skiped_chunk_size[i] = data_chunk_size[i] / skip_size[i];
          if (skiped_dims_size[i] % skiped_chunk_size[i] != 0)
          {
            skiped_chunks[i] = skiped_dims_size[i] / skiped_chunk_size[i] + 1;
          }
          else
          {
            skiped_chunks[i] = skiped_dims_size[i] / skiped_chunk_size[i];
          }
        }
      }

      current_chunk_id = ft_rank; //Each process deal with one chunk one time, starting from its rank

      //#ifdef DEBUG
      if (ft_rank == 0)
      {
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
    void PrintEndpointInfo()
    {
      if (!virtual_array_flag)
      {
        endpoint->PrintInfo();
      }
      else
      {
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
    void Apply(Stencil<UDFOutputType> (*UDF)(const Stencil<T> &), Array<BType> *B = nullptr)
    {
      Transform(UDF, B);
    }

    template <class UDFOutputType, class BType = UDFOutputType>
    void Transform(Stencil<UDFOutputType> (*UDF)(const Stencil<T> &), Array<BType> &B)
    {
      Transform(UDF, &B);
    }

    template <class UDFOutputType, class BType = UDFOutputType>
    void Transform(Stencil<UDFOutputType> (*UDF)(const Stencil<T> &))
    {
      Transform(UDF, nullptr);
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
    void Transform(Stencil<UDFOutputType> (*UDF)(const Stencil<T> &), Array<BType> *B = nullptr)
    {
      //Set up the input data for LoadNextChunk
      InitializeApplyInput(UDF);

      //reset timer to zero
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

      while (load_ret == 1)
      {
        unsigned long long cell_target_g_location_rm;
        result_vector_index = 0;

        //unsigned long long lrm;
        t_start = AU_WTIME;

#if defined(_OPENMP)
        size_t *prefix;
#endif

#if defined(_OPENMP)
#pragma omp parallel
#endif
        {
          std::vector<unsigned long long> cell_coordinate(data_dims, 0), cell_coordinate_ol(data_dims, 0), global_cell_coordinate(data_dims, 0);
          unsigned long long offset_ol;
          Stencil<T> cell_target(0, &current_chunk_data[0], cell_coordinate_ol, current_chunk_ol_size);
          if (has_padding_value_flag)
          {
            cell_target.SetPadding(padding_value);
          }
          cell_target.SetChunkID(prev_chunk_id);
          Stencil<UDFOutputType> cell_return_stencil;
          UDFOutputType cell_return_value;
          unsigned long long cell_target_g_location_rm;
          int is_mirror_value = 0;
          std::vector<unsigned long long> skip_chunk_coordinate(data_dims, 0), skip_chunk_coordinate_start(data_dims, 0);
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
          for (unsigned long long i = 0; i < current_chunk_cells; i++)
          {
            ROW_MAJOR_ORDER_REVERSE_MACRO(i, current_chunk_size, current_chunk_size.size(), cell_coordinate)

            if (skip_flag == 1)
            {
              skip_flag_on_cell = 0;
              for (int id = 0; id < data_dims; id++)
              {
                //The coordinate of the skip chunk this coordinate belong to
                skip_chunk_coordinate[id] = std::floor(cell_coordinate[id] / skip_size[id]);
                skip_chunk_coordinate_start[id] = skip_chunk_coordinate[id] * skip_size[id]; //first cell's coodinate of this skip chunk
                if (skip_chunk_coordinate_start[id] != cell_coordinate[id])
                { //we only run on the first  element of this skip chunk
                  skip_flag_on_cell = 1;
                  break;
                }
              }

              if (skip_flag_on_cell == 1)
                continue; //          for (unsigned long long i = 0; i < current_chunk_cells; i++)
              assert(i < current_chunk_cells);
            }

            //Get the coodinate with overlapping
            //Also, get the global coodinate of the cell in original array
            for (int ii = 0; ii < data_dims; ii++)
            {
              if (cell_coordinate[ii] + ol_origin_offset[ii] < current_chunk_ol_size[ii])
              {
                cell_coordinate_ol[ii] = cell_coordinate[ii] + ol_origin_offset[ii];
              }
              else
              {
                cell_coordinate_ol[ii] = current_chunk_ol_size[ii] - 1;
              }
              //get the global coordinate
              global_cell_coordinate[ii] = current_chunk_start_offset[ii] + cell_coordinate[ii];
            }

            //Update the offset with overlapping
            ROW_MAJOR_ORDER_MACRO(current_chunk_ol_size, current_chunk_ol_size.size(), cell_coordinate_ol, offset_ol);
            if (endpoint != NULL && endpoint->GetEndpointType() != EP_DIR)
            {
              ROW_MAJOR_ORDER_MACRO(data_size, data_size.size(), global_cell_coordinate, cell_target_g_location_rm)
              cell_target.SetLocation(offset_ol, cell_coordinate_ol, cell_coordinate, current_chunk_size, ol_origin_offset, current_chunk_ol_size, global_cell_coordinate, cell_target_g_location_rm);
            }
            else
            {
              cell_target.SetLocation(offset_ol, cell_coordinate_ol, cell_coordinate, current_chunk_size, ol_origin_offset, current_chunk_ol_size, cell_coordinate_ol, offset_ol);
            }
            //Set the global coodinate of the cell as the ID of the cell
            //Disable it for performance.
            //RowMajorOrder(data_dims_size, global_cell_coordinate)
            //ROW_MAJOR_ORDER_MACRO(data_dims_size, data_dims_size.size(), global_cell_coordinate, cell_target_g_location_rm)
            //cell_target.set_my_g_location_rm(cell_target_g_location_rm);

            cell_return_stencil = UDF(cell_target); // Called by C++
            cell_return_value = cell_return_stencil.get_value();

            if (vector_type_flag == true)
            {
              cell_return_stencil.GetShape(output_vector_shape);
            }

            if (save_result_flag)
            {
              if (skip_flag)
              {
#if defined(_OPENMP)
                vec_private.push_back(cell_return_value);
#else
                current_result_chunk_data[result_vector_index] = cell_return_value;
                result_vector_index = result_vector_index + 1;
#endif
                //When skip_flag is set, there is no need to have apply_replace_flag
                //Todo: in future
                //if(apply_replace_flag == 1){
                //  current_chunk_data[i] = cell_return_value; //Replace the orginal data
                //}
              }
              else
              {
                current_result_chunk_data[i] = cell_return_value; //cell_return =  cell_return.
                if (apply_replace_flag == 1)
                {
                  std::memcpy(&current_chunk_data[offset_ol], &cell_return_value, sizeof(T));
                }
              }
            }
          } //end for loop, finish the processing on a single chunk in row-major direction
#if defined(_OPENMP)
          prefix[ithread + 1] = vec_private.size();
#pragma omp barrier
#pragma omp single
          {
            for (int i = 1; i < (nthreads + 1); i++)
            {
              prefix[i] += prefix[i - 1];
            }
            if (current_result_chunk_data.size() != prefix[nthreads])
            {
              std::cout << "Wrong output size ! prefix[nthreads] =" << prefix[nthreads] << ", current.size() = " << current_result_chunk_data.size() << " \n ";
            }
          } //end of omp for
          std::copy(vec_private.begin(), vec_private.end(), current_result_chunk_data.begin() + prefix[ithread]);
          clear_vector(vec_private);
#endif
        } //end of omp parallel

#if defined(_OPENMP)
        delete[] prefix;
#endif
        time_udf = AU_WTIME - t_start + time_udf;

        //////////////////////////////////////
        //end of processing on a single chunk
        /////////////////////////////////////

        t_start = AU_WTIME;

        if (B != nullptr)
        {

          std::vector<unsigned long long> B_data_size;
          std::vector<int> B_data_chunk_size, B_data_overlap_size;

          if (B->GetEndpointType() == EP_DIR && GetEndpointType() == EP_DIR)
          {
            std::vector<std::string> dir_file_list = GetDirFile();
            B->SetDirFile(dir_file_list);
            std::vector<int> dir_chunk_size = GetDirChunkSize();
            B->SetDirChunkSize(dir_chunk_size);
          }

          if (vector_type_flag)
          {
            //output_vector_shape
            size_t vector_size;
            std::vector<unsigned long long> current_chunk_start_offset_v = current_result_chunk_start_offset, current_chunk_end_offset_v = current_result_chunk_end_offset;
            void *data_point;
            // data_point = FlatVector(current_result_chunk_data, output_vector_flat_direction_index, current_chunk_start_offset_v, current_chunk_end_offset_v, vector_size);
            // InferOutputSize(B_data_size, B_data_chunk_size, B_data_overlap_size, vector_size);

            if (is_the_last_chunk && (previous_output_vector_shape.size() == 0))
            {
              //int normal_chunk_output_size = data_chunk_size[i] / (current_chunk_ol_size[i] / output_vector_shape[i]);
              previous_output_vector_shape = output_vector_shape;
              for (int i = 0; i < output_vector_shape.size(); i++)
              {
                if (skip_not_aligned_w_array_flag && skip_not_aligned_w_array_index == i)
                {
                  previous_output_vector_shape[i] = data_chunk_size[i] / (current_chunk_ol_size[i] / output_vector_shape[i]);
                }
              }
            }
            //PrintVector("Debug:  output_vector_shape = ", output_vector_shape);
            //PrintVector("Debug:  current_chunk_start_offset_v = ", current_chunk_start_offset_v);
            //PrintVector("Debug:  current_chunk_end_offset_v = ", current_chunk_end_offset_v);
            //PrintVector("Debug:  previous_output_vector_shape = ", previous_output_vector_shape);

            data_point = InsertOutputVV2WriteV(current_result_chunk_data, output_vector_shape, current_chunk_start_offset_v, current_chunk_end_offset_v, is_the_last_chunk, previous_output_vector_shape);
            CalculateOutputSize(B_data_size, B_data_chunk_size, B_data_overlap_size);
            //PrintVector("Debug: create B_data_size = ", B_data_size);

            B->CreateEndpoint(B_data_size, B_data_chunk_size, B_data_overlap_size);
            //PrintVector("Debug: write current_chunk_start_offset_v = ", current_chunk_start_offset_v);
            //PrintVector("Debug: write current_chunk_end_offset_v = ", current_chunk_end_offset_v);
            B->WriteEndpoint(current_chunk_start_offset_v, current_chunk_end_offset_v, data_point);
            free(data_point);
          }
          else
          {
            InferOutputSize(B_data_size, B_data_chunk_size, B_data_overlap_size, 0);
            B->CreateEndpoint(B_data_size, B_data_chunk_size, B_data_overlap_size);
            //B->WriteEndpoint(current_chunk_start_offset, current_chunk_end_offset, &current_result_chunk_data[0]);
            if (!skip_flag)
            {
              //PrintVector("current_chunk_start_offset = ", current_chunk_start_offset);
              //PrintVector("current_chunk_end_offset = ", current_chunk_end_offset);
              B->WriteArray(current_result_chunk_start_offset, current_chunk_end_offset, current_result_chunk_data);
            }
            else
            {
              //PrintVector("current_chunk_start_offset = ", current_result_chunk_start_offset);
              //PrintVector("current_chunk_end_offset = ", current_result_chunk_end_offset);
              B->WriteArray(current_result_chunk_start_offset, current_result_chunk_end_offset, current_result_chunk_data);
            }
          }
        }
        time_write = time_write + AU_WTIME - t_start;

        if (vector_type_flag == true)
        {
          previous_output_vector_shape = output_vector_shape;
        }

        t_start = AU_WTIME;
        load_ret = LoadNextChunk(current_result_chunk_data_size);
        current_result_chunk_data.resize(current_result_chunk_data_size);
        time_read = time_read + AU_WTIME - t_start;

      } // end of while:: no more chunks to process

      //May start a empty write for collective I/O
      if ((data_total_chunks % ft_size != 0) && (current_chunk_id >= data_total_chunks) && (current_chunk_id < (data_total_chunks + ft_size - (data_total_chunks % ft_size))) && B != nullptr)
      {
        //std::cout << "current_chunk_id = " << current_chunk_id << std::endl;
        //std::cout << "leftover_chunks  = " << data_total_chunks % ft_size << std::endl;
        //std::cout << "data_total_chunks  = " << data_total_chunks << std::endl;
        std::vector<unsigned long long> null_start; //Start offset on disk
        std::vector<unsigned long long> null_end;   //End offset on disk
        void *data_point = nullptr;
        null_start.resize(data_dims, 0);
        null_end.resize(data_dims, 0);
        B->WriteEndpoint(null_start, null_end, data_point);
      }

      return;
    }

    int WriteArray(const std::vector<unsigned long long> &start_p, const std::vector<unsigned long long> &end_p, std::vector<T> &data_p)
    {
      //InitializeApplyInput();
      if (!virtual_array_flag)
      {
        return endpoint->Write(start_p, end_p, static_cast<void *>(data_p.data()));
      }
      else
      {

        int n = attribute_endpoint_vector.size();
        for (int i = 0; i < n; i++)
        {
          //std::cout << "write " << i << " \n" ;
          void *current_chunk_data_void_p = ExtractAttributeFromVirtualArrayVector(data_p, i, attribute_endpoint_vector[i]->GetDataElementType(), attribute_endpoint_vector[i]->GetDataElementTypeSize());
          attribute_endpoint_vector[i]->Write(start_p, end_p, current_chunk_data_void_p);
          free(current_chunk_data_void_p);
        }
        return 1;
      }
    }

    int WriteArray(std::vector<unsigned long long> &start_p, std::vector<unsigned long long> &end_p, std::vector<std::vector<T>> data_p)
    {
      AU_EXIT("You should not be here !");
      return 0;
    }

    int WriteEndpoint(std::vector<unsigned long long> &start_p, std::vector<unsigned long long> &end_p, void *data)
    {
      if (!virtual_array_flag)
        return endpoint->Write(start_p, end_p, data);

      return 0;
    }

    int ReadEndpoint(std::vector<unsigned long long> &start_p, std::vector<unsigned long long> &end_p, void *data)
    {
      return endpoint->Read(start_p, end_p, data);
    }

    /**
 * @brief Calculate the Size of Output array (B)
 * 
 * @param data_size_p, the size of the output array 
 * @param data_chunk_size_p , the chunk size of the output array
 * @param data_overlap_size_p , the overlap size of the output array
 */
    void inline CalculateOutputSize(std::vector<unsigned long long> &data_size_p, std::vector<int> &data_chunk_size_p, std::vector<int> &data_overlap_size_p)
    {
      if (skip_flag)
      {
        data_size_p = skiped_dims_size;
        data_chunk_size_p = skiped_chunk_size;
        data_overlap_size_p = data_overlap_size; //Todo:  need to consider data_overlap size
      }
      else
      {
        data_size_p = data_size;
        data_chunk_size_p = data_chunk_size;
        data_overlap_size_p = data_overlap_size;
      }

      //we may update the data_size_p/data_chunk_size_p/data_overlap_size_p if vector used
      //output_vector_shape is the parameter
      if (vector_type_flag)
      {
        int rank = data_size_p.size();
        for (int i = 0; i < output_vector_shape.size(); i++)
        {
          if (i >= rank)
          {
            data_size_p.push_back(output_vector_shape[i]);
            data_chunk_size_p.push_back(output_vector_shape[i]);
            data_overlap_size_p.push_back(0);
          }
          else
          {
            if (skip_not_aligned_w_array_flag && skip_not_aligned_w_array_index == i)
            {
              if (!is_the_last_chunk)
              {
                data_size_p[i] = (data_size_p[i] - 1) * output_vector_shape[i];
                data_size_p[i] = data_size_p[i] + ((data_size[i] % skip_size[i]) * output_vector_shape[i]) / data_chunk_size[i];
              }
              else
              {
                //we need to update the output_vector_shape,  data_chunk_size, current_chunk_ol_size
                //PrintVector("data_chunk_size");
                int normal_chunk_output_size = data_chunk_size[i] / (current_chunk_ol_size[i] / output_vector_shape[i]);
                PrintScalar("data_chunk_size[i] =", data_chunk_size[i]);
                PrintScalar("current_chunk_ol_size[i] =", current_chunk_ol_size[i]);
                PrintScalar("output_vector_shape[i] =", output_vector_shape[i]);
                PrintScalar("normal_chunk_output_size =", normal_chunk_output_size);
                PrintScalar("skip_size[i] = ", skip_size[i]);
                PrintScalar("data_size[i] = ", data_size[i]);
                PrintScalar("data_size[i] % skip_size[i] = ", data_size[i] % skip_size[i]);

                data_size_p[i] = (data_size_p[i] - 1) * normal_chunk_output_size;
                data_size_p[i] = data_size_p[i] + ((data_size[i] % skip_size[i]) * normal_chunk_output_size) / data_chunk_size[i];
              }
            }
            else
            {
              data_size_p[i] = data_size_p[i] * output_vector_shape[i];
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
    void inline InferOutputSize(std::vector<unsigned long long> &data_size_p, std::vector<int> &data_chunk_size_p, std::vector<int> &data_overlap_size_p, size_t output_vector_size)
    {
      if (skip_flag)
      {
        if (!vector_type_flag)
        {
          data_size_p = skiped_dims_size;
          data_chunk_size_p = skiped_chunk_size;
          data_overlap_size_p = data_overlap_size; //Todo:  need to consider data_overlap size
        }
        else //skip_flag == true and vector_type_flag == true
        {
          int data_dims_t;
          std::vector<unsigned long long> skiped_dims_size_t;
          std::vector<int> skiped_chunk_size_t;
          std::vector<int> data_overlap_size_t;

          if (output_vector_flat_direction_index > (data_dims - 1))
          { //We save the output vector as new dimensions
            data_dims_t = data_dims + 1;
            skiped_dims_size_t.resize(data_dims_t);
            skiped_chunk_size_t.resize(data_dims_t);
            data_overlap_size_t.resize(data_dims_t);
            for (int k = 0; k < data_dims; k++)
            {
              skiped_dims_size_t[k] = skiped_dims_size[k];
              data_overlap_size_t[k] = data_overlap_size[k];
              skiped_chunk_size_t[k] = skiped_chunk_size[k];
            }
            skiped_dims_size_t[data_dims] = output_vector_size;
            data_overlap_size_t[data_dims] = 0;
            skiped_chunk_size_t[data_dims] = output_vector_size;
          }
          else
          { //We save the output vector within existing dimensions
            data_dims_t = data_dims;
            skiped_dims_size_t.resize(data_dims_t);
            skiped_chunk_size_t.resize(data_dims_t);
            data_overlap_size_t.resize(data_dims_t);
            for (int k = 0; k < data_dims; k++)
            {
              skiped_dims_size_t[k] = skiped_dims_size[k];
              if (k == output_vector_flat_direction_index)
                skiped_dims_size_t[k] = skiped_dims_size_t[k] * output_vector_size;
              data_overlap_size_t[k] = data_overlap_size[k];
              skiped_chunk_size_t[k] = skiped_chunk_size[k];
            }
          }

          data_size_p = skiped_dims_size_t;
          data_chunk_size_p = skiped_chunk_size_t;
          data_overlap_size_p = data_overlap_size_t;
        }
      }
      else
      { //skip_flag == false
        if (vector_type_flag == 0)
        {
          data_size_p = data_size;
          data_chunk_size_p = data_chunk_size;
          data_overlap_size_p = data_overlap_size;
        }
        else //skip_flag == false and vector_type_flag == true
        {
          int data_dims_t;
          std::vector<unsigned long long> dims_size_t;
          std::vector<int> chunk_size_t;
          std::vector<int> data_overlap_size_t;

          if (output_vector_flat_direction_index > (data_dims - 1))
          {
            data_dims_t = data_dims + 1;
            dims_size_t.resize(data_dims_t);
            chunk_size_t.resize(data_dims_t);
            data_overlap_size_t.resize(data_dims_t);
            for (int k = 0; k < data_dims; k++)
            {
              dims_size_t[k] = data_size[k];
              chunk_size_t[k] = data_chunk_size[k];
              data_overlap_size_t[k] = data_overlap_size[k];
            }
            dims_size_t[data_dims] = output_vector_size;
            data_overlap_size_t[data_dims] = 0;
            chunk_size_t[data_dims] = output_vector_size;
          }
          else
          {
            data_dims_t = data_dims;
            dims_size_t.resize(data_dims_t);
            chunk_size_t.resize(data_dims_t);
            data_overlap_size_t.resize(data_dims_t);
            for (int k = 0; k < data_dims; k++)
            {
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

    int inline CreateEndpoint(std::vector<unsigned long long> data_size_p, std::vector<int> data_chunk_size_p, std::vector<int> data_overlap_size_p)
    {
      if (!virtual_array_flag && endpoint->GetOpenFlag())
        return 0;

      if (is_endpoint_created_flag)
        return 0;

      //cout << "CreateEndpoint: is_endpoint_created_flag " << is_endpoint_created_flag << "\n";
      is_endpoint_created_flag = true;

      data_size = data_size_p;
      data_chunk_size = data_chunk_size_p;
      data_overlap_size = data_overlap_size_p;

      if (virtual_array_flag)
      {
        for (int i = 0; i < attribute_endpoint_vector.size(); i++)
        {
          if (attribute_endpoint_vector[i]->GetOpenFlag())
            return 0;
          attribute_endpoint_vector[i]->SetDimensions(data_size);
          attribute_endpoint_vector[i]->Create();
          attribute_endpoint_vector[i]->Close(); //call it to make sure it is consistency
        }
        return 0;
      }
      else
      {
        AuEndpointDataType type_p = InferDataType<T>();
        endpoint->SetDimensions(data_size);
        endpoint->SetDataElementType(type_p);
        return endpoint->Create();
      }
    }

    int ReadArray(const std::vector<unsigned long long> &start, const std::vector<unsigned long long> &end, std::vector<T> &data_vector)
    {
      //InitializeApplyInput();
      unsigned long long data_vector_size;
      COUNT_CELLS(start, end, data_vector_size);

      //std::vector<T> data_vector;
      data_vector.resize(data_vector_size);
      if (!virtual_array_flag)
      {
        endpoint->Read(start, end, &data_vector[0]);
      }
      else
      {
        int n = attribute_endpoint_vector.size();
        std::vector<AuEndpointDataTypeUnion> current_chunk_data_union_vector;
        for (int i = 0; i < n; i++)
        {
          int element_type_size = attribute_endpoint_vector[i]->GetDataElementTypeSize();
          void *current_chunk_data_temp = (void *)malloc(data_vector_size * element_type_size);
          if (!current_chunk_data_temp)
          {
            AU_EXIT("Not enough memory");
          }
          attribute_endpoint_vector[i]->Read(start, end, current_chunk_data_temp);
          current_chunk_data_union_vector = attribute_endpoint_vector[i]->Void2Union(current_chunk_data_temp, data_vector_size);
          InsertAttribute2VirtualArrayVector(current_chunk_data_union_vector, attribute_endpoint_vector[i]->GetDataElementType(), data_vector, i);
          free(current_chunk_data_temp);
        }
      }
      return 0;
    }
    inline std::vector<T> ReadArray(const std::vector<unsigned long long> start, const std::vector<unsigned long long> end) //used by the old code
    {
      std::vector<T> data_vector;
      ReadArray(start, end, data_vector);
      return data_vector;
    }

    bool HasNextChunk()
    {
      if (current_chunk_id >= data_total_chunks || current_chunk_id < 0)
      {
        return false;
      }

      return true;
    }

    /**
   * @brief update current_chunk_id
   * 
   * @return int 
   */
    void SchduleChunkNext()
    {
      prev_chunk_id = current_chunk_id;
      //Next chunk id
      if (!reverse_apply_direction_flag)
      {
        current_chunk_id = current_chunk_id + ft_size;
      }
      else
      {
        current_chunk_id = current_chunk_id - ft_size;
      }
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
    int LoadNextChunk(unsigned long long &result_vector_size)
    {
      if (!HasNextChunk())
      {
        return 0;
      }

      //We only handle last chunk when skip is not even with the array size
      if (current_chunk_id == (data_total_chunks - 1) && skip_not_aligned_w_array_flag)
        is_the_last_chunk = true;

      result_vector_size = 0;

      current_chunk_cells = 1;
      current_result_chunk_cells = 1;
      current_chunk_ol_cells = 1;

      std::vector<unsigned long long> chunk_coordinate = RowMajorOrderReverse(current_chunk_id, data_chunked_size);
      std::vector<unsigned long long> skiped_chunk_coordinate;
      if (skip_flag)
        skiped_chunk_coordinate = RowMajorOrderReverse(current_chunk_id, skiped_chunks);

      //calculate the start and end of a chunk
      for (int i = 0; i < data_dims; i++)
      {
        if (data_chunk_size[i] * chunk_coordinate[i] < data_size[i])
        {
          current_chunk_start_offset[i] = data_chunk_size[i] * chunk_coordinate[i];
        }
        else
        {
          current_chunk_start_offset[i] = data_size[i];
        }

        if (current_chunk_start_offset[i] + data_chunk_size[i] - 1 < data_size[i])
        {
          current_chunk_end_offset[i] = current_chunk_start_offset[i] + data_chunk_size[i] - 1;
        }
        else
        {
          current_chunk_end_offset[i] = data_size[i] - 1;
        }

        assert((current_chunk_end_offset[i] - current_chunk_start_offset[i] + 1 >= 0));
        current_chunk_size[i] = current_chunk_end_offset[i] - current_chunk_start_offset[i] + 1;
        current_chunk_cells = current_chunk_cells * current_chunk_size[i];

        //Deal with the result chunks size
        if (!skip_flag)
        {
          current_result_chunk_start_offset[i] = current_chunk_start_offset[i];
          current_result_chunk_end_offset[i] = current_chunk_end_offset[i];
          current_result_chunk_cells = current_chunk_cells;
        }
        else
        {
          if (skiped_chunk_coordinate[i] * skiped_chunk_size[i] < skiped_dims_size[i])
          {
            current_result_chunk_start_offset[i] = skiped_chunk_coordinate[i] * skiped_chunk_size[i];
          }
          else
          {
            current_result_chunk_start_offset[i] = skiped_dims_size[i];
          }

          if (current_result_chunk_start_offset[i] + skiped_chunk_size[i] - 1 < skiped_dims_size[i])
          {
            current_result_chunk_end_offset[i] = current_result_chunk_start_offset[i] + skiped_chunk_size[i] - 1;
          }
          else
          {
            current_result_chunk_end_offset[i] = skiped_dims_size[i] - 1;
          }
          assert((current_result_chunk_end_offset[i] - current_result_chunk_start_offset[i] + 1 >= 0));
          current_result_chunk_cells = current_result_chunk_cells * (current_result_chunk_end_offset[i] - current_result_chunk_start_offset[i] + 1);
        }

        //Deal with overlapping
        //Starting coordinate for the data chunk with overlapping
        if (current_chunk_start_offset[i] <= data_overlap_size[i])
        {
          current_chunk_ol_start_offset[i] = 0;
        }
        else
        {
          current_chunk_ol_start_offset[i] = current_chunk_start_offset[i] - data_overlap_size[i];
        }
        //Original coordinate offset for each, used to get gloabl coordinate in Apply
        ol_origin_offset[i] = current_chunk_start_offset[i] - current_chunk_ol_start_offset[i];

        //Ending oordinate for the data chunk with overlapping
        if (current_chunk_end_offset[i] + data_overlap_size[i] < data_size[i])
        {
          current_chunk_ol_end_offset[i] = current_chunk_end_offset[i] + data_overlap_size[i];
        }
        else
        {
          current_chunk_ol_end_offset[i] = data_size[i] - 1;
        }
        assert((current_chunk_ol_end_offset[i] - current_chunk_ol_start_offset[i] + 1 >= 0));
        current_chunk_ol_size[i] = current_chunk_ol_end_offset[i] - current_chunk_ol_start_offset[i] + 1;
        current_chunk_ol_cells = current_chunk_ol_cells * current_chunk_ol_size[i];
      }

      current_chunk_data.resize(current_chunk_ol_cells);
      if (save_result_flag == 1)
      {
        if (!skip_flag)
        {
          result_vector_size = current_chunk_cells;
        }
        else
        {
          result_vector_size = current_result_chunk_cells;
        }
      }

      if (view_flag == 1)
      {
        for (int i = 0; i < data_dims; i++)
        {
          current_chunk_ol_start_offset[i] = current_chunk_ol_start_offset[i] + view_start_offset[i];
          current_chunk_ol_end_offset[i] = current_chunk_ol_end_offset[i] + view_start_offset[i];
        }
      }

      //update current_chunk_id
      SchduleChunkNext();

      //Return  1, data read into   local_chunk_data
      //Return  0, end of file (no data left to handle)
      //Return -1: error happen
      //Read data between local_chunk_start_offset and local_chunk_end_offset
      //current_chunk_data.resize(current_chunk_cells);
      //return data_on_disk->ReadData(current_chunk_start_offset, current_chunk_end_offset, current_chunk_data);
      if (!virtual_array_flag)
      {
        endpoint->Read(current_chunk_ol_start_offset, current_chunk_ol_end_offset, &current_chunk_data[0]);
        return 1;
      }
      else
      {
        int n = attribute_endpoint_vector.size();
        std::vector<AuEndpointDataTypeUnion> current_chunk_data_union_vector;
        for (int i = 0; i < n; i++)
        {
          int element_type_size = attribute_endpoint_vector[i]->GetDataElementTypeSize();
          void *current_chunk_data_temp = (void *)malloc(current_chunk_ol_cells * element_type_size);
          if (!current_chunk_data_temp)
          {
            AU_EXIT("Not enough memory");
          }
          attribute_endpoint_vector[i]->Read(current_chunk_ol_start_offset, current_chunk_ol_end_offset, current_chunk_data_temp);
          current_chunk_data_union_vector = attribute_endpoint_vector[i]->Void2Union(current_chunk_data_temp, current_chunk_ol_cells);
          InsertAttribute2VirtualArrayVector(current_chunk_data_union_vector, attribute_endpoint_vector[i]->GetDataElementType(), current_chunk_data, i);
          free(current_chunk_data_temp);
        }
        return 1;
      }
    }

    inline int EnableApplyStride(const std::vector<int> &skip_size_p)
    {
      return SetStride(skip_size_p);
    }

    inline int SetStride(const std::vector<int> &skip_size_p)
    {
      skip_size = skip_size_p;
      skip_flag = true;
      return 0;
    }

    void SetVectorDirection(OutputVectorFlatDirection flat_direction_index)
    {
      output_vector_flat_direction_index = flat_direction_index;
    }

    /**
   * @brief A geneic verion of function to control how to deal with output vector during the run of Apply on Array. The output of Apply in this way is vector of vector (2D). We need to convert it into vector (1D). Also we need to control the shape of the 1D vector to write into output Array.
   * 
   * @param flat_direction,  the direction to flat the 2D vector to 1D vector
   * @param flat_shape, the shape of the data after the flat from 2D to the 2D
   */
    void ControlOutputVector(OutputVectorFlatDirection flat_direction, std::vector<size_t> flat_shape)
    {
      output_vector_flat_direction_index = flat_direction;
      output_vector_flat_shape = flat_shape;
    }

    template <typename... Is>
    inline T operator()(Is... indexs) const
    {
      std::vector<int> iv{{indexs...}};
      std::vector<unsigned long long> start;
      std::vector<unsigned long long> end;
      std::vector<T> data_v;

      start.resize(iv.size());
      end.resize(iv.size());
      data_v.resize(iv.size());

      for (int i = 0; i < iv.size(); i++)
      {
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
    template <typename... Is>
    inline T GetValue(Is... indexs)
    {
      std::vector<int> iv{{indexs...}};
      std::vector<unsigned long long> start;
      std::vector<unsigned long long> end;

      start.resize(iv.size());
      end.resize(iv.size());

      for (int i = 0; i < iv.size(); i++)
      {
        start[i] = iv[i];
        end[i] = iv[i];
      }

      if (virtual_array_flag == 0)
      {
        std::vector<T> data_v;
        data_v.resize(1);
        endpoint->Read(start, end, static_cast<void *>(data_v.data()));
        return data_v[0];
      }
      else
      {
        std::vector<T> data_v;
        ReadArray(start, end, data_v);
        return data_v[0];
      }
    }

    template <typename... Is>
    inline void SetValue(T data_p, Is... indexs) const
    {
      std::vector<int> iv{{indexs...}};
      std::vector<unsigned long long> start;
      std::vector<unsigned long long> end;
      std::vector<T> data_v;

      start.resize(iv.size());
      end.resize(iv.size());
      data_v.resize(1);

      data_v[0] = data_p;
      for (int i = 0; i < iv.size(); i++)
      {
        start[i] = iv[i];
        end[i] = iv[i];
      }

      endpoint->Write(start, end, static_cast<void *>(data_v.data()));
    }

    template <class AttributeType>
    void PushBackAttribute(std::string data_endpoint)
    {
      if (attribute_endpoint_vector.size() == 0)
        virtual_array_flag = true;
      Endpoint *attribute_endpoint = EndpointFactory::NewEndpoint(data_endpoint);
      AuEndpointDataType data_element_type = InferDataType<AttributeType>();
      attribute_endpoint->SetDataElementType(data_element_type);
      if (attribute_endpoint->GetEndpointType() == EP_MEMORY)
        endpoint_memory_flag = true;
      attribute_endpoint_vector.push_back(attribute_endpoint);
    }

    template <class AttributeType>
    int AppendAttribute(const std::string &data_endpoint)
    {
      if (attribute_endpoint_vector.size() == 0)
        virtual_array_flag = true;
      Endpoint *attribute_endpoint = EndpointFactory::NewEndpoint(data_endpoint);
      AuEndpointDataType data_element_type = InferDataType<AttributeType>();
      attribute_endpoint->SetDataElementType(data_element_type);
      if (attribute_endpoint->GetEndpointType() == EP_MEMORY)
        endpoint_memory_flag = true;
      attribute_endpoint_vector.push_back(attribute_endpoint);
      attribute_array_data_endpoint_info.push_back(data_endpoint);
      //add to clean
      endpoint_clean_vector[attribute_endpoint] = true;
      return 0;
    }

    template <class AttributeType>
    int InsertAttribute(const std::string &data_endpoint, const int index)
    {
      if (attribute_endpoint_vector.size() == 0)
        virtual_array_flag = true;
      Endpoint *attribute_endpoint = EndpointFactory::NewEndpoint(data_endpoint);
      AuEndpointDataType data_element_type = InferDataType<AttributeType>();
      attribute_endpoint->SetDataElementType(data_element_type);
      attribute_endpoint_vector.insert(attribute_endpoint_vector.begin() + index, attribute_endpoint);
      attribute_array_data_endpoint_info.insert(attribute_array_data_endpoint_info.begin() + index, data_endpoint);
      if (attribute_endpoint->GetEndpointType() == EP_MEMORY)
        endpoint_memory_flag = true;
      //add to clean
      endpoint_clean_vector[attribute_endpoint] = true;
      return 0;
    }

    //index is zero based
    int EraseAttribute(const int &index)
    {
      if (attribute_endpoint_vector[index] != NULL)
      {
        delete attribute_endpoint_vector[index];
      }
      attribute_endpoint_vector.erase(attribute_endpoint_vector.begin() + index);
      attribute_array_data_endpoint_info.erase(attribute_array_data_endpoint_info.begin() + index);
      return 0;
    }

    int GetAttribute(const int &index, std::string &endpoint_id)
    {
      if (index < attribute_array_data_endpoint_info.size())
      {
        endpoint_id = attribute_array_data_endpoint_info[index];
        return 0;
      }
      return -1;
    }

    std::vector<Endpoint *> GetAttributeEndpoint()
    {
      return attribute_endpoint_vector;
    }

    bool GetVirtualArrayFlag()
    {
      return virtual_array_flag;
    }

    void SetVirtualArrayFlag(bool flag_p)
    {
      virtual_array_flag = true;
    }

    void SetDirOutputRegexReplace(std::regex &regex_p, std::string &replace_str_p)
    {
      dir_output_regex = regex_p;
      dir_output_replace_str = replace_str_p;
    }

    void SetDirInputRegexSearch(std::regex &regex_p)
    {
      dir_input_regex = regex_p;
    }

    AuEndpointType GetEndpointType()
    {
      if (!virtual_array_flag)
      {
        return endpoint->GetEndpointType();
      }
      else
      {
        return attribute_endpoint_vector[0]->GetEndpointType();
      }
    }

    std::vector<std::string> GetDirFile()
    {
      return endpoint->GetDirFileVector();
    }

    void SetDirFile(std::vector<std::string> &file_list)
    {
      endpoint->SetDirFileVector(file_list);
    }

    std::vector<int> GetDirChunkSize()
    {
      return endpoint->GetDirChunkSize();
    }

    inline void SetDirChunkSize(std::vector<int> &dir_chunk_size_p)
    {
      endpoint->SetDirChunkSize(dir_chunk_size_p);
    }

    inline int SetChunkSize(std::vector<int> data_chunk_size_p)
    {
      data_chunk_size = data_chunk_size_p;
      return 0;
    }

    inline int GetChunkSize(std::vector<int> &data_chunk_size_p)
    {
      data_chunk_size_p = data_chunk_size;
      return 0;
    }
    inline std::vector<int> GetChunkSize()
    {
      std::vector<int> data_chunk_size_p;
      GetChunkSize(data_chunk_size_p);
      return data_chunk_size_p;
    }

    inline int SetChunkSizeByMem(size_t max_mem_size)
    {
      set_chunk_size_by_mem_flag = true;
      set_chunk_size_by_mem_max_mem_size = max_mem_size;
      return 0;
    }

    inline int SetChunkSizeByDim(int dim_rank)
    {
      chunk_size_by_user_by_dimension_flag = true;
      data_auto_chunk_dim_index = dim_rank;
      return 0;
    }

    inline int GetArraySize(std::vector<unsigned long long> &size_p)
    {
      if (endpoint != NULL)
      {
        endpoint->ExtractMeta();
        data_size = endpoint->GetDimensions();
        size_p = data_size;
        return 0;
      }
      return -1;
    }

    inline int SetArraySize(const std::vector<unsigned long long> &size_p)
    {
      data_size = size_p;
      return 0;
    }

    inline int GetArrayRank(int &rank)
    {
      if (endpoint != NULL)
      {
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
    int Backup(std::string data_endpoint_p)
    {
      //std::string target_array_data_endpoint_info = data_endpoint_p;
      //Endpoint *target_endpoint = EndpointFactory::NewEndpoint(data_endpoint_p);

      AuEndpointType target_endpoint_type;
      std::string endpoint_info;
      ExtractEndpointTypeInfo(data_endpoint_p, target_endpoint_type, endpoint_info);

      int ret;
      if (endpoint_memory_flag == true && target_endpoint_type == EP_HDF5)
      {
        std::vector<std::string> arg_v;
        arg_v.push_back(endpoint_info);
        ret = endpoint->Control(0, arg_v);
      }
      else
      {
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
    int Restore(std::string data_endpoint_p)
    {
      std::string target_array_data_endpoint_info = data_endpoint_p;
      Endpoint *target_endpoint = EndpointFactory::NewEndpoint(data_endpoint_p);
      int ret;
      if (endpoint_memory_flag == true && target_endpoint->GetEndpointType() == EP_HDF5)
      {
        std::vector<std::string> arg_v;
        arg_v.push_back(target_endpoint->GetEndpointInfo());
        ret = endpoint->Control(1, arg_v);
      }
      else
      {
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
    int Clone(T intial_value)
    {
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
    int Clone()
    {
      //std::string intial_value_str = ""; //Empty string
      std::vector<std::string> op_str;

      endpoint->Control(DASH_ENABLE_LOCAL_MIRROR_CODE, op_str);
      return 0;
    }

    int Merge(int Op)
    {
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
    int Fill(T fill_value)
    {
      if (!ft_rank)
      {
        unsigned long long total_size = 1;
        std::vector<unsigned long long> start_p(data_size.size());
        std::vector<unsigned long long> end_p(data_size.size());

        for (int i = 0; i < data_size.size(); i++)
        {
          start_p[i] = 0;
          end_p[i] = data_size[i] - 1;
          total_size = total_size * data_size[i];
        }
        std::vector<T> default_value_v(total_size, fill_value);
        endpoint->Write(start_p, end_p, static_cast<void *>(default_value_v.data()));
      }

      return 0;
    }

    inline int SetEndpoint(const string &endpoint_id)
    {
      array_data_endpoint_info = endpoint_id;
      endpoint = EndpointFactory::NewEndpoint(endpoint_id);
      AuEndpointDataType data_element_type = InferDataType<T>();
      endpoint->SetDataElementType(data_element_type);

      if (endpoint->GetEndpointType() == EP_MEMORY)
      {
        endpoint_memory_flag = true;
      }
      endpoint_clean_vector[endpoint] = true;
      return 0;
    }
    inline int GetEndpoint(string &endpoint_id)
    {
      endpoint_id = array_data_endpoint_info;
      return 0;
    }

    /**
   * @brief pass command cmd to Endpoint of Array
   * 
   * @param cmd_p : int cmd (specific to Endpoint)
   * @param arg_v_p : a vector of arg. It is typed as string and specific to cmd_p
   */
    int ControlEndpoint(int cmd_p, std::vector<std::string> &arg_v_p)
    {
      return endpoint->Control(cmd_p, arg_v_p);
    }

    int ControlEndpoint(int cmd_p)
    {
      std::vector<std::string> arg_v_p;
      return endpoint->Control(cmd_p, arg_v_p);
    }

    //For old code
    inline int EndpointControl(int cmd_p, std::vector<std::string> &arg_v_p)
    {
      return ControlEndpoint(cmd_p, arg_v_p);
    }

    void inline ReportCost()
    {
      std::vector<double> mpi_stats_read, mpi_stats_udf, mpi_stats_write;
      MPIReduceStats(time_read, mpi_stats_read);
      MPIReduceStats(time_udf, mpi_stats_udf);
      MPIReduceStats(time_write, mpi_stats_write);

      if (ft_rank == 0)
      {
        std::cout << "Timing Results of All " << std::endl;
        std::cout << "Read      time (s) : max = " << mpi_stats_read[0] << ", min = " << mpi_stats_read[1] << ", ave = " << mpi_stats_read[2] / ft_size << std::endl;
        std::cout << "UDF       time (s) : max = " << mpi_stats_udf[0] << ", min = " << mpi_stats_udf[0] << ", ave = " << mpi_stats_udf[0] / ft_size << std::endl;
        std::cout << "Write     time (s) : max = " << mpi_stats_write[0] << ", min = " << mpi_stats_write[0] << ", ave = " << mpi_stats_write[0] / ft_size << std::endl;
        fflush(stdout);
      }
    }
    //Old-code
    inline void ReportTime()
    {
      ReportCost();
    }

    inline int GetReadCost(vector<double> &cost_stats)
    {
      MPIReduceStats(time_read, cost_stats);
      cost_stats[2] = cost_stats[2] / ft_size;
      return 0;
    }

    inline int GetWriteCost(vector<double> &cost_stats)
    {
      MPIReduceStats(time_write, cost_stats);
      cost_stats[2] = cost_stats[2] / ft_size;
      return 0;
    }
    inline int GetComputingCost(vector<double> &cost_stats)
    {
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
    int SetTag(const std::string &name, const PType value)
    {
      AuEndpointDataType data_element_type = InferDataType<PType>();
      //std::cout << AU_DOUBLE << ", " << data_element_type << "\n";
      return endpoint->WriteAttribute(name, &value, data_element_type, 1);
    }

    int SetTag(const std::string &name, const std::string value)
    {
      return endpoint->WriteAttribute(name, &value[0], AU_STRING, value.length());
    }

    template <class PType>
    int SetTag(const std::string &name, const std::vector<PType> value)
    {
      AuEndpointDataType data_element_type = InferDataType<PType>();
      return endpoint->WriteAttribute(name, &value[0], data_element_type, value.size());
    }
    /**
   * @brief Get the Attribute object
   * 
   * @tparam AType 
   * @param name 
   * @param value , return 
   * @return int 
   */
    template <class PType>
    int GetTag(const std::string &name, PType &value)
    {
      AuEndpointDataType data_element_type = InferDataType<PType>();
      return endpoint->ReadAttribute(name, &value, data_element_type);
    }

    int GetTag(const std::string &name, std::string &value)
    {
      size_t str_len = endpoint->GetAttributeSize(name, AU_STRING);
      value.resize(str_len);
      return endpoint->ReadAttribute(name, &value[0], AU_STRING, value.length());
    }
    template <class PType>
    int GetTag(const std::string &name, std::vector<PType> &value)
    {
      AuEndpointDataType data_element_type = InferDataType<PType>();
      size_t vec_len = endpoint->GetAttributeSize(name, data_element_type);
      value.resize(vec_len);
      return endpoint->ReadAttribute(name, &value[0], data_element_type, value.size());
    }

    int GetAllTagName(std::vector<string> &tag_name)
    {
      return endpoint->Control(OP_LIST_TAG, tag_name);
    }

    //
    //Below code are not used and just kept in case
    ///
    /**
   * @brief merge below to EndpointControl
   * 
   */
    void DisableCollectiveIO()
    {
      if (endpoint->GetEndpointType() == EP_HDF5)
      {
        endpoint->DisableCollectiveIO();
      }
    }

    void EnableCollectiveIO()
    {
      if (endpoint->GetEndpointType() == EP_HDF5)
      {
        endpoint->EnableCollectiveIO();
      }
    }

    inline int CreateVisFile(FTVisType vis_type)
    {
      if (endpoint->GetEndpointType() == EP_HDF5 && vis_type == FT_VIS_XDMF)
      {
        std::vector<std::string> arg_v_p;

        return endpoint->Control(OP_CREATE_VIS_SCRIPT, arg_v_p);
      }
      else
      {
        AU_EXIT("FT only supports Create XDMF on HDF5 now !\n");
      }
    }

    inline int CreateVisFile()
    {
      // && vis_type == FT_VIS_XDMF
      if (endpoint->GetEndpointType() == EP_HDF5)
      {
        std::vector<std::string> arg_v_p;

        return endpoint->Control(OP_CREATE_VIS_SCRIPT, arg_v_p);
      }
      else
      {
        AU_EXIT("FT only supports Create XDMF on HDF5 now !\n");
      }
    }

    inline int UpdateOverlap()
    {
    }

    inline int SetDirectOutput()
    {
      set_direct_output_flag = true;
    }

  }; // class of array
} // namespace FT

namespace AU = FT;
#endif