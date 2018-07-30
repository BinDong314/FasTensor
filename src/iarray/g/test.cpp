
#include "iarray.h"

//Please run this with 16 mpi ranks
int main(int argc, char *argv[]){

  MPI_Init(&argc, &argv);
  int mpi_size, mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&mpi_size); 
  MPI_Comm_rank(MPI_COMM_WORLD,&mpi_rank);

  //mpi_rank_global = mpi_rank;
  const int cart_dim = int( std::sqrt( double( mpi_size ) ) );
  if( cart_dim * cart_dim != mpi_size ) {
    std::cerr << "Numer of MPI tasks must be a perfect square" << std::endl;
    return 1; 
  }

  std::vector<unsigned long long> dims(2, 64);
  std::vector<int> ghost_size(2, 2); 
  std::vector<int> chunk_size(2, 16);
  int data_type = IA_INTEGER;

  int iArray_id = iArray_create(dims, ghost_size, chunk_size, data_type);

  std::vector<unsigned long long> global_start(2, 0);
  std::vector<unsigned long long> global_count(2, 0);

  std::vector<int> cart_grid_dim(2, 4);
  std::vector<int> cart_period(2, 1);
  const int cart_reorder = 0;
  MPI_Comm cart_comm;
  MPI_Cart_create(MPI_COMM_WORLD, 2, &cart_grid_dim[0], &cart_period[0], cart_reorder, &cart_comm);
  std::vector<int> cart_coord(2, -1);
  MPI_Cart_coords(cart_comm, mpi_rank, 2, &cart_coord[0]);

  
  std::ostringstream ss;   ss << cart_coord[ 0 ] << '_' << cart_coord[ 1 ];
  std::ofstream os( ss.str().c_str() );
  os << "Rank:  " << mpi_rank << std::endl
     << "Coord: " << cart_coord[ 0 ] << ", " << cart_coord[ 1 ] << std::endl;
  os << std::endl;
  os << "Array  dim: " << dims[0] <<  ", " << dims[1] << std::endl;
  os << "Chunk size: " << chunk_size[0] <<  ", " << chunk_size[1] << std::endl;
  os << "Ghost size: " << ghost_size[0] <<  ", " << ghost_size[1] << std::endl;
  
  os << std::endl << "Compute grid" << std::endl;
  PrintCartesianGrid( os, cart_comm, cart_dim, cart_dim);
  os << std::endl;

  //Start to write data
  int data_buf_size = 1;
  for(int i = 0; i < 2; i++){
    global_start[i] = cart_coord[i]*chunk_size[i];
    global_count[i] = chunk_size[i];
    data_buf_size = data_buf_size * global_count[i];
  }
  std::vector<float> data_buf(data_buf_size, mpi_rank);
  std::vector<float> data_buf_temp(data_buf_size, -1);
  
  os << "Global start : " << global_start[0] << ", " << global_start[1] << std::endl;
  os << "Global count : " << global_count[0] << ", " << global_count[1] << std::endl;

  os << "Array (initialized)" << std::endl;
  int data_buf_tmp_index  = 0;
  for(int i = 0; i < global_count[0]; i++ ){
    for(int j = 0; j < global_count[1]; j++ ){
      data_buf[data_buf_tmp_index] = mpi_rank*10 + data_buf_tmp_index;
      os << data_buf[data_buf_tmp_index] << ' ';
      data_buf_tmp_index = data_buf_tmp_index + 1;
    }
    os << std::endl;
  }
  os << std::endl;
  
  iArray_write_local(iArray_id, global_start, global_count, &(data_buf[0]));

  iArray_read_local(iArray_id, global_start, global_count, &(data_buf_temp[0]));

  os << "Array (read after writing)" << std::endl;
  data_buf_tmp_index  = 0;
  for(int i = 0; i < global_count[0]; i++ ){
    for(int j = 0; j < global_count[1]; j++ ){
      os << data_buf_temp[data_buf_tmp_index] << ' ';
      data_buf_tmp_index=data_buf_tmp_index+1;
    }
    os << std::endl;
   }
  os << std::endl;

  //Exchange data
  iArray_ghost_exchange(iArray_id);

  //Start to read back the data with ghost zone
  data_buf_size = 1;
  for(int i = 0; i < 2; i++){
    global_start[i] = cart_coord[i]*chunk_size[i];
    global_count[i] = chunk_size[i];
    if(global_start[i] + chunk_size[i] +  ghost_size[i] < dims[i]){
      global_count[i] = chunk_size[i] + ghost_size[i];
    }
    
    if(global_start[i]  >= 1){
      global_start[i] = global_start[i] -  ghost_size[i];
      global_count[i] = global_count[i] +  ghost_size[i];
    }
    data_buf_size = data_buf_size * global_count[i];
  }
  
  data_buf.resize(data_buf_size);
  std::fill(data_buf.begin(), data_buf.end(), -1);
  
  iArray_read_local(iArray_id, global_start, global_count, &(data_buf[0]));

  os << "Read Global start : " << global_start[0] << ", " << global_start[1] << std::endl;
  os << "Read Global count : " << global_count[0] << ", " << global_count[1] << std::endl;

  os << "Array after exchange: " << std::endl;
  data_buf_tmp_index  = 0;
  for(int i = 0; i < global_count[0]; i++ ){
    for(int j = 0; j < global_count[1]; j++ ){
      os << data_buf[data_buf_tmp_index] << ' ';
      data_buf_tmp_index = data_buf_tmp_index + 1;
    }
    os << std::endl;
  }
  os << std::endl;

  std::vector<unsigned long long> global_dims_test;
  std::vector<int> local_start_test, local_count_test; 
  
  iArray_get_global_dims(iArray_id, global_dims_test);
  iArray_get_local_dims(iArray_id, local_start_test, local_count_test);
  os << "Test: read global dims:  " << global_dims_test[0] << ", " << global_dims_test[1] << std::endl;
  os << "Test: read local start:  " << local_start_test[0] << ", " << local_start_test[1] << std::endl;
  os << "Test: read local count:  " << local_count_test[0] << ", " << local_count_test[1] << std::endl;

  iArray_delete(iArray_id);
  MPI_Finalize(); 
  
}
