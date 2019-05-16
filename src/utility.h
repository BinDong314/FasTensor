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

#ifndef ARRAY_UDF_UTILITY
#define ARRAY_UDF_UTILITY

#include <complex> // std::complex
#include <vector>
#include <type_traits>
#include <cstring>
#include <cmath>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

double time_address_cal = 0, row_major_order_cal = 0, pre_row_major_order_cal = 0, data_access_time = 0, trail_run_time = 0;

#define ROW_MAJOR_ORDER_MACRO(dsize, dsize_len, coordinate, offset) \
  {                                                                 \
    offset = coordinate[0];                                         \
    for (int iii = 1; iii < dsize_len; iii++)                       \
    {                                                               \
      offset = offset * dsize[iii] + coordinate[iii];               \
    }                                                               \
  }

inline unsigned long long RowMajorOrder(std::vector<unsigned long long> dsize, std::vector<unsigned long long> coordinate)
{
  unsigned long long offset = coordinate[0];
  int n = dsize.size();
  for (int i = 1; i < n; i++)
  {
    offset = offset * dsize[i] + coordinate[i];
  }
  return offset;
}

#define ROW_MAJOR_ORDER_REVERSE_MACRO(offset, dsize, dsize_len, result_coord_v) \
  {                                                                             \
    unsigned long long temp_offset = offset;                                    \
    for (int iii = dsize_len - 1; iii >= 1; iii--)                              \
    {                                                                           \
      result_coord_v[iii] = temp_offset % dsize[iii];                           \
      temp_offset = temp_offset / dsize[iii];                                   \
    }                                                                           \
    result_coord_v[0] = temp_offset;                                            \
  }

inline std::vector<unsigned long long> RowMajorOrderReverse(unsigned long long offset, std::vector<unsigned long long> dsize)
{
  int n = dsize.size();
  std::vector<unsigned long long> original_coordinate;
  original_coordinate.resize(n);
  //unsigned long long reminder;
  for (unsigned long long i = n - 1; i >= 1; i--)
  {
    original_coordinate[i] = offset % dsize[i];
    offset = offset / dsize[i];
  }
  //Last dimenstion
  original_coordinate[0] = offset;

#ifdef DEBUG
  std::cout << "offset = " << offset << std::endl;
  std::cout << "dsize [] = " << dsize[0] << ", " << dsize[1] << std::endl;
  std::cout << "original_coordinate [] = " << original_coordinate[0] << ", " << original_coordinate[1] << std::endl;
#endif

  return original_coordinate;
}

inline std::vector<unsigned long long> NextCoordinateAfterSkipWithinChunk(std::vector<unsigned long long> coordinate_at_chunk, std::vector<unsigned long long> skip_size, std::vector<unsigned long long> skiped_chunks_per_orig_chunk)
{
  int rank = coordinate_at_chunk.size();
  //Determine if coodinate_at_chunk is the first element of the the the skip chunk (not partition chunk)
  //TODO: we now assume each skip chunk starts with (0, 0, 0 , ...), i.e., the left-most element
  //      we can add some skip to chunk_coordinate_start inside a chunk.
  //      We also assume one cell per skiped chunk is processed
  std::vector<unsigned long long> chunk_coordinate, chunk_coordinate_start;
  unsigned long long total_chunks = 1;
  unsigned long long chunk_id;
  int skip_flag = 0;
  chunk_coordinate.resize(rank);
  chunk_coordinate_start.resize(rank);
  for (int i = 0; i < rank; i++)
  {
    //The coordinate of the skip chunk this coordinate belong to
    chunk_coordinate[i] = coordinate_at_chunk[i] / skip_size[i];
    total_chunks = total_chunks * skiped_chunks_per_orig_chunk[i];
  }
  //The id of the skip chunk that this choodinate belong
  chunk_id = RowMajorOrder(skiped_chunks_per_orig_chunk, chunk_coordinate);
  //chunk_coordinate = RowMajorOrderReverse(chunk_id, skip_chunks);
  for (int i = 0; i < rank; i++)
  {
    chunk_coordinate_start[i] = chunk_coordinate[i] * skip_size[i]; //first cell's coodinate of this skip chunk
    if (chunk_coordinate_start[i] != coordinate_at_chunk[i])        //we only run on the first  element of this skip chunk
      skip_flag = 1;                                                //break;
  }

  //Current cell is the first one
  if (skip_flag == 0)
    return coordinate_at_chunk;

  //Go to skip this cell and jump to next cell at next skip chunk
  chunk_id = chunk_id + 1;
  if (chunk_id >= total_chunks)
  {
    return {};
  }
  else
  {
    chunk_coordinate = RowMajorOrderReverse(chunk_id, skiped_chunks_per_orig_chunk);
    for (int i = 0; i < rank; i++)
      chunk_coordinate_start[i] = chunk_coordinate[i] * skip_size[i]; //first cell's coodinate of next skip chunk
    return chunk_coordinate_start;
  }
}
//Return
// 1: yes, skip this point
// 0: no,  run computing on it
inline int SkipIt(std::vector<unsigned long long> coordinate_at_chunk, std::vector<unsigned long long> skip_size)
{
  int rank = coordinate_at_chunk.size();
  //Determine if coodinate_at_chunk is the first element of the the the skip chunk (not partition chunk)
  //TODO: we now assume each skip chunk starts with (0, 0, 0 , ...), i.e., the left-most element
  //      we can add some skip to chunk_coordinate_start inside a chunk.
  //      We also assume one cell per skiped chunk is processed
  std::vector<unsigned long long> chunk_coordinate, chunk_coordinate_start;
  //unsigned long long total_chunks = 1;
  //unsigned long long chunk_id;
  int skip_flag = 0;
  chunk_coordinate.resize(rank);
  chunk_coordinate_start.resize(rank);
  for (int i = 0; i < rank; i++)
  {
    //The coordinate of the skip chunk this coordinate belong to
    chunk_coordinate[i] = std::floor(coordinate_at_chunk[i] / skip_size[i]);
    chunk_coordinate_start[i] = chunk_coordinate[i] * skip_size[i]; //first cell's coodinate of this skip chunk
    if (chunk_coordinate_start[i] != coordinate_at_chunk[i])
    { //we only run on the first  element of this skip chunk
      skip_flag = 1;
      break;
    }
  }
  //The id of the skip chunk that this choodinate belong
  //chunk_id = RowMajorOrder(skiped_chunks_per_orig_chunk, chunk_coordinate);
  //chunk_coordinate = RowMajorOrderReverse(chunk_id, skip_chunks);
  //for(int i = 0; i < rank; i++){
  //  chunk_coordinate_start[i] = chunk_coordinate[i] * skip_size[i];  //first cell's coodinate of this skip chunk
  //  if(chunk_coordinate_start[i] != coordinate_at_chunk[i]){  //we only run on the first  element of this skip chunk
  //    skip_flag = 1;  // break;
  //  }
  //}
  return skip_flag;
}

void InsertIntoVirtualVectorVoidP(void *insert_vector, void *virtual_vector, int index, size_t length, int type_lenght)
{
}

//T2 should be a compund data structure
template <class T1, class T2>
void InsertIntoVirtualVector(const std::vector<T1> &insert_vector, std::vector<T2> &virtual_vector, int index)
{
  assert(insert_vector.size() == virtual_vector.size());
  size_t n = insert_vector.size();
  for (size_t i = 0; i < n; i++)
  {
    //memcpy(&(virtual_vector[i])+sizeof(T1)*index, &(insert_vector[i]), sizeof(T1));
    virtual_vector[i].set_value(index, insert_vector[i]);
  }
}

//T1: std::vector<float>
//T2: int
template <>
void InsertIntoVirtualVector(const std::vector<std::vector<float>> &insert_vector, std::vector<int> &virtual_vector, int index)
{
  printf("Not supported  at %s :%d \n", __FILE__, __LINE__);
  exit(-1);
}

//T1: std::vector<float>
//T2: float
template <>
void InsertIntoVirtualVector(const std::vector<std::vector<float>> &insert_vector, std::vector<float> &virtual_vector, int index)
{
  printf("Not supported  at %s :%d \n", __FILE__, __LINE__);
  exit(-1);
}

//T1: std::vector<std::complex<float>>
//T2: int
template <>
void InsertIntoVirtualVector(const std::vector<std::vector<std::complex<float>>> &insert_vector, std::vector<int> &virtual_vector, int index)
{
  printf("Not supported  at %s :%d \n", __FILE__, __LINE__);
  exit(-1);
}

template <>
void InsertIntoVirtualVector(const std::vector<float> &insert_vector, std::vector<float> &virtual_vector, int index)
{
  printf("Not supported  at %s :%d \n", __FILE__, __LINE__);
  exit(-1);
}

template <>
void InsertIntoVirtualVector(const std::vector<int> &insert_vector, std::vector<int> &virtual_vector, int index)
{
  printf("Not supported  at %s :%d \n", __FILE__, __LINE__);
  exit(-1);
}

template <>
void InsertIntoVirtualVector(const std::vector<float> &insert_vector, std::vector<int> &virtual_vector, int index)
{
  printf("Not supported  at %s :%d \n", __FILE__, __LINE__);
  exit(-1);
}

template <>
void InsertIntoVirtualVector(const std::vector<int> &insert_vector, std::vector<float> &virtual_vector, int index)
{
  printf("Not supported  at %s :%d \n", __FILE__, __LINE__);
  exit(-1);
}

template <typename VT>
inline void PrintVector(std::string name, std::vector<VT> v)
{
  int n = v.size();
  std::cout << name << ": ";
  if (n > 60)
  {
    for (int i = 0; i < 30; i++)
    {
      std::cout << v[i] << ",";
    }
    std::cout << " ... ";
    for (int i = n - 30; i < n; i++)
    {
      std::cout << v[i] << ",";
    }
  }
  else
  {
    for (int i = 0; i < n; i++)
    {
      std::cout << v[i] << ",";
    }
  }
  std::cout << std::endl;
}

//https://ideone.com/SDEgq
template <typename T>
struct extract_value_type
{
  typedef T value_type;
};

template <template <typename, typename...> class X, typename T, typename... Args>
struct extract_value_type<X<T, Args...>> //specialization
{
  typedef T value_type;
};

//Check the type of value for vector element
//Wether it is equal to T
// 0: not the same type
// 1: same type
template <typename C, typename T>
int is_base_type_T()
{
  typedef typename extract_value_type<C>::value_type value_type;
  //std::cout << "Result : " << std::is_same<value_type, T>::value << std::endl;
  //static_assert(std::is_same<value_type, T>::value, "mismatched");
  return std::is_same<value_type, T>::value;
}

template <typename T>
struct is_vector : public std::false_type
{
};

template <typename T, typename A>
struct is_vector<std::vector<T, A>> : public std::true_type
{
};

//1: vector type
//0: other types
template <typename T>
int is_vector_type()
{
  return is_vector<T>{};
}

//http://coliru.stacked-crooked.com/view?id=e579b7d68ba42805cfde5a309e2a23e5-e54ee7a04e4b807da0930236d4cc94dc
//Usage:     std::cout << is_same_type<int, double>() << std::endl;
template <typename T, typename U>
struct is_same_type
{
  static const bool value = false;
};
template <typename T>
struct is_same_type<T, T>
{
  static const bool value = true;
};
template <typename T, typename U>
bool is_same_types() { return is_same_type<T, U>::value; }

//flat vector of vector to 1D vector
template <typename T>
void *vv2v(std::vector<std::vector<T>> &v)
{
  T *rv = (T *)malloc(v.size() * v[0].size() * sizeof(T)); //Assuming all rows have the same size
  if (rv == NULL)
  {
    printf("Not enough memory (in *vv2v) !\n");
    exit(-1);
  }
  for (unsigned i = 0; i < v.size(); i++)
  {
    memcpy(rv + v[i].size() * i, &(v[i][0]), v[i].size() * sizeof(T));
  }

  //printf("size = %d , %f , %f, %f \n", v[0].size(), rv[0], rv[1], rv);

  return (void *)rv;
}

template <typename T>
void *vv2v(std::vector<T> &v)
{
  printf("Not supported %s:%d \n", __FILE__, __LINE__);
  return NULL;
}

//flat vector of vector to 1D vector
// direction specify row-major or colum major
//  0: row major, e.g., for a 2 by 2 vector
//     v[0][0] v[0][1] v[1][0] v[1][1]
//  1: column major, e.g., for a 2 by 2 vector
//     v[0][0] v[1][0] v[0][1] v[1][1]
//Lots of todo:
//  We need a dimension infomation to guide flat when v is flat with different directions
//  Basically, we need to support direction
template <typename T>
void *flat_vector(std::vector<std::vector<T>> &v, int direction)
{
  if (direction == 1)
  {
    //printf("In new fat !\n");
    T *rv = (T *)malloc(v.size() * v[0].size() * sizeof(T)); //Assuming all rows have the same size
    if (rv == NULL)
    {
      printf("Not enough memory (in *vv2v) !\n");
      exit(-1);
    }
    for (unsigned i = 0; i < v[0].size(); i++)
    {
      for (unsigned j = 0; j < v.size(); j++)
      {
        memcpy(rv + v.size() * i + j, &(v[j][i]), sizeof(T));
      }
    }
    return (void *)rv;
  }
  else
  {
    return vv2v(v);
  }

  //printf("size = %d , %f , %f, %f \n", v[0].size(), rv[0], rv[1], rv);
}

template <typename T>
void *flat_vector(std::vector<T> &v, int direction)
{
  printf("Not supported %s:%d \n", __FILE__, __LINE__);
  return NULL;
}

template <typename T>
void *vv2v_empty(std::vector<std::vector<T>> &v)
{
  if (v[0].size() == 0)
  {
    T *rv = (T *)malloc(v.size() * sizeof(T)); //Assuming all rows have the same size
    if (rv == NULL)
    {
      printf("Not enough memory (in *vv2v) !\n");
      exit(-1);
    }
    return (void *)rv;
  }
  else
  {
    return NULL;
  }
}

template <typename T>
void *vv2v_empty(std::vector<T> &v)
{
  return NULL;
}

//return
//1 file exists
//0 file does not exist
int file_exist(const char *filename)
{
  struct stat buffer;
  return (stat(filename, &buffer) == 0);
}

double au_timer_global__inside_use = 0;
void au_time_start()
{
  au_timer_global__inside_use = MPI_Wtime();
}

void au_time_elap(char *info_str)
{

  double time_per_rank = MPI_Wtime() - au_timer_global__inside_use;
  int mpi_rank, mpi_size;
  double time_max, time_min, time_sum;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

  MPI_Allreduce(&time_per_rank, &time_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(&time_per_rank, &time_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
  MPI_Allreduce(&time_per_rank, &time_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  if (mpi_rank == 0)
  {
    printf(" %s:max=%f, min=%f, ave=%f, rank 0=%f\n ", info_str, time_max, time_min, time_sum / mpi_size, time_per_rank);
    fflush(stdout);
  }

  //reset to current time
  au_timer_global__inside_use = MPI_Wtime();
}

#endif
