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

#ifndef Array_UDF_DATA
#define Array_UDF_DATA

#include "hdf5.h" //right now, we only have code for HDF5
#include "array_udf_attribute.h"
//#include "array_udf_ga.h"
#include "array_udf_h5.h"
#include "array_udf_iarray.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>

#define DELAY_TO_CREATE 1

using namespace std;

//
//I/O layer
template <class T>
class Data
{
private:
  std::string fn;
  std::string gn;
  std::string dn;
  DataOrigin d_orig;
  NVSFile nvs_f;
  H5Data<T> *nvs_f_handle = NULL;    //Can be void * to support more IO drivers
  IArrayData<T> *vs_f_handle = NULL; // Can be void * to support more IO drivers
  CacheFlag cache_flag = AU_NOCACHE; //the flag for the cache; No Cache by default.
  int data_dims_t;
  std::vector<unsigned long long> data_dims_size_t;
  int data_type_class_t;
  int vector_type_flag = 0;
  int filter_amount;
  int create_storage_space_flag = 0;

  int output_vector_size = 0;
  int output_vector_flat_direction_index = 0;

public:
  Data(){};

  Data(std::string fn_p, std::string gn_p, std::string dn_p)
  {
    fn = fn_p;
    gn = gn_p;
    dn = dn_p;
    nvs_f_handle = new H5Data<T>(fn_p, gn_p, dn_p);
    return;
  };

  Data(std::string fn_p, std::string gn_p, std::string dn_p, int delay_to_create)
  {
    fn = fn_p;
    gn = gn_p;
    dn = dn_p;
    nvs_f_handle = new H5Data<T>(fn_p, gn_p, dn_p, delay_to_create);
    return;
  };

  Data(std::string fn_p, std::string gn_p, std::string dn_p, DataOrigin d_orig_p, NVSFile nvs_f_p)
  {
    fn = fn_p;
    gn = gn_p;
    dn = dn_p;
    d_orig = d_orig_p;
    nvs_f = nvs_f_p;
    if (AU_COMPUTED == d_orig_p)
    {
      //For write: Delay the create to happen
      //Because we don't know the size, and where to store the data
      vs_f_handle = NULL;                                //new GAData(std::string fn_p, std::string gn_p, std::string dn_p);
      nvs_f_handle = new H5Data<T>(fn_p, gn_p, dn_p, 1); //In case we want to store data onto disk
    }
    else if (nvs_f_p == AU_HDF5)
    {
      //For read : if file exists, then open it
      nvs_f_handle = new H5Data<T>(fn_p, gn_p, dn_p);
    }
    else
    {
      //some other formats.
      printf("We does not support this yet.\n");
      exit(0);
    }

    return;
  };

  Data(std::string fn_p, std::string gn_p, std::string dn_p, DataOrigin d_orig_p, NVSFile nvs_f_p, int iArray_handle)
  {
    fn = fn_p;
    gn = gn_p;
    dn = dn_p;
    nvs_f_handle = new H5Data<T>(fn_p, gn_p, dn_p, 1); //In case we want to store data onto disk
    vs_f_handle = new IArrayData<T>(fn, gn, dn);
    vs_f_handle->SetHandle(iArray_handle);

    data_dims_t = vs_f_handle->GetRank();
    data_dims_size_t = vs_f_handle->GetDimSize();
    switch (vs_f_handle->GetDataType())
    {
    case AU_INTEGER:
      data_type_class_t = H5T_INTEGER;
      break;
    case AU_FLOAT:
      data_type_class_t = H5T_FLOAT;
      break;
    }
    cache_flag = AU_CACHED;
    return;
  };

  ~Data()
  {
    //printf("Destructor in Data !\n");
    if (vs_f_handle != NULL)
    {
      delete vs_f_handle;
      vs_f_handle = NULL;
    }

    if (nvs_f_handle != NULL)
    {
      delete nvs_f_handle;
      nvs_f_handle = NULL;
    }
  }

  std::vector<unsigned long long> GetDimSize()
  {
    if (cache_flag == AU_NOCACHE)
    {
      return nvs_f_handle->GetDimSize();
    }
    else
    {
      return vs_f_handle->GetDimSize();
    }
  }

  int ReadData(std::vector<unsigned long long> start, std::vector<unsigned long long> end, std::vector<T> &data)
  {
    if (cache_flag == AU_NOCACHE)
    {
      return nvs_f_handle->ReadData(start, end, data);
    }
    else
    {
      //printf("Read at %d  with %f \n", start[0], data[0]);
      return vs_f_handle->ReadData(start, end, data);
    }
  }

  T ReadPreloadPointAtOffset(unsigned long long offset)
  {
    return vs_f_handle->ReadPreloadPointAtOffset(offset);
  }

  int ReadDataStripingMem(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data, unsigned long long hym_start, unsigned long long hym_stride, unsigned long long hym_count)
  {
    if (cache_flag == AU_NOCACHE)
    {
      return nvs_f_handle->ReadDataStripingMem(start, end, data, hym_start, hym_stride, hym_count);
    }
    else
    {
      //printf("Read at %d  with %f \n", start[0], data[0]);
      return vs_f_handle->ReadDataStripingMem(start, end, data, hym_start, hym_stride, hym_count);
    }
  }

  int WriteData(std::vector<unsigned long long> start, std::vector<unsigned long long> end, std::vector<T> data)
  {
    if (cache_flag == AU_NOCACHE)
    {
      return nvs_f_handle->WriteData(start, end, data);
    }
    else
    {
      return vs_f_handle->WriteData(start, end, data);
    }
  }

  int WriteDataVoid(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
  {
    if (cache_flag == AU_NOCACHE)
    {
      return nvs_f_handle->WriteDataVoidP(start, end, data);
    }
    else
    {
      return vs_f_handle->WriteDataVoidP(start, end, data);
    }
  }

  void DisableCollectivIO()
  {
    if (cache_flag == AU_NOCACHE)
    {
      return nvs_f_handle->DisableCollectivIO();
    }
  }

  int CreateStorageSpace(int data_dims, std::vector<unsigned long long> &data_dims_size, int data_type_class, std::vector<int> data_overlap_size, std::vector<int> data_chunk_size_t)
  {

    //printf("Debug: create ss, in udf_io.\n");
    data_dims_t = data_dims;
    data_dims_size_t = data_dims_size;
    data_type_class_t = data_type_class;
    if (cache_flag == AU_NOCACHE)
    {
      return nvs_f_handle->CreateNVSFile(data_dims, data_dims_size, data_type_class, data_overlap_size);
    }
    else
    {
      create_storage_space_flag = 1;
      AUType t = (AUType)data_type_class; //Type convert happens here
      //vs_f_handle = new GAData<T>(fn, gn, dn);
      vs_f_handle = new IArrayData<T>(fn, gn, dn);
      return vs_f_handle->Create(data_dims, data_dims_size, t, data_overlap_size, data_chunk_size_t);
    }
  }

  int ClearStorageSpace()
  {
    create_storage_space_flag = 0;
    if (cache_flag == AU_NOCACHE)
    {
      return 0; //Do nothing
    }
    else
    {
      vs_f_handle->Destroy();
      delete vs_f_handle;
      vs_f_handle = NULL;
      return 0;
    }
  }

  int GetCreateStorageSpaceFlag()
  {
    return create_storage_space_flag;
  }

  int GetTypeClass()
  {
    if (cache_flag == AU_NOCACHE)
    {
      return nvs_f_handle->GetTypeClass();
    }
    else
    {
      return vs_f_handle->GetTypeClass();
    }
  }

  std::string GetFileName()
  {
    if (cache_flag == AU_NOCACHE)
    {
      return nvs_f_handle->GetFileName();
    }
    else
    {
      return vs_f_handle->GetFileName();
    }
  }

  int GetVSHandle()
  {
    if (cache_flag != AU_NOCACHE)
    {
      return vs_f_handle->GetHandle();
    }
    return -1;
  }

  std::string GetGroupName()
  {
    if (cache_flag == AU_NOCACHE)
    {
      return nvs_f_handle->GetGroupName();
    }
    else
    {
      return vs_f_handle->GetGroupName();
    }
  }
  std::string GetDatasetName()
  {
    if (cache_flag == AU_NOCACHE)
    {
      return nvs_f_handle->GetDatasetName();
    }
    else
    {
      return vs_f_handle->GetDatasetName();
    }
  }

  void Nonvolatile()
  {
    //if(cache_flag == AU_NOCACHE){
    //vs_f_handle->CreateNVSFile(data_dims_t, data_dims_size_t, data_type_class_t);
    //}else{
    if (cache_flag == AU_CACHED)
    {
      vs_f_handle->CreateNVSFile(data_dims_t, data_dims_size_t, data_type_class_t);
      //printf("In Nonvolatile: data_dims =%d, type_class = %d \n", data_dims_t, data_type_class_t);
      //PrintVector("Dims:", data_dims_size_t);
      //return;
      return vs_f_handle->Nonvolatile();
    }
  }

  void DiableCache()
  {
    cache_flag = AU_NOCACHE;
  }

  void EnableCache()
  {
    //only create the space in GA when users want to cache
    vs_f_handle = new IArrayData<T>(fn, gn, dn);
    cache_flag = AU_CACHED;
  }

  void DataSyncCrossNode()
  {
    if (cache_flag == AU_NOCACHE)
    {
      return nvs_f_handle->Nonvolatile();
    }
    else
    {
      return vs_f_handle->Sync();
    }
  }

  void SetFilterAmount(int p)
  {
    filter_amount = p;
    if (cache_flag == AU_NOCACHE)
    {
      return nvs_f_handle->SetFilterAmount(p);
    }
    else
    {
      return vs_f_handle->SetFilterAmount(p);
    }
  }

  void SetOutputVector(int vsize, int flat_direction_index)
  {
    vector_type_flag = 1;
    output_vector_size = vsize;
    output_vector_flat_direction_index = flat_direction_index;

    if (cache_flag == AU_NOCACHE)
    {
      return nvs_f_handle->SetOutputVector(vsize, flat_direction_index);
    }
    else
    {
      return vs_f_handle->SetOutputVector(vsize, flat_direction_index);
    }
  }

  void PreLoad()
  {
    std::vector<unsigned long long> array_size = nvs_f_handle->GetDimSize();
    int type_class = nvs_f_handle->GetTypeClass();
    int rank = nvs_f_handle->GetRank();
    int mpi_rank, mpi_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    AUType type_t = (AUType)type_class;
    std::vector<int> data_overlap_size;
    data_overlap_size.resize(rank);
    std::vector<int> data_chunk_size_t;
    data_chunk_size_t.resize(rank);
    std::vector<unsigned long long> start, end;
    start.resize(rank);
    end.resize(rank);

    unsigned long long part_striping_size = 1;
    for (int i = 0; i < rank; i++)
    {
      data_chunk_size_t[i] = array_size[i];
      data_overlap_size[i] = 0;
      start[i] = 0;
      end[i] = array_size[i] - 1;
      part_striping_size = part_striping_size * array_size[i];
    }

    EnableCache(); //This will create new IArray object
    //data_overlap_size.resize();
    //if(vs_f_handle == NULL) vs_f_handle = new IArrayData<T>(fn, gn, dn);
    //set array_size == chunk_size to force each mpi_rank has its own copy of the whole data
    vs_f_handle->Create(rank, array_size, type_t, data_overlap_size, data_chunk_size_t);

    //At this memoent , we assume each mpi_rank pre-load the whole array
    //But, in case the array is large, we load it
    /* unsigned long long part_striping_size; */
    /* int  max_dims_rank = rank-1; */
    /* for (int i = 0; i < rank -1 ; i++){ */
    /*   if(array_size[i] > array_size[max_dims_rank]) */
    /*     max_dims_rank = i; */
    /* } */

    /* if(mpi_size < array_size[max_dims_rank]){ */
    /*   //usually partition along the last dimension */
    /*   part_striping_size = ceil((double)array_size[max_dims_rank]/(double)mpi_size); */
    /*   for(int i = 0; i < rank; i++){ */
    /*     if (i != max_dims_rank){ */
    /*       start[i] = 0; end[i] = array_size[i]-1; */
    /*     }else{ */
    /*       start[i] = part_striping_size*mpi_rank; */
    /*       if(mpi_rank == mpi_size -1){ */
    /*         end[i] = array_size[rank-1]-1; */
    /*       }else{ */
    /*         end[i] = part_striping_size*(mpi_rank+1)-1; */
    /*       } */
    /*     } */
    /*   } */
    /* }else{ */
    /*   printf("Need more code to support the partition for small array for pre_load. \n"); */
    /*   exit(0); */
    /*   ///\* */
    /*   //part_striping_size = 1; */
    /*   //for(int i = 0; i < rank; i++){ */
    /*   //  part_striping_size = part_striping_size * array_size[i]; */
    /*   //} */
    /*   //part_striping_size = ceil(part_striping_size/mpi_size); */
    /*   //double rootth=1.0/rank; */
    /*   //unsigned long long square_size = std::pow(part_striping_size, rootth); */
    /*   //\*\/ */
    /* } */

    //part_striping_size = 1;
    //for(int i = 0; i < rank; i++)
    //part_striping_size = part_striping_size * (end[i]-start[i]+1);

    std::vector<T> data_v;
    data_v.resize(part_striping_size);
    nvs_f_handle->ReOpenAsReadOnly();
    nvs_f_handle->ReadDataAll(data_v);
    //std::cout << "rank : " << mpi_rank << ", start: " << start[0] << ", end: " << end[0] << ", strip size = " << part_striping_size << ", at start = " << data_v[start[0]] << ", at end-1 =" << data_v[end[0] - 1] << ", at end=" << data_v[end[0]] << std::endl;

    //vs_f_handle->WriteData(start, end, data_v);
    //This write will go to local memory of each mpi_rank
    vs_f_handle->WriteDataAllDirect(data_v);
    //vs_f_handle->GASync();

    //std::cout << " After rank : " << mpi_rank << ", start: " << start[0] << ", end: "<< end[0]  << ", strip size = " << part_striping_size  << ", at start = "<< data_v[start[0]] << ", at end-1 =" << data_v[end[0]-1]  << ", at end="<< data_v[end[0]]  << ", at 10th= " << ", data_v[10]=" << data_v[10]<< std::endl;
  }
};

#endif
