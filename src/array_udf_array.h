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

#ifndef ARRAY_UDF_ARRAY
#define ARRAY_UDF_ARRAY

//#define HAND_CODE_2D

//#include <boost/fusion/adapted/struct.hpp>
//#include <boost/fusion/include/for_each.hpp>
//#include <boost/phoenix/phoenix.hpp>

#include <assert.h>
#include "mpi.h"
#include "array_udf_io.h"
#include "array_udf_stencil.h"
#include "utility.h"
#include "array_udf_attribute.h"
//#include "array_udf_ga.h"
#include "array_udf_h5.h"

//#define ENABLE_OPENMP 1
#ifdef ENABLE_OPENMP
#include <omp.h>
#else
#define omp_get_thread_num() 0
#endif

//#define DEBUG 1
int save_result_flag = 1;
int trail_run_flag = 0;
int row_major_chunk_flag = 0;
unsigned long long per_core_mem_size_limit = 1073741824; // 1GB,  240 000 000; // Try a smller size 24000000000; //50GB/24 per core Byte
//https://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine
//https://stackoverflow.com/questions/349889/how-do-you-determine-the-amount-of-linux-system-ram-in-c

double time_of_update_host_zone = 0;

//Comment out following test codes
//int      arraystoreloadflat    = 0;
//int      handop_flag           = 0;
//int      single_step_flag      = 0;
//#define  HANDOP3D   1

using namespace std;

//T is  the type of actuall data in this array.
//The AttriType is optional to express the type of attribute arrays in Virtual array
//By defualt, it is equal to first type T
//T: input type; AttrType: Output
template <class T, class AttrType = T>
class Array
{
private:
  int mpi_rank; //MPI information
  int mpi_size; //MPI information

  DataOrigin d_orig;    //orginal of the data
  NVSFile nvs_f;        //the type of NVS file
  CacheFlag cache_flag; //the flag for the cache

  int data_created_flag = 0; //Indicate whether data was created before
  Data<T> *data = NULL;      //the data

  std::vector<int> data_chunk_size;                       //size of each chunk (user provide)
  std::vector<int> data_overlap_size;                     //size of overlapping  (user provide)
  int data_dims;                                          //The dimensioins of data_on_disk , equal to dsize.size()
  std::vector<unsigned long long> data_dims_size;         //The size of each dimension (global, extracted from data_on_disk)
  unsigned long long data_total_chunks;                   //The total number of chunks (global)
  std::vector<unsigned long long> data_chunked_dims_size; //The number of chunks per dimenstion

  int current_chunk_id;              //Id of the current chunk (in memory) to apply UDF
  std::vector<T> current_chunk_data; //Pointer to data of current chunk

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
  std::vector<long long> ol_origin_offset;                       //Size of the chunk, euqal to end_offset - start_offset
  std::vector<AttrType> current_result_chunk_data;               //Pointer to data of current chunk
  //std::vector<AttrType>           current_result_chunk_data_temp;         //Pointer to data of current chunk

  int processed_chunks_count = 0;
  double time_read = 0, time_udf = 0, time_write = 0, time_create = 0, time_sync = 0, t_start, t_end, time_nonvolatile = 0;
  //class Array<float>**            sub_array_vector;
  int skip_flag = 0;                                //For striping based Apply
  std::vector<unsigned long long> skip_size;        //Size to ship after each operation
  std::vector<unsigned long long> skiped_dims_size; //Size of the data after
  std::vector<unsigned long long> skiped_chunks;    //# of chunks after skip
  std::vector<int> skiped_chunk_size;               //Size of each chunk after skip
  //std::vector<unsigned long long>  skiped_chunks_per_orig_chunk;  //The skip during each orig chunk
  std::vector<unsigned long long> skip_pads; //The padding between two contigious skip chunks.
  int virtual_array_flag = 0;
  std::vector<Array<AttrType> *> attributes;
  int padding_value_set_flag = 0;
  T padding_value;
  std::vector<int> padding_size;
  int vector_type_flag = 0;
  int filter_amount; // see blow output_vector_size
  int output_element_different_type_flag = 0;
  int output_element_type_class;
  int mirror_value_flag = 0;
  std::vector<T> mirror_values;
  int set_apply_direction_flag = 0;
  int apply_direction;
  int apply_replace_flag = 0;
  int apply_writeback_flag = 0;
  int preload_flag = 0;

  std::vector<T> cpp_vec_input;
  int cpp_vec_flag = 0;

  int output_vector_size = 0;
  int output_vector_flat_direction_index = 0;

public:
  //Do Nothing
  Array(){};

  //For virtual array
  Array(DataOrigin type)
  {
    time_read = 0;
    time_write = 0;
    time_udf = 0;
    time_create = 0;
    time_sync = 0;
    time_nonvolatile = 0;
    if (type == AU_VIRTUAL)
    {
      virtual_array_flag = 1;
    }
    else
    {
      printf("Unknow DataOrigin type in Array() constructor. \n");
      exit(-1);
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    current_chunk_id = mpi_rank; //Each process deal with one chunk one time, starting from its rank
  };

  //To store the result. The result file is delayed to be created
  Array(std::string fn, std::string gn, std::string dn)
  {
    time_read = 0;
    time_write = 0;
    time_udf = 0;
    time_create = 0;
    time_sync = 0;
    time_nonvolatile = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    data = new Data<T>(fn, gn, dn, DELAY_TO_CREATE);
  };

  //Input file:
  //HDF Specific Intialization
  //   fn: file name
  //   gn: group name
  //   dn: data set name
  //cs and os: user defined chunk size and overlap size
  Array(std::string fn, std::string gn, std::string dn, std::vector<int> cs, std::vector<int> os)
  {
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    data = new Data<T>(fn, gn, dn);

    data_chunk_size = cs;
    data_overlap_size = os;

    time_read = 0;
    time_write = 0;
    time_udf = 0;
    time_create = 0;
    time_sync = 0;
    time_nonvolatile = 0;

    data_dims_size = data->GetDimSize();
    data_dims = data_dims_size.size();

    if (row_major_chunk_flag == 1)
    {
      if (mpi_rank == 0)
        std::cout << "Auto row-major partition ... " << std::endl;
      unsigned long long node_size_limit = 1;
      for (int i = 0; i < data_dims; i++)
      {
        node_size_limit = data_dims_size[i] * node_size_limit;
      }
      node_size_limit = node_size_limit / mpi_size;
      if (node_size_limit > (per_core_mem_size_limit / sizeof(T)))
      {
        node_size_limit = per_core_mem_size_limit / sizeof(T);
        if (mpi_rank == 0)
        {
          std::cout << "Hiting the memory size limit !" << std::endl;
        }
      }
      std::vector<unsigned long long> chunk_size_temp = RowMajorOrderReverse(node_size_limit, data_dims_size);
      if (mpi_rank == 0)
      {
        std::cout << " Chunk size from limit =  " << node_size_limit << std::endl;
        for (int i = 0; i < data_dims; i++)
        {
          std::cout << chunk_size_temp[i] << std::endl;
        }
      }

      //Round it to the width of the data
      int replace_flag = 1;
      for (int i = data_dims - 1; i > 0; i--)
      {
        if (chunk_size_temp[i] != data_dims_size[i])
        {
          chunk_size_temp[i] = data_dims_size[i];
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
        if (mpi_rank == 0)
          std::cout << " New chunk size: ";
        for (int i = data_dims - 1; i >= 0; i--)
        {
          data_chunk_size[i] = chunk_size_temp[i];
          if (mpi_rank == 0)
            std::cout << data_chunk_size[i] << " , ";
        }
        if (mpi_rank == 0)
          std::cout << std::endl;
      }
      else
      {
        if (mpi_rank == 0)
          std::cout << " No auto-chunk ! " << std::endl;
      }
    } //End of row-major chunking

    current_chunk_start_offset.resize(data_dims);
    current_chunk_end_offset.resize(data_dims);
    current_chunk_size.resize(data_dims);

    current_result_chunk_start_offset.resize(data_dims);
    current_result_chunk_end_offset.resize(data_dims);

    current_chunk_ol_start_offset.resize(data_dims);
    current_chunk_ol_end_offset.resize(data_dims);
    current_chunk_ol_size.resize(data_dims);

    data_chunked_dims_size.resize(data_dims);
    ol_origin_offset.resize(data_dims);

    data_total_chunks = 1;
    if (mpi_rank == 0)
      std::cout << "data rank = " << data_dims << std::endl;
    for (int i = 0; i < data_dims; i++)
    {
      if (data_dims_size[i] % data_chunk_size[i] == 0)
      {
        data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i];
      }
      else
      {
        data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i] + 1;
      }
      data_total_chunks = data_total_chunks * data_chunked_dims_size[i];
    }
#ifdef DEBUG
    if (mpi_rank == 0)
    {
      std::cout << "File    : " << fn << std::endl;
      std::cout << "Group   : " << gn << std::endl;
      std::cout << "Dataset : " << dn << std::endl;

      std::cout << "data size  = ";
      for (int i = 0; i < data_dims; i++)
      {
        std::cout << ", " << data_dims_size[i];
      }
      std::cout << std::endl;

      std::cout << "chunk size  = ";
      for (int i = 0; i < data_dims; i++)
      {
        std::cout << ", " << data_chunk_size[i];
      }
      std::cout << std::endl;

      std::cout << "overlap size = ";
      for (int i = 0; i < data_dims; i++)
      {
        std::cout << ", " << os[i];
      }
      std::cout << std::endl;
    }
#endif

    if (data_total_chunks % mpi_size != 0)
    {
      data->DisableCollectivIO();
    }

    current_chunk_id = mpi_rank; //Each process deal with one chunk one time, starting from its rank
  };

  //Input:
  // d_orig: the original of the data: AU_NVS (from disk or other persistent device)
  //                                   AU_COMPUTED (generated by UDF)
  //                                   AU_NV  (from cached memory)
  // nvs_f:  Non-volatile file types: AU_HDF5, AU_NETCDF, AU_AUDIOS
  //         Note: nvs_f can be orginal place to retrive data or finale place to store the data
  //
  //some nvs_f's detail information (HDF like)
  //   fn: file name
  //   gn: group name
  //   dn: data set name
  //cs and os: user defined chunk size and overlap size
  Array(DataOrigin d_orig_p, NVSFile nvs_f_p, std::string fn, std::string gn, std::string dn, std::vector<int> cs, std::vector<int> os)
  {
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    time_read = 0;
    time_write = 0;
    time_udf = 0;
    time_create = 0;
    time_sync = 0;
    time_nonvolatile = 0;
    d_orig = d_orig_p;
    nvs_f = nvs_f_p;

    typedef typename extract_value_type<T>::value_type value_type;

    if (is_vector_type<AttrType>())
    {
      vector_type_flag = 1;
      //std::cout << "Vector has same type as T : " << is_base_type_T<AttrType, T>() << std::endl;
      if (is_base_type_T<AttrType, T>() == 0)
      {
        output_element_different_type_flag = 1;
        if (is_base_type_T<AttrType, int>())
        {
          //if (!mpi_rank)
          //  printf("In Array init: output element type is int \n ");
          output_element_type_class = H5T_INTEGER;
        }
        else if (is_base_type_T<AttrType, float>())
        {
          //if (!mpi_rank)
           // printf("In Array init: output element type is float \n ");
          output_element_type_class = H5T_FLOAT;
        }
        else if (is_base_type_T<AttrType, std::complex<float>>())
        {
          if (!mpi_rank)
            printf("In Array init: output element type is std::complex<float> \n ");
          output_element_type_class = H5T_COMPOUND;
        }
        else
        {
          printf("In Array init: not support type: %s:%d \n ", __FILE__, __LINE__);
          exit(-1);
        }
      }
    }
    else
    {
      if (is_same_types<AttrType, T>() == false)
      {
        output_element_different_type_flag = 1;
        if (is_same_types<AttrType, int>())
        {
          //if (!mpi_rank) printf("In Array init: output element type is int \n ");
          output_element_type_class = H5T_INTEGER;
        }
        else if (is_same_types<AttrType, float>())
        {
          //if (!mpi_rank)
          //  printf("In Array init: output element type is float \n ");
          output_element_type_class = H5T_FLOAT;
        }
        else
        {
          printf("In Array init: not support type \n ");
          exit(-1);
        }
      }
    }
    //Data is computed from another file
    if (AU_COMPUTED == d_orig_p)
    {
      data = new Data<T>(fn, gn, dn, d_orig_p, nvs_f_p);
      return;
    }

    //Data is from disk file
    data = new Data<T>(fn, gn, dn, d_orig_p, nvs_f_p);
    data_chunk_size = cs;
    data_overlap_size = os;
    data_dims_size = data->GetDimSize();
    data_dims = data_dims_size.size();

    if (row_major_chunk_flag == 1)
    {
      if (mpi_rank == 0)
        std::cout << "Auto row-major partition ... " << std::endl;
      unsigned long long node_size_limit = 1;
      for (int i = 0; i < data_dims; i++)
      {
        node_size_limit = data_dims_size[i] * node_size_limit;
      }
      node_size_limit = node_size_limit / mpi_size;
      if (node_size_limit > (per_core_mem_size_limit / sizeof(T)))
      {
        node_size_limit = per_core_mem_size_limit / sizeof(T);
        if (mpi_rank == 0)
        {
          std::cout << "Using the memory size as limit !" << std::endl;
        }
      }
      std::vector<unsigned long long> chunk_size_temp = RowMajorOrderReverse(node_size_limit, data_dims_size);
      if (mpi_rank == 0)
      {
        std::cout << " Node_size_limit =  " << node_size_limit << std::endl;
        for (int i = 0; i < data_dims; i++)
        {
          std::cout << chunk_size_temp[i] << std::endl;
        }
      }

      int replace_flag = 1;
      for (int i = data_dims - 1; i > 0; i--)
      {
        if (chunk_size_temp[i] != data_dims_size[i])
        {
          chunk_size_temp[i] = data_dims_size[i];
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
        if (mpi_rank == 0)
          std::cout << " New chunk size: ";
        for (int i = data_dims - 1; i >= 0; i--)
        {
          data_chunk_size[i] = chunk_size_temp[i];
          if (mpi_rank == 0)
            std::cout << data_chunk_size[i] << " , ";
        }
        if (mpi_rank == 0)
          std::cout << std::endl;
      }
      else
      {
        if (mpi_rank == 0)
          std::cout << " No auto-chunk ! " << std::endl;
      }
    } //End of row-major chunking

    current_chunk_start_offset.resize(data_dims);
    current_chunk_end_offset.resize(data_dims);
    current_chunk_size.resize(data_dims);

    current_result_chunk_start_offset.resize(data_dims);
    current_result_chunk_end_offset.resize(data_dims);

    current_chunk_ol_start_offset.resize(data_dims);
    current_chunk_ol_end_offset.resize(data_dims);
    current_chunk_ol_size.resize(data_dims);

    data_chunked_dims_size.resize(data_dims);
    ol_origin_offset.resize(data_dims);

    data_total_chunks = 1;
#ifdef DEBUG
    if (mpi_rank == 0)
      std::cout << "data rank = " << data_dims << std::endl;
#endif

    for (int i = 0; i < data_dims; i++)
    {
      if (data_dims_size[i] % data_chunk_size[i] == 0)
      {
        data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i];
      }
      else
      {
        data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i] + 1;
      }
      data_total_chunks = data_total_chunks * data_chunked_dims_size[i];
    }

    //#ifdef DEBUG
    if (mpi_rank == 0)
    {
      std::cout << "File    : " << fn << std::endl;
      std::cout << "Group   : " << gn << std::endl;
      std::cout << "Dataset : " << dn << std::endl;

      std::cout << "data size  = ";
      for (int i = 0; i < data_dims; i++)
      {
        std::cout << ", " << data_dims_size[i];
      }
      std::cout << std::endl;

      std::cout << "chunk size  = ";
      for (int i = 0; i < data_dims; i++)
      {
        std::cout << ", " << data_chunk_size[i];
      }
      std::cout << std::endl;

      std::cout << "overlap size = ";
      for (int i = 0; i < data_dims; i++)
      {
        std::cout << ", " << os[i];
      }
      std::cout << std::endl;
      std::cout << "Total chunks =  " << data_total_chunks << std::endl;
    }
    //#endif

    if (data_total_chunks % mpi_size != 0)
    {
      data->DisableCollectivIO();
    }

    current_chunk_id = mpi_rank; //Each process deal with one chunk one time, starting from its rank
  };

  //Input:
  // d_orig: the original of the data: AU_NVS (from disk or other persistent device)
  //                                   AU_COMPUTED (generated by UDF)
  //                                   AU_NV  (from cached memory)
  // nvs_f:  Non-volatile file types: AU_HDF5, AU_NETCDF, AU_AUDIOS
  //         Note: nvs_f can be orginal place to retrive data or finale place to store the data
  //
  //some nvs_f's detail information (HDF like)
  //   fn: file name
  //   gn: group name
  //   dn: data set name
  //cs and os: user defined chunk size and overlap size
  Array(DataOrigin d_orig_p, NVSFile nvs_f_p, std::string fn, std::string gn, std::string dn, GlobalArrayOperation gaop)
  {
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    d_orig = d_orig_p;
    nvs_f = nvs_f_p;

    time_read = 0;
    time_write = 0;
    time_udf = 0;
    time_create = 0;
    time_sync = 0;
    time_nonvolatile = 0;

    data = new Data<T>(fn, gn, dn, d_orig_p, nvs_f_p);
    data_dims_size = data->GetDimSize();
    data_dims = data_dims_size.size();
#ifdef DEBUG
    if (mpi_rank == 0)
    {
      std::cout << "File    : " << fn << std::endl;
      std::cout << "Group   : " << gn << std::endl;
      std::cout << "Dataset : " << dn << std::endl;
      std::cout << "data size  = ";
      for (int i = 0; i < data_dims; i++)
      {
        std::cout << ", " << data_dims_size[i];
      }
      std::cout << std::endl;
    }
#endif

    if (gaop == AU_PRELOAD)
    {
      cache_flag = AU_CACHED;
      preload_flag = 1;
      data->PreLoad();

      current_chunk_start_offset.resize(data_dims);
      current_chunk_end_offset.resize(data_dims);
      current_chunk_size.resize(data_dims);
      current_result_chunk_start_offset.resize(data_dims);
      current_result_chunk_end_offset.resize(data_dims);
      current_chunk_ol_start_offset.resize(data_dims);
      current_chunk_ol_end_offset.resize(data_dims);
      current_chunk_ol_size.resize(data_dims);
      data_chunked_dims_size.resize(data_dims);
      ol_origin_offset.resize(data_dims);

      data_overlap_size.resize(data_dims);
      data_chunk_size.resize(data_dims);
      for (int i = 0; i < data_dims; i++)
      {
        data_chunk_size[i] = data_dims_size[i];
        data_overlap_size[i] = 0;
      }

      data_total_chunks = 1;
      for (int i = 0; i < data_dims; i++)
      {
        if (data_dims_size[i] % data_chunk_size[i] == 0)
        {
          data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i];
        }
        else
        {
          data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i] + 1;
        }
        data_total_chunks = data_total_chunks * data_chunked_dims_size[i];
      }

      //#ifdef DEBUG
      if (mpi_rank == 0)
      {
        std::cout << "File    : " << fn << std::endl;
        std::cout << "Group   : " << gn << std::endl;
        std::cout << "Dataset : " << dn << std::endl;

        std::cout << "data size  = ";
        for (int i = 0; i < data_dims; i++)
        {
          std::cout << ", " << data_dims_size[i];
        }
        std::cout << std::endl;

        std::cout << "chunk size  = ";
        for (int i = 0; i < data_dims; i++)
        {
          std::cout << ", " << data_chunk_size[i];
        }
        std::cout << std::endl;

        std::cout << "overlap size = ";
        for (int i = 0; i < data_dims; i++)
        {
          std::cout << ", " << data_overlap_size[i];
        }
        std::cout << std::endl;
        std::cout << "Total chunks =  " << data_total_chunks << std::endl;
      }
      //#endif
      //for pre load, each process read the whole chunk
      current_chunk_id = 0; //Each process deal with one chunk one time, starting from its rank
    }
  };

  //Input:
  // d_orig: the original of the data: AU_NVS (from disk or other persistent device)
  //                                   AU_COMPUTED (generated by UDF)
  //                                   AU_NV  (from cached memory)
  // nvs_f:  Non-volatile file types: AU_HDF5, AU_NETCDF, AU_AUDIOS
  //         Note: nvs_f can be orginal place to retrive data or finale place to store the data
  //
  //some nvs_f's detail information (HDF like)
  //   fn: file name
  //   gn: group name
  //   dn: data set name
  //cs and os: user defined chunk size and overlap size
  //Used by regrid
  Array(DataOrigin d_orig_p, NVSFile nvs_f_p, std::string fn, std::string gn, std::string dn, std::vector<int> cs, std::vector<int> os, int vs_handle)
  {
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    time_read = 0;
    time_write = 0;
    time_udf = 0;
    time_create = 0;
    time_sync = 0;
    time_nonvolatile = 0;
    d_orig = d_orig_p;
    nvs_f = nvs_f_p;

    //Data is from disk file
    data = new Data<T>(fn, gn, dn, d_orig_p, nvs_f_p, vs_handle);
    data_chunk_size = cs;
    data_overlap_size = os;

    data_dims_size = data->GetDimSize();
    //printf("At array init: Old handle = %d, new handle = %d \n", vs_handle, data->GetVSHandle());

    data_dims = data_dims_size.size();

    current_chunk_start_offset.resize(data_dims);
    current_chunk_end_offset.resize(data_dims);
    current_chunk_size.resize(data_dims);

    current_result_chunk_start_offset.resize(data_dims);
    current_result_chunk_end_offset.resize(data_dims);

    current_chunk_ol_start_offset.resize(data_dims);
    current_chunk_ol_end_offset.resize(data_dims);
    current_chunk_ol_size.resize(data_dims);

    data_chunked_dims_size.resize(data_dims);
    ol_origin_offset.resize(data_dims);

    data_total_chunks = 1;

    for (int i = 0; i < data_dims; i++)
    {
      if (data_dims_size[i] % data_chunk_size[i] == 0)
      {
        data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i];
      }
      else
      {
        data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i] + 1;
      }
      data_total_chunks = data_total_chunks * data_chunked_dims_size[i];
    }

    //#ifdef DEBUG
    if (mpi_rank == 0)
    {
      std::cout << "data rank = " << data_dims << std::endl;
      std::cout << "File    : " << fn << std::endl;
      std::cout << "Group   : " << gn << std::endl;
      std::cout << "Dataset : " << dn << std::endl;
      std::cout << "data rank = " << data_dims << std::endl;
      std::cout << "data size  = ";
      for (int i = 0; i < data_dims; i++)
      {
        std::cout << ", " << data_dims_size[i];
      }
      std::cout << std::endl;

      std::cout << "chunk size  = ";
      for (int i = 0; i < data_dims; i++)
      {
        std::cout << ", " << data_chunk_size[i];
      }
      std::cout << std::endl;

      std::cout << "overlap size = ";
      for (int i = 0; i < data_dims; i++)
      {
        std::cout << ", " << os[i];
      }
      std::cout << std::endl;
      std::cout << "Total chunks =  " << data_total_chunks << std::endl;
    }
    //#endif

    cache_flag = AU_CACHED;
    current_chunk_id = mpi_rank; //Each process deal with one chunk one time, starting from its rank
  };

  Array(std::vector<T> &vec)
  {
    //Using the c++'s std::vector to intialize Array
    int len = vec.size();
    cpp_vec_input.resize(len);

    for (int i = 0; i < len; i++)
      cpp_vec_input[i] = vec[i];

    cpp_vec_flag = 1;
  }

  ~Array()
  {
#ifdef DEBUG
    printf("Destructor in Array !\n");
#endif
    if (data != NULL)
    {
      delete data;
      data = NULL;
    }
  }

  void DisableCache()
  {
    cache_flag = AU_NOCACHE;
    data->DiableCache();
  }

  void EnableCache()
  {
    if (mpi_rank == 0)
      printf("Enable cache flag !\n");
    cache_flag = AU_CACHED;
    data->EnableCache();
  }

  void Nonvolatile()
  {
    t_start = MPI_Wtime();
    if (cache_flag == AU_CACHED)
    {
      data->Nonvolatile();
    }
    t_end = MPI_Wtime();
    time_nonvolatile = time_nonvolatile + t_end - t_start;
  }

  //Return  1, data read into   local_chunk_data
  //Return  0, end of file (no data left to handle)
  //Return -1: error happen
  int LoadNextChunk()
  {
    // std::cout << "Load chunk id =" << current_chunk_id << ", data_total_chunks = " << data_total_chunks << std::endl;
    if (current_chunk_id >= data_total_chunks || current_chunk_id < 0)
    {
      return 0;
    }

    //std::cout << "In LoadNextChun(): skiped_chunks[0]: " << skiped_chunks[0]  << ", current_chunk_id: " << current_chunk_id << std::endl;
    current_chunk_cells = 1;
    current_result_chunk_cells = 1;
    current_chunk_ol_cells = 1;

    std::vector<unsigned long long> chunk_coordinate = RowMajorOrderReverse(current_chunk_id, data_chunked_dims_size);
    std::vector<unsigned long long> skiped_chunk_coordinate;
    if (skip_flag == 1)
      skiped_chunk_coordinate = RowMajorOrderReverse(current_chunk_id, skiped_chunks);

    //calculate the chunk start and end
    for (int i = 0; i < data_dims; i++)
    {
      if (data_chunk_size[i] * chunk_coordinate[i] < data_dims_size[i])
      {
        current_chunk_start_offset[i] = data_chunk_size[i] * chunk_coordinate[i];
      }
      else
      {
        current_chunk_start_offset[i] = data_dims_size[i];
      }

      if (current_chunk_start_offset[i] + data_chunk_size[i] - 1 < data_dims_size[i])
      {
        current_chunk_end_offset[i] = current_chunk_start_offset[i] + data_chunk_size[i] - 1;
      }
      else
      {
        current_chunk_end_offset[i] = data_dims_size[i] - 1;
      }

      assert((current_chunk_end_offset[i] - current_chunk_start_offset[i] + 1 >= 0));
      current_chunk_size[i] = current_chunk_end_offset[i] - current_chunk_start_offset[i] + 1;
      current_chunk_cells = current_chunk_cells * current_chunk_size[i];

      //Deal with the result chunks size
      if (skip_flag == 0)
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
      if (current_chunk_end_offset[i] + data_overlap_size[i] < data_dims_size[i])
      {
        current_chunk_ol_end_offset[i] = current_chunk_end_offset[i] + data_overlap_size[i];
      }
      else
      {
        current_chunk_ol_end_offset[i] = data_dims_size[i] - 1;
      }
      assert((current_chunk_ol_end_offset[i] - current_chunk_ol_start_offset[i] + 1 >= 0));
      current_chunk_ol_size[i] = current_chunk_ol_end_offset[i] - current_chunk_ol_start_offset[i] + 1;
      current_chunk_ol_cells = current_chunk_ol_cells * current_chunk_ol_size[i];
    }

#ifdef DEBUG
    /*if (mpi_rank == 0)
    {
      PrintVector<unsigned long long>("current_result_chunk_start_offset", current_result_chunk_start_offset);
      PrintVector<unsigned long long>("current_result_chunk_end_offset", current_result_chunk_end_offset);
      std::cout << "current_result_chunk_cells : " << current_result_chunk_cells << std::endl;
      std::cout << "Load chunk id =" << current_chunk_id << " ...  done , at proc  " << mpi_rank << std::endl;
    }*/

    if (mpi_rank == mpi_size - 1)
    {
      //std::cout << "data_dims_size =" << data_dims_size[]

      // PrintVector<unsigned long long>("data_chunk_size = ", data_chunk_size);
      PrintVector<unsigned long long>("data_dims_size = ", data_dims_size);
      std::cout << "Load chunk id =" << current_chunk_id << ", at proc " << mpi_rank << std::endl;
      std::cout << "chunk_coordinate[] =" << chunk_coordinate[0] << "," << chunk_coordinate[1] << "," << chunk_coordinate[2] << std::endl;
      std::cout << "current_chunk_start_offset [] =" << current_chunk_start_offset[0] << "," << current_chunk_start_offset[1] << "," << current_chunk_start_offset[2] << std::endl;
      std::cout << "current_chunk_end_offset [] =" << current_chunk_end_offset[0] << "," << current_chunk_end_offset[1] << "," << current_chunk_end_offset[2] << std::endl;
      std::cout << "current_chunk_size [] =" << current_chunk_size[0] << "," << current_chunk_size[1] << "," << current_chunk_size[2] << std::endl;
      std::cout << "data_total_chunks = " << data_total_chunks << std::endl;

      std::cout << "current_chunk_ol_start_offset [] =" << current_chunk_ol_start_offset[0] << "," << current_chunk_ol_start_offset[1] << std::endl;
      std::cout << "current_chunk_ol_end_offset [] =" << current_chunk_ol_end_offset[0] << "," << current_chunk_ol_end_offset[1] << std::endl;
      std::cout << "current_chunk_ol_size [] = " << current_chunk_ol_size[0] << ", " << current_chunk_ol_size[1] << std::endl;
      std::cout << "ol_origin_offset [] = " << ol_origin_offset[0] << ", " << ol_origin_offset[1] << " \n"
                << std::endl;
    }
#endif

    //Next chunk id
    if (set_apply_direction_flag == 0)
    {
      current_chunk_id = current_chunk_id + mpi_size;
    }
    else
    {
      current_chunk_id = current_chunk_id - mpi_size;
    }
    current_chunk_data.resize(current_chunk_ol_cells);
    if (save_result_flag == 1)
    {
      if (skip_flag == 0)
      {
        current_result_chunk_data.resize(current_chunk_cells); //current_chunk_cells
        //std::memset(&current_result_chunk_data[0], 0, sizeof(&current_result_chunk_data[0]));
        //std::cout << "current_chunk_cells= " << current_chunk_cells << std::endl;
      }
      else
      {
        current_result_chunk_data.resize(current_result_chunk_cells);
        //current_result_chunk_data_temp.resize(current_result_chunk_cells);
        //std::memset(&current_result_chunk_data[0], 0, sizeof(&current_result_chunk_data[0]));
        //std::cout << "current_result_chunk_cells = " << current_result_chunk_cells << std::endl;
      }
    }
    //Return  1, data read into   local_chunk_data
    //Return  0, end of file (no data left to handle)
    //Return -1: error happen
    //Read data between local_chunk_start_offset and local_chunk_end_offset
    //current_chunk_data.resize(current_chunk_cells);
    //return data_on_disk->ReadData(current_chunk_start_offset, current_chunk_end_offset, current_chunk_data);
    if (virtual_array_flag == 0)
    {
      data->ReadData(current_chunk_ol_start_offset, current_chunk_ol_end_offset, current_chunk_data);
      return 1;
    }
    else
    {
      int n = attributes.size();
      Data<AttrType> *ah;
      unsigned long long hym_count = 1;
      std::vector<AttrType> current_chunk_data_temp;
      current_chunk_data_temp.resize(current_chunk_ol_cells);
      for (int i = 0; i < n; i++)
      {
        ah = attributes[i]->GetDataHandle();
        //ah->ReadDataStripingMem(current_chunk_ol_start_offset, current_chunk_ol_end_offset, &current_chunk_data[0], i, n, hym_count);
        ah->ReadData(current_chunk_ol_start_offset, current_chunk_ol_end_offset, current_chunk_data_temp);
        // printf("Load attribute %s,  i=%d (n=%d): value =  %f, %f\n", ah->GetDatasetName().c_str(), i, n, current_chunk_data_temp[0], current_chunk_data_temp[1]);
        InsertIntoVirtualVector<AttrType, T>(current_chunk_data_temp, current_chunk_data, i);
        //std::cout << current_chunk_data[0] << std::endl;
        //std::cout << current_chunk_data[1] << std::endl;
      }
      current_chunk_data_temp.resize(0);
      return 1;
    }
  }

  inline AttrType hand_optimized_code_conv(T *current_chunk_data, unsigned long long current_chunk_cells, unsigned long long offset_ol, std::vector<unsigned long long> &cell_coordinate_ol, std::vector<unsigned long long> &current_chunk_ol_size)
  {
    T cell_return_value;

    std::vector<unsigned long long> neighborhood_cell;
    neighborhood_cell.resize(2);
    unsigned long long neighborhood_cell_offset = offset_ol;

    if (neighborhood_cell_offset >= current_chunk_cells)
      neighborhood_cell_offset = current_chunk_cells - 1;
    cell_return_value = current_chunk_data[neighborhood_cell_offset] * 1;

    neighborhood_cell[0] = cell_coordinate_ol[0];
    neighborhood_cell[1] = cell_coordinate_ol[1] + 1;
    ROW_MAJOR_ORDER_MACRO(current_chunk_ol_size, current_chunk_ol_size.size(), neighborhood_cell, neighborhood_cell_offset);
    if (neighborhood_cell_offset >= current_chunk_cells)
      neighborhood_cell_offset = current_chunk_cells - 1;
    cell_return_value = cell_return_value + current_chunk_data[offset_ol] * 2;

    neighborhood_cell[0] = cell_coordinate_ol[0] + 1;
    neighborhood_cell[1] = cell_coordinate_ol[1] + 1;
    ROW_MAJOR_ORDER_MACRO(current_chunk_ol_size, current_chunk_ol_size.size(), neighborhood_cell, neighborhood_cell_offset);
    if (neighborhood_cell_offset >= current_chunk_cells)
      neighborhood_cell_offset = current_chunk_cells - 1;
    cell_return_value = cell_return_value + current_chunk_data[offset_ol] * 3;

    neighborhood_cell[0] = cell_coordinate_ol[0] + 1;
    neighborhood_cell[1] = cell_coordinate_ol[1] + 0;
    ROW_MAJOR_ORDER_MACRO(current_chunk_ol_size, current_chunk_ol_size.size(), neighborhood_cell, neighborhood_cell_offset);
    if (neighborhood_cell_offset >= current_chunk_cells)
      neighborhood_cell_offset = current_chunk_cells - 1;
    cell_return_value = cell_return_value + current_chunk_data[offset_ol] * 4;

    //return cell_return_value;
  }

  //Only for test
  void ApplyTest(AttrType (*UDF)(const Stencil<T> &), Array<AttrType> *B)
  {
    LoadNextChunk();
    for (int i = 0; i < 100; i++)
      std::cout << current_chunk_data[i] << std::endl;
    return;
  }

  //Only for test
  void ApplyV(T (*UDF)(const Stencil<T> &), std::vector<T> &result)
  {
    int len_cpp_vector_input = cpp_vec_input.size();
    std::vector<unsigned long long> cell_coordinate(1);
    std::vector<unsigned long long> chunk_size(1);
    chunk_size[0] = len_cpp_vector_input;

    T cell_return_value;
    result.resize(len_cpp_vector_input);
    for (int i = 0; i < len_cpp_vector_input; i++)
    {
      cell_coordinate[0] = i;
      Stencil<T> cell_target(0, &cpp_vec_input[0], cell_coordinate, chunk_size);
      cell_return_value = UDF(cell_target); // Called by python
      result[0] = cell_return_value;
    }
  }

  void UpdateCPPVector(std::vector<T> &v)
  {
    int len = v.size();
    cpp_vec_input.resize(len);
    for (int i = 0; i < len; i++)
      cpp_vec_input[i] = v[i];
  }

//The interface to accept UDF function.
//If user want to use python interface, the UDF has "void *" parameter
//In native c++, the UDF accepts "const Stencil<T> &"
//Main reason: for performance and usability.
#ifdef PY_ARRAYUDF
  void Apply(T (*UDF)(void *), Array<T> *B){
#else
  void Apply(AttrType (*UDF)(const Stencil<T> &), Array<AttrType> *B)
  {
#endif

#ifdef HAND_CODE_2D
      printf("Using hand optimized code ! \n");
#endif

  if (set_apply_direction_flag == 0)
  {
    current_chunk_id = mpi_rank; //Reset to starting point
  }
  else
  {
    current_chunk_id = data_total_chunks - mpi_size + mpi_rank; //Reset to starting point
  }

  //if(mpi_rank == 0 || mpi_rank == mpi_size-1 ){printf("rank : %d , Current chunk id=%d\n",mpi_rank, current_chunk_id); fflush(stdout);}
  //Give the trail run to go and get the maximum overlap size
#ifdef DEBUG
  if (mpi_rank == 0)
  {
    if (trail_run_flag == 1)
    {
      std::cout << "Trailrun start  ... " << std::endl;
    }
    else
    {
      std::cout << "Trailrun is disabled !  " << std::endl;
    }
  }
#endif
  if (trail_run_flag == 1)
  {
    t_start = MPI_Wtime();
    std::vector<T> trail_data(0);
    Stencil<T> trail_cell(data_dims, &trail_data[0]);
#ifdef PY_ARRAYUDF
    UDF(&trail_cell);
#else
      UDF(trail_cell);
#endif
    trail_cell.get_trail_run_result(&data_overlap_size[0]);
    t_end = MPI_Wtime();
    trail_run_time = trail_run_time + t_end - t_start;
    if (mpi_rank == 0)
    {
      std::cout << "Trailrun results: overlap size = : ";
      for (int i = 0; i < data_dims; i++)
      {
        std::cout << ", " << data_overlap_size[i];
      }
      std::cout << std::endl;
    }
  }

  t_start = MPI_Wtime();
  if (B != NULL)
  {
    if (virtual_array_flag == 0)
    {
      if (skip_flag == 1)
      {
        //Todo:  need to consider data_overlap size
        if (vector_type_flag == 0)
        {
          if (output_element_different_type_flag == 0)
          {
            B->CreateStorageSpace(data_dims, skiped_dims_size, skiped_chunk_size, data_overlap_size, data->GetTypeClass(), data_total_chunks);
          }
          else
          {
            //printf("I am here to create new storage space ! \n\n\n");
            B->CreateStorageSpace(data_dims, skiped_dims_size, skiped_chunk_size, data_overlap_size, output_element_type_class, data_total_chunks);
          }
        }
        else //skip_flag == 1 and vector_type_flag == 1
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

          if (output_element_different_type_flag == 0)
          {
            B->CreateStorageSpace(data_dims_t, skiped_dims_size_t, skiped_chunk_size_t, data_overlap_size_t, data->GetTypeClass(), data_total_chunks);
          }
          else
          {
            B->CreateStorageSpace(data_dims_t, skiped_dims_size_t, skiped_chunk_size_t, data_overlap_size_t, output_element_type_class, data_total_chunks);
          }
          //B->SetFilterAmount(filter_amount);
          B->SetOutputVectorIO(output_vector_size, output_vector_flat_direction_index);
        }
      }
      else
      { //skip_flag == 0
        if (vector_type_flag == 0)
        {
          if (output_element_different_type_flag == 0)
          {
            B->CreateStorageSpace(data_dims, data_dims_size, data_chunk_size, data_overlap_size, data->GetTypeClass(), data_total_chunks);
          }
          else
          {
            B->CreateStorageSpace(data_dims, data_dims_size, data_chunk_size, data_overlap_size, output_element_type_class, data_total_chunks);
          }
        }
        else //skip_flag == 0 and vector_type_flag == 1
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
              dims_size_t[k] = data_dims_size[k];
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
              dims_size_t[k] = data_dims_size[k];
              if (k == output_vector_flat_direction_index)
                dims_size_t[k] = dims_size_t[k] * output_vector_size;
              // dims_size_t[k] = data_dims_size[k] * output_vector_size;
              chunk_size_t[k] = data_chunk_size[k];
              data_overlap_size_t[k] = data_overlap_size[k];
            }
          }
          if (output_element_different_type_flag == 0)
          {
            B->CreateStorageSpace(data_dims_t, dims_size_t, chunk_size_t, data_overlap_size_t, data->GetTypeClass(), data_total_chunks);
          }
          else
          {
            B->CreateStorageSpace(data_dims_t, dims_size_t, chunk_size_t, data_overlap_size_t, output_element_type_class, data_total_chunks);
          }
          B->SetOutputVectorIO(output_vector_size, output_vector_flat_direction_index);
        }
      }
      if (data_total_chunks % mpi_size != 0)
      {
        B->DisableCollectivIO(B->GetFileName());
      }
    }
    else
    { //virtual_array_flag == 1
      Data<AttrType> *ah;
      ah = attributes[0]->GetDataHandle();
      B->CreateStorageSpace(data_dims, data_dims_size, data_chunk_size, data_overlap_size, ah->GetTypeClass(), data_total_chunks);
      if (data_total_chunks % mpi_size != 0)
      {
        B->DisableCollectivIO(B->GetFileName());
      }
    }
  }
  else
  { //(B == NULL)
    if (apply_writeback_flag == 0)
    {
      save_result_flag = 0;
    }
    else
    {
      save_result_flag = 1;
    }
  }

  t_end = MPI_Wtime();
  time_create = time_create + t_end - t_start;

#ifdef DEBUG
  if (mpi_rank == 0)
  {
    printf("Start to load the first chunk !\n");
    fflush(stdout);
  }
#endif

  t_start = MPI_Wtime();
  int load_ret = LoadNextChunk();
  t_end = MPI_Wtime();
  time_read = time_read + t_end - t_start;

  MPI_Barrier(MPI_COMM_WORLD);
#ifdef DEBUG
  if (mpi_rank < 1)
  {
    std::cout << "At mpi rank =" << mpi_rank << ", Loading the first chunk ... done ! current_chunk_cells = " << current_chunk_cells << ", next_chunk_id =  " << current_chunk_id << std::endl;
  }
#endif

  MPI_Barrier(MPI_COMM_WORLD);

  std::vector<unsigned long long> cell_coordinate(data_dims, 0), cell_coordinate_ol(data_dims, 0), global_cell_coordinate(data_dims, 0);
  unsigned long long offset_ol;

  //Start real UDF function running
  unsigned long long result_vector_index = 0;
  int is_mirror_value = 0;
  //Apply UDF to eah cell of local chunk, untill no chunk
  while (load_ret == 1)
  {
    processed_chunks_count = processed_chunks_count + 1;
    unsigned long long cell_target_g_location_rm;
    result_vector_index = 0;
    //t_start = MPI_Wtime();
    //Start to process a chunk
    if (set_apply_direction_flag == 0)
    { //in row-major direction
      AttrType cell_return_value;
      //unsigned long long lrm;
      Stencil<T> cell_target(0, &current_chunk_data[0], cell_coordinate_ol, current_chunk_ol_size);
      //#ifdef  ENABLE_OPENMP
      //#pragma omp parallel
      //{
      //#pragma omp for firstprivate(cell_target, cell_coordinate , global_cell_coordinate, offset_ol, cell_return_value, cell_coordinate_ol, is_mirror_value, t_start, t_end)
      for (unsigned long long i = 0; i < current_chunk_cells; i++)
      {
        //Get the coordinate (HDF5 uses row major layout)
        //cell_coordinate = RowMajorOrderReverse(i, current_chunk_size);
        ROW_MAJOR_ORDER_REVERSE_MACRO(i, current_chunk_size, current_chunk_size.size(), cell_coordinate)
        if (skip_flag == 1)
        {
          if (SkipIt(cell_coordinate, skip_size) == 1)
            continue;
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
        //offset_ol = RowMajorOrder(current_chunk_ol_size, cell_coordinate_ol);
        ROW_MAJOR_ORDER_MACRO(current_chunk_ol_size, current_chunk_ol_size.size(), cell_coordinate_ol, offset_ol);
        cell_target.SetLocation(offset_ol, cell_coordinate_ol, cell_coordinate, current_chunk_size, ol_origin_offset, current_chunk_ol_size);
        //Set the global coodinate of the cell as the ID of the cell
        //Disable it for performance.
        //RowMajorOrder(data_dims_size, global_cell_coordinate)
        ROW_MAJOR_ORDER_MACRO(data_dims_size, data_dims_size.size(), global_cell_coordinate, cell_target_g_location_rm)
        cell_target.set_my_g_location_rm(cell_target_g_location_rm);
        is_mirror_value = 0;
        if (mirror_value_flag == 1)
        {
          for (int iii = 0; iii < mirror_values.size(); iii++)
          {
            if (current_chunk_data[offset_ol] == mirror_values[iii])
            {
              is_mirror_value = 1;
              break;
            }
          }
        }

        //Just for the test of performnace
        //if(cell_coordinate[0] >= 16383 || cell_coordinate[1] >= 16383)

        if (is_mirror_value == 0)
        {
#ifdef HAND_CODE_2D
          cell_return_value = hand_optimized_code_conv(&current_chunk_data[0], current_chunk_cells, offset_ol, cell_coordinate_ol, current_chunk_ol_size);
#else
            //Not mirrow value, we need to run the UDF to get results
#ifdef PY_ARRAYUDF
            cell_return_value = UDF(&cell_target); // Called by python
#else

            t_start = MPI_Wtime();

            cell_return_value = UDF(cell_target); // Called by C++

            t_end = MPI_Wtime();
            time_udf = t_end - t_start + time_udf;
#endif
#endif

#ifdef DEBUG
          if (current_chunk_id == 1)
          {
            std::cout << "cell_coordinate    = " << cell_coordinate[0] << ", " << cell_coordinate[1] << std::endl;
            std::cout << "ol_origin_offsest  = " << ol_origin_offset[0] << ", " << ol_origin_offset[1] << std::endl;
            std::cout << "cell_coordinate_ol = " << cell_coordinate_ol[0] << ", " << cell_coordinate_ol[1] << std::endl;
            std::cout << "offset_ol = " << offset_ol << std::endl;
            std::cout << std::endl;
          }
#endif
        }
        else
        {
          //This is a mirrow value, copy it into result directly
          //Using memcpy to avail error in template of T
          memset(&cell_return_value, 0, sizeof(T));
          std::memcpy(&cell_return_value, &current_chunk_data[offset_ol], sizeof(T));
        }

        if (save_result_flag == 1)
        {
          if (skip_flag == 1)
          {
            current_result_chunk_data[result_vector_index] = cell_return_value;
            result_vector_index = result_vector_index + 1;
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
              //current_chunk_data[offset_ol] = cell_return_value; //Replace the orginal data
              std::memcpy(&current_chunk_data[offset_ol], &cell_return_value, sizeof(T));
            }
            //std::cout << "current_result_chunk_data[" <<i << "] = "  << current_result_chunk_data[i] << std::endl;
          }
        }
      } //finish the processing on a single chunk in row-major direction
    }
    else
    { //go to reverse direction
      AttrType cell_return_value;
      Stencil<T> cell_target(0, &current_chunk_data[0], cell_coordinate_ol, current_chunk_ol_size);
      for (long long i = current_chunk_cells - 1; i >= 0; i--)
      { //
        //Get the coordinate (HDF5 uses row major layout)
        cell_coordinate = RowMajorOrderReverse(i, current_chunk_size);
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
        offset_ol = RowMajorOrder(current_chunk_ol_size, cell_coordinate_ol);
        cell_target.SetLocation(offset_ol, cell_coordinate_ol, cell_coordinate, current_chunk_size, ol_origin_offset, current_chunk_ol_size);
        //Set the global coodinate of the cell as the ID of the cell
        //Disable it for performance.
        cell_target.set_my_g_location_rm(RowMajorOrder(data_dims_size, global_cell_coordinate));

        is_mirror_value = 0;
        if (mirror_value_flag == 1)
        {
          for (int iii = 0; iii < mirror_values.size(); iii++)
          {
            if (current_chunk_data[offset_ol] == mirror_values[iii])
            {
              is_mirror_value = 1;
              break;
            }
          }
        }

        if (is_mirror_value == 0)
        {
          //t_start =  MPI_Wtime();
#ifdef PY_ARRAYUDF
          cell_return_value = UDF(&cell_target); // Called by python
#else
            cell_return_value = UDF(cell_target);  // Called by C++
#endif
          //t_end   =  MPI_Wtime();
          //time_udf = t_end-t_start + time_udf;
        }
        else
        {
          //This is a mirro value , copy the result into result directly
          memset(&cell_return_value, 0, sizeof(T));
          std::memcpy(&cell_return_value, &current_chunk_data[offset_ol], sizeof(T));
        }

        if (save_result_flag == 1)
        {
          current_result_chunk_data[i] = cell_return_value; //cell_return =  cell_return.
          if (apply_replace_flag == 1)
          {
            //current_chunk_data[offset_ol] = cell_return_value; //Replace the orginal data
            std::memcpy(&current_chunk_data[offset_ol], &cell_return_value, sizeof(T));
          }
#ifdef DEBUG
          std::cout << "current_result_chunk_data[" << i << "] = " << current_result_chunk_data[i] << std::endl;
#endif
        }
      } //finish the processing on a single chunk
    }   //end of direction
    //////////////////////////////////////
    //end of processing on a single chunk
    /////////////////////////////////////
    //t_end = MPI_Wtime();
    //time_udf = t_end - t_start + time_udf;

    //MPI_Barrier(MPI_COMM_WORLD);
    //#ifdef DEBUG
    if (mpi_rank == 0)
      std::cout << "Process data of chunk [ " << current_chunk_id << "] at rank 0 ... done !" << std::endl;
    //#endif
    //if(single_step_flag == 1)    // goto report_results_mark;
    t_start = MPI_Wtime();
    if (save_result_flag == 1)
    {
      if (apply_writeback_flag == 0)
      {
        //Write result to B
        if (skip_flag == 0)
        {
          B->SaveResult(current_chunk_start_offset, current_chunk_end_offset, current_result_chunk_data);
          //if(mpi_rank == 0 || mpi_rank = (mpi_size - 1)) {printf("Write A: At rank = %d \n", mpi_rank); PrintVector("Start: ", current_chunk_start_offset); PrintVector("End: ", current_chunk_end_offset); PrintVector("(Right after write at apply) written data: ", current_result_chunk_data); fflush(stdout);}
        }
        else
        {
          B->SaveResult(current_result_chunk_start_offset, current_result_chunk_end_offset, current_result_chunk_data);
        }
      }
      else
      {
        //Write result to A
        // i.e., overwrite the original data
        if (skip_flag == 0)
        {
          //SaveResult(current_chunk_start_offset, current_chunk_end_offset, current_result_chunk_data);
          data->WriteDataVoid(current_chunk_start_offset, current_chunk_end_offset, &(current_result_chunk_data[0]));
          //data->WriteDataVoid(current_result_chunk_start_offset, current_result_chunk_end_offset, &(current_result_chunk_data[0]));
          //if(mpi_rank == 2) {printf("Write B: At rank = %d \n", mpi_rank); PrintVector("Start: ", current_chunk_start_offset); PrintVector("End: ", current_chunk_end_offset); PrintVector("(Right after write at apply) written data: ", current_result_chunk_data); fflush(stdout);}
        }
        else
        {
          //SaveResult(current_result_chunk_start_offset, current_result_chunk_end_offset, current_result_chunk_data);
          data->WriteDataVoid(current_result_chunk_start_offset, current_result_chunk_end_offset, &(current_result_chunk_data[0]));
        }
      }
    }
    t_end = MPI_Wtime();
    time_write = time_write + t_end - t_start;
    t_start = MPI_Wtime();
    load_ret = LoadNextChunk();
    t_end = MPI_Wtime();
    time_read = time_read + t_end - t_start;
  } //finish all chunks

  if (set_apply_direction_flag == 0)
  {
    current_chunk_id = mpi_rank; //Reset to starting point
  }
  else
  {
    current_chunk_id = data_total_chunks - mpi_size + mpi_rank; //Reset to starting point
  }
  //Sync data across the nodes, mostly used for GA
  //if(save_result_flag == 1){
  // t_start =  MPI_Wtime();
  //DataSyncCrossNode();
  // t_end   =  MPI_Wtime();  time_sync = time_sync + t_end - t_start;
  //}
  report_results_mark :
#ifdef REPORT_TIMING_RESULT
      ReportTime();
#endif
  return;
}

int
GetRank()
{
  return data_dims;
}

std::vector<unsigned long long> GetDims()
{
  return data_dims_size;
}

void ReportTime()
{
  double time_read_max, time_write_max, time_udf_max, time_address_cal_max, row_major_order_cal_max, pre_row_major_order_cal_max, data_access_time_max, time_create_max, time_sync_max, time_of_update_host_zone_max, time_nonvolatile_max;
  double time_read_min, time_write_min, time_udf_min, time_address_cal_min, row_major_order_cal_min, pre_row_major_order_cal_min, data_access_time_min, time_create_min, time_sync_min, time_of_update_host_zone_min, time_nonvolatile_min;
  double time_read_sum, time_write_sum, time_udf_sum, time_address_cal_sum, row_major_order_cal_sum, pre_row_major_order_cal_sum, data_access_time_sum, time_create_sum, time_sync_sum, time_of_update_host_zone_sum, time_nonvolatile_sum;
  double trail_run_time_max, trail_run_time_min, trail_run_time_sum;

  MPI_Allreduce(&time_read, &time_read_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(&time_write, &time_write_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(&time_udf, &time_udf_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(&time_create, &time_create_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(&time_sync, &time_sync_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(&time_of_update_host_zone, &time_of_update_host_zone_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

  MPI_Allreduce(&time_read, &time_read_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
  MPI_Allreduce(&time_write, &time_write_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
  MPI_Allreduce(&time_udf, &time_udf_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
  MPI_Allreduce(&time_create, &time_create_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
  MPI_Allreduce(&time_sync, &time_sync_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
  MPI_Allreduce(&time_of_update_host_zone, &time_of_update_host_zone_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);

  MPI_Allreduce(&time_read, &time_read_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  MPI_Allreduce(&time_write, &time_write_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  MPI_Allreduce(&time_udf, &time_udf_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  MPI_Allreduce(&time_create, &time_create_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  MPI_Allreduce(&time_sync, &time_sync_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  MPI_Allreduce(&time_of_update_host_zone, &time_of_update_host_zone_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  MPI_Allreduce(&time_nonvolatile, &time_nonvolatile_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(&time_nonvolatile, &time_nonvolatile_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
  MPI_Allreduce(&time_nonvolatile, &time_nonvolatile_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  MPI_Allreduce(&time_address_cal, &time_address_cal_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(&time_address_cal, &time_address_cal_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
  MPI_Allreduce(&time_address_cal, &time_address_cal_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  MPI_Allreduce(&row_major_order_cal, &row_major_order_cal_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(&row_major_order_cal, &row_major_order_cal_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
  MPI_Allreduce(&row_major_order_cal, &row_major_order_cal_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  MPI_Allreduce(&pre_row_major_order_cal, &pre_row_major_order_cal_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(&pre_row_major_order_cal, &pre_row_major_order_cal_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
  MPI_Allreduce(&pre_row_major_order_cal, &pre_row_major_order_cal_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  MPI_Allreduce(&data_access_time, &data_access_time_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(&data_access_time, &data_access_time_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
  MPI_Allreduce(&data_access_time, &data_access_time_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  MPI_Allreduce(&trail_run_time, &trail_run_time_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(&trail_run_time, &trail_run_time_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
  MPI_Allreduce(&trail_run_time, &trail_run_time_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  if (mpi_rank == 0)
  {
    std::cout << "Results on Rank 0" << std::endl;
    std::cout << "Trailrun  time (s)  :  = " << trail_run_time << std::endl;
    std::cout << "Create    time (s)  :  = " << time_create << std::endl;
    std::cout << "Ghost(u)  time (s)  :  = " << time_of_update_host_zone << std::endl;
    std::cout << "Read      time (s)  :  = " << time_read << std::endl;
    std::cout << "UDF       time (s)  :  = " << time_udf << std::endl;
    //std::cout << "->AdrCal  time (s)  :  = " << time_address_cal << std::endl;
    //std::cout << "------->Pre-RMO  time (s) :  = " << pre_row_major_order_cal << std::endl;
    //std::cout << "------->    RMO  time (s) :  = " << row_major_order_cal << std::endl;
    //std::cout << "->DA      time (s)  :  = " << data_access_time << std::endl;
    std::cout << "Write     time (s)  :  = " << time_write << std::endl;
    std::cout << "Sync      time (s)  :  = " << time_sync << std::endl;
    std::cout << "Nonvol..  time (s)  :  = " << time_nonvolatile << std::endl;
    fflush(stdout);
  }

  if (mpi_rank == 0)
  {
    std::cout << "Results of All " << std::endl;
    std::cout << "Trailrun  time (s) : max = " << trail_run_time_max << ", min = " << trail_run_time_min << ", ave = " << trail_run_time_sum / mpi_size << std::endl;
    std::cout << "Create    time (s) : max = " << time_create_max << ", min = " << time_create_min << ", ave = " << time_create_sum / mpi_size << std::endl;
    std::cout << "Ghost(u)  time (s) : max = " << time_of_update_host_zone_max << ", min = " << time_of_update_host_zone_min << ", ave = " << time_of_update_host_zone_sum / mpi_size << std::endl;
    std::cout << "Read      time (s) : max = " << time_read_max << ", min = " << time_read_min << ", ave = " << time_read_sum / mpi_size << std::endl;
    std::cout << "UDF       time (s) : max = " << time_udf_max << ", min = " << time_udf_min << ", ave = " << time_udf_sum / mpi_size << std::endl;
    //std::cout << "->AdrCal  time (s) : max = "  << time_address_cal_max        << ", min = "  << time_address_cal_min        << ", ave = " << time_address_cal_sum/mpi_size << std::endl;
    //std::cout << "------->Pre-RMO time (s) : max = " << pre_row_major_order_cal_max << ", min = "  << pre_row_major_order_cal_min << ", ave = " << pre_row_major_order_cal_sum/mpi_size << std::endl;
    //std::cout << "------->    RMO time (s) : max = " << row_major_order_cal_max     << ", min = "  << row_major_order_cal_min     << ", ave = " << row_major_order_cal_sum/mpi_size << std::endl;
    //std::cout << "->DA      time (s) : max = " << data_access_time_max     << ", min = "  << data_access_time_min     << ", ave = " << data_access_time_sum/mpi_size << std::endl;
    std::cout << "Write     time (s) : max = " << time_write_max << ", min = " << time_write_min << ", ave = " << time_write_sum / mpi_size << std::endl;
    std::cout << "Sync      time (s) : max = " << time_sync_max << ", min = " << time_sync_min << ", ave = " << time_sync_sum / mpi_size << std::endl;
    std::cout << "Nonvol..  time (s) : max = " << time_nonvolatile_max << ", min = " << time_nonvolatile_min << ", ave = " << time_nonvolatile_sum / mpi_size << std::endl;
    fflush(stdout);
  }
}

int CreateStorageSpace(int data_dims_t, std::vector<unsigned long long> data_dims_size_t, std::vector<int> data_chunk_size_t, std::vector<int> data_overlap_size_t, int type_class_t, unsigned long long data_total_chunks_p_t)
{
  //
  //update the information for Array itself
  //
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

  data_dims = data_dims_t;
  data_dims_size = data_dims_size_t;
  data_chunk_size = data_chunk_size_t;
  data_overlap_size = data_overlap_size_t;
  data_total_chunks = data_total_chunks_p_t;

  //Init other attributes of B for applying UDF on B
  current_chunk_start_offset.resize(data_dims);
  current_chunk_end_offset.resize(data_dims);
  current_chunk_size.resize(data_dims);

  current_result_chunk_start_offset.resize(data_dims);
  current_result_chunk_end_offset.resize(data_dims);

  current_chunk_ol_start_offset.resize(data_dims);
  current_chunk_ol_end_offset.resize(data_dims);
  current_chunk_ol_size.resize(data_dims);

  data_chunked_dims_size.resize(data_dims);
  ol_origin_offset.resize(data_dims);

  data_total_chunks = 1;
  //#ifdef DEBUG
  if (mpi_rank == 0)
  {
    std::cout << "In Create file: Data rank = " << data_dims << ", at mpi_rank = " << mpi_rank << std::endl;
    std::cout << "File    : " << data->GetFileName() << std::endl;
    std::cout << "Group   : " << data->GetGroupName() << std::endl;
    std::cout << "Dataset : " << data->GetDatasetName() << std::endl;

    std::cout << "data size  = ";
    for (int i = 0; i < data_dims; i++)
      std::cout << ", " << data_dims_size[i];
    std::cout << std::endl;

    std::cout << "chunk size  = ";
    for (int i = 0; i < data_dims; i++)
      std::cout << ", " << data_chunk_size[i];
    std::cout << std::endl;

    std::cout << "overlap size = ";
    for (int i = 0; i < data_dims; i++)
      std::cout << ", " << data_overlap_size[i];
    std::cout << std::endl;
  }
  //#endif
  for (int i = 0; i < data_dims; i++)
  {
    if (data_dims_size[i] % data_chunk_size[i] == 0)
    {
      data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i];
    }
    else
    {
      data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i] + 1;
    }
    data_total_chunks = data_total_chunks * data_chunked_dims_size[i];
  }

  if (data_total_chunks % mpi_size != 0)
  {
    data->DisableCollectivIO();
  }

  if (set_apply_direction_flag == 0)
  {
    current_chunk_id = mpi_rank; //Reset to starting point
  }
  else
  {
    current_chunk_id = data_total_chunks - mpi_size + mpi_rank; //Reset to starting point
  }

  //Todo: double check the required sizes are equal to the existing one
  //if(data_created_flag == 1){
  //  return 0;
  //}
  //data_created_flag = 1;
  if (data->GetCreateStorageSpaceFlag() == 1 && cache_flag == AU_CACHED)
  {
    data->ClearStorageSpace();
  }

  /*
    if(data != NULL){
      std::string fn_t = data->GetFileName();
      std::string gn_t = data->GetGroupName();
      std::string dn_t = data->GetDatasetName();
      delete data;  data=NULL;
      data   =  new Data<T>(fn_t, gn_t, dn_t, AU_COMPUTED, AU_HDF5);
      if(cache_flag == AU_CACHED) data->EnableCache();
    }else{
      printf("Error in create storage. B= NULL");
      exit(-1);
      }*/
  //end for the update
  return data->CreateStorageSpace(data_dims, data_dims_size, type_class_t, data_overlap_size, data_chunk_size);
}

void DisableCollectivIO(std::string fn)
{
  data->DisableCollectivIO();
  if (mpi_rank == 0)
  {
    std::cout << "Disable Collective IO for " << fn << std::endl;
  }
}

int SaveResult(std::vector<unsigned long long> current_chunk_start_offset, std::vector<unsigned long long> current_chunk_end_offset, std::vector<T> current_result_chunk_data_p)
{
  return data->WriteData(current_chunk_start_offset, current_chunk_end_offset, current_result_chunk_data_p);
}

std::string GetFileName()
{
  return data->GetFileName();
}

std::string GetDatasetName()
{
  return data->GetDatasetName();
}

std::string GetGroupName()
{
  return data->GetGroupName();
}

CacheFlag ToBeCached()
{
  return cache_flag;
}

void DataSyncCrossNode()
{
  if (virtual_array_flag != 0)
    data->DataSyncCrossNode();
}

void SetApplyStripSize(std::vector<int> skip_size_p)
{
  if (skip_size_p.size() != data_dims)
  {
    printf("Skip vector's size must be equal the number of dimensions of the array ! \n");
    exit(0);
  }
#ifdef DEBUG
  PrintVector<int>("Set apply's striping size: ", skip_size_p);
#endif
  skiped_dims_size.resize(data_dims);
  skip_size.resize(data_dims);
  skiped_chunks.resize(data_dims);
  skiped_chunk_size.resize(data_dims);
  //skiped_chunks_per_orig_chunk.resize(data_dims)
  for (int i = 0; i < data_dims; i++)
  {
    if (data_dims_size[i] % skip_size_p[i] != 0 || data_chunk_size[i] % skip_size_p[i] != 0)
    {
      printf("Striping size must be aligned with size of both array and chunk ! \n");
      exit(0);
    }
    skiped_dims_size[i] = data_dims_size[i] / skip_size_p[i];
    skiped_chunk_size[i] = data_chunk_size[i] / skip_size_p[i];
    if (skiped_dims_size[i] % skiped_chunk_size[i] != 0)
    {
      skiped_chunks[i] = skiped_dims_size[i] / skiped_chunk_size[i] + 1;
    }
    else
    {
      skiped_chunks[i] = skiped_dims_size[i] / skiped_chunk_size[i];
    }
    //skiped_chunks_per_orig_chunk[i] = data_chunk_size[i]/skip_size_p[i];
    skip_size[i] = skip_size_p[i];
  }

#ifdef DEBUG
  if (mpi_rank == 0)
  {
    PrintVector<unsigned long long>("After setting skip_size", skip_size);
    PrintVector<unsigned long long>("After setting skiped_dims_size ", skiped_dims_size);
    //PrintVector<int>("After setting skiped_chunk_size ", skiped_chunk_size);
  }
#endif

  skip_flag = 1;
}

// 0: (by default) it is row-major on each chunk
// 1:  the reverse on each chunk
// 2: column major (to do)
// 3: the revese on column major
void SetApplyDirection(int direction_t)
{
  if (direction_t != 0)
  {
    if (direction_t == 1)
    {
      set_apply_direction_flag = 1;
      apply_direction = 1;
    }
    else
    {
      printf("Not supported direction (Todo) !\n");
    }
  }
  else
  {
    set_apply_direction_flag = 0;
    apply_direction = 0;
  }
}

//Replaced by "SetOutputVector"
void SetApplyFilters(int filters)
{
  vector_type_flag = 1;
  filter_amount = filters;
#ifdef DEBUG
  printf("Set apply's filters size: %d\n", filter_amount);
#endif
}

//This function tells ArrayUDF that the output of each UDF is a
// vector and also sets the size of output vector by a single UDF
// Now, we assume a single UDF has 1D vector as output
// It is a generic version of "SetApplyFilters"
//
// The "direction" specify how to flat output vector
// For example, when running a UDF on a cell (i, j) of a 2D array
//  flat_direction_index = 0: store result vector in (i : i + vsize, j)
//  flat_direction_index = 1: store result vector in (i,             j: j + vsize)
//  flat_direction_index = 2: store result vector as new dimension of a 3D array (i, j, 0:vsize)
//
//Todo: we can infer "vsize" in futher version
//      we can also support multiple dimensional vector
void SetOutputVector(int vsize, int flat_direction_index)
{
  vector_type_flag = 1;
  output_vector_size = vsize;
  output_vector_flat_direction_index = flat_direction_index;
#ifdef DEBUG
  printf("Set output vector size : %d, %d \n", vsize, direction);
#endif
}

//Not finished yet
void SetApplyPadding(T padding_value_p, std::vector<int> padding_size_p)
{
  padding_value_set_flag = 1;
  padding_value = padding_value_p;
  padding_size = padding_size_p;
}

//can't return reference
//https://stackoverflow.com/questions/752658/is-the-practice-of-returning-a-c-reference-variable-evil
inline T operator()(const int idx) const
{
  if (virtual_array_flag == 0)
  {
    std::vector<unsigned long long> start;
    std::vector<unsigned long long> end;
    std::vector<T> data_v;
    start.resize(1);
    end.resize(1);
    data_v.resize(1);
    start[0] = idx;
    end[0] = idx;
    data->ReadData(start, end, data_v);
    //std::cout << "data_v "  << std::endl;
    return data_v[0];
  }
  else
  {
    int n = attributes.size();
    std::vector<AttrType> temp;
    temp.resize(n);
    T result;
    for (int i = 0; i < n; i++)
    {
      temp[i] = attributes[i]->operator()(idx);
#ifdef DEBUG
      std::cout << "Attribute " << i << ": " << temp[i] << std::endl;
#endif
    }
    memset(&result, 0, sizeof(T));
    std::memcpy(&result, &temp[0], sizeof(T));
    //std::cout << "Size of T :  " << sizeof(T) << ", Size of AttrType :  " << sizeof(AttrType) << std::endl;
    return result;
  }
}

inline T operator()(int idx, int idy) const
{
  if (virtual_array_flag == 0)
  {
    std::vector<unsigned long long> start;
    std::vector<unsigned long long> end;
    std::vector<T> data_v;
    start.resize(2);
    end.resize(2);
    data_v.resize(1);
    start[0] = idx;
    start[1] = idy;
    end[0] = idx;
    end[1] = idy;
    data->ReadData(start, end, data_v);
    return data_v[0];
  }
  else
  {
    int n = attributes.size();
    std::vector<AttrType> temp;
    temp.resize(n);
    T result;
    for (int i = 0; i < n; i++)
    {
      temp[i] = attributes[i]->operator()(idx, idy);
    }
    memset(&result, 0, sizeof(T));
    std::memcpy(&result, &temp[0], sizeof(T));
    return result;
  }
}

inline T operator()(int idx, int idy, int idz) const
{
  if (virtual_array_flag == 0)
  {
    if (preload_flag == 0)
    {
      std::vector<unsigned long long> start(3, 0);
      std::vector<unsigned long long> end(3, 0);
      std::vector<T> data_v(1, 0);
      //start.resize(3);  end.resize(3); data_v.resize(1);
      start[0] = idx;
      start[1] = idy;
      start[2] = idz;
      end[0] = idx;
      end[1] = idy;
      end[2] = idz;
      data->ReadData(start, end, data_v);
      return data_v[0];
    }
    else
    { //preload_flag == q
      unsigned long long offset;
      std::vector<unsigned long long> coordinate(3, 0);
      coordinate[0] = idx;
      coordinate[1] = idy;
      coordinate[2] = idz;
      ROW_MAJOR_ORDER_MACRO(data_dims_size, data_dims_size.size(), coordinate, offset)
      //printf(" idxyz = (%d, %d, %d), offset=%lld \n", idx, idy, idz, offset);
      return data->ReadPreloadPointAtOffset(offset);
    }
  }
  else
  {
    int n = attributes.size();
    std::vector<AttrType> temp;
    temp.resize(n);
    T result;
    for (int i = 0; i < n; i++)
    {
      temp[i] = attributes[i]->operator()(idx, idy, idz);
    }
    memset(&result, 0, sizeof(T));
    std::memcpy(&result, &temp[0], sizeof(T));
    return result;
  }
}

inline T operator()(int idx, int idy, int idz, int ida) const
{
  if (virtual_array_flag == 0)
  {
    std::vector<unsigned long long> start;
    std::vector<unsigned long long> end;
    std::vector<T> data_v;
    start.resize(4);
    end.resize(4);
    data_v.resize(1);
    start[0] = idx;
    start[1] = idy;
    start[2] = idz;
    start[3] = ida;
    end[0] = idx;
    end[1] = idy;
    end[2] = idz;
    end[3] = ida;
    data->ReadData(start, end, data_v);
    return data_v[0];
  }
  else
  {
    int n = attributes.size();
    std::vector<AttrType> temp;
    temp.resize(n);
    T result;
    for (int i = 0; i < n; i++)
    {
      temp[i] = attributes[i]->operator()(idx, idy, idz, ida);
    }
    memset(&result, 0, sizeof(T));
    std::memcpy(&result, &temp[0], sizeof(T));
    return result;
  }
}

inline void set(const int idx, T value) const
{
  std::vector<unsigned long long> start;
  std::vector<unsigned long long> end;
  std::vector<T> data_v;
  data_v.resize(1);
  data_v[0] = value;
  start.resize(1);
  end.resize(1);
  start[0] = idx;
  end[0] = idx;
  data->WriteData(start, end, data_v);
}

inline void set(const int idx, const int idy, T value) const
{
  std::vector<unsigned long long> start;
  std::vector<unsigned long long> end;
  std::vector<T> data_v;
  data_v.resize(1);
  data_v[0] = value;
  start.resize(2);
  end.resize(2);
  start[0] = idx;
  start[1] = idy;
  end[0] = idx;
  end[1] = idy;
  data->WriteData(start, end, data_v);
}

inline void set(const int idx, const int idy, const int idz, T value) const
{
  std::vector<unsigned long long> start;
  std::vector<unsigned long long> end;
  std::vector<T> data_v;
  data_v.resize(1);
  data_v[0] = value;
  start.resize(3);
  end.resize(3);
  start[0] = idx;
  start[1] = idy;
  start[2] = idz;
  end[0] = idx;
  end[1] = idy;
  end[2] = idz;
  data->WriteData(start, end, data_v);
}

inline void set(const int idx, const int idy, const int idz, const int ida, T value) const
{
  std::vector<unsigned long long> start;
  std::vector<unsigned long long> end;
  std::vector<T> data_v;
  data_v.resize(1);
  data_v[0] = value;
  start.resize(4);
  end.resize(4);
  start[0] = idx;
  start[1] = idy;
  start[2] = idz;
  start[3] = ida;
  end[0] = idx;
  end[1] = idy;
  end[2] = idz;
  end[3] = ida;
  data->WriteData(start, end, data_v);
}

//void appendAttributes(Array<AttrType> *a){
//  attributes.push_back(a);
//}

//template<typename... Args>
//void SetAttributes(Array<AttrType> *first, Args... args){
//  appendAttributes(first);
//  return SetAttributes(args...);
//}

void SetAttributes(Array<AttrType> *a1, Array<AttrType> *a2, Array<AttrType> *a3)
{
  //Save the attributes
  attributes.push_back(a1);
  attributes.push_back(a2);
  attributes.push_back(a3);
  //Todo: check the dimenson of a1 = a2 = a3

  //Extract some information from a1 ... a2.
  data_chunk_size = a1->GetChunkSize();
  data_overlap_size = a1->GetOverlapSize();
  data_dims_size = a1->GetDimSize();
  data_dims = data_dims_size.size();

  current_chunk_start_offset.resize(data_dims);
  current_chunk_end_offset.resize(data_dims);
  current_chunk_size.resize(data_dims);

  current_result_chunk_start_offset.resize(data_dims);
  current_result_chunk_end_offset.resize(data_dims);

  current_chunk_ol_start_offset.resize(data_dims);
  current_chunk_ol_end_offset.resize(data_dims);
  current_chunk_ol_size.resize(data_dims);

  data_chunked_dims_size.resize(data_dims);
  ol_origin_offset.resize(data_dims);

  data_total_chunks = 1;

  for (int i = 0; i < data_dims; i++)
  {
    if (data_dims_size[i] % data_chunk_size[i] == 0)
    {
      data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i];
    }
    else
    {
      data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i] + 1;
    }
    data_total_chunks = data_total_chunks * data_chunked_dims_size[i];
  }
}

void SetAttributes(Array<AttrType> *a1, Array<AttrType> *a2, Array<AttrType> *a3, Array<AttrType> *a4, Array<AttrType> *a5)
{
  //Save the attributes
  attributes.push_back(a1);
  attributes.push_back(a2);
  attributes.push_back(a3);
  attributes.push_back(a4);
  attributes.push_back(a5);

  //Todo: check the dimenson of a1 = a2 = a3

  //Extract some information from a1 ... a2.
  data_chunk_size = a1->GetChunkSize();
  data_overlap_size = a1->GetOverlapSize();
  data_dims_size = a1->GetDimSize();
  data_dims = data_dims_size.size();

  current_chunk_start_offset.resize(data_dims);
  current_chunk_end_offset.resize(data_dims);
  current_chunk_size.resize(data_dims);

  current_result_chunk_start_offset.resize(data_dims);
  current_result_chunk_end_offset.resize(data_dims);

  current_chunk_ol_start_offset.resize(data_dims);
  current_chunk_ol_end_offset.resize(data_dims);
  current_chunk_ol_size.resize(data_dims);

  data_chunked_dims_size.resize(data_dims);
  ol_origin_offset.resize(data_dims);

  data_total_chunks = 1;

  for (int i = 0; i < data_dims; i++)
  {
    if (data_dims_size[i] % data_chunk_size[i] == 0)
    {
      data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i];
    }
    else
    {
      data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i] + 1;
    }
    data_total_chunks = data_total_chunks * data_chunked_dims_size[i];
  }
}

void SetAttributes(Array<AttrType> *a1, Array<AttrType> *a2, Array<AttrType> *a3, Array<AttrType> *a4, Array<AttrType> *a5, Array<AttrType> *a6)
{
  //Save the attributes
  attributes.push_back(a1);
  attributes.push_back(a2);
  attributes.push_back(a3);
  attributes.push_back(a4);
  attributes.push_back(a5);
  attributes.push_back(a6);

  //Todo: check the dimenson of a1 = a2 = a3

  //Extract some information from a1 ... a2.
  data_chunk_size = a1->GetChunkSize();
  data_overlap_size = a1->GetOverlapSize();
  data_dims_size = a1->GetDimSize();
  data_dims = data_dims_size.size();

  current_chunk_start_offset.resize(data_dims);
  current_chunk_end_offset.resize(data_dims);
  current_chunk_size.resize(data_dims);

  current_result_chunk_start_offset.resize(data_dims);
  current_result_chunk_end_offset.resize(data_dims);

  current_chunk_ol_start_offset.resize(data_dims);
  current_chunk_ol_end_offset.resize(data_dims);
  current_chunk_ol_size.resize(data_dims);

  data_chunked_dims_size.resize(data_dims);
  ol_origin_offset.resize(data_dims);

  data_total_chunks = 1;

  for (int i = 0; i < data_dims; i++)
  {
    if (data_dims_size[i] % data_chunk_size[i] == 0)
    {
      data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i];
    }
    else
    {
      data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i] + 1;
    }
    data_total_chunks = data_total_chunks * data_chunked_dims_size[i];
  }
}

void SetAttributes(Array<AttrType> *a1, Array<AttrType> *a2, Array<AttrType> *a3, Array<AttrType> *a4, Array<AttrType> *a5, Array<AttrType> *a6, Array<AttrType> *a7, Array<AttrType> *a8, Array<AttrType> *a9, Array<AttrType> *a10, Array<AttrType> *a11, Array<AttrType> *a12)
{
  //Save the attributes
  attributes.push_back(a1);
  attributes.push_back(a2);
  attributes.push_back(a3);
  attributes.push_back(a4);
  attributes.push_back(a5);
  attributes.push_back(a6);
  attributes.push_back(a7);
  attributes.push_back(a8);
  attributes.push_back(a9);
  attributes.push_back(a10);
  attributes.push_back(a11);
  attributes.push_back(a12);

  //Todo: check the dimenson of a1 = a2 = a3

  //Extract some information from a1 ... a2.
  data_chunk_size = a1->GetChunkSize();
  data_overlap_size = a1->GetOverlapSize();
  data_dims_size = a1->GetDimSize();
  data_dims = data_dims_size.size();

  current_chunk_start_offset.resize(data_dims);
  current_chunk_end_offset.resize(data_dims);
  current_chunk_size.resize(data_dims);

  current_result_chunk_start_offset.resize(data_dims);
  current_result_chunk_end_offset.resize(data_dims);

  current_chunk_ol_start_offset.resize(data_dims);
  current_chunk_ol_end_offset.resize(data_dims);
  current_chunk_ol_size.resize(data_dims);

  data_chunked_dims_size.resize(data_dims);
  ol_origin_offset.resize(data_dims);

  data_total_chunks = 1;

  for (int i = 0; i < data_dims; i++)
  {
    if (data_dims_size[i] % data_chunk_size[i] == 0)
    {
      data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i];
    }
    else
    {
      data_chunked_dims_size[i] = data_dims_size[i] / data_chunk_size[i] + 1;
    }
    data_total_chunks = data_total_chunks * data_chunked_dims_size[i];
  }
}

std::vector<int> GetChunkSize()
{
  return data_chunk_size;
}

std::vector<int> GetOverlapSize()
{
  return data_overlap_size;
}

std::vector<unsigned long long> GetDimSize()
{
  return data_dims_size;
}

Data<T> *GetDataHandle()
{
  return data;
}

void SetFilterAmount(int p)
{
  data->SetFilterAmount(p);
}

void SetOutputVectorIO(int vsize, int flat_direction_index)
{
  data->SetOutputVector(vsize, flat_direction_index);
}

int GetCreatedFlag()
{
  return data_created_flag;
}

void SetMirrorValue(std::vector<T> mvalue)
{
  mirror_value_flag = 1;
  mirror_values = mvalue;
}

void EnableApplyReplace()
{
  if (mpi_rank == 0)
    printf("Enable replace flag ! \n ");
  apply_replace_flag = 1;
}

void DisableApplyReplace()
{
  if (mpi_rank == 0)
    printf("Enable replace flag ! \n ");
  apply_replace_flag = 0;
}

void EnableApplyWriteback()
{
  if (mpi_rank == 0)
    printf("Enable writeback & replace flag ! \n");
  //apply_replace_flag = 1;  //We need to replace the data ;; Why?
  apply_writeback_flag = 1;
}

void EnableAutoRowMajorChunk()
{
  row_major_chunk_flag = 1;
}

void DisableApplyWriteback()
{
  if (mpi_rank == 0)
    printf("Disable writeback & replace flag ! \n");
  //apply_replace_flag = 0;  //We need to replace the data
  apply_writeback_flag = 0;
}

int GetVSHandle()
{
  return data->GetVSHandle();
}

template <typename targetT>
Array<targetT> *Regrid()
{
  Array<targetT> *Y = new Array<targetT>(AU_NV, AU_HDF5, data->GetFileName(), data->GetGroupName(), data->GetDatasetName(), data_chunk_size, data_overlap_size, data->GetVSHandle());
  //int hd=data->GetVSHandle();
  //int hd2=Y->GetVSHandle();
  //printf("Ar regrid: old handle = %d, new handle = %d \n", hd, hd2);
  return Y;
}
}
;

#endif
