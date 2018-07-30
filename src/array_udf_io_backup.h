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

#include "hdf5.h"  //right now, we only have code for HDF5 
#include <string>
#include <iostream>
#include <vector>

#define    DELAY_TO_CREATE 1
#define    SDS_UDF_COLLECTIVE_IO 0

using namespace std;

//These information are HDF5 related.
template <class T>
class Data{
private:
  hid_t                fid=-1, gid=-1, did=-1, dataspace_id=-1, memspace_id=-1;
  string               fn_str, gn_str, dn_str;
  std::vector<hsize_t> dims_out;
  int                  rank;
  hid_t                datatype;
  H5T_class_t          type_class;                 /* datatype class */
  hid_t                plist_id=-1, plist_cio_id=-1;
public: 
  Data(){
  };

  //For write: create the file after the UDF is executed  
  Data(std::string fn, std::string gn, std::string dn, int delay_flag){
    if(delay_flag == 1){
      fn_str = fn;
      gn_str = gn;
      dn_str = dn;
    }

#ifdef SDS_UDF_COLLECTIVE_IO
    plist_cio_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist_cio_id, H5FD_MPIO_COLLECTIVE);
#else    
    plist_cio_id = H5P_DEFAULT;
#endif
  }

 //For read : if file exists, if open it
  Data(std::string fn, std::string gn, std::string dn){
    plist_id      = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

    fid  = H5Fopen(fn.c_str(), H5F_ACC_RDWR, plist_id);
    if(fid < 0){
      std::cout << "Error happens in open file " << fn << std::endl; exit(-1);
    }
    
    std::string root_dir = "/";
    if (gn != root_dir){
      //std::cout << "Open Group : " << gn << std::endl;
      gid  = H5Gopen(fid, gn.c_str(), H5P_DEFAULT);
      did  = H5Dopen(gid,  dn.c_str(),  H5P_DEFAULT);
    }else{
      did  = H5Dopen(fid,  dn.c_str(),  H5P_DEFAULT);
    }
    
    datatype       = H5Dget_type(did);     /* datatype handle */ 
    type_class     = H5Tget_class(datatype);  
    dataspace_id   = H5Dget_space(did);
    rank = H5Sget_simple_extent_ndims(dataspace_id);
    dims_out.resize(rank);
    H5Sget_simple_extent_dims(dataspace_id, &dims_out[0], NULL);
    
#ifdef SDS_UDF_COLLECTIVE_IO
    plist_cio_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist_cio_id, H5FD_MPIO_COLLECTIVE);
#else    
    plist_cio_id = H5P_DEFAULT;
#endif

  };

  Data(std::string fn, std::string gn, std::string dn, int data_dims, std::vector<unsigned long long> data_dims_size, std::vector<int> data_chunk_size){
    //Create a new file  with data_dims, data_dims_size, data_chunk_size
    
  };
  
  ~Data(){
    if(plist_id >= 0)
      H5Pclose(plist_id);
    if(plist_cio_id >= 0)
      H5Pclose(plist_cio_id);
    if(dataspace_id >= 0)
      H5Sclose(dataspace_id);
    if(memspace_id >= 0)
      H5Sclose(memspace_id);
    if(did >= 0)
      H5Dclose(did);
    if(gid >= 0)
      H5Gclose(gid);
    if(fid >= 0)
      H5Fclose(fid);
  }
  std::vector<unsigned long long>  GetDimSize(){
    return dims_out;
  }
  
  int ReadData(std::vector<unsigned long long> start, std::vector<unsigned long long> end, std::vector<T> &data){
    std::vector<unsigned long long> offset, count;
    offset.resize(rank);
    count.resize(rank);
    for (int i = 0; i < rank; i++ ){
      offset[i] = start[i];
      count[i]  = end[i] - start[i] + 1; //Starting from zero
    }
    memspace_id =  H5Screate_simple(rank, &count[0], NULL);
    H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, &offset[0], NULL, &count[0] , NULL);

    int ret;
    switch (type_class){
      case H5T_INTEGER:
       // ret = H5Dread(did, H5T_NATIVE_INT, memspace_id, dataspace_id, H5P_DEFAULT, &data[0]);
	ret = H5Dread(did, H5T_NATIVE_INT, memspace_id, dataspace_id, plist_cio_id, &data[0]);
       
        //dataset.read(data, PredType::NATIVE_INT, memspace, dataspace);
        break;
      case H5T_FLOAT:
       //ret = H5Dread(did, H5T_NATIVE_FLOAT, memspace_id, dataspace_id, H5P_DEFAULT, &data[0]);
        ret = H5Dread(did, H5T_NATIVE_FLOAT, memspace_id, dataspace_id, plist_cio_id, &data[0]);
        //dataset.read(data, PredType::NATIVE_FLOAT, memspace, dataspace);
        break;
      default:
        std::cout  << "Unsupported datatype in SDS_UDF_IO Class/Read Data ! " << std::endl;
        exit(-1);
        break;
    }
    
#ifdef DEBUG
    for (auto i = data.begin(); i != data.end(); ++i)
      std::cout << *i << ' ';
#endif    

    if(ret < 0){
      return -1;
    } 
    
    return 1;
  }

  int WriteData(std::vector<unsigned long long> start, std::vector<unsigned long long> end, std::vector<T> data){
    std::vector<unsigned long long> offset, count;
    offset.resize(rank);
    count.resize(rank);
    for (int i = 0; i < rank; i++ ){
      offset[i] = start[i];
      count[i]  = end[i] - start[i] + 1; //Starting from zero
    }
    memspace_id =  H5Screate_simple(rank, &count[0], NULL);
    H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, &offset[0], NULL, &count[0], NULL);
    
    int ret;
    switch (type_class){
      case H5T_INTEGER:
        ret = H5Dwrite(did, H5T_NATIVE_INT, memspace_id, dataspace_id, plist_cio_id, &data[0]);
        break;
      case H5T_FLOAT:
        ret = H5Dwrite(did, H5T_NATIVE_FLOAT, memspace_id, dataspace_id, plist_cio_id, &data[0]);
        break;
      default:
        std::cout  << "Unsupported datatype in SDS_UDF_IO Class/Read Data ! " << std::endl;
        exit(-1);
        break;
    }
    if(ret < 0 ){
      std::cout  << "Error happened in writing data ! " << std::endl;
      exit(-1);
    }

    //flush data onto disk
    H5Fflush(fid, H5F_SCOPE_GLOBAL);
    return 1;
  }

  void DisableCollectivIO(){
    H5Pclose(plist_cio_id);
    plist_cio_id = H5P_DEFAULT;
  }
  
  int CreateDiskFile(int data_dims, std::vector<unsigned long long> &data_dims_size, int data_type_class){
    std::string root_dir = "/";
    plist_id      = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

    fid  = H5Fcreate(fn_str.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
    if (gn_str != root_dir)
      gid  = H5Gcreate(fid, gn_str.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    type_class  = (H5T_class_t)data_type_class;
    rank = data_dims;
    dims_out = data_dims_size;
    hid_t  ts_id;
    ts_id  = H5Screate_simple(rank, &dims_out[0], NULL);
     
    switch (type_class){
      case H5T_INTEGER:
        if (gn_str != root_dir){
          did = H5Dcreate(gid, dn_str.c_str(), H5T_STD_I32BE, ts_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        }else{
          did = H5Dcreate(fid, dn_str.c_str(), H5T_STD_I32BE, ts_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        }
        break;
      case H5T_FLOAT:
        if (gn_str != root_dir){
          did = H5Dcreate(gid, dn_str.c_str(), H5T_IEEE_F32LE, ts_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        }else{
          did = H5Dcreate(fid, dn_str.c_str(), H5T_IEEE_F32LE, ts_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        }
        break;
      default:
        std::cout  << "Unsupported datatype in SDS_UDF_IO Class/Read Data ! " << std::endl;
        exit(-1);
        break;
    }
    //datatype  = H5Dget_type(did);     /* datatype handle */ 
    //type_class     = H5Tget_class(datatype);  
    dataspace_id = H5Dget_space(did);
  }

  int GetTypeClass(){
      return type_class; 
  }
  
  std::string GetFileName(){
    return fn_str;
  }

  std::string GetGroupName(){
    return gn_str;
  }
  std::string GetDatasetName(){
    return dn_str;
  }

};

#endif
