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
  iArrayLocal      local_array;
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
int dump_debug_to_file(int iArray_id, std::string name_ext);
int iArray_get_global_chunk_size(int iArray_id, std::vector<int> &cs);
int iArray_get_global_ghost_size(int iArray_id, std::vector<int> &gs);
int iArray_get_global_rank(int iArray_id);
int iArray_write_local_all_directly(int iArray_id, void *buf, size_t buf_size);

int iArray_read_offset_direct(int iArray_handle, unsigned long long offset, void *data_at_offset, int type_size);

template < typename T >
void WriteLocalArray( T* pdata, const iArrayLocal& g, T *buf) {
    iArrayLocalAccessor< T > a( pdata, g );
    size_t buf_index  = 0;
    std::vector<int> coordinate;
    switch(g.rank){
      case 1:
        coordinate.resize(1);
        for( int row = 0; row != a.Layout().count[0]; ++row ) {
          coordinate[0] =  row;
          a(coordinate) = buf[buf_index];
            buf_index  = buf_index + 1;
        }
        break;
      case 2:
        coordinate.resize(2);
        for( int row = 0; row != a.Layout().count[0]; ++row ) {
          for( int column = 0; column != a.Layout().count[1]; ++column ) {
            coordinate[0] =  row;
            coordinate[1] =  column;
            a(coordinate) = buf[buf_index];
            buf_index  = buf_index + 1;
          }
        }
        break;
      case 3:
        coordinate.resize(3);
        for( int row = 0; row != a.Layout().count[0]; ++row ) {
          for( int column = 0; column != a.Layout().count[1]; ++column ) {
            for( int z = 0;     z != a.Layout().count[2]; ++z ) {
              coordinate[0] =  row;
              coordinate[1] =  column;
              coordinate[2]=z;
              a(coordinate) = buf[buf_index];
              buf_index  = buf_index + 1;
          }
        }
        }
        break;
    }
}

template < typename T >
void ReadLocalArray( T* pdata, const iArrayLocal& g, T *buf) {
    iArrayLocalAccessor< T > a( pdata, g );
    size_t buf_index  = 0;
    //int mpi_size, mpi_rank;
    //MPI_Comm_size(MPI_COMM_WORLD,&mpi_size); 
    //MPI_Comm_rank(MPI_COMM_WORLD,&mpi_rank);
    switch(g.rank){
      case 1:
        {std::vector<int> coordinate(1);
        //     coordinate.resize(1);
        for( int row = 0; row != a.Layout().count[0]; ++row ) {
          coordinate[0] =  row;
          buf[buf_index] =  a(coordinate);
          buf_index  = buf_index + 1;
        }}
        break;
      case 2:
        //coordinate.resize(2);
        {std::vector<int> coordinate(2);
        for( int row = 0; row != a.Layout().count[0]; ++row ) {
          for( int column = 0; column != a.Layout().count[1]; ++column ) {
            coordinate[0] =  row;             coordinate[1] =  column;
            buf[buf_index] =  a(coordinate);
            buf_index  = buf_index + 1;
          }
        }
        }
        break;
      case 3:
        //coordinate.resize(3);
        {std::vector<int> coordinate(3);
        for( int row = 0; row != a.Layout().count[0]; ++row ) {
          for( int column = 0; column != a.Layout().count[1]; ++column ) {
            for( int z = 0; z != a.Layout().count[2]; ++z ) {
              coordinate[0] = row;
              coordinate[1] = column;
              coordinate[2] = z;
              buf[buf_index] =  a(coordinate);
              buf_index  = buf_index + 1;
          }
          }
        }
        }
        break;
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
