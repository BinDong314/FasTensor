#ifndef _IN_MEMORY_ARRAY_H_
#define _IN_MEMORY_ARRAY_H_

#include "iarray-helper.h"
#include <map>


//See https://support.hdfgroup.org/ftp/HDF5/current/src/unpacked/src/H5Tpublic.h
//for reference 
typedef enum iArrayDataType {
    IA_NO_TYPE         = -1,  /*error                                      */
    IA_INTEGER          = 0,   /*integer types                              */
    IA_FLOAT            = 1,   /*floating-point types                       */
    IA_TIME             = 2,   /*date and time types                        */
    IA_STRING           = 3,   /*character string types                     */
    IA_BITFIELD         = 4,   /*bit field types                            */
    IA_OPAQUE           = 5,   /*opaque types                               */
    IA_COMPOUND         = 6,   /*compound types                             */
    IA_REFERENCE        = 7,   /*reference types                            */
    IA_ENUM		 = 8,	/*enumeration types                          */
    IA_VLEN		 = 9,	/*Variable-Length types                      */
    IA_NCLASSES                /*this must be last                          */
}iArrayDataType;


struct iArray{
  int mpi_rank;
  int mpi_size;
  MPI_Comm cartcomm;
  std::vector<int> mpi_cart_coords;

  int array_rank; //= g_dimension.size();
  int array_data_type;
  std::vector<unsigned long long> g_dimension;
  std::vector<int> g_ghost_size;
  std::vector<int> g_chunk_size;
  std::vector<int> l_start;  //g_dimension/g_chunk_sze * mpi_rank
  std::vector<int> l_count;  // = g_chunk_size
  std::vector<int> l_offset;  //l_start - g_ghost_size
  Array2D          local_array;
  void            *local_data;
};

typedef struct iArray iArray;


int iArray_create(std::vector<unsigned long long> dims, std::vector<int> ghost_size, std::vector<int> chunk_size, int data_type);
int iArray_read_local(int iArray_id, std::vector<unsigned long long> global_start, std::vector<unsigned long long> global_count, void *buf);
int iArray_write_local(int iArray_id, std::vector<unsigned long long> global_start, std::vector<unsigned long long> global_count, void *buf);
int iArray_ghost_exchange(int iArray_id);
int iArray_delete(int iArray_id);
int iArray_get_global_dims(int iArray_id, std::vector<unsigned long long> & dims);
int iArray_get_local_dims(int iArray_id, std::vector<int> &start, std::vector<int> &count);
int iArray_get_element_type(int iArray_id);


template < typename T >
void WriteLocalArray( T* pdata, const Array2D& g, T *buf) {
    Array2DAccessor< T > a( pdata, g );
    int buf_index  = 0;
    for( int row = 0; row != a.Layout().height; ++row ) {
        for( int column = 0; column != a.Layout().width; ++column ) {
          a( row, column) = buf[buf_index];
          buf_index  = buf_index + 1;
        }
    }
}

template < typename T >
void ReadLocalArray( T* pdata, const Array2D& g, T *buf) {
    Array2DAccessor< T > a( pdata, g );
    int buf_index  = 0;
    int mpi_size, mpi_rank;
    MPI_Comm_size(MPI_COMM_WORLD,&mpi_size); 
    MPI_Comm_rank(MPI_COMM_WORLD,&mpi_rank);
    
    for( int row = 0; row != a.Layout().height; ++row ) {
        for( int column = 0; column != a.Layout().width; ++column ) {
          buf[buf_index] = a(row, column);
          buf_index = buf_index + 1;
          //if(mpi_rank == 0 || mpi_rank == 5){
          //  std::cout << ", " << a(row, column) << std::endl;
          //}
        }
    }
}

template < typename T > MPI_Datatype CreateArrayElementType(){
  const bool T_is_int = std::is_same<T, int>::value;
  if(T_is_int) return MPI_INT;
  const bool T_is_float = std::is_same<T, float>::value;
  if(T_is_float) return MPI_FLOAT;
  return MPI_DATATYPE_NULL;
}


#endif
