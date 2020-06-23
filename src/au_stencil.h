
/*
 *ArrayUDF Copyright (c) 2017, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy).  All rights reserved.

 *If you have questions about your rights to use or distribute this software, please contact Berkeley Lab's Innovation & Partnerships Office at  IPO@lbl.gov.

* NOTICE. This Software was developed under funding from the U.S. Department of Energy and the U.S. Government consequently retains certain rights. As such, the U.S. Government has been granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software to reproduce, distribute copies to the public, prepare derivative works, and perform publicly and display publicly, and to permit other to do so. 
 */

/**
 *
 * Email questions to dbin@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#ifndef SDS_UDF_STENCIL
#define SDS_UDF_STENCIL

#include "mpi.h"
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include "au_utility.h"
#include "au_array_iterator.h"
#include "au_utility.h"

using namespace std;
extern double time_address_cal, row_major_order_cal, pre_row_major_order_cal, data_access_time;

template <class T>
class Stencil
{
private:
  T value;
  int has_set_output_value_flag = false;
  std::vector<unsigned long long> my_location; //This is the coodinate with overlapping
  std::vector<unsigned long long> global_coordinate;
  unsigned long long global_coordinate_lineared;

  unsigned long long my_g_location_rm; //lineared form of my coordinates in original big array.
  unsigned long long chunk_id;
  T *chunk_data_pointer = NULL;
  unsigned long long chunk_data_size = 1;
  unsigned long long chunk_data_size_no_ol = 1;
  unsigned long long my_offset_no_ol;             //for hist
  std::vector<unsigned long long> chunk_dim_size; //This is the size with over-lapping
  int dims;
  std::vector<int> coordinate_shift;
  std::vector<unsigned long long> coordinate;
  int trail_run_flag = 0; //When rail_run_flag = 1, it records the maximum overlap size
  int padding_value_set_flag = 0;
  T padding_value;
  int mpi_rank, mpi_size;

public:
  //For test only
  Stencil(){};
  Stencil(T v)
  {
    value = v;
  };

  // For trail run
  Stencil(int dims_input, T *chunk)
  {
    dims = dims_input;
    coordinate_shift.resize(dims_input);
    coordinate.resize(dims_input);
    set_trail_run_flag();
    chunk_data_pointer = chunk;

#ifdef DEBUG
    MpiRankSize(&mpi_rank, &mpi_size);
#endif
  }

  //For production
  Stencil(unsigned long long my_offset, T *chunk, std::vector<unsigned long long> &my_coordinate, std::vector<unsigned long long> &chunk_size)
  {
#ifdef DEBUG
    MpiRankSize(&mpi_rank, &mpi_size);
    std::cout << "my value =" << value << ", coordinate = (" << my_coordinate[0] << ", " << my_coordinate[1] << " )" << std::endl;
#endif
    chunk_data_pointer = chunk;
    dims = chunk_size.size();
    chunk_dim_size.resize(dims);
    my_location.resize(dims);
    coordinate_shift.resize(dims);
    coordinate.resize(dims);
    chunk_data_size = 1;
    for (int i = 0; i < dims; i++)
    {
      chunk_data_size = chunk_data_size * chunk_size[i];
      chunk_dim_size[i] = chunk_size[i];
      my_location[i] = my_coordinate[i];
    }

    chunk_data_size = chunk_data_size - 1;
    if (my_offset > chunk_data_size)
    {
      std::cout << "Error in intializing Stencil(). my_offset  = " << my_offset << ", chunk_data_size = " << chunk_data_size << std::endl;
      exit(-1);
    }
    else
    {
      value = chunk[my_offset];
    }
  };

  ~Stencil()
  {
    /*if (coordinate_shift != NULL)
      free(coordinate_shift);
    if (coordinate != NULL)
      free(coordinate);*/
  }

  //The followings are inline functions to overload () operator on Stencils
  //It accepts offsets (i1, i2, i3, ...) and return the values at these offsets.
  //So fat, we define the function for 1D, 2D, 3D seperately.
  //TO DO: more flexiable arguments to support any dimensions.

  //1D
  inline T &operator()(const int i1) const
  {

    std::vector<int> coordinate_shift(1);
    std::vector<unsigned long long> coordinate(1);

    coordinate_shift[0] = i1;
    for (int i = 0; i < 1; i++)
    {
      if (coordinate_shift[i] == 0)
      {
        coordinate[i] = my_location[i];
      }
      else if (coordinate_shift[i] > 0)
      {
        coordinate[i] = my_location[i] + coordinate_shift[i];
        if (coordinate[i] >= chunk_dim_size[i]) //Check boundary :: Be careful with size overflow
          coordinate[i] = chunk_dim_size[i] - 1;
      }
      else
      {
        coordinate[i] = -coordinate_shift[i]; //Convert to unsigned long long
        if (my_location[i] <= coordinate[i])
        {
          coordinate[i] = 0;
        }
        else
        {
          coordinate[i] = my_location[i] - coordinate[i]; //Check boundary :: Be careful with size overflow
        }
      }
    }
    if (coordinate[0] <= chunk_data_size)
    {
      return chunk_data_pointer[coordinate[0]];
    }
    else
    {
      std::cout << "Error in operator(). shift_offset=" << coordinate[0] << ",chunk_data_size=" << chunk_data_size << ",chunk_dim_size[]=" << chunk_dim_size[0] << "," << chunk_dim_size[1] << "," << chunk_dim_size[2] << ",my_location[]=" << my_location[0] << "," << my_location[1] << "," << my_location[2] << ", coordinate_shift[]=" << coordinate_shift[0] << "," << coordinate_shift[1] << "," << coordinate_shift[2] << std::endl;
      exit(-1);
    }
  }

  //2D
  inline T operator()(const int i1, const int i2) const
  {
    std::vector<int> coordinate_shift(2);
    std::vector<unsigned long long> coordinate(2);

    if (trail_run_flag == 1)
    {
      if (std::abs(i1) > coordinate_shift[0])
        coordinate_shift[0] = std::abs(i1);
      if (std::abs(i2) > coordinate_shift[1])
        coordinate_shift[1] = std::abs(i2);
      return chunk_data_pointer[0];
    }

    if (i1 == 0 && i2 == 0)
      return value;

    //if(mpi_rank == 0) PrintVector("At rank 0, in (), chunk_dim_size=",  chunk_dim_size);
    coordinate_shift[0] = i1;
    coordinate_shift[1] = i2;
    for (int i = 0; i < 2; i++)
    {
      //if(chunk_dim_size[i] > 768 ) {printf("At mpi_rank %d , Warning in runing () operator's chunk size =  %lld \n ", mpi_rank, chunk_dim_size[i]); fflush(stdout); exit(-1);}
      if (coordinate_shift[i] == 0)
      {
        coordinate[i] = my_location[i];
      }
      else if (coordinate_shift[i] > 0)
      {
        coordinate[i] = my_location[i] + coordinate_shift[i];
        if (coordinate[i] >= chunk_dim_size[i]) //Check boundary :: Be careful with size overflow
          coordinate[i] = chunk_dim_size[i] - 1;
      }
      else
      {
        coordinate[i] = -coordinate_shift[i]; //Convert to unsigned long long
        if (my_location[i] <= coordinate[i])
        {
          coordinate[i] = 0;
        }
        else
        {
          coordinate[i] = my_location[i] - coordinate[i]; //Check boundary :: Be careful with size overflow
        }
      }
    }
    unsigned long long shift_offset = coordinate[0];
    for (int i = 1; i < 2; i++)
    {
      shift_offset = shift_offset * chunk_dim_size[i] + coordinate[i];
    }
    if (shift_offset <= chunk_data_size)
    {
      return chunk_data_pointer[shift_offset];
    }
    else
    {
      std::cout << "MPI Rank = " << mpi_rank << ", Error in 2D operator(). shift_offset=" << shift_offset << ",chunk_data_size=" << chunk_data_size << ",chunk_dim_size[]=" << chunk_dim_size[0] << "," << chunk_dim_size[1] << ", my_location[]=" << my_location[0] << "," << my_location[1] << ", coordinate_shift[]=" << coordinate_shift[0] << "," << coordinate_shift[1] << std::endl;
      exit(-1);
    }
  }

  //3D
  inline T &operator()(const int i1, const int i2, const int i3) const
  {
    std::vector<int> coordinate_shift(3);
    std::vector<unsigned long long> coordinate(3);

    if (trail_run_flag == 1)
    {
      if (std::abs(i1) > coordinate_shift[0])
        coordinate_shift[0] = std::abs(i1);
      if (std::abs(i2) > coordinate_shift[1])
        coordinate_shift[1] = std::abs(i2);
      if (std::abs(i3) > coordinate_shift[2])
        coordinate_shift[2] = std::abs(i3);

      return chunk_data_pointer[0];
    }

    coordinate_shift[0] = i1;
    coordinate_shift[1] = i2;
    coordinate_shift[2] = i3;
    for (int i = 0; i < 3; i++)
    {
      if (coordinate_shift[i] == 0)
      {
        coordinate[i] = my_location[i];
      }
      else if (coordinate_shift[i] > 0)
      {
        coordinate[i] = my_location[i] + coordinate_shift[i];
        if (coordinate[i] >= chunk_dim_size[i]) //Check boundary :: Be careful with size overflow
          coordinate[i] = chunk_dim_size[i] - 1;
      }
      else
      {
        coordinate[i] = -coordinate_shift[i]; //Convert to unsigned long long
        if (my_location[i] <= coordinate[i])
        {
          coordinate[i] = 0;
        }
        else
        {
          coordinate[i] = my_location[i] - coordinate[i]; //Check boundary :: Be careful with size overflow
        }
      }
    }
    unsigned long long shift_offset = coordinate[0];
    for (int i = 1; i < 3; i++)
    {
      shift_offset = shift_offset * chunk_dim_size[i] + coordinate[i];
    }
    if (shift_offset <= chunk_data_size)
    {
      return chunk_data_pointer[shift_offset];
    }
    else
    {
      std::cout << "Error in operator(). shift_offset=" << shift_offset << ",chunk_data_size=" << chunk_data_size << ",chunk_dim_size[]=" << chunk_dim_size[0] << "," << chunk_dim_size[1] << "," << chunk_dim_size[2] << ",my_location[]=" << my_location[0] << "," << my_location[1] << "," << my_location[2] << ", coordinate_shift[]=" << coordinate_shift[0] << "," << coordinate_shift[1] << "," << coordinate_shift[2] << std::endl;
      exit(-1);
    }
  }

  inline T operator()(int i1, int i2, int i3, int i4)
  {
    std::vector<int> coordinate_shift(4);
    std::vector<unsigned long long> coordinate(4);

    size_t shift_offset = 1;
    std::cout << "At ArrayUDF_Stencil.h: we does not have the code for 4D offsets yet. You can DIY it by coping above 3D codes." << std::endl;
    exit(0);
  }

  template <class TO>
  inline void operator=(TO &others)
  {
    value = others;
    has_set_output_value_flag = true;
  }

  bool has_output_value()
  {
    return has_set_output_value_flag;
  }

  inline std::vector<T> Read(std::vector<int> &start_offset, std::vector<int> &end_offset) const
  {
    std::vector<T> rv;
    int rank_temp = start_offset.size();
    std::vector<size_t> start_offset_size_t, end_offset_size_t;
    std::vector<unsigned long long> count_size_t;
    start_offset_size_t.resize(rank_temp);
    end_offset_size_t.resize(rank_temp);
    count_size_t.resize(rank_temp);

    size_t n = 1;
    for (int i = 0; i < rank_temp; i++)
    {
      count_size_t[i] = (end_offset[i] - start_offset[i] + 1);
      n = n * count_size_t[i];
      assert(start_offset[i] >= 0); //ArrayIterator any only process positive offset
      assert(end_offset[i] >= 0);
      start_offset_size_t[i] = start_offset[i];
      end_offset_size_t[i] = end_offset[i] + 1;
    }

    if (count_size_t == chunk_dim_size)
    {
      std::vector<T> rv2(chunk_data_pointer, chunk_data_pointer + n);
      //std::cout << "read all !" << std::endl;
      return rv2;
    }

    rv.resize(n);

    size_t offset, rv_offset = 0;
    std::vector<size_t> coordinate;
    coordinate.resize(rank_temp);
    for (ArrayIterator<size_t> c(start_offset_size_t, end_offset_size_t); c; ++c)
    {
      //PrintVector("ArrayIterator_c: ", c);
      for (int j = 0; j < rank_temp; j++)
      {
        coordinate[j] = my_location[j] + c[j];
        if (coordinate[j] >= chunk_dim_size[j]) //Check boundary :: Be careful with size overflow
          coordinate[j] = chunk_dim_size[j] - 1;
      }

      ROW_MAJOR_ORDER_MACRO(chunk_dim_size, rank_temp, coordinate, offset);
      assert(offset <= chunk_data_size);
      rv[rv_offset] = chunk_data_pointer[offset];
      rv_offset = rv_offset + 1;
    }

    return rv;
  }

  int Write(std::vector<int> &start_offset, std::vector<int> &end_offset, std::vector<T> &data) const
  {
    int rank_temp = start_offset.size();
    std::vector<size_t> start_offset_size_t, end_offset_size_t;
    start_offset_size_t.resize(rank_temp);
    end_offset_size_t.resize(rank_temp);
    size_t n = 1;
    for (int i = 0; i < rank_temp; i++)
    {
      n = n * (end_offset[i] - start_offset[i] + 1);
      assert(start_offset[i] >= 0); //ArrayIterator any only process positive offset
      assert(end_offset[i] >= 0);
      start_offset_size_t[i] = start_offset[i];
      end_offset_size_t[i] = end_offset[i] + 1;
    }

    size_t offset, rv_offset = 0;
    std::vector<size_t> coordinate;
    coordinate.resize(rank_temp);
    for (ArrayIterator<size_t> c(start_offset_size_t, end_offset_size_t); c; ++c)
    {
      for (int j = 0; j < rank_temp; j++)
      {
        coordinate[j] = my_location[j] + c[j];
        if (coordinate[j] >= chunk_dim_size[j]) //Check boundary :: Be careful with size overflow
          coordinate[j] = chunk_dim_size[j] - 1;
      }
      ROW_MAJOR_ORDER_MACRO(chunk_dim_size, rank_temp, coordinate, offset);
      assert(offset <= chunk_data_size);
      chunk_data_pointer[offset] = data[rv_offset];
      rv_offset = rv_offset + 1;
    }

    return 0;
  }

  T get_value()
  {
    return value;
  }

  unsigned long long get_local_neighbors_count_at_left() const
  {
    return chunk_data_size_no_ol - my_offset_no_ol;
  }

  //Rightnow, for the 1D data
  /* T  get_next_neighbor(){ */
  /*   //calculate next point along row-major order */
  /*   //find the value */

  /*   std::vector<unsigned long long> next_coodinate; */
  /*   unsigned long long              next_pos; */
  /*   next_coodinate = RowMajorOrderReverse(n_location, chunk_dim_size_no_ol); */

  /*   //Get the coodinate with overlapping */
  /*   for (int ii=0; ii < dims; ii++){ */
  /*     if (next_coodinate[ii] + ol_origin_offset[ii] < chunk_dim_size[ii]){ */
  /*       next_coodinate[ii] = next_coodinate[ii] + ol_origin_offset[ii]; */
  /*     }else{ */
  /*       next_coodinate[ii] = chunk_dim_size[ii]-1; */
  /*     } */
  /*   } */
  /*   next_pos = RowMajorOrder(chunk_dim_size, next_coodinate); */
  /*   n_location = n_location + 1; */
  /*   if(n_location > chunk_data_size_no_ol){ */
  /*     n_at_end_flag = 1; */
  /*   } */
  /*   return chunk_data_pointer[next_pos]; */
  /* } */

  //int get_next_at_end_flag(){
  //  return n_at_end_flag;
  //}

  T get_prev_value()
  {
    //calculate next point
    //find the value
    return value;
  }

  void SetLocation(unsigned long long my_offset, std::vector<unsigned long long> &my_coordinate, std::vector<unsigned long long> &my_location_no_ol_p, std::vector<unsigned long long> &chunk_dim_size_no_ol_p, std::vector<long long> ol_origin_offset_p, std::vector<unsigned long long> current_chunk_ol_size)
  {
    //value                = chunk_data_pointer[my_offset];
    if (my_offset > chunk_data_size)
    {
      std::cout << "Error in intializing Stencil(). my_offset  = " << my_offset << ", chunk_data_size = " << chunk_data_size << std::endl;
      exit(-1);
    }
    else
    {
      value = chunk_data_pointer[my_offset];
    }

    chunk_data_size_no_ol = 1;
    int rank = my_coordinate.size();
    for (int i = 0; i < rank; i++)
    {
      my_location[i] = my_coordinate[i];
      //my_location_no_ol[i]    = my_location_no_ol_p[i];
      //chunk_dim_size_no_ol[i] = chunk_dim_size_no_ol_p[i];
      //ol_origin_offset[i]     = ol_origin_offset_p[i];
      chunk_data_size_no_ol = chunk_data_size_no_ol * chunk_dim_size_no_ol_p[i];
      chunk_dim_size[i] = current_chunk_ol_size[i];
    }
    chunk_data_size_no_ol = chunk_data_size_no_ol - 1; //start from 0
    //my_offset_no_ol       = RowMajorOrder(chunk_dim_size_no_ol_p, my_location_no_ol_p) - 1;
    ROW_MAJOR_ORDER_MACRO(chunk_dim_size_no_ol_p, chunk_dim_size_no_ol_p.size(), my_location_no_ol_p, my_offset_no_ol);
    my_offset_no_ol = my_offset_no_ol - 1;
    //p_location           = n_location;
  }

  /**
   * @brief Set the Location object
   * 
   * @param my_offset  within the local chunk
   * @param my_coordinate within the local chunk
   * @param my_location_no_ol_p 
   * @param chunk_dim_size_no_ol_p chunk size withiout considering overlap
   * @param ol_origin_offset_p  chunk offset from the global (0, 0)
   * @param current_chunk_ol_size overlap size
   * @param global_coordinate  current cell's global coodinate
   * @param global_coordinate_lm  linearized global_coordinate
   */
  void SetLocation(unsigned long long &my_offset, std::vector<unsigned long long> &my_coordinate, std::vector<unsigned long long> &my_location_no_ol_p, std::vector<unsigned long long> &chunk_dim_size_no_ol_p, std::vector<long long> &ol_origin_offset_p, std::vector<unsigned long long> &current_chunk_ol_size, std::vector<unsigned long long> &global_coordinate_p, unsigned long long &global_coordinate_lineared_p)
  {
    if (my_offset > chunk_data_size)
    {
      std::cout << "Error in intializing Stencil(). my_offset  = " << my_offset << ", chunk_data_size = " << chunk_data_size << std::endl;
      exit(-1);
    }
    else
    {
      value = chunk_data_pointer[my_offset];
    }

    chunk_data_size_no_ol = 1;
    int rank = my_coordinate.size();
    for (int i = 0; i < rank; i++)
    {
      my_location[i] = my_coordinate[i];
      //my_location_no_ol[i]    = my_location_no_ol_p[i];
      //chunk_dim_size_no_ol[i] = chunk_dim_size_no_ol_p[i];
      //ol_origin_offset[i]     = ol_origin_offset_p[i];
      chunk_data_size_no_ol = chunk_data_size_no_ol * chunk_dim_size_no_ol_p[i];
      chunk_dim_size[i] = current_chunk_ol_size[i];
    }
    chunk_data_size_no_ol = chunk_data_size_no_ol - 1; //start from 0
    //my_offset_no_ol       = RowMajorOrder(chunk_dim_size_no_ol_p, my_location_no_ol_p) - 1;
    ROW_MAJOR_ORDER_MACRO(chunk_dim_size_no_ol_p, chunk_dim_size_no_ol_p.size(), my_location_no_ol_p, my_offset_no_ol);
    my_offset_no_ol = my_offset_no_ol - 1;

    my_g_location_rm = global_coordinate_lineared_p;

    global_coordinate = global_coordinate_p;
    global_coordinate_lineared = global_coordinate_lineared_p;
  }

  /**
   * @brief return the global coodinate of the current Stencil
   * 
   * @return std::vector<unsigned long long> 
   */
  std::vector<unsigned long long> GetCoordinate() const
  {
    return global_coordinate;
  }

  void set_trail_run_flag()
  {
    trail_run_flag = 1;
    for (int i = 0; i < dims; i++)
      coordinate_shift[i] = 0;
  }

  void get_trail_run_result(int *overlap_size)
  {
    if (trail_run_flag == 1)
    {
      for (int i = 0; i < dims; i++)
        overlap_size[i] = coordinate_shift[i];
      trail_run_flag = 0;
    }
  }

  void set_my_g_location_rm(unsigned long long lrm)
  {
    my_g_location_rm = lrm;
  }

  unsigned long long get_my_g_location_rm() const
  {
    return my_g_location_rm;
  }

  //Use the global coordinate of the cell as the id
  //It is usefull for parallel processing
  unsigned long long get_id() const
  {
    return my_g_location_rm;
  }

  void SetApplyPadding(T padding_value_p)
  {
    padding_value_set_flag = 1;
    padding_value = padding_value_p;
  }
};

#endif
