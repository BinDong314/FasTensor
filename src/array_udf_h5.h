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

#ifndef Array_UDF_H5DATA
#define Array_UDF_H5DATA

#include "hdf5.h" //right now, we only have code for HDF5
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "utility.h"

#define DELAY_TO_CREATE 1
#define SDS_UDF_COLLECTIVE_IO 1

using namespace std;

typedef struct
{
  float re; /*real part */
  float im; /*imaginary part */
} complex_t;

//These information are HDF5 related.
template <class T>
class H5Data
{
private:
  hid_t fid = -1, gid = -1, did = -1, dataspace_id = -1, memspace_id = -1;
  string fn_str, gn_str, dn_str;
  std::vector<hsize_t> dims_out;
  int rank;
  hid_t datatype;
  H5T_class_t type_class; /* datatype class */
  hid_t plist_id = -1, plist_cio_id = -1;
  int vector_type_flag = 0;
  int filter_amount;

  int output_vector_size = 0;
  int output_vector_flat_direction_index = -1;
  int mpi_rank, mpi_size;

  //h5_mem_type for read/write
  //h5_disk_type for create
  hid_t h5_mem_type, h5_disk_type;

  //For VDS file list;
  std::vector<std::string> FileVDSList;
  std::vector<H5Data<T> *> FileVDSPList;

public:
  H5Data(){};

  //For write: create the file after the UDF is executed
  H5Data(std::string fn, std::string gn, std::string dn, int delay_flag)
  {
    if (delay_flag == 1)
    {
      fn_str = fn;
      gn_str = gn;
      dn_str = dn;
    }
    find_h5_type(h5_mem_type, h5_disk_type);

    if (is_vector_type<T>())
      vector_type_flag = 1;

#ifdef SDS_UDF_COLLECTIVE_IO
    plist_cio_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist_cio_id, H5FD_MPIO_COLLECTIVE);
#else
    plist_cio_id = H5P_DEFAULT;
#endif

    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  }

  //For read : if file exists, if open it
  H5Data(std::string fn, std::string gn, std::string dn)
  {
    fn_str = fn;
    gn_str = gn;
    dn_str = dn;

    find_h5_type(h5_mem_type, h5_disk_type);

    plist_id = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

    if (is_vector_type<T>())
      vector_type_flag = 1;

    MPI_Barrier(MPI_COMM_WORLD);

    fid = H5Fopen(fn.c_str(), H5F_ACC_RDONLY, plist_id);
    if (fid < 0)
    {
      std::cout << "Error happens in open file " << fn << std::endl;
      exit(-1);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    std::string root_dir = "/";
    if (gn != root_dir)
    {
      //std::cout << "Open Group : " << gn << std::endl;
      gid = H5Gopen(fid, gn.c_str(), H5P_DEFAULT);
      did = H5Dopen(gid, dn.c_str(), H5P_DEFAULT);
    }
    else
    {
      did = H5Dopen(fid, dn.c_str(), H5P_DEFAULT);
    }

    datatype = H5Dget_type(did); /* datatype handle */
    type_class = H5Tget_class(datatype);
    dataspace_id = H5Dget_space(did);
    rank = H5Sget_simple_extent_ndims(dataspace_id);
    dims_out.resize(rank);
    H5Sget_simple_extent_dims(dataspace_id, &dims_out[0], NULL);

    //printf("Constructor in H5Data: plist_id= %d, did=%d, fid=%d !\n", plist_id, did, fid);

#ifdef SDS_UDF_COLLECTIVE_IO
    plist_cio_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist_cio_id, H5FD_MPIO_COLLECTIVE);
#else
    plist_cio_id = H5P_DEFAULT;
#endif

    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    if (is_VDS())
    {
      std::string output_file_meata;
      output_file_meata = fn_str + ".vds-meta";

      if (!mpi_rank)
        std::cout << "I am openning a VDS file" << std::endl;
      getFileVDSList(output_file_meata, FileVDSList);
    }
  };

  //For read : if file exists, if open it
  //The "i" and "j" is useless, just to be called by getFileVDSList
  //To avoid recursive calling
  H5Data(std::string fn, std::string gn, std::string dn, int i, int j)
  {
    fn_str = fn;
    gn_str = gn;
    dn_str = dn;

    plist_id = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

    if (is_vector_type<T>())
      vector_type_flag = 1;

    MPI_Barrier(MPI_COMM_WORLD);

    fid = H5Fopen(fn.c_str(), H5F_ACC_RDONLY, plist_id);
    if (fid < 0)
    {
      std::cout << "Error happens in open file " << fn << std::endl;
      exit(-1);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    std::string root_dir = "/";
    if (gn != root_dir)
    {
      //std::cout << "Open Group : " << gn << std::endl;
      gid = H5Gopen(fid, gn.c_str(), H5P_DEFAULT);
      did = H5Dopen(gid, dn.c_str(), H5P_DEFAULT);
    }
    else
    {
      did = H5Dopen(fid, dn.c_str(), H5P_DEFAULT);
    }

    datatype = H5Dget_type(did); /* datatype handle */
    type_class = H5Tget_class(datatype);
    dataspace_id = H5Dget_space(did);
    rank = H5Sget_simple_extent_ndims(dataspace_id);
    dims_out.resize(rank);
    H5Sget_simple_extent_dims(dataspace_id, &dims_out[0], NULL);

    //printf("Constructor in H5Data: plist_id= %d, did=%d, fid=%d !\n", plist_id, did, fid);

#ifdef SDS_UDF_COLLECTIVE_IO
    plist_cio_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist_cio_id, H5FD_MPIO_COLLECTIVE);
#else
    plist_cio_id = H5P_DEFAULT;
#endif

    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  };

  H5Data(std::string fn, std::string gn, std::string dn, int data_dims, std::vector<unsigned long long> data_dims_size, std::vector<int> data_chunk_size)
  {
    //Create a new file  with data_dims, data_dims_size, data_chunk_size

    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  };

  ~H5Data()
  {
    //printf("Destructor in H5Data: plist_id= %d, did=%d, fid=%d !\n", plist_id, did, fid);
    if (plist_id > 0)
      H5Pclose(plist_id);
    if (plist_cio_id > 0)
      H5Pclose(plist_cio_id);
    if (dataspace_id > 0)
      H5Sclose(dataspace_id);
    if (memspace_id > 0)
      H5Sclose(memspace_id);
    if (did > 0)
      H5Dclose(did);
    if (gid > 0)
    {
      std::string root_dir = "/";
      if (gn_str != root_dir)
        H5Gclose(gid);
    }

    if (fid > 0)
    {
      H5Fflush(fid, H5F_SCOPE_GLOBAL);
      H5Fclose(fid);
    }
    plist_id = -1;
    plist_cio_id = -1;
    dataspace_id = -1;
    memspace_id = -1;
    did = -1;
    gid = -1;
    fid = -1;
  }
  std::vector<unsigned long long> GetDimSize()
  {
    return dims_out;
  }

  int ReadData(std::vector<unsigned long long> start, std::vector<unsigned long long> end, std::vector<T> &data)
  {
    if (is_VDS())
    {
      if (!mpi_rank)
        std::cout << "Read VDS: group= " << gn_str << ", dset =" << dn_str << ", file list:" << std::endl;

      //Starting from here, we assume the VDS datasets ordered in row order
      //  |-----|------|
      //  |VDS1 | VDS1 | ....
      //  |_____|______|
      //We may need to deal with column order
      //
      //We also assume read acorss all VDSs
      //See example/das/das-fft-full.cpp for access pattern
      if (FileVDSList.size() <= 0)
      {
        std::cout << "Can not find VDS datasets to read \n";
        exit(-1);
      }

      if (((end[1] + 1) % FileVDSList.size()) != 0)
      {
        std::cout << "# of VDS datasets does not match the size of merged array\n";
        exit(-1);
      }

      au_time_start();

      std::vector<unsigned long long> v_end(2);
      v_end[0] = end[0];
      v_end[1] = ((end[1] + 1) / FileVDSList.size()) - 1;
      std::vector<T> v_data;
      v_data.resize((v_end[1] - start[1] + 1) * (v_end[0] - start[0] + 1));
      //H5Data<T> *h5p;
      for (int i = 0; i < FileVDSList.size(); i++)
      {
        if ((!mpi_rank) && (i % 400 == 0))
          std::cout << "VDS index " << i << ", " << FileVDSList[i] << std::endl;
        OpenReadCloseSingleFile(FileVDSList[i], gn_str, dn_str, start, v_end, v_data);
        //h5p = FileVDSPList[i];
        //h5p->ReadData(start, v_end, v_data);
        InsertVDSIntoGlobalSpace(i, start, v_end, v_data, data, start, end);
      }
      v_data.resize(0);
      clear_vector(v_data);
      au_time_elap("Read data ");
      return 1;
    }
    else
    {
      std::vector<unsigned long long> offset, count;
      offset.resize(rank);
      count.resize(rank);
      for (int i = 0; i < rank; i++)
      {
        offset[i] = start[i];
        count[i] = end[i] - start[i] + 1; //Starting from zero
      }

      memspace_id = H5Screate_simple(rank, &count[0], NULL);
      H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, &offset[0], NULL, &count[0], NULL);

      int ret;
      /*
      switch (type_class)
      {
      case H5T_INTEGER:
        // ret = H5Dread(did, H5T_NATIVE_INT, memspace_id, dataspace_id, H5P_DEFAULT, &data[0]);
        ret = H5Dread(did, H5T_NATIVE_SHORT, memspace_id, dataspace_id, plist_cio_id, &data[0]);
        //dataset.read(data, PredType::NATIVE_INT, memspace, dataspace);
        break;
      case H5T_FLOAT:
        //ret = H5Dread(did, H5T_NATIVE_FLOAT, memspace_id, dataspace_id, H5P_DEFAULT, &data[0]);
        ret = H5Dread(did, H5T_NATIVE_FLOAT, memspace_id, dataspace_id, plist_cio_id, &data[0]);
        //dataset.read(data, PredType::NATIVE_FLOAT, memspace, dataspace);
        break;
      default:
        std::cout << "Unsupported datatype in  " << __FILE__ << __LINE__ << std::endl;
        exit(-1);
        break;
      }*/
      ret = H5Dread(did, h5_mem_type, memspace_id, dataspace_id, plist_cio_id, &data[0]);

#ifdef DEBUG
      for (auto i = data.begin(); i != data.end(); ++i)
        std::cout << *i << ' ';
#endif

      if (ret < 0)
      {
        return -1;
      }

      return 1;
    }

    return 1;
  }

  int ReadDataAll(std::vector<T> &data)
  {
    int ret;
    switch (type_class)
    {
    case H5T_INTEGER:
      // ret = H5Dread(did, H5T_NATIVE_INT, memspace_id, dataspace_id, H5P_DEFAULT, &data[0]);
      ret = H5Dread(did, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, plist_cio_id, &data[0]);
      //dataset.read(data, PredType::NATIVE_INT, memspace, dataspace);
      break;
    case H5T_FLOAT:
      //ret = H5Dread(did, H5T_NATIVE_FLOAT, memspace_id, dataspace_id, H5P_DEFAULT, &data[0]);
      ret = H5Dread(did, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, plist_cio_id, &data[0]);
      //dataset.read(data, PredType::NATIVE_FLOAT, memspace, dataspace);
      break;
    default:
      std::cout << "Unsupported datatype in SDS_UDF_IO Class/Read Data ! " << std::endl;
      exit(-1);
      break;
    }

    assert(ret >= 0);
    //if(ret < 0){
    //  return -1;
    //}

    return 1;
  }

  //Todo: for 2D, 3D, or ...
  int ReadDataStripingMem(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data, unsigned long long hym_start, unsigned long long hym_stride, unsigned long long hym_count)
  {
    std::vector<unsigned long long> offset, count, mem_count;
    offset.resize(rank);
    count.resize(rank);
    mem_count.resize(rank);
    for (int i = 0; i < rank; i++)
    {
      offset[i] = start[i];
      count[i] = end[i] - start[i] + 1; //Starting from zero
      mem_count[i] = count[i] * hym_stride;
    }
    H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, &offset[0], NULL, &count[0], NULL);

    //std:: cout << "hym_start = " << hym_start << ", hym_stride = " << hym_stride << ", hym_count =" << hym_count << std::endl;
    memspace_id = H5Screate_simple(rank, &mem_count[0], NULL);
    H5Sselect_hyperslab(memspace_id, H5S_SELECT_SET, &hym_start, &hym_stride, &count[0], NULL);
    int ret;
    switch (type_class)
    {
    case H5T_INTEGER:
      // ret = H5Dread(did, H5T_NATIVE_INT, memspace_id, dataspace_id, H5P_DEFAULT, &data[0]);
      ret = H5Dread(did, H5T_NATIVE_INT, memspace_id, dataspace_id, plist_cio_id, data);

      //dataset.read(data, PredType::NATIVE_INT, memspace, dataspace);
      break;
    case H5T_FLOAT:
      //ret = H5Dread(did, H5T_NATIVE_FLOAT, memspace_id, dataspace_id, H5P_DEFAULT, &data[0]);
      ret = H5Dread(did, H5T_NATIVE_FLOAT, memspace_id, dataspace_id, plist_cio_id, data);
      //dataset.read(data, PredType::NATIVE_FLOAT, memspace, dataspace);
      break;
    default:
      std::cout << "Unsupported datatype in SDS_UDF_IO Class/Read Data ! " << std::endl;
      exit(-1);
      break;
    }

#ifdef DEBUG
    for (auto i = data.begin(); i != data.end(); ++i)
      std::cout << *i << ' ';
#endif

    if (ret < 0)
    {
      return -1;
    }

    return 1;
  }

  int WriteDataVoidP(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
  {
    std::vector<unsigned long long> offset, count;
    offset.resize(rank); //rank might be start.size()+1 when T is vector
    count.resize(rank);
    for (int i = 0; i < start.size(); i++)
    {
      offset[i] = start[i];
      count[i] = end[i] - start[i] + 1; //Starting from zero
    }

    memspace_id = H5Screate_simple(rank, &count[0], NULL);
    H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, &offset[0], NULL, &count[0], NULL);

    //
    //
    int ret;
    switch (type_class)
    {
    case H5T_INTEGER:
    {
      ret = H5Dwrite(did, H5T_NATIVE_INT, memspace_id, dataspace_id, plist_cio_id, data);
    }
    break;
    case H5T_FLOAT:
    {
      ret = H5Dwrite(did, H5T_NATIVE_FLOAT, memspace_id, dataspace_id, plist_cio_id, data);
    }
    break;
    default:
      std::cout << "Unsupported datatype in SDS_UDF_IO Class/Read Data ! " << std::endl;
      exit(-1);
      break;
    }
    // free(int_new_data_ptr);
    //
    //if(int_new_data_ptr   != NULL){ printf("free int"); }

    if (ret < 0)
    {
      std::cout << "Error happened in writing data ! " << std::endl;
      exit(-1);
    }

    //flush data onto disk
    H5Fflush(fid, H5F_SCOPE_GLOBAL);
    return 1;
  }

  template <class DataType>
  int WriteData(std::vector<unsigned long long> start, std::vector<unsigned long long> end, std::vector<DataType> data)
  {
    Open(H5F_ACC_RDWR);
    std::vector<unsigned long long> offset, count;
    offset.resize(rank); //rank might be start.size()+1 when T is vector
    count.resize(rank);
    int data_rank = start.size();

    if (vector_type_flag == 1)
    {
      if (output_vector_flat_direction_index > (data_rank - 1))
      { //Add new dimension
        assert(rank > data_rank);
        for (int i = 0; i < data_rank; i++)
        {
          offset[i] = start[i];
          count[i] = end[i] - start[i] + 1; //Starting from zero
        }
        offset[rank - 1] = 0;
        count[rank - 1] = output_vector_size; //data[0].size();
      }
      else
      {
        for (int i = 0; i < data_rank; i++)
        {
          offset[i] = start[i];
          if (i == output_vector_flat_direction_index)
          {
            count[i] = (end[i] - start[i] + 1) * output_vector_size;
          }
          else
          {
            count[i] = end[i] - start[i] + 1;
          }
        }
        //printf("output_vector_flat_direction_index = %d \n", output_vector_flat_direction_index);
        //printf("output_vector_size = %d \n", output_vector_size);
        //PrintVector<unsigned long long>(" offset", offset);
        //PrintVector<unsigned long long>(" count", count);
      }
    }
    else
    { //vector_type_flag == 0
      for (int i = 0; i < data_rank; i++)
      {
        offset[i] = start[i];
        count[i] = end[i] - start[i] + 1; //Starting from zero
      }
    }

    memspace_id = H5Screate_simple(rank, &count[0], NULL);
    H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, &offset[0], NULL, &count[0], NULL);
    //printf("Write: start = %lld, %lld, end = %lld, %lld, offset = %lld %lld, count = %lld  %lld, output_vector_size = %d, data.size = %d \n", start[0], start[1], end[0], end[1], offset[0], offset[1], count[0], count[1], output_vector_size, data.size());

    //complex_t tmp; /*used only to compute offsets */
    //hid_t complex_id = H5Tcreate(H5T_COMPOUND, sizeof(complex_t));
    //H5Tinsert(complex_id, "real", HOFFSET(complex_t, re), H5T_NATIVE_FLOAT);
    //H5Tinsert(complex_id, "imaginary", HOFFSET(complex_t, im), H5T_NATIVE_FLOAT);

    au_time_start();

    //
    //
    int ret;
    /*
    switch (type_class)
    {
    case H5T_INTEGER:
    {
      if (vector_type_flag == 1)
      {
        void *int_temp_ptr = vv2v(data);
        ret = H5Dwrite(did, H5T_NATIVE_INT, memspace_id, dataspace_id, plist_cio_id, int_temp_ptr);
        if (int_temp_ptr != NULL)
          free(int_temp_ptr);
      }
      else
      {
        ret = H5Dwrite(did, H5T_NATIVE_INT, memspace_id, dataspace_id, plist_cio_id, &data[0]);
      }
    }
    break;
    case H5T_FLOAT:
    {
      if (vector_type_flag == 1)
      {
        void *float_new_data_ptr;
        if (output_vector_flat_direction_index == 0)
        {
          float_new_data_ptr = flat_vector(data, 1);
        }
        else
        {
          float_new_data_ptr = vv2v(data);
        }
        ret = H5Dwrite(did, H5T_NATIVE_FLOAT, memspace_id, dataspace_id, plist_cio_id, float_new_data_ptr);
        if (float_new_data_ptr != NULL)
          free(float_new_data_ptr);
      }
      else
      {
        ret = H5Dwrite(did, H5T_NATIVE_FLOAT, memspace_id, dataspace_id, plist_cio_id, &data[0]);
      }
    }
    break;
    case H5T_COMPOUND:
    {
      if (vector_type_flag == 1)
      {
        //void *float_new_data_ptr = vv2v(data);
        void *float_new_data_ptr = flat_vector(data, 1);
        ret = H5Dwrite(did, complex_id, memspace_id, dataspace_id, plist_cio_id, float_new_data_ptr);
        if (float_new_data_ptr != NULL)
          free(float_new_data_ptr);
      }
      else
      {
        ret = H5Dwrite(did, complex_id, memspace_id, dataspace_id, plist_cio_id, &data[0]);
      }
    }
    break;

    default:
      std::cout << "Unsupported datatype in  " << __FILE__ << ": " << __LINE__ << std::endl;
      exit(-1);
      break;
    }
    */

    if (vector_type_flag == 1)
    {
      void *float_new_data_ptr;
      if (output_vector_flat_direction_index == 0)
      {
        float_new_data_ptr = flat_vector(data, 1);
      }
      else
      {
        float_new_data_ptr = vv2v(data);
        //float_new_data_ptr = vv2v_mem_e(data);
      }
      ret = H5Dwrite(did, h5_mem_type, memspace_id, dataspace_id, plist_cio_id, float_new_data_ptr);
      if (float_new_data_ptr != NULL)
        free(float_new_data_ptr);
    }
    else
    {
      ret = H5Dwrite(did, h5_mem_type, memspace_id, dataspace_id, plist_cio_id, &data[0]);
    }
    // free(int_new_data_ptr);
    //
    //if(int_new_data_ptr   != NULL){ printf("free int"); }

    if (ret < 0)
    {
      std::cout << "Error happened in writing data ! " << std::endl;
      exit(-1);
    }

    //flush data onto disk
    H5Fflush(fid, H5F_SCOPE_GLOBAL);
    Close();
    au_time_elap("Write data ");

    return 1;
  }

  void DisableCollectivIO()
  {
    if (plist_cio_id > 0)
      H5Pclose(plist_cio_id);
    plist_cio_id = H5P_DEFAULT;
  }

  int CreateNVSFile(int data_dims, std::vector<unsigned long long> &data_dims_size, int data_type_class, std::vector<int> data_overlap_size)
  {
    //    printf("Debug: in h5 create file.\n");
    std::string root_dir = "/";
    plist_id = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

    if (is_vector_type<T>())
      vector_type_flag = 1;

    if (file_exist(fn_str.c_str()) == 0)
    {
      fid = H5Fcreate(fn_str.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
    }
    else
    {
      fid = H5Fopen(fn_str.c_str(), H5F_ACC_RDWR, plist_id);
      if (fid < 0)
        fid = H5Fcreate(fn_str.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
    }

    // Does not work: https://support.hdfgroup.org/ftp/HDF5/examples/misc-examples/h5grpexist.c
    //https://support.hdfgroup.org/ftp/HDF5/current/src/unpacked/examples/h5_interm_group.c
    /*if (gn_str != root_dir){
      int status = H5Gget_objinfo(fid, gn_str.c_str(), 0, NULL);
      if (status == 0) { //group exists 
        gid  = H5Gopen(fid, gn_str.c_str(), H5P_DEFAULT);
        if(gid < 0 )
          gid  = H5Gcreate(fid, gn_str.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      }else{
        gid  = H5Gcreate(fid, gn_str.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      }
      }*/
    if (gn_str != root_dir)
    {
      //printf("Debug: %s:%d\n", __FILE__, __LINE__);
      if (H5Lexists(fid, gn_str.c_str(), H5P_DEFAULT) == 0)
      {
        gid = H5Gcreate(fid, gn_str.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      }
      else
      {
        gid = H5Gopen(fid, gn_str.c_str(), H5P_DEFAULT);
      }
    }
    /*else
    {
      gid = fid;
    }*/

    //printf("Debug: %s:%d\n", __FILE__, __LINE__);

    type_class = (H5T_class_t)data_type_class;
    rank = data_dims;
    dims_out = data_dims_size;
    hid_t ts_id;
    ts_id = H5Screate_simple(rank, &dims_out[0], NULL);

    // complex_t tmp; /*used only to compute offsets */
    //hid_t complex_id = H5Tcreate(H5T_COMPOUND, sizeof(complex_t));
    //H5Tinsert(complex_id, "real", HOFFSET(complex_t, re), H5T_NATIVE_FLOAT);
    //H5Tinsert(complex_id, "imaginary", HOFFSET(complex_t, im), H5T_NATIVE_FLOAT);

    /*
    switch (type_class)
    {
    case H5T_INTEGER:
      if (gn_str != root_dir)
      {
        if (H5Lexists(gid, dn_str.c_str(), H5P_DEFAULT) > 0)
        {
          H5Ldelete(gid, dn_str.c_str(), H5P_DEFAULT); //we delete
        }
        did = H5Dcreate(gid, dn_str.c_str(), H5T_STD_I32LE, ts_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      }
      else
      {
        if (H5Lexists(fid, dn_str.c_str(), H5P_DEFAULT) > 0)
        {
          H5Ldelete(fid, dn_str.c_str(), H5P_DEFAULT); //we delete
        }
        did = H5Dcreate(fid, dn_str.c_str(), H5T_STD_I32LE, ts_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      }
      break;
    case H5T_FLOAT:
      if (gn_str != root_dir)
      {
        if (H5Lexists(gid, dn_str.c_str(), H5P_DEFAULT) > 0)
        {
          H5Ldelete(gid, dn_str.c_str(), H5P_DEFAULT); //we delete
        }

        did = H5Dcreate(gid, dn_str.c_str(), H5T_IEEE_F32LE, ts_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      }
      else
      {
        if (H5Lexists(fid, dn_str.c_str(), H5P_DEFAULT) > 0)
        {
          H5Ldelete(fid, dn_str.c_str(), H5P_DEFAULT); //we delete
        }
        did = H5Dcreate(fid, dn_str.c_str(), H5T_IEEE_F32LE, ts_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      }
      break;
    case H5T_COMPOUND:
      std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
      exit(-1);
      break;
    default:
      std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
      exit(-1);
      break;
    }
    */

    if (gn_str != root_dir)
    {
      if (H5Lexists(gid, dn_str.c_str(), H5P_DEFAULT) > 0)
      {
        H5Ldelete(gid, dn_str.c_str(), H5P_DEFAULT); //we delete
      }

      did = H5Dcreate(gid, dn_str.c_str(), h5_disk_type, ts_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    }
    else
    {
      if (H5Lexists(fid, dn_str.c_str(), H5P_DEFAULT) > 0)
      {
        H5Ldelete(fid, dn_str.c_str(), H5P_DEFAULT); //we delete
      }
      did = H5Dcreate(fid, dn_str.c_str(), h5_disk_type, ts_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    }
    //datatype  = H5Dget_type(did);     /* datatype handle */
    //type_class     = H5Tget_class(datatype);
    dataspace_id = H5Dget_space(did);

    //Close all created file/group/data for Coumpund output
    Close();
    return 0;
  }

  int GetTypeClass()
  {
    return type_class;
  }

  int GetRank()
  {
    return rank;
  }

  std::string GetFileName()
  {
    return fn_str;
  }

  std::string GetGroupName()
  {
    return gn_str;
  }
  std::string GetDatasetName()
  {
    return dn_str;
  }

  void Nonvolatile()
  {
    //flush data onto disk
    H5Fflush(fid, H5F_SCOPE_GLOBAL);
  }

  void SetFilterAmount(int p)
  {
    filter_amount = p;
  }

  void SetOutputVector(int vsize, int flat_direction_index)
  {
    vector_type_flag = 1;
    output_vector_size = vsize;
    output_vector_flat_direction_index = flat_direction_index;
  }

  void ReOpenAsReadOnly()
  {
    if (plist_id > 0)
      H5Pclose(plist_id);
    if (plist_cio_id > 0)
      H5Pclose(plist_cio_id);
    if (dataspace_id > 0)
      H5Sclose(dataspace_id);
    if (memspace_id > 0)
      H5Sclose(memspace_id);
    if (did > 0)
      H5Dclose(did);
    if (gid > 0)
      H5Gclose(gid);
    if (fid > 0)
      H5Fclose(fid);

    plist_id = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

    fid = H5Fopen(fn_str.c_str(), H5F_ACC_RDONLY, plist_id);
    if (fid < 0)
    {
      std::cout << "Error happens in open file " << fn_str << std::endl;
      exit(-1);
    }

    std::string root_dir = "/";
    if (gn_str != root_dir)
    {
      //std::cout << "Open Group : " << gn << std::endl;
      gid = H5Gopen(fid, gn_str.c_str(), H5P_DEFAULT);
      did = H5Dopen(gid, dn_str.c_str(), H5P_DEFAULT);
    }
    else
    {
      did = H5Dopen(fid, dn_str.c_str(), H5P_DEFAULT);
    }
    datatype = H5Dget_type(did); /* datatype handle */
    type_class = H5Tget_class(datatype);
    dataspace_id = H5Dget_space(did);
    rank = H5Sget_simple_extent_ndims(dataspace_id);
    dims_out.resize(rank);
    H5Sget_simple_extent_dims(dataspace_id, &dims_out[0], NULL);

    //#ifdef SDS_UDF_COLLECTIVE_IO
    //plist_cio_id = H5Pcreate(H5P_DATASET_XFER);
    //H5Pset_dxpl_mpio(plist_cio_id, H5FD_MPIO_COLLECTIVE);
    //#else
    plist_cio_id = H5P_DEFAULT;
    //#endif
  }

  void Close()
  {
    if (plist_id > 0)
      H5Pclose(plist_id);
    if (plist_cio_id > 0)
      H5Pclose(plist_cio_id);
    if (dataspace_id > 0)
      H5Sclose(dataspace_id);
    if (memspace_id > 0)
      H5Sclose(memspace_id);
    if (did > 0)
      H5Dclose(did);
    if (gid > 0)
      H5Gclose(gid);
    if (fid > 0)
      H5Fclose(fid);
    plist_id = -1;
    plist_cio_id = -1;
    dataspace_id = -1;
    memspace_id = -1;
    did = -1;
    gid = -1;
    fid = -1;
  }

  //H5F_ACC_RDWR
  //H5F_ACC_RDONLY
  //H5F_ACC_SWMR_WRITE
  //H5F_ACC_SWMR_READ
  void Open(int flag)
  {
    //make sure it is closed
    if (plist_id > 0)
      H5Pclose(plist_id);
    if (plist_cio_id > 0)
      H5Pclose(plist_cio_id);
    if (dataspace_id > 0)
      H5Sclose(dataspace_id);
    if (memspace_id > 0)
      H5Sclose(memspace_id);
    if (did > 0)
      H5Dclose(did);
    if (gid > 0)
      H5Gclose(gid);
    if (fid > 0)
      H5Fclose(fid);

    plist_id = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

    fid = H5Fopen(fn_str.c_str(), flag, plist_id);
    if (fid < 0)
    {
      std::cout << "Error happens in open file " << fn_str << std::endl;
      exit(-1);
    }

    std::string root_dir = "/";
    if (gn_str != root_dir)
    {
      //std::cout << "Open Group : " << gn << std::endl;
      gid = H5Gopen(fid, gn_str.c_str(), H5P_DEFAULT);
      did = H5Dopen(gid, dn_str.c_str(), H5P_DEFAULT);
    }
    else
    {
      did = H5Dopen(fid, dn_str.c_str(), H5P_DEFAULT);
    }
    datatype = H5Dget_type(did); /* datatype handle */
    type_class = H5Tget_class(datatype);
    dataspace_id = H5Dget_space(did);
    rank = H5Sget_simple_extent_ndims(dataspace_id);
    dims_out.resize(rank);
    H5Sget_simple_extent_dims(dataspace_id, &dims_out[0], NULL);

#ifdef SDS_UDF_COLLECTIVE_IO
    plist_cio_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist_cio_id, H5FD_MPIO_COLLECTIVE);
#else
    plist_cio_id = H5P_DEFAULT;
#endif
  }

  int is_VDS()
  {
    std::string output_file_meata;
    output_file_meata = fn_str + ".vds-meta";
    struct stat buffer;
    return (stat(output_file_meata.c_str(), &buffer) == 0);
  }

  //Used to read VDS's file list
  //One file at a time and all processes read the same file
  //Each process figure out is own start/end
  template <class DataType>
  int OpenReadCloseSingleFile(std::string fna, std::string gna, std::string dna, std::vector<unsigned long long> starta, std::vector<unsigned long long> enda, std::vector<DataType> &dataa)
  {

    hid_t v_fid = -1, v_gid = -1, v_did = -1, v_plist_id;
    v_plist_id = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(v_plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

    v_fid = H5Fopen(fna.c_str(), H5F_ACC_RDONLY, v_plist_id);
    if (v_fid < 0)
    {
      std::cout << "Error happens in open file " << fna << std::endl;
      exit(-1);
    }

    std::string root_dir = "/";
    if (gna != root_dir)
    {
      //std::cout << "Open Group : " << gn << std::endl;
      v_gid = H5Gopen(v_fid, gna.c_str(), H5P_DEFAULT);
      v_did = H5Dopen(v_gid, dna.c_str(), H5P_DEFAULT);
    }
    else
    {
      v_did = H5Dopen(v_fid, dna.c_str(), H5P_DEFAULT);
    }

    hid_t v_datatype = H5Dget_type(v_did); /* datatype handle */
    H5T_class_t v_type_class = H5Tget_class(v_datatype);
    hid_t v_dataspace_id = H5Dget_space(v_did);
    int v_rank = H5Sget_simple_extent_ndims(v_dataspace_id);

    //printf("Constructor in H5Data: plist_id= %d, did=%d, fid=%d !\n", plist_id, did, fid);

    hid_t v_plist_cio_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(v_plist_cio_id, H5FD_MPIO_COLLECTIVE);
    std::vector<unsigned long long> v_offset, v_count;
    v_offset.resize(v_rank);
    v_count.resize(v_rank);
    for (int i = 0; i < v_rank; i++)
    {
      v_offset[i] = starta[i];
      v_count[i] = enda[i] - starta[i] + 1; //Starting from zero
    }

    hid_t v_memspace_id = H5Screate_simple(v_rank, &v_count[0], NULL);
    H5Sselect_hyperslab(v_dataspace_id, H5S_SELECT_SET, &v_offset[0], NULL, &v_count[0], NULL);

    int ret;
    switch (v_type_class)
    {
    case H5T_INTEGER:
      //ret = H5Dread(v_did, H5T_NATIVE_INT, v_memspace_id, v_dataspace_id, v_plist_cio_id, &dataa[0]);
      ret = H5Dread(v_did, H5T_NATIVE_SHORT, v_memspace_id, v_dataspace_id, v_plist_cio_id, &dataa[0]);
      break;
    case H5T_FLOAT:
      ret = H5Dread(v_did, H5T_NATIVE_FLOAT, v_memspace_id, v_dataspace_id, v_plist_cio_id, &dataa[0]);
      break;
    default:
      std::cout << "Unsupported datatype in  " << __FILE__ << __LINE__ << std::endl;
      exit(-1);
      break;
    }

    H5Sclose(v_memspace_id);
    H5Sclose(v_dataspace_id);
    H5Pclose(v_plist_id);
    H5Pclose(v_plist_cio_id);
    H5Dclose(v_did);
    if (v_gid > 0)
      H5Gclose(v_gid);
    H5Fclose(v_fid);

    if (ret < 0)
    {
      printf("Some error happen in reading HDF5 \n");
      exit(-1);
    }
    return 1;
  }

  //Read the file into FileVDSList
  bool getFileVDSList(std::string fileName, std::vector<std::string> &FileVDSList)
  {

    // Open the File
    std::ifstream in(fileName.c_str());

    // Check if object is valid
    if (!in)
    {
      std::cerr << "Cannot open the File : " << fileName << std::endl;
      return false;
    }

    std::string str;
    // Read the next line from File untill it reaches the end.
    while (std::getline(in, str))
    {
      // Line contains string of length > 0 then save it in vector
      if (str.size() > 0)
      {
        FileVDSList.push_back(str);
        //FileVDSPList.push_back(new H5Data<T>(str, gn_str, dn_str, 0, 0));
      }
    }
    //Close The File
    in.close();
    return true;
  }

  //Still assume row order
  template <class DataType>
  void InsertVDSIntoGlobalSpace(int v_index, std::vector<unsigned long long> &v_start, std::vector<unsigned long long> &v_end, std::vector<DataType> &v_data, std::vector<DataType> &g_data, std::vector<unsigned long long> &g_start, std::vector<unsigned long long> &g_end)
  {
    //for row
    unsigned long long v_vector_start, v_cols, v_rows, g_vector_start, g_cols;
    v_cols = v_end[1] - v_start[1] + 1;
    g_cols = g_end[1] - g_start[1] + 1;
    v_rows = v_end[0] - v_start[0] + 1;
    for (int i = 0; i < v_rows; i++)
    {
      g_vector_start = i * g_cols + v_index * v_cols;
      v_vector_start = i * v_cols;

      std::copy(v_data.begin() + v_vector_start, v_data.begin() + v_vector_start + v_cols, g_data.begin() + g_vector_start);
      /*for (int j = 0; j < v_cols; j++)
      {
        g_data[g_vector_start + j] = v_data[v_vector_start + j];
      }*/
    }
  }

  //https://support.hdfgroup.org/HDF5/Tutor/datatypes.html
  //mem_type: for read/write
  //disk_type: for create
  void find_h5_type(hid_t &mem_type, hid_t &disk_type)
  {
    if (std::is_same<T, int>::value)
    {
      mem_type = H5T_NATIVE_INT;
      disk_type = H5T_STD_I32LE;
    }
    else if (std::is_same<T, short>::value)
    {
      mem_type = H5T_NATIVE_SHORT;
      disk_type = H5T_STD_I16LE;
    }
    else if (std::is_same<T, long>::value)
    {
      mem_type = H5T_NATIVE_LONG;
      disk_type = H5T_STD_I64LE;
    }
    else if (std::is_same<T, long long>::value)
    {
      mem_type = H5T_NATIVE_LLONG;
      disk_type = H5T_STD_I64LE;
    }
    else if (std::is_same<T, unsigned int>::value)
    {
      mem_type = H5T_NATIVE_UINT;
      disk_type = H5T_STD_U32LE;
    }
    else if (std::is_same<T, unsigned short>::value)
    {
      mem_type = H5T_NATIVE_USHORT;
      disk_type = H5T_STD_U16LE;
    }
    else if (std::is_same<T, unsigned long>::value)
    {
      mem_type = H5T_NATIVE_ULONG;
      disk_type = H5T_STD_U64LE;
    }
    else if (std::is_same<T, unsigned long long>::value)
    {
      mem_type = H5T_NATIVE_ULLONG;
      disk_type = H5T_STD_U64LE;
    }
    else if (std::is_same<T, float>::value)
    {
      mem_type = H5T_NATIVE_FLOAT;
      disk_type = H5T_IEEE_F32LE;
      if (mpi_rank == 0)
      {
        std::cout << "H5 type is [float] , H5T_IEEE_F32LE =" << H5T_IEEE_F32LE << " \n";
      }
    }
    else if (std::is_same<T, double>::value)
    {
      mem_type = H5T_NATIVE_DOUBLE;
      disk_type = H5T_IEEE_F64LE;
      if (mpi_rank == 0)
      {
        std::cout << "H5 type is [double] \n";
      }
    }
    else
    {
      std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
      exit(-1);
    }
  }
};

#endif
