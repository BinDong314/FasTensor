
#include "iarray.h"
#include <string.h>
#include <math.h>
#include <cstring>
#include <iostream>
#include <cstdint>

std::map<int, iArray *> iArrayMap;


int iArray_create(std::vector<unsigned long long> dims, std::vector<int> ghost_size, std::vector<int> chunk_size, int data_type){

  //if(dims.size() != 2){
  //  std::cout << "iArray only supports 2D array now " << std::endl;
  //  exit(0);
  //}
  iArray *iArray_new =  new iArray();
  int     numtasks = 0; 
  int task = -1;

  MPI_Comm_size( MPI_COMM_WORLD, &numtasks );
  //const int DIM = int( std::sqrt( double( numtasks ) ) );
  //if( DIM * DIM != numtasks ) {
  //  std::cerr << "Numer of MPI tasks must be a perfect square" << std::endl;
  //  return 1; 
  //}
  
  std::vector< int > cart_dims;
  int each_rank_has_one_copy_flag = 1;
  for(int i = 0 ; i < dims.size(); i++){
    if( dims[i]  != chunk_size[i]) each_rank_has_one_copy_flag = 0;
  }

  std::vector< int > coords(dims.size(), -1 );
  if(each_rank_has_one_copy_flag != 1){
    cart_dims.resize(dims.size());
    for(int i = 0 ; i < dims.size(); i++){
      cart_dims[i] = ceil(dims[i] / chunk_size[i]);
    }

    std::vector< int > periods(dims.size(), 1 ); //periodic in both dimensions
    const int reorder = 0; //false - no reorder, is it actually used ?
    MPI_Comm cartcomm; // communicator for cartesian grid

    MPI_Cart_create( MPI_COMM_WORLD, dims.size(), &cart_dims[ 0 ], &periods[ 0 ], reorder, &cartcomm ); 
    iArray_new->cartcomm = cartcomm;
    MPI_Comm_rank( cartcomm, &task );
#ifdef DEBUG
    if(task == 0){
      std::cout << "Cart dims for iArray: ";
      for(int i = 0 ; i < dims.size(); i++){
        std::cout << cart_dims[i] << " , ";
      }
      std::cout << "\n";
    }
#endif
    MPI_Cart_coords( cartcomm, task, dims.size(), &coords[ 0 ] );
    iArray_new->mpi_rank = task;
    iArray_new->mpi_size = numtasks;
    iArray_new->mpi_cart_coords = coords;
  }else{
    //std::vector< int > coords(1, 0);
    iArray_new->cartcomm = MPI_COMM_WORLD;
    MPI_Comm_rank( MPI_COMM_WORLD, &task);
    iArray_new->mpi_rank = task;
    iArray_new->mpi_size = numtasks;
    coords[0]=task;
    iArray_new->mpi_cart_coords = coords;
  }
      
  iArray_new->g_dimension = dims;
  iArray_new->array_rank = dims.size();
  iArray_new->g_ghost_size = ghost_size;
  iArray_new->g_chunk_size = chunk_size;
  iArray_new->array_data_type  = data_type;

  iArray_new->l_start.resize(dims.size());
  iArray_new->l_count.resize(dims.size());
  iArray_new->l_offset.resize(dims.size());
  std::vector< int > local_size;
  size_t local_data_size_temp = 1;
  local_size.resize(dims.size());
  for(int i = 0; i < dims.size(); i++){
    if(chunk_size[i] < dims[i]){
      iArray_new->l_start[i] = chunk_size[i] * coords[i];
    }else if(chunk_size[i] == dims[i]){
      //each mpi rank has its own copy of the whole array
      iArray_new->l_start[i] = 0;
    }else{
      printf("new iArray now must have smaller chunk size than global dimension size !\n");
      //Todo: we may consider an extended array 
      exit(-1);
    }
    iArray_new->l_count[i] = chunk_size[i];
    iArray_new->l_offset[i] = iArray_new->l_start[i] - ghost_size[i];
    local_size[i] = chunk_size[i] + 2*ghost_size[i];
    local_data_size_temp = local_size[i] * local_data_size_temp;
  }

  //iArray_new->local_array = iArrayLocalAccessor( local_size[1], local_size[0],  local_size[1]);
  iArray_new->local_array = iArrayLocal(local_size, local_size);
  
  switch(data_type){
    case IA_INTEGER:
      iArray_new->local_data  = malloc(local_data_size_temp * sizeof(int));
      memset(iArray_new->local_data, local_data_size_temp * sizeof(int), 0);
      break;
    case IA_FLOAT:
      //printf("I am a float Iarray, size = %ul !\n", local_data_size_temp * sizeof(float));
      iArray_new->local_data  = malloc(local_data_size_temp * sizeof(float));
      memset(iArray_new->local_data, local_data_size_temp * sizeof(float), 0);
      break;
    default:
      printf("Not supported type in iArray_create \n");
      break;
  }
  
  int index = iArrayMap.size();
  iArrayMap[index] = iArray_new;
  return index;
}

iArray *find_iArray(int iArray_id){
  return iArrayMap[iArray_id];
}

int iArray_read_offset_direct(int iArray_handle, unsigned long long offset, void *data_at_offset, int type_size){
  iArray *iap = find_iArray(iArray_handle);
  memcpy(data_at_offset, iap->local_data + offset*type_size, type_size);
}

int iArray_read_local(int iArray_id, std::vector<unsigned long long> global_start, std::vector<unsigned long long> global_count, void *buf){
  //conver global start/count into local
  //At this point, we assume that read happen at local buffer
  iArray *iap = find_iArray(iArray_id);
  std::vector<int> local_start(iap->array_rank), local_count(iap->array_rank);
  //local_start.resize(); local_count.resize(iap->array_rank);
  for(int i = 0;  i < iap->array_rank; i++){
    local_start[i] = global_start[i] - iap->l_offset[i];
    local_count[i] = global_count[i];
  }

  //if(iap->mpi_rank == 0 || iap->mpi_rank == 5){
  //  std::cout << "mpi rank = " << iap->mpi_rank << ", local start = " << local_start[0]<<", " << local_start[1] << ", local count = " << local_count[0] << ", "<< local_count[1] << std::endl;
  // }
  //create region
  iArrayLocal local_region =SubArrayRegionContigious(iap->local_array, local_start, local_count);
#ifdef DEBUG
  if(iap->mpi_rank == 2){
    std::cout << "R global start: " << global_start[0] << ", " << global_start[1] << std::endl;
    std::cout << "R global count: " << global_count[0] << ", " << global_count[1] << std::endl;
    std::cout << "R local  ofset: " << iap->l_offset[0] << ", " << iap->l_offset[1] << std::endl;
    std::cout << "R local  start: " << local_start[0] << ", " << local_start[1] << std::endl;
    std::cout << "R local  count: " << local_count[0] << ", " << local_count[1] << std::endl;
  }
#endif
  //read the data
  switch(iap->array_data_type){
    case IA_FLOAT:
      ReadLocalArray<float>(static_cast<float *>(iap->local_data), local_region, static_cast<float *>(buf));
      break;
    case IA_INTEGER:
      ReadLocalArray<int>(static_cast<int     *>(iap->local_data), local_region, static_cast<int  *>(buf));
      break;
    default:
      printf("Un supported data type in iArray_read_local. ");
      break;
  }
    
}


int iArray_write_local(int iArray_id, std::vector<unsigned long long> global_start, std::vector<unsigned long long> global_count, void *buf){
 //conver global start/count into local
  //At this point, we assume that read happen at local buffer

  iArray *iap = find_iArray(iArray_id);
  std::vector<int> local_start, local_count;
  local_start.resize(iap->array_rank); local_count.resize(iap->array_rank);
  for(int i = 0;  i < iap->array_rank; i++){
    local_start[i] = global_start[i] - iap->l_offset[i];
    local_count[i] = global_count[i];
  }
  //create region
  iArrayLocal local_region =SubArrayRegionContigious(iap->local_array, local_start, local_count);

#ifdef DEBUG
  if(iap->mpi_rank == 2){
    std::cout << "W global start: " << global_start[0] << ", " << global_start[1] << std::endl;
    std::cout << "W global count: " << global_count[0] << ", " << global_count[1] << std::endl;
    std::cout << "W local  ofset: " << iap->l_offset[0] << ", " << iap->l_offset[1] << std::endl;
    std::cout << "W local  start: " << local_start[0] << ", " << local_start[1] << std::endl;
    std::cout << "W local  count: " << local_count[0] << ", " << local_count[1] << std::endl;
  }
#endif
  //write the data
  switch(iap->array_data_type){
    case IA_FLOAT:
      WriteLocalArray<float>(static_cast<float *>(iap->local_data), local_region, static_cast<float *>(buf));
      break;
    case IA_INTEGER:
      WriteLocalArray<int>(static_cast<int *>(iap->local_data), local_region, static_cast<int  *>(buf));
      break;
    default:
      printf("Un supported data type in iArray_write_local. ");
      break;
  }

}


//Copy the whole buf into local iArray_id
//Use this when "the ghost size = 0 && buf is well organized"
//In ArrayUDF, is is used to perform pre-load.
int iArray_write_local_all_directly(int iArray_id, void *buf, size_t buf_size){
  //conver global start/count into local
  //At this point, we assume that read happen at local buffer
  iArray *iap = find_iArray(iArray_id);
  //std::vector<int> local_start, local_count;
  std::memcpy(iap->local_data, buf, buf_size);
}

int iArray_ghost_exchange(int iArray_id){
  iArray *iap = find_iArray(iArray_id);
  typedef std::vector< TransferInfo > VTI;
  
  //int stencilWidth = iap->g_ghost_size[1] * 2;
  //int stencilHeight = iap->g_ghost_size[0] * 2;
  //Skip the exchange when there is no ghost_zone
  int has_ghost_zone_flag = 0;
  for(int i = 0; i < iap->array_rank ;  i++){
    if(iap->g_ghost_size[i] > 0) has_ghost_zone_flag = 1;
  }
  if(has_ghost_zone_flag == 0) return 0;
  
  std::pair< VTI, VTI > transferInfoArrays;

    //write the data
  switch(iap->array_data_type){
    case IA_FLOAT:
      transferInfoArrays  =CreateSendRecvArrays<float>(static_cast<float *>(iap->local_data), iap->cartcomm, iap->mpi_rank, iap->local_array, iap->g_ghost_size);
      break;
    case IA_INTEGER:
      transferInfoArrays  =CreateSendRecvArrays<int>(static_cast<int *>(iap->local_data), iap->cartcomm, iap->mpi_rank, iap->local_array, iap->g_ghost_size);
      break;
    default:
      printf("Un supported data type in iArray_ghost_exchange. ");
      break;
  }
  //dump_debug_to_file(iArray_id, "_before_exchange");
  ExchangeData( transferInfoArrays.first, transferInfoArrays.second );
  //dump_debug_to_file(iArray_id, "_after_exchange");
}


int iArray_delete(int iArray_id){
  iArray *iap = find_iArray(iArray_id);
  if(iap != NULL){
    if(iap->local_data != NULL) free(iap->local_data);
    delete(iap);
    iArrayMap[iArray_id] =  NULL;
  }
}


int iArray_get_global_rank(int iArray_id){
  iArray *iap = find_iArray(iArray_id);
  return iap->array_rank;
}

int iArray_get_global_dims(int iArray_id, std::vector<unsigned long long> &dims){
  iArray *iap = find_iArray(iArray_id);
  dims = iap-> g_dimension;
  return 0;
}

int iArray_get_global_ghost_size(int iArray_id, std::vector<int> &gs){
  iArray *iap = find_iArray(iArray_id);
  gs = iap->g_ghost_size;
  return 0;
}

int iArray_get_global_chunk_size(int iArray_id, std::vector<int> &cs){
  iArray *iap = find_iArray(iArray_id);
  cs = iap->g_chunk_size;
  return 0;
}


int iArray_get_local_dims(int iArray_id, std::vector<int> &start, std::vector<int> &count){
  iArray *iap = find_iArray(iArray_id);
  start = iap->l_start;
  count = iap->l_count;
  return 0;
}


int iArray_get_element_type(int iArray_id){
  iArray *iap = find_iArray(iArray_id);
  return iap->array_data_type;
}


int dump_debug_to_file(int iArray_id, std::string name_ext){
  iArray *iap = find_iArray(iArray_id);
  std::ostringstream ss;   ss << iap->mpi_cart_coords[0] << '_' << iap->mpi_cart_coords[1] << name_ext;
  std::ofstream os(ss.str().c_str());
  os << "Rank:  " << iap->mpi_rank << std::endl
     << "Coord: " << iap->mpi_cart_coords[0] << ", " << iap->mpi_cart_coords[1] << std::endl;
  os << std::endl;
  os << "Array  dim: " << iap->g_dimension[0] <<  ", "  << iap->g_dimension[1] << std::endl;
  os << "Chunk size: " << iap->g_chunk_size[0] <<  ", " << iap->g_chunk_size[1] << std::endl;
  os << "Ghost size: " << iap->g_ghost_size[0] <<  ", " << iap->g_ghost_size[1] << std::endl;
  os << "Local start: " << iap->l_start[0]      <<  ", " << iap->l_start[1] << std::endl;
  os << "Local count: " << iap->l_count[0]      <<  ", " << iap->l_count[1] << std::endl;
  
  os << std::endl << "Compute grid" << std::endl;
  const int cart_dim = int( std::sqrt( double( iap->mpi_size ) ) );
  PrintCartesianGrid( os, iap->cartcomm, cart_dim, cart_dim);
  os << std::endl;
  os << "Array's local data" << std::endl;
  int data_buf_tmp_index  = 0;
  float *fp; int *ip;

  if(iap->array_data_type == IA_FLOAT){
    fp = (float *)iap->local_data;
  }else if(iap->array_data_type == IA_INTEGER){
    ip = (int *)iap->local_data;
  }
  
  for(int i = iap->l_start[0]-1; i < iap->l_start[0] + iap->l_count[0]+1; i++ ){
    for(int j = iap->l_start[1]-1; j < iap->l_start[1] + iap->l_count[1]+1; j++ ){
      //data_buf[data_buf_tmp_index] = mpi_rank + data_buf_tmp_index;
      switch(iap->array_data_type){
        case IA_FLOAT:
          os << fp[data_buf_tmp_index] << ' ';
          break;
        case IA_INTEGER:
          os << ip[data_buf_tmp_index] << ' ';
          break;
        default:
          printf("Un supported data type in iArray_ghost_exchange. ");
          break;
      }
      data_buf_tmp_index = data_buf_tmp_index + 1;
    }
    os << std::endl;
  }
  os << std::endl;
}
