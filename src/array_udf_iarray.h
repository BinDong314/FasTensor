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

#ifndef ARRAY_UDF_IARRAY
#define ARRAY_UDF_IARRAY

#include "iarray.h"

extern double time_of_update_host_zone;

template <class T>
class IArrayData
{
private:
  int type;
  int iarray_handle = -1;

  int rank;
  std::vector<int64_t> dims;
  std::vector<int> chunk;
  std::vector<int> overlap;
  std::vector<int> lo, hi, ld;

  std::string fn;
  std::string gn;
  std::string dn;
  H5Data<T> *nvs_f_handle = NULL; //Can be void * to support more IO drivers
  //For write: create the file after the UDF is executed
  int vector_type_flag = 0;
  int filter_amount;
  int mpi_rank;
  int mpi_size;
  int ghost_flag = 0;
  AUType data_type_class_t;

public:
  IArrayData(std::string fn_p, std::string gn_p, std::string dn_p)
  {
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    fn = fn_p;
    gn = gn_p;
    dn = dn_p;
    if (is_vector_type<T>())
      vector_type_flag = 1;
  }

  IArrayData()
  {
    if (is_vector_type<T>())
      vector_type_flag = 1;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  }

  ~IArrayData()
  {
    if (iarray_handle >= 0)
    {
      iArray_delete(iarray_handle);
      iarray_handle = -1;
    }
    if (nvs_f_handle != NULL)
    {
      delete nvs_f_handle;
      nvs_f_handle = NULL;
    }
  }

  std::vector<unsigned long long> GetDimSize()
  {
    std::vector<unsigned long long> dims_t;
    dims_t.resize(rank);
    for (int i = 0; i < rank; i++)
      dims_t[i] = (unsigned long long)dims[i];
    return dims_t;
  }

  int GetRank()
  {
    return rank;
  }

  void Destroy()
  {
    if (iarray_handle >= 0)
    {
      iArray_delete(iarray_handle);
      iarray_handle = -1;
    }
  }

  int Create(int data_dims, std::vector<unsigned long long> &data_dims_size, AUType data_type_class, std::vector<int> data_overlap_size, std::vector<int> data_chunk_size_t)
  {
#ifdef DEBUG
    if (mpi_rank == 0)
    {
      printf("Debug: create iArray, in udf_ga. ghost size = (%d) \n", data_overlap_size[0]);
      fflush(stdout);
    }
#endif
    rank = data_dims;
    type = data_type_class;
    dims.resize(rank);
    lo.resize(rank);
    hi.resize(rank);
    ld.resize(rank - 1);
    overlap.resize(rank);
    chunk.resize(rank);
    data_type_class_t = data_type_class;
    for (int i = 0; i < rank; i++)
    {
      dims[i] = (int)data_dims_size[i];
      overlap[i] = (int)data_overlap_size[i];
      chunk[i] = (int)data_chunk_size_t[i];
      if (data_overlap_size[i] != 0)
        ghost_flag = 1;
    }

    switch (data_type_class)
    {
    case AU_INTEGER:
      iarray_handle = iArray_create(data_dims_size, overlap, chunk, AU_INTEGER);
      break;
    case AU_FLOAT:
      //printf("Create a float array !\n");
      iarray_handle = iArray_create(data_dims_size, overlap, chunk, AU_FLOAT);
      break;
    default:
      std::cout << "Unsupported datatype in array_udf_ga.h ! " << std::endl;
      exit(-1);
      break;
    }
    return 0;
  }

  T ReadPreloadPointAtOffset(unsigned long long offset)
  {
    T data_at_offset;
    iArray_read_offset_direct(iarray_handle, offset, &data_at_offset, sizeof(T));
    return data_at_offset;
  }

  int ReadData(std::vector<unsigned long long> start, std::vector<unsigned long long> end, std::vector<T> &data)
  {
    int has_ghost_zone = 0;
    for (int i = 0; i < overlap.size(); i++)
    {
      if (overlap[0] == 1)
      {
        has_ghost_zone = 1;
        break;
      }
    }
    double t_start_time = MPI_Wtime();
    if (has_ghost_zone == 1)
      UpdateGhost();
    double t_end_time = MPI_Wtime();
    time_of_update_host_zone = time_of_update_host_zone + t_end_time - t_start_time;

    std::vector<unsigned long long> global_count(start.size()); // global_count.resize(start.size());
    for (int i = 0; i < start.size(); i++)
      global_count[i] = end[i] - start[i] + 1;

    iArray_read_local(iarray_handle, start, global_count, &data[0]);

    //if(mpi_rank == 2 ) { PrintVector("Start: ", start); PrintVector("End: ", end); printf("At rank %d, data vector's size = %d ,,, ", mpi_rank, data.size()); PrintVector(" Rank's read data: ", data);  fflush(stdout);}
    return 1;
  }

  int ReadDataVoid(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
  {
    int has_ghost_zone = 0;
    for (int i = 0; i < overlap.size(); i++)
    {
      if (overlap[0] == 1)
      {
        has_ghost_zone = 1;
        break;
      }
    }
    double t_start_time = MPI_Wtime();
    if (has_ghost_zone == 1)
      UpdateGhost();
    double t_end_time = MPI_Wtime();
    time_of_update_host_zone = time_of_update_host_zone + t_end_time - t_start_time;

    std::vector<unsigned long long> global_count;
    global_count.resize(start.size());
    for (int i = 0; i < start.size(); i++)
      global_count[i] = end[i] - start[i] + 1;

    iArray_read_local(iarray_handle, start, global_count, data);

    //if(mpi_rank == 2 ) { PrintVector("Start: ", start); PrintVector("End: ", end); printf("At rank %d, data vector's size = %d ,,, ", mpi_rank, data.size()); PrintVector(" Rank's read data: ", data);  fflush(stdout);}
    return 1;
  }

  int ReadDataStripingMem(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data, unsigned long long hym_start, unsigned long long hym_stride, unsigned long long hym_count)
  {
    printf("No memspace based striping read for IArray yet !\n");
    return 0;
  }

  //To start a asio
  //https://gist.github.com/rsms/771059
  int WriteData(std::vector<unsigned long long> start, std::vector<unsigned long long> end, std::vector<T> data)
  {
    std::vector<unsigned long long> global_count, global_start;
    global_count.resize(rank);
    global_start.resize(rank);
    for (int i = 0; i < start.size(); i++)
    {
      global_count[i] = end[i] - start[i] + 1;
      global_start[i] = start[i];
    }
    void *temp_ptr;
    if (vector_type_flag == 1)
    {
      global_start[rank - 1] = 0;
      global_count[rank - 1] = filter_amount;
      temp_ptr = vv2v(data);
      if (temp_ptr != NULL)
      {
        iArray_write_local(iarray_handle, global_start, global_count, temp_ptr);
        free(temp_ptr);
      }
    }
    else
    {
      iArray_write_local(iarray_handle, global_start, global_count, &data[0]);
    }
    return 1;
  }

  int WriteDataVoidP(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
  {
    std::vector<unsigned long long> global_count;
    global_count.resize(start.size());
    for (int i = 0; i < start.size(); i++)
      global_count[i] = end[i] - start[i] + 1;
    iArray_write_local(iarray_handle, start, global_count, data);
    //if(mpi_rank < 1  ) { printf("At rank = %d\n", mpi_rank);PrintVector("Start: ", start); PrintVector("End: ", end); PrintVector(" Rank's read data after writing ia: ", data); fflush(stdout);}
    return 1;
  }

  int WriteDataAllDirect(std::vector<T> data)
  {
    iArray_write_local_all_directly(iarray_handle, &(data[0]), data.size() * sizeof(T));
    return 0;
  }

  int CreateNVSFile(int data_dims, std::vector<unsigned long long> &data_dims_size, int data_type_class)
  {
    //printf("CreateNVSFile in ga.h \n");
    nvs_f_handle = new H5Data<T>(fn, gn, dn, (int)1); //1: delay_flag
    if (is_vector_type<T>())
      vector_type_flag = 1;

    std::vector<int> overlap_int;
    overlap_int.resize(overlap.size());
    for (int i = 0; i < overlap.size(); i++)
    {
      overlap_int[i] = (int)overlap[i];
    }

    nvs_f_handle->CreateNVSFile(data_dims, data_dims_size, data_type_class, overlap_int);
    return 0;
  }

  void Nonvolatile()
  {
    if (iarray_handle < 0)
    {
      printf("Invalide handle at Nonvolatile !\n");
      exit(-1);
    }
    std::vector<int> start_lo, count_lo;
    iArray_get_local_dims(iarray_handle, start_lo, count_lo);

    std::vector<unsigned long long> start, end;
    int64_t mem_size = 1;
    start.resize(rank);
    end.resize(rank);
    for (int i = 0; i < rank; i++)
    {
      mem_size = mem_size * count_lo[i];
      start[i] = start_lo[i];
      end[i] = start_lo[i] + count_lo[i] - 1;
    }
    //std::cout << "rank : " << mpi_rank << ", local start = " << start[0] << ", " << start[1] << ", end=" << end[0] << "," << end[1] << std::endl;
    if (vector_type_flag == 0)
    {
      std::vector<T> data;
      data.resize(mem_size);
      ReadData(start, end, data);
      nvs_f_handle->WriteData(start, end, data);
      nvs_f_handle->Nonvolatile();
    }
    else
    {
      //To do something
      // The input type T is std::vector,
      //Hence, data is vector of vector.
      std::vector<T> data;
      data.resize(mem_size);
      void *data_temp_voidp;
      data_temp_voidp = vv2v_empty(data);
      data.resize(0);
      ReadDataVoid(start, end, data_temp_voidp);
      nvs_f_handle->WriteDataVoidP(start, end, data_temp_voidp);
      nvs_f_handle->Nonvolatile();
    }
  }

  void Sync()
  {
  }

  int GetTypeClass()
  {
    return type;
  }

  std::string GetFileName()
  {
    return fn;
  }

  std::string GetGroupName()
  {
    return gn;
  }
  std::string GetDatasetName()
  {
    return dn;
  }

  void UpdateGhost()
  {
    //MPI_Barrier(MPI_COMM_WORLD);
    iArray_ghost_exchange(iarray_handle);
    //MPI_Barrier(MPI_COMM_WORLD);
  }

  void SetFilterAmount(int p)
  {
    filter_amount = p;
  }

  int GetHandle()
  {
    return iarray_handle;
  }

  void SetHandle(int h)
  {
    iarray_handle = h;
    //We think the file associated with h already exists
    //We also need to update the file information associated with it
    rank = iArray_get_global_rank(h);
    type = iArray_get_element_type(h);
    dims.resize(rank);
    lo.resize(rank);
    hi.resize(rank);
    ld.resize(rank - 1);
    overlap.resize(rank);
    chunk.resize(rank);

    std::vector<long long unsigned int> dims_temp;
    dims_temp.resize(rank);
    iArray_get_global_dims(h, dims_temp);
    iArray_get_global_ghost_size(h, overlap);
    iArray_get_global_chunk_size(h, chunk);

    for (int i = 0; i < rank; i++)
    {
      //dims[i]= (int)data_dims_size[i];
      //overlap[i] = (int)data_overlap_size[i];
      //chunk[i] = (int)data_chunk_size_t[i];
      dims[i] = dims_temp[i];
      if (overlap[i] != 0)
        ghost_flag = 1;
    }
  }

  int UpdateHandle(int h)
  {
    iarray_handle = h;
    return 0;
  }

  int GetDataType()
  {
    return iArray_get_element_type(iarray_handle);
  }
};

#endif
