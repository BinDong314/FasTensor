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

//see au.h for its definations
extern int au_mpi_size_global;
extern int au_mpi_rank_global;

#ifndef ARRAY_UDF_ARRAY_H
#define ARRAY_UDF_ARRAY_H

#include "mpi.h"
#include "au_endpoint.h"
#include "au_stencil.h"
#include "au_utility.h"
#include "au_type.h"
#include "au_endpoint_factory.h"
#include "au_mpi.h"
#include "au_merge.h"
#include <assert.h>
#include <stdarg.h>
#include <regex>

namespace AU
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
  Endpoint *endpoint;

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

  unsigned long long current_chunk_id; //Id of the current chunk (in memory) to apply UDF

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

  std::vector<Endpoint *> attribute_endpoint_vector; //vector of endpoints for a virtual array

  //For directory
  std::regex dir_input_regex, dir_output_regex;
  std::string dir_output_replace_str;

  //Flag variable
  bool skip_flag = false;
  bool view_flag = false;
  bool cpp_vec_flag = false;
  bool virtual_array_flag = false;
  bool save_result_flag = true;
  bool reverse_apply_direction_flag = false;
  bool mirror_value_flag = false;
  bool apply_replace_flag = false;
  bool vector_type_flag = false;
  bool chunk_size_by_user_flag = false;
  bool chunk_size_by_user_by_dimension_flag = false;
  bool endpoint_memory_flag = false;

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

    AuEndpointDataType data_element_type = InferDataType<T>();
    endpoint->SetDataElementType(data_element_type);
    chunk_size_by_user_by_dimension_flag = true;
    if (endpoint->GetEndpointType() == EP_MEMORY)
      endpoint_memory_flag = true;
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
    if (endpoint->GetEndpointType() == EP_MEMORY)
    {
      endpoint_memory_flag = true;
      endpoint->SetDimensions(size_p);
      endpoint->Create();
    }
    data_size = size_p;
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
   * @brief Update the chunk size, given chunk_size_by_user_flag/chunk_size_by_user_by_dimension_flag
   * 
   * @param chunk_size_p 
   * @param overlap_size_p 
   * @param data_size 
   */
  void UpdateChunkSize(std::vector<int> &chunk_size_p, std::vector<unsigned long long> &data_size)
  {

    if (endpoint->GetEndpointType() == EP_DIR)
    {
      //chunk_size_p = endpoint->GetChunkSize();
    }

    //partition by dimensions
    if (chunk_size_by_user_by_dimension_flag)
    {
    }
    //return the default chunk_size

    //optimal chunk_size
  }

  void UpdateOverlapSize(std::vector<int> &overlap_size_p)
  {
    if (endpoint->GetEndpointType() == EP_DIR || chunk_size_by_user_by_dimension_flag)
    {
      for (int i = 0; i < data_dims; i++)
      {
        overlap_size_p[i] = 0;
      }
    }
    //optimal chunk_size
  }

  void InitializeApplyInput()
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

    //UpdateChunkSize(data_chunk_size, data_size);
    //UpdateOverlapSize(data_overlap_size);

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
        if (data_size[i] % skip_size[i] != 0 || data_chunk_size[i] % skip_size[i] != 0)
        {
          AU_EXIT("Strip size must be aligned with size of both array and chunk ! \n");
        }
        skiped_dims_size[i] = data_size[i] / skip_size[i];
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

    current_chunk_id = au_mpi_rank_global; //Each process deal with one chunk one time, starting from its rank

    //#ifdef DEBUG
    if (au_mpi_rank_global == 0)
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

  template <class UDFOutputType, class BType = UDFOutputType>
  void Apply(Stencil<UDFOutputType> (*UDF)(const Stencil<T> &), Array<BType> *B = nullptr)
  {
    //Set up the input data for LoadNextChunk
    InitializeApplyInput();
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
            for (int i = 0; i < data_dims; i++)
            {
              //The coordinate of the skip chunk this coordinate belong to
              skip_chunk_coordinate[i] = std::floor(cell_coordinate[i] / skip_size[i]);
              skip_chunk_coordinate_start[i] = skip_chunk_coordinate[i] * skip_size[i]; //first cell's coodinate of this skip chunk
              if (skip_chunk_coordinate_start[i] != cell_coordinate[i])
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
          if (endpoint->GetEndpointType() != EP_DIR)
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
          size_t vector_size;
          std::vector<unsigned long long> current_chunk_start_offset_v = current_result_chunk_start_offset, current_chunk_end_offset_v = current_result_chunk_end_offset;
          void *data_point;
          data_point = FlatVector(current_result_chunk_data, output_vector_flat_direction_index, current_chunk_start_offset_v, current_chunk_end_offset_v, vector_size);
          InferOutputSize(B_data_size, B_data_chunk_size, B_data_overlap_size, vector_size);
          B->CreateEndpoint(B_data_size, B_data_chunk_size, B_data_overlap_size);
          B->WriteEndpoint(current_chunk_start_offset_v, current_chunk_end_offset_v, data_point);
          free(data_point);
        }
        else
        {
          InferOutputSize(B_data_size, B_data_chunk_size, B_data_overlap_size, 0);
          B->CreateEndpoint(B_data_size, B_data_chunk_size, B_data_overlap_size);
          //B->WriteEndpoint(current_chunk_start_offset, current_chunk_end_offset, &current_result_chunk_data[0]);
          B->WriteArray(current_chunk_start_offset, current_chunk_end_offset, current_result_chunk_data);
        }
      }
      time_write = time_write + AU_WTIME - t_start;

      t_start = AU_WTIME;
      load_ret = LoadNextChunk(current_result_chunk_data_size);
      current_result_chunk_data.resize(current_result_chunk_data_size);
      time_read = time_read + AU_WTIME - t_start;

    } // end of while:: no more chunks to process

    return;
  }

  int WriteArray(std::vector<unsigned long long> &start_p, std::vector<unsigned long long> &end_p, std::vector<T> &data_p)
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

  std::vector<T> ReadArray(std::vector<unsigned long long> start, std::vector<unsigned long long> end)
  {
    //InitializeApplyInput();
    unsigned long long data_vector_size;
    COUNT_CELLS(start, end, data_vector_size);

    std::vector<T> data_vector;
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
    //Next chunk id
    if (!reverse_apply_direction_flag)
    {
      current_chunk_id = current_chunk_id + au_mpi_size_global;
    }
    else
    {
      current_chunk_id = current_chunk_id - au_mpi_size_global;
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

  void EnableApplyStride(std::vector<int> skip_size_p)
  {
    skip_size = skip_size_p;
    skip_flag = true;
  }

  void SetVectorDirection(OutputVectorFlatDirection flat_direction_index)
  {
    output_vector_flat_direction_index = flat_direction_index;
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
      data_v = ReadArray(start, end);
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

  void SetDirChunkSize(std::vector<int> &dir_chunk_size_p)
  {
    endpoint->SetDirChunkSize(dir_chunk_size_p);
  }

  /**
 * @brief Set the Size object
 * 
 * @param size_p 
 */
  void SetSize(std::vector<unsigned long long> size_p)
  {
    data_size = size_p;
  }

  /**
   * @brief write data to another endpoint type, e.g., HDF5 in disk
   * 
   * @param data_endpoint 
   * @return int 
   */
  int Nonvolatile(std::string data_endpoint_p)
  {
    std::string target_array_data_endpoint_info = data_endpoint_p;
    Endpoint *target_endpoint = EndpointFactory::NewEndpoint(data_endpoint_p);

    int ret;
    if (endpoint_memory_flag == true && target_endpoint->GetEndpointType() == EP_HDF5)
    {
      ret = endpoint->SpecialOperator(0, target_endpoint->GetEndpointInfo());
    }
    else
    {
      AU_EXIT("Nonvolatile: only support MEMORY to HDF5 now!");
    }

    return ret;
  }

  /**
    * @brief load HDF5 array to IN_MEMORY array   
  * 
  * @param data_endpoint_p 
  * @return int 
  */
  int Volatile(std::string data_endpoint_p)
  {
    std::string target_array_data_endpoint_info = data_endpoint_p;
    Endpoint *target_endpoint = EndpointFactory::NewEndpoint(data_endpoint_p);
    int ret;
    if (endpoint_memory_flag == true && target_endpoint->GetEndpointType() == EP_HDF5)
    {
      ret = endpoint->SpecialOperator(1, target_endpoint->GetEndpointInfo());
    }
    else
    {
      AU_EXIT("Volatile: only support HDF5 to MEMORY now!");
    }

    return ret;
  }

  /**
   * @brief create a local mirror (clone) of array
   *        with the inital value
   * @param intial_value 
   * @return int 
   */
  int Clone(T intial_value)
  {
    std::string intial_value_str = std::to_string(intial_value);
    endpoint->SpecialOperator(DASH_ENABLE_LOCAL_MIRROR_CODE, intial_value_str);
    return 0;
  }

  /**
   * @brief create a local miroor (clone of array) without intial value
   *        so, it needs to copy the whole array
   * @return int 
   */
  int Clone()
  {
    std::string intial_value_str = ""; //Empty string
    endpoint->SpecialOperator(DASH_ENABLE_LOCAL_MIRROR_CODE, intial_value_str);
    return 0;
  }

  int Merge(int Op)
  {
    std::string op_str = std::to_string(Op);
    endpoint->SpecialOperator(DASH_MERGE_MIRRORS_CODE, op_str);
    return 0;
  }

  /**
   * @brief Fill the array with value
   * 
   * @param value 
   * @return int 
   */
  int Fill(T &value)
  {
  }

}; // class of array

} // namespace AU
#endif
