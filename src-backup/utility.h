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

#include <vector>
#include <type_traits>
#include <cstring>


using namespace std;

double time_address_cal = 0, row_major_order_cal = 0, pre_row_major_order_cal=0, data_access_time = 0, trail_run_time = 0;
inline unsigned long long RowMajorOrder(std::vector<unsigned long long> dsize, std::vector<unsigned long long> coordinate){
  unsigned long long offset = coordinate[0];
  int n = dsize.size();
  for (int i = 1; i < n; i++){
    offset = offset*dsize[i] + coordinate[i];
  }
  return offset;
}

inline std::vector<unsigned long long> RowMajorOrderReverse(unsigned long long offset, std::vector<unsigned long long> dsize){
  int n = dsize.size();
  std::vector<unsigned long long> original_coordinate;
  original_coordinate.resize(n);
  //unsigned long long reminder; 
  for (unsigned long long i = n-1; i >= 1; i--){
    //reminder = 
    original_coordinate[i] = offset % dsize[i];
    offset   = offset / dsize[i]; 
  }
  //Last dimenstion 
  original_coordinate[0] = offset;

#ifdef DEBUG
  std::cout << "offset = " << offset << std::endl;
  std::cout << "dsize [] = " << dsize[0] << ", " << dsize[1]<< std::endl;
  std::cout << "original_coordinate [] = " << original_coordinate[0] << ", " << original_coordinate[1]<< std::endl;
#endif
  
  return original_coordinate;
}


inline std::vector<unsigned long long> NextCoordinateAfterSkipWithinChunk(std::vector<unsigned long long> coordinate_at_chunk, std::vector<unsigned long long> skip_size, std::vector<unsigned long long> skiped_chunks_per_orig_chunk){
  int rank = coordinate_at_chunk.size();
  //Determine if coodinate_at_chunk is the first element of the the the skip chunk (not partition chunk)
  //TODO: we now assume each skip chunk starts with (0, 0, 0 , ...), i.e., the left-most element
  //      we can add some skip to chunk_coordinate_start inside a chunk.
  //      We also assume one cell per skiped chunk is processed
  std::vector<unsigned long long> chunk_coordinate, chunk_coordinate_start;
  unsigned long long total_chunks = 1;
  unsigned long long chunk_id;  int skip_flag = 0;
  chunk_coordinate.resize(rank); chunk_coordinate_start.resize(rank);
  for(int i = 0; i < rank; i++){
    //The coordinate of the skip chunk this coordinate belong 
    chunk_coordinate[i] = coordinate_at_chunk[i]/skip_size[i];
    total_chunks = total_chunks * skiped_chunks_per_orig_chunk[i];
  }
  //The id of the skip chunk that this choodinate belong
  chunk_id = RowMajorOrder(skiped_chunks_per_orig_chunk, chunk_coordinate);
  //chunk_coordinate = RowMajorOrderReverse(chunk_id, skip_chunks);
  for(int i = 0; i < rank; i++){
    chunk_coordinate_start[i] = chunk_coordinate[i] * skip_size[i];  //first cell's coodinate of this skip chunk
    if(chunk_coordinate_start[i] != coordinate_at_chunk[i])  //we only run on the first  element of this skip chunk
      skip_flag = 1;   //break;
  }

  //Current cell is the first one
  if(skip_flag == 0)
    return coordinate_at_chunk;

  //Go to skip this cell and jump to next cell at next skip chunk
  chunk_id = chunk_id + 1;
  if(chunk_id >= total_chunks){
    return {};
  }else{
    chunk_coordinate = RowMajorOrderReverse(chunk_id, skiped_chunks_per_orig_chunk);
    for(int i = 0; i < rank; i++)
      chunk_coordinate_start[i] = chunk_coordinate[i] * skip_size[i];  //first cell's coodinate of next skip chunk
    return chunk_coordinate_start;
  }
}


template<typename VT>
inline void PrintVector(std::string name, std::vector<VT> v){
  int n=v.size();
  std::cout <<  name << ": ";
  if(n > 30 ){
     for(int i = 0; i < 10; i++){
    	std::cout << v[i] << ",";
     }
     std::cout << " ... " ;
    for(int i = n - 10; i < n ; i++){
    	std::cout << v[i] << ",";
    }
  }else{
    for(int i = 0; i < n; i++){
    	std::cout << v[i] << ",";
     }
  }
  std::cout << std::endl;
}


template<typename T> struct is_vector : public std::false_type {};

template<typename T, typename A> struct is_vector<std::vector<T, A>> : public std::true_type {};


//1: vector type
//0: other types
template <typename T>
int is_vector_type(){
  return is_vector<T>{};
}


//http://coliru.stacked-crooked.com/view?id=e579b7d68ba42805cfde5a309e2a23e5-e54ee7a04e4b807da0930236d4cc94dc
//Usage:     std::cout << is_same_type<int, double>() << std::endl;
template<typename T, typename U>
struct is_same_type
{
    static const bool value = false;
};
template<typename T>
struct is_same_type<T, T>
{
    static const bool value = true;
};
template<typename T, typename U>
bool is_same_types() { return is_same_type<T, U>::value; }


//flat vector of vector to 1D vector
template<typename T>
void *vv2v(std::vector<std::vector<T>>  &v) {
  T *rv = (T*)malloc(v.size() * v[0].size() * sizeof(T)); //Assuming all rows have the same size
  if(rv == NULL){
    printf("Not enough memory (in *vv2v) !\n"); exit(-1);
  }
  for (unsigned i = 0; i < v.size(); i++)
    memcpy(rv + v[i].size() * i, &(v[i][0]), v[i].size() * sizeof(T));
  return (void *)rv;
}

template<typename T>
void *vv2v(std::vector<T>  &v) {
  return NULL;
}

#endif
