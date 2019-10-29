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

#ifndef ARRAY_UDF_GADATA
#define ARRAY_UDF_GADATA

#include "ga.h"
#include "macdecls.h"
#include "array_udf_h5.h"

int  ga_name_index = 0; //A global variable attached name string of GA
char ga_name_pre[]="AU-GAdataset-"; //Hence, each ga has name suchas "AU-GAdataset-0", 

extern double time_of_update_host_zone;

template <class T>
class GAData{
private:
  int  type;
  int  ga_handle = 0;
  int  rank;
  std::vector<int64_t> dims;
  std::vector<int64_t> chunk;
  std::vector<int64_t> overlap;
  std::vector<int64_t> lo, hi, ld;
  std::string fn;
  std::string gn;
  std::string dn;
  H5Data<T>     *nvs_f_handle = NULL; //Can be void * to support more IO drivers
  int ga_me;
  int ga_nprocs;
  //For write: create the file after the UDF is executed
  int                            vector_type_flag = 0;
  int                            filter_amount;
  int    mpi_rank;
  int    mpi_size;
  int ghost_flag = 0;

public:
  GAData(std::string fn_p, std::string gn_p, std::string dn_p){
    /*Initialize GA */
    //GA_Initialize();
    /*Initialize Memory Allocator (MA) */
    //Todo: how to figure out  stack and heap size
    //if(! MA_init(C_DBL, stack, heap) ) GA_Error("MA_init failed",stack+heap);
    ga_me     = GA_Nodeid();
    ga_nprocs = GA_Nnodes();
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    fn = fn_p; gn = gn_p; dn = dn_p;
  }
  
  GAData(){
    /*Initialize GA */
    //GA_Initialize();
    /*Initialize Memory Allocator (MA) */
    //Todo: how to figure out  stack and heap size
    //if(! MA_init(C_DBL, stack, heap) ) GA_Error("MA_init failed",stack+heap);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    ga_me     = GA_Nodeid();
    ga_nprocs = GA_Nnodes();
    
  }

  ~GAData(){
    if(mpi_rank == 0) {printf("Destructor in GA: ga_handle = %d !\n",ga_handle); fflush(stdout);}
    //GA_Terminate();
    if(ga_handle != 0) { GA_Destroy(ga_handle); ga_handle = 0; };
    if(nvs_f_handle != NULL) {delete nvs_f_handle; nvs_f_handle=NULL;}
  }
  
  std::vector<unsigned long long>  GetDimSize(){
    std::vector<unsigned long long> dims_t;
    for(int i=0; i< rank; i++)
      dims_t[i]=(unsigned long long)dims[i];
    return dims_t;
  }
  
  void DestroyGA(){
    if(ga_handle != 0) { GA_Destroy(ga_handle); ga_handle = 0; };
  }
  
  int CreateGA(int data_dims, std::vector<unsigned long long> &data_dims_size, AUType data_type_class, std::vector<int> data_overlap_size, std::vector<int> data_chunk_size_t){
    #ifdef DEBUG
    if(mpi_rank == 0){printf("Debug: create GA, in udf_ga. ghost size = (%d,) \n", data_overlap_size[0]); fflush(stdout);}
    #endif
    rank = data_dims;
    type = data_type_class;
    dims.resize(rank); lo.resize(rank); hi.resize(rank); ld.resize(rank-1); overlap.resize(rank); chunk.resize(rank);

    for(int i = 0;i < rank; i++){
      dims[i]= (int64_t)data_dims_size[i];
      overlap[i] = (int64_t)data_overlap_size[i];
      chunk[i] = (int64_t)data_chunk_size_t[i];
      if(data_overlap_size[i] != 0) ghost_flag = 1;
    }
    
    char name_t[128];
    sprintf(name_t, "%s%d\n", ga_name_pre, ga_name_index);
    ga_name_index++;
    if(mpi_rank == 0) {printf("Debug: create GA DONE, before create %d, name = %s \n", rank, name_t);}
    
    switch (data_type_class){
      case AU_INTEGER:
        if(ghost_flag == 0){
          ga_handle = NGA_Create64(C_INT, data_dims,  &(dims[0]),  name_t, &chunk[0]);
        }else{
          ga_handle = NGA_Create_ghosts64(C_INT, data_dims,  &(dims[0]), &(overlap[0]), name_t, &chunk[0]);
          if(mpi_rank == 0) {printf("Create GA array with ghost zone, voerlap size = (%d, %d), chunk_size = (%d, %d),  ga_hanlde=%d \n", overlap[0], overlap[1], chunk[0], chunk[1], ga_handle); fflush(stdout);}
        }
        if (!ga_handle){printf("Create GA failed !"); exit(-1);};
        break;
      case AU_FLOAT:
        //printf("Create Floated GA, dims[0]=%d \n", dims[0]);
        if(ghost_flag == 0){
          ga_handle = NGA_Create64(C_FLOAT, data_dims, &(dims[0]), name_t, &chunk[0]);
        }else{
          ga_handle = NGA_Create_ghosts64(C_FLOAT, data_dims, &(dims[0]), &(overlap[0]), name_t, &chunk[0]);
        }
        if (!ga_handle) {printf("Create GA failed !"); exit(-1);};
        break;
      default:
        std::cout  << "Unsupported datatype in array_udf_ga.h ! " << std::endl;
        exit(-1);
        break;
    }
    std::vector<int64_t> dist_lo, dist_hi;
    dist_lo.resize(rank); dist_hi.resize(rank);
    NGA_Distribution64(ga_handle, ga_me, &(dist_lo[0]), &(dist_hi[0]));

    {printf("Create ga_rank = %d, dist_lo = (%lld, %lld), dist_hi = (%lld, %lld)  \n", ga_me, dist_lo[0], dist_lo[1], dist_hi[0], dist_hi[1]); fflush(stdout);}
    //printf("Debug: create GA DONE, in udf_ga.\n");
    return 0;
  }
  

  int ReadData(std::vector<unsigned long long> start, std::vector<unsigned long long> end, std::vector<T> &data){
    
    std::vector<int64_t> dist_lo, dist_hi;
    dist_lo.resize(rank); dist_hi.resize(rank);
    NGA_Distribution64(ga_handle, ga_me, &(dist_lo[0]), &(dist_hi[0]));

    {printf("Read ga_rank = %d, dist_lo = (%lld, %lld), dist_hi = (%lld, %lld)  \n", ga_me, dist_lo[0], dist_lo[1], dist_hi[0], dist_hi[1]); fflush(stdout);}

    for (int i = 0; i < (rank-1); i++){
      ld[i] = (int64_t)(end[i]-start[i]+1);
    }

    for (int i = 0; i < (rank); i++){
      lo[i]=(int64_t)start[i];
      hi[i]=(int64_t)end[i];
      assert(lo[i] >= (dist_lo[i] - overlap[i]));
      assert(hi[i] <= (dist_hi[i] + overlap[i]));
    }

    
    /*
    if(mpi_rank == 136  ) { PrintVector("Read at 136: lo: ", dist_lo); PrintVector(" hi: ", dist_hi);  fflush(stdout);}
    if(mpi_rank == 165  ) { PrintVector("Read at 165: lo: ", dist_lo); PrintVector(" hi: ", dist_hi);  fflush(stdout);} */

    //if(mpi_rank < 3  ) {   fflush(stdout);}


    //std::vector<int64_t> dims_t, ld_t;
    //dims_t.resize(rank); ld_t.resize(rank);
    if(ghost_flag == 0){
      if (rank > 1){
        NGA_Get64(ga_handle, &(lo[0]),  &(hi[0]), &(data[0]),  &(ld[0]));
      }else{
        NGA_Get64(ga_handle, &(lo[0]),  &(hi[0]), &(data[0]),  NULL);
      }
    }else{
      double t_start_time =  MPI_Wtime();
      GAUpdateGhost();
      double t_end_time =  MPI_Wtime();
      time_of_update_host_zone = t_end_time - t_start_time;
      
      if (rank > 1){
        NGA_Get_ghost_block64(ga_handle, &(lo[0]),  &(hi[0]), &(data[0]),  &(ld[0]));
        //NGA_Access_ghosts(ga_handle,   );
        //NGA_Access_ghosts64(ga_handle, &dims_t[0], &data[0], &ld_t[0]);
      }else{
        NGA_Get_ghost_block64(ga_handle, &(lo[0]),  &(hi[0]), &(data[0]),  NULL);
        //NGA_Access_ghosts64(ga_handle, &dims_t[0], &data[0], &ld_t[0]);
      }
      //  if(mpi_rank == 0  )  { PrintVector("0   ld: ", ld); fflush(stdout);}
      //  if(mpi_rank == 136 ) { PrintVector("136 ld: ", ld); fflush(stdout);}
      // if(mpi_rank == 165 ) { PrintVector("165 ld: ", ld); fflush(stdout);}
    }
    //printf("Got %f\n", data[0]);
    //NGA_Release_ghosts(ga_handle);
    //NGA_Release_update_ghosts(ga_handle);
    
    if(mpi_rank == 0  ) { PrintVector("Start: ", start); PrintVector("End: ", end); printf("At rank %d, data vector's size = %d ,,, ", mpi_rank, data.size()); PrintVector(" Rank's read data: ", data);  fflush(stdout);}
    //if(mpi_rank == 136 ) { PrintVector("136 Start: ", start); PrintVector("136 End: ", end); PrintVector(" Rank 136's read data: ", data); fflush(stdout);}
    //if(mpi_rank == 165 ) { PrintVector("165 Start: ", start); PrintVector("165 End: ", end); PrintVector(" Rank 165's read data: ", data); fflush(stdout);}

    return 1;
  }

  int ReadDataStripingMem(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data, unsigned long long hym_start, unsigned long long hym_stride, unsigned long long hym_count){
    printf("No memspace based striping read for GA yet !\n");
    return 0;
  }

  int WriteData(std::vector<unsigned long long> start, std::vector<unsigned long long> end, std::vector<T> data){
    for (int i = 0; i < (rank-1); i++){
      ld[i] = (int64_t)(end[i]-start[i]+1);
    }
    
    for (int i = 0; i < (rank); i++){
      lo[i]=(int64_t)start[i]; hi[i]=(int64_t)end[i];
    }

    //if(mpi_rank == 0  ) { PrintVector("Start: ", start); PrintVector("End: ", end); PrintVector(" Rank 0's written data: ", data); fflush(stdout);}
    //if(mpi_rank == 136 ) { PrintVector("136 Start: ", start); PrintVector("136 End: ", end); PrintVector(" Rank 136's written data: ", data); fflush(stdout);}
    //if(mpi_rank == 165 ) { PrintVector("165 Start: ", start); PrintVector("165 End: ", end); PrintVector(" Rank 165's written data: ", data); fflush(stdout);}

    if (rank > 1){
      NGA_Put64(ga_handle,  &(lo[0]),  &(hi[0]), &(data[0]),  &(ld[0]));
    }else{
      NGA_Put64(ga_handle,  &(lo[0]),  &(hi[0]), &(data[0]),  NULL);
    }

    /*
    if (rank > 1){
      NGA_Get64(ga_handle,  &(lo[0]),  &(hi[0]), &(data[0]),  &(ld[0]));
    }else{
      NGA_Get64(ga_handle,  &(lo[0]),  &(hi[0]), &(data[0]),  NULL);
      }*/

    std::vector<int64_t> dist_lo, dist_hi;
    dist_lo.resize(rank); dist_hi.resize(rank);
    NGA_Distribution64(ga_handle, ga_me, &(dist_lo[0]), &(dist_hi[0]));

    {printf("Write ga_rank = %d, dist_lo = (%lld, %lld), dist_hi = (%lld, %lld)  \n", ga_me, dist_lo[0], dist_lo[1], dist_hi[0], dist_hi[1]); fflush(stdout);}


    //if(mpi_rank == 0  ) { PrintVector("0 Start: ", start); PrintVector("End: ", end); PrintVector("(Right after write) Rank 0's written data: ", data); fflush(stdout);}
    //if(mpi_rank == 136 ) { PrintVector("136 Start: ", start); PrintVector("136 End: ", end); PrintVector("(Right after write) Rank 136's written data: ", data); fflush(stdout);}
    //if(mpi_rank == 165 ) { PrintVector("165 Start: ", start); PrintVector("165 End: ", end); PrintVector("(Right after write) Rank 165's written data: ", data); fflush(stdout);}

    return 1;
  }

  int CreateNVSFile(int data_dims, std::vector<unsigned long long> &data_dims_size, int data_type_class){
    //printf("CreateNVSFile in ga.h \n");
    nvs_f_handle = new H5Data<T>(fn, gn,dn, 1); //1: delay_flag
    std::vector<int> overlap_int;
    overlap_int.resize(overlap.size());
    for(int i = 0; i < overlap.size(); i++){
      overlap_int[i] = (int)overlap[i];
    }
    
    nvs_f_handle->CreateNVSFile(data_dims, data_dims_size, data_type_class, overlap_int);
  }

  void Nonvolatile(){
    //To make sure all operatons on GA were done. 
    //GASync();
    //flush data onto disk
    std::vector<unsigned long long> start, end;
   
    NGA_Distribution64(ga_handle, ga_me, &(lo[0]), &(hi[0]));
    for (int i = 0; i < (rank-1); i++){
      ld[i]=(int64_t)(hi[i]-lo[i]+1);
    }

    int64_t mem_size = 1; start.resize(rank);end.resize(rank);
    for (int i = 0; i < rank; i++){
      mem_size =  mem_size * (hi[i]-lo[i] + 1);
      start[i] = lo[i];
      end[i] = hi[i];
    }

    //#ifdef DEBUG
    if(mpi_rank == 0 || mpi_rank== 136 || mpi_rank == 165){printf("Distributions before writing: MPI-rank %d, GA-rank %d, lo=(%lld, %lld), hi=(%lld, %lld) \n", mpi_rank, ga_me, lo[0], hi[0], lo[1], hi[1]); fflush(stdout);}
    //#endif

    std::vector<T> data;
    data.resize(mem_size);
    NGA_Get64(ga_handle, &(lo[0]), &(hi[0]), &(data[0]), &(ld[0]));
    //ReadData(start, end, data);

    if(mpi_rank == 0  ) { PrintVector("Start: ", start); PrintVector("End: ", end); PrintVector(" Rank 0's written data: ", data); fflush(stdout);}
    if(mpi_rank == 136 ) { PrintVector("136 Start: ", start); PrintVector("136 End: ", end); PrintVector(" Rank 136's written data: ", data); fflush(stdout);}
    if(mpi_rank == 165 ) { PrintVector("165 Start: ", start); PrintVector("165 End: ", end); PrintVector(" Rank 165's written data: ", data); fflush(stdout);}

    nvs_f_handle->WriteData(start, end, data);
    nvs_f_handle->Nonvolatile();
  }

  int GetTypeClass(){
    return type; 
  }
  
  std::string GetFileName(){
    return fn;
  }

  std::string GetGroupName(){
    return gn;
  }
  std::string GetDatasetName(){
    return dn;
  }

  void GASync(){
    GA_Sync(); 	  
  }

  void GAUpdateGhost(){
    if(ghost_flag == 1){
      //GA_Sync(); 	  
      GA_Update_ghosts(ga_handle);
      //GA_Sync(); 	  
      //NGA_Release_ghosts(ga_handle);
      //NGA_Release_update_ghosts(ga_handle);
    }
  }
  void SetFilterAmount(int p){
    filter_amount = p;
  }

};

#endif
