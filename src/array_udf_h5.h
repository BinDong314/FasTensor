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

#include "hdf5.h"  //right now, we only have code for HDF5 
#include <string>
#include <iostream>
#include <vector>
#include "utility.h"

#define    DELAY_TO_CREATE 1
#define    SDS_UDF_COLLECTIVE_IO 1


using namespace std;

//These information are HDF5 related.
template <class T>
class H5Data{
private:
  hid_t                fid=-1, gid=-1, did=-1, dataspace_id=-1, memspace_id=-1;
  string               fn_str, gn_str, dn_str;
  std::vector<hsize_t> dims_out;
  int                  rank;
  hid_t                datatype;
  H5T_class_t          type_class;                 /* datatype class */
  hid_t                plist_id=-1, plist_cio_id=-1;
  int                  vector_type_flag = 0;
  int                  filter_amount;
public: 
  H5Data(){
  };

  //For write: create the file after the UDF is executed  
  H5Data(std::string fn, std::string gn, std::string dn, int delay_flag){
    if(delay_flag == 1){
      fn_str = fn;
      gn_str = gn;
      dn_str = dn;
    }

    if(is_vector_type<T>()) vector_type_flag = 1;

#ifdef SDS_UDF_COLLECTIVE_IO
    plist_cio_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist_cio_id, H5FD_MPIO_COLLECTIVE);
#else    
    plist_cio_id = H5P_DEFAULT;
#endif
  }

 //For read : if file exists, if open it
  H5Data(std::string fn, std::string gn, std::string dn){
    fn_str = fn;
    gn_str = gn;
    dn_str = dn;

    plist_id      = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

    if(is_vector_type<T>()) vector_type_flag = 1;

    fid  = H5Fopen(fn.c_str(), H5F_ACC_RDONLY, plist_id);
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

    //printf("Constructor in H5Data: plist_id= %d, did=%d, fid=%d !\n", plist_id, did, fid);

#ifdef SDS_UDF_COLLECTIVE_IO
    plist_cio_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist_cio_id, H5FD_MPIO_COLLECTIVE);
#else    
    plist_cio_id = H5P_DEFAULT;
#endif

  };

  H5Data(std::string fn, std::string gn, std::string dn, int data_dims, std::vector<unsigned long long> data_dims_size, std::vector<int> data_chunk_size){
    //Create a new file  with data_dims, data_dims_size, data_chunk_size
    
  };
  
  ~H5Data(){
    //printf("Destructor in H5Data: plist_id= %d, did=%d, fid=%d !\n", plist_id, did, fid);
    if(plist_id > 0)
      H5Pclose(plist_id);
    if(plist_cio_id > 0)
      H5Pclose(plist_cio_id);
    if(dataspace_id > 0)
      H5Sclose(dataspace_id);
    if(memspace_id > 0)
      H5Sclose(memspace_id);
    if(did > 0)
      H5Dclose(did);
    if(gid > 0)
      H5Gclose(gid);
    if(fid > 0){
      H5Fflush(fid, H5F_SCOPE_GLOBAL);H5Fclose(fid);
    }
    plist_id = -1; plist_cio_id=-1;dataspace_id=-1;memspace_id=-1;did=-1;gid=-1;fid=-1;
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

   int ReadDataAll(std::vector<T> &data){
     int ret;
     switch (type_class){
       case H5T_INTEGER:
         // ret = H5Dread(did, H5T_NATIVE_INT, memspace_id, dataspace_id, H5P_DEFAULT, &data[0]);
         ret = H5Dread(did, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, plist_cio_id,  &data[0]);
         //dataset.read(data, PredType::NATIVE_INT, memspace, dataspace);
         break;
      case H5T_FLOAT:
        //ret = H5Dread(did, H5T_NATIVE_FLOAT, memspace_id, dataspace_id, H5P_DEFAULT, &data[0]);
        ret = H5Dread(did, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, plist_cio_id, &data[0]);
        //dataset.read(data, PredType::NATIVE_FLOAT, memspace, dataspace);
        break;
       default:
        std::cout  << "Unsupported datatype in SDS_UDF_IO Class/Read Data ! " << std::endl;
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
  int ReadDataStripingMem(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data, unsigned long long hym_start, unsigned long long hym_stride, unsigned long long hym_count){
    std::vector<unsigned long long> offset, count, mem_count; 
    offset.resize(rank);  count.resize(rank);  mem_count.resize(rank);
    for (int i = 0; i < rank; i++ ){
      offset[i] = start[i];  count[i]  = end[i] - start[i] + 1; //Starting from zero
      mem_count[i] = count[i] * hym_stride;
    }
    H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, &offset[0], NULL, &count[0] , NULL);

    //std:: cout << "hym_start = " << hym_start << ", hym_stride = " << hym_stride << ", hym_count =" << hym_count << std::endl;
    memspace_id =  H5Screate_simple(rank, &mem_count[0], NULL);
    H5Sselect_hyperslab(memspace_id, H5S_SELECT_SET, &hym_start,  &hym_stride, &count[0], NULL);
    int ret;
    switch (type_class){
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
  
  int WriteDataVoidP(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data){
    std::vector<unsigned long long> offset, count;
    offset.resize(rank); //rank might be start.size()+1 when T is vector 
    count.resize(rank);
    for (int i = 0; i < start.size(); i++ ){
      offset[i] = start[i];
      count[i]  = end[i] - start[i] + 1; //Starting from zero
    }
    
    memspace_id =  H5Screate_simple(rank, &count[0], NULL);
    H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, &offset[0], NULL, &count[0], NULL);
   
    //
    //
    int ret;
    switch (type_class){
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
        std::cout  << "Unsupported datatype in SDS_UDF_IO Class/Read Data ! " << std::endl;
        exit(-1);
        break;
    }
    // free(int_new_data_ptr);
    //
    //if(int_new_data_ptr   != NULL){ printf("free int"); }
    
    if(ret < 0 ){
      std::cout  << "Error happened in writing data ! " << std::endl;
      exit(-1);
    }

   

    //flush data onto disk
    H5Fflush(fid, H5F_SCOPE_GLOBAL);
    return 1;
  }

    int WriteData(std::vector<unsigned long long> start, std::vector<unsigned long long> end, std::vector<T> data){
    std::vector<unsigned long long> offset, count;
    offset.resize(rank); //rank might be start.size()+1 when T is vector 
    count.resize(rank);
    for (int i = 0; i < start.size(); i++ ){
      offset[i] = start[i];
      count[i]  = end[i] - start[i] + 1; //Starting from zero
    }
    if(vector_type_flag == 1){
      offset[rank-1] = 0 ;
      count[rank-1]  = filter_amount; //data[0].size();
    }
    
    memspace_id =  H5Screate_simple(rank, &count[0], NULL);
    H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, &offset[0], NULL, &count[0], NULL);
   
    //
    //
    int ret;
    switch (type_class){
      case H5T_INTEGER:
        {
          if(vector_type_flag == 1){
            void *int_temp_ptr = vv2v(data);
            ret = H5Dwrite(did, H5T_NATIVE_INT, memspace_id, dataspace_id, plist_cio_id, int_temp_ptr);
            if(int_temp_ptr != NULL) free(int_temp_ptr);
          }else{
            ret = H5Dwrite(did, H5T_NATIVE_INT, memspace_id, dataspace_id, plist_cio_id, &data[0]);
          }
        }
        break;
      case H5T_FLOAT:
        {
          if(vector_type_flag == 1){
            void *float_new_data_ptr = vv2v(data);
            ret = H5Dwrite(did, H5T_NATIVE_FLOAT, memspace_id, dataspace_id, plist_cio_id, float_new_data_ptr);
            if(float_new_data_ptr != NULL) free(float_new_data_ptr);
          }else{
            ret = H5Dwrite(did, H5T_NATIVE_FLOAT, memspace_id, dataspace_id, plist_cio_id, &data[0]);
          }
        }
        break;
      default:
        std::cout  << "Unsupported datatype in SDS_UDF_IO Class/Read Data ! " << std::endl;
        exit(-1);
        break;
    }
    // free(int_new_data_ptr);
    //
    //if(int_new_data_ptr   != NULL){ printf("free int"); }
    
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
  
  int CreateNVSFile(int data_dims, std::vector<unsigned long long> &data_dims_size, int data_type_class, std::vector<int> data_overlap_size){
    //    printf("Debug: in h5 create file.\n");
    std::string root_dir = "/";
    plist_id      = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

    if(is_vector_type<T>()) vector_type_flag = 1;

    if(file_exist(fn_str.c_str()) == 0){
      fid  = H5Fcreate(fn_str.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
    }else{
      fid  = H5Fopen(fn_str.c_str(), H5F_ACC_RDWR, plist_id);
      if(fid < 0)
        fid  = H5Fcreate(fn_str.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
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
    if (gn_str != root_dir){
      if(H5Lexists(fid, gn_str.c_str(), H5P_DEFAULT) == 0){
        gid  = H5Gcreate(fid, gn_str.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      }else{
        gid  = H5Gopen(fid, gn_str.c_str(), H5P_DEFAULT);
      }
    }
    
    type_class  = (H5T_class_t)data_type_class;
    rank = data_dims;
    dims_out = data_dims_size;
    hid_t  ts_id;
    ts_id  = H5Screate_simple(rank, &dims_out[0], NULL);
     
    switch (type_class){
      case H5T_INTEGER:
        if (gn_str != root_dir){
          if(H5Lexists(gid, dn_str.c_str(), H5P_DEFAULT) == 0){
            did = H5Dcreate(gid, dn_str.c_str(), H5T_STD_I32BE, ts_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
          }else{
            did  = H5Dopen(gid,  dn_str.c_str(),  H5P_DEFAULT);
          }
        }else{
          if(H5Lexists(gid, dn_str.c_str(), H5P_DEFAULT) == 0){
            did = H5Dcreate(fid, dn_str.c_str(), H5T_STD_I32BE, ts_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
          }else{            
            did  = H5Dopen(gid,  dn_str.c_str(),  H5P_DEFAULT);
          } 
        }
        break;
      case H5T_FLOAT:
        if (gn_str != root_dir){
          if(H5Lexists(gid, dn_str.c_str(), H5P_DEFAULT) == 0){
            did = H5Dcreate(gid, dn_str.c_str(), H5T_IEEE_F32LE, ts_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
          }else{
            did  = H5Dopen(gid,  dn_str.c_str(),  H5P_DEFAULT);
          }
        }else{
          if(H5Lexists(fid, dn_str.c_str(), H5P_DEFAULT) == 0){
            did = H5Dcreate(fid, dn_str.c_str(), H5T_IEEE_F32LE, ts_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
          }else{
            did  = H5Dopen(fid,  dn_str.c_str(),  H5P_DEFAULT);
          }
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

  int GetRank(){
      return rank; 
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

  void Nonvolatile(){
    //flush data onto disk
    H5Fflush(fid, H5F_SCOPE_GLOBAL);
  }

  void SetFilterAmount(int p){
    filter_amount = p;
  }

  void ReOpenAsReadOnly(){
    if(plist_id > 0)
      H5Pclose(plist_id);
    if(plist_cio_id > 0)
      H5Pclose(plist_cio_id);
    if(dataspace_id > 0)
      H5Sclose(dataspace_id);
    if(memspace_id > 0)
      H5Sclose(memspace_id);
    if(did > 0)
      H5Dclose(did);
    if(gid > 0)
      H5Gclose(gid);
    if(fid > 0)
      H5Fclose(fid);

    plist_id      = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

    fid  = H5Fopen(fn_str.c_str(), H5F_ACC_RDONLY, plist_id);
    if(fid < 0){
      std::cout << "Error happens in open file " << fn_str << std::endl; exit(-1);
    }
    
    std::string root_dir = "/";
    if (gn_str != root_dir){
      //std::cout << "Open Group : " << gn << std::endl;
      gid  = H5Gopen(fid, gn_str.c_str(), H5P_DEFAULT);
      did  = H5Dopen(gid,  dn_str.c_str(),  H5P_DEFAULT);
    }else{
      did  = H5Dopen(fid,  dn_str.c_str(),  H5P_DEFAULT);
    }
    datatype       = H5Dget_type(did);     /* datatype handle */ 
    type_class     = H5Tget_class(datatype);  
    dataspace_id   = H5Dget_space(did);
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
};

#endif
