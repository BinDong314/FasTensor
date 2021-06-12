

/*
****************************

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy). 
All rights reserved.

If you have questions about your rights to use or distribute this software,
please contact Berkeley Lab's Intellectual Property Office at
IPO@lbl.gov.

NOTICE.  This Software was developed under funding from the U.S. Department
of Energy and the U.S. Government consequently retains certain rights.  As
such, the U.S. Government has been granted for itself and others acting on
its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the
Software to reproduce, distribute copies to the public, prepare derivative 
works, and perform publicly and display publicly, and to permit others to do so.


****************************


*** License Agreement ***

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy). 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

(1) Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

(2) Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

(3) Neither the name of the University of California, Lawrence Berkeley
National Laboratory, U.S. Dept. of Energy nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

You are under no obligation whatsoever to provide any bug fixes, patches,
or upgrades to the features, functionality or performance of the source
code ("Enhancements") to anyone; however, if you choose to make your
Enhancements available either publicly, or directly to Lawrence Berkeley
National Laboratory, without imposing a separate written license agreement
for such Enhancements, then you hereby grant the following license: a
non-exclusive, royalty-free perpetual license to install, use, modify,
prepare derivative works, incorporate into other computer software,
distribute, and sublicense such enhancements or derivative works thereof,
in binary and source code form.
*/

/**
 *
 * Author: Bin Dong dbin@lbl.gov
 * Web: https://crd.lbl.gov/bin-dong
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
#include "ft_utility.h"
#include "ft_array_view_access.h"
#include "ft_utility.h"

using namespace std;
extern double time_address_cal, row_major_order_cal, pre_row_major_order_cal, data_access_time;

template <class T>
class Stencil
{
private:
  T value;
  int has_set_output_value_flag = false;
  std::vector<unsigned long long> my_location; //This is the coodinate with overlapping
  std::vector<unsigned long long> my_location_no_ol;
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
  //std::vector<int> coordinate_shift;
  //std::vector<unsigned long long> coordinate;

  bool trail_run_flag = false; //When rail_run_flag = 1, it records the maximum overlap size
  mutable std::vector<int> trail_run_overlap_size;
  //int padding_value_set_flag = 0;
  //T padding_value;
  int mpi_rank, mpi_size;

  std::vector<unsigned long long> global_data_size;
  /**
   * @brief is_output_vector_flag;
   * 
   */
  bool is_output_vector_flag;
  std::vector<size_t> output_vector_shape;
  OutputVectorFlatDirection output_vector_flat_direction;

  //padding value
  bool has_padding_value_flag = false;
  T padding_value;

  bool is_stencil_tag = false;
  std::map<std::string, std::string> stencil_tag_map;

  //Used by operator ()
  mutable std::vector<int> coordinate_shift;
  mutable std::vector<unsigned long long> coordinate;
  mutable std::vector<int> ov;

public:
  //For test only
  Stencil(){};

  // For trail run
  Stencil(int dims_input, T *chunk)
  {
    dims = dims_input;
    //coordinate_shift.resize(dims_input);
    //coordinate.resize(dims_input);
    chunk_data_pointer = chunk;
    //et_trail_run_flag();
    trail_run_flag = true;
    trail_run_overlap_size.resize(dims_input);
    std::fill(trail_run_overlap_size.begin(), trail_run_overlap_size.end(), 0);
  }

  /**
   * @brief Construct a new Stencil object contains the value 
   *        Mostly, it is used as output
   * 
   * @param value, value to be contained 
   */
  Stencil(T value_p)
  {
    value = value_p;
  }

  /**
   * @brief Construct a new Stencil object contains the value
   *        and the value has the shape for n-dimensional array
   *        Mostly, it is used as output
   * 
   * @param value, the value to be contained in Stencil object
   * @param shape, the shape fo the output data
   */
  Stencil(T value_p, std::vector<size_t> shape_p)
  {
    value = value_p;
    is_output_vector_flag = true;
    output_vector_shape = shape_p;
  }

  //For production
  Stencil(unsigned long long my_offset, T *chunk, std::vector<unsigned long long> &my_coordinate, std::vector<unsigned long long> &chunk_size, std::vector<unsigned long long> &global_data_size_p)
  {
#ifdef DEBUG
    MpiRankSize(&mpi_rank, &mpi_size);
    std::cout << "my value =" << value << ", coordinate = (" << my_coordinate[0] << ", " << my_coordinate[1] << " )" << std::endl;
#endif
    chunk_data_pointer = chunk;
    dims = chunk_size.size();
    chunk_dim_size.resize(dims);
    my_location.resize(dims);
    global_data_size.resize(dims);

    coordinate_shift.resize(dims);
    coordinate.resize(dims);

    chunk_data_size = 1;
    for (int i = 0; i < dims; i++)
    {
      chunk_data_size = chunk_data_size * chunk_size[i];
      chunk_dim_size[i] = chunk_size[i];
      my_location[i] = my_coordinate[i];
      global_data_size[i] = global_data_size_p[i];
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

  /*
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
  */

  //Generic version of the ()
  template <typename... Is>
  inline T operator()(Is... offsets) const
  {
    //std::vector<int> ov{{offsets...}};
    ov = {offsets...};
    int ov_rank = ov.size();
    if (dims != ov_rank)
    {
      AU_EXIT("The # of offsets " + std::to_string(ov_rank) + " is not equal to the data's: " + std::to_string(dims));
    }

    //std::cout << "Call new index ! \n";
    //move below to the main function
    //std::vector<int> coordinate_shift(dims);
    //std::vector<unsigned long long> coordinate(dims);

    if (trail_run_flag)
    {
      for (int i = 0; i < ov_rank; i++)
      {
        if (std::abs(ov[i]) > trail_run_overlap_size[i])
          trail_run_overlap_size[i] = std::abs(ov[i]);
      }
      return chunk_data_pointer[0];
    }

    for (int i = 0; i < ov_rank; i++)
    {
      coordinate_shift[i] = ov[i];
      if (coordinate_shift[i] == 0)
      {
        coordinate[i] = my_location[i];
      }
      else if (coordinate_shift[i] > 0)
      {

        coordinate[i] = my_location[i] + coordinate_shift[i];
        if (coordinate[i] >= chunk_dim_size[i])
        { //Check boundary :: Be careful with size overflow
          coordinate[i] = chunk_dim_size[i] - 1;
          //return the padding_value if go beyond boundary and has padding seting
          if (has_padding_value_flag)
          {
            return padding_value;
          }
        }
      }
      else
      {
        coordinate[i] = -coordinate_shift[i]; //Convert to unsigned long long
        if (my_location[i] < coordinate[i])
        {
          //return the padding_value if go beyond boundary and has padding seting
          if (has_padding_value_flag)
          {
            return padding_value;
          }
          coordinate[i] = 0;
        }
        else
        {
          coordinate[i] = my_location[i] - coordinate[i]; //Check boundary :: Be careful with size overflow
        }
      }
    }
    unsigned long long shift_offset = coordinate[0];
    for (int i = 1; i < ov_rank; i++)
    {
      shift_offset = shift_offset * chunk_dim_size[i] + coordinate[i];
    }

    if (shift_offset <= chunk_data_size)
    {
      return chunk_data_pointer[shift_offset];
    }
    else
    {
      PrintVector("ov = ", ov);
      PrintVector("coordinate = ", coordinate);
      PrintVector("chunk_dim_size = ", chunk_dim_size);
      AU_EXIT("Error in operator() of Stencil");
    }
  }

  //Generic version of the ()
  inline T ReadPoint(std::vector<int> &ov) const
  {
    //std::vector<int> ov{{offsets...}};
    int ov_rank = ov.size();
    if (dims != ov_rank)
    {
      AU_EXIT("The # of offsets " + std::to_string(ov_rank) + " is not equal to the data's: " + std::to_string(dims));
    }

    //std::cout << "Call new index ! \n";
    std::vector<int> coordinate_shift(dims);
    std::vector<unsigned long long> coordinate(dims);

    for (int i = 0; i < ov_rank; i++)
    {
      coordinate_shift[i] = ov[i];
      if (coordinate_shift[i] == 0)
      {
        coordinate[i] = my_location[i];
      }
      else if (coordinate_shift[i] > 0)
      {

        coordinate[i] = my_location[i] + coordinate_shift[i];
        if (coordinate[i] >= chunk_dim_size[i])
        { //Check boundary :: Be careful with size overflow
          coordinate[i] = chunk_dim_size[i] - 1;
          //return the padding_value if go beyond boundary and has padding seting
          if (has_padding_value_flag)
          {
            return padding_value;
          }
        }
      }
      else
      {
        coordinate[i] = -coordinate_shift[i]; //Convert to unsigned long long
        if (my_location[i] < coordinate[i])
        {
          //return the padding_value if go beyond boundary and has padding seting
          if (has_padding_value_flag)
          {
            return padding_value;
          }
          coordinate[i] = 0;
        }
        else
        {
          coordinate[i] = my_location[i] - coordinate[i]; //Check boundary :: Be careful with size overflow
        }
      }
    }
    unsigned long long shift_offset = coordinate[0];
    for (int i = 1; i < ov_rank; i++)
    {
      shift_offset = shift_offset * chunk_dim_size[i] + coordinate[i];
    }

    if (shift_offset <= chunk_data_size)
    {
      return chunk_data_pointer[shift_offset];
    }
    else
    {
      PrintVector("ov = ", ov);
      PrintVector("coordinate = ", coordinate);
      PrintVector("chunk_dim_size = ", chunk_dim_size);
      AU_EXIT("Error in operator() of Stencil");
    }
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

  void ReadHoodBorder(std::vector<T> &rv, std::vector<int> &start_offset, std::vector<int> &end_offset) const
  {
    int rank_temp = start_offset.size();
    size_t element_count = rv.size();

    std::vector<unsigned long long> count_size_t(rank_temp);
    for (int i = 0; i < rank_temp; i++)
    {
      count_size_t[i] = (end_offset[i] - start_offset[i] + 1);
    }

    if (rank_temp == 1)
    {
      for (int iii = start_offset[0]; iii <= end_offset[0]; iii++)
      {
        rv[iii] = this->operator()(iii);
      }
    }

    if (rank_temp == 2)
    {
      //std::cout << "call out of border\n";
      for (int iii = start_offset[0]; iii <= end_offset[0]; iii++)
      {
        for (int jjj = start_offset[1]; jjj <= end_offset[1]; jjj++)
        {
          rv[iii * count_size_t[1] + jjj] = this->operator()(iii, jjj);
        }
      }
    }

    unsigned long long array_buffer_offset = 0;
    std::vector<int> ord(start_offset.begin(), start_offset.end());
    for (unsigned long long i = 0; i < element_count; i++)
    {
      //ROW_MAJOR_ORDER_MACRO(chunk_dim_size, chunk_dim_size.size(), ord, array_buffer_offset);
      // VIEW_ACCESS_HELP_P(view_v, view_buffer_offset, array_v, array_buffer_offset, 1, read_write_code, sizeof(T));
      rv[i] = ReadPoint(ord);
      //view_buffer_offset++;
      ITERATOR_MACRO(ord, start_offset, end_offset); //update ord by increasing "1", row-major order
    }
  }

  /**
   * @brief read neighborhood
   * 
   * @param start_offset 
   * @param end_offset 
   * @return std::vector<T> 
   */
  inline int ReadNeighbors(std::vector<int> &start_offset, std::vector<int> &end_offset, std::vector<T> &rv) const
  {
    //std::vector<T> rv;
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
      //assert(start_offset[i] >= 0); //ArrayIterator any only process positive offset
      //assert(end_offset[i] >= 0);
      start_offset_size_t[i] = start_offset[i];
      end_offset_size_t[i] = end_offset[i] + 1;
    }

    //PrintVector("count_size_t = ", count_size_t);
    //PrintVector("chunk_dim_size = ", chunk_dim_size);

    if (count_size_t == chunk_dim_size)
    {
      //copy(&dataArray[0], &dataArray[dataArraySize], back_inserter(dataVec));
      //std::vector<T> rv2(chunk_data_pointer, chunk_data_pointer + n);
      //std::cout << "read all ! n = " << n << std::endl;
      //return rv2;
      std::copy(&chunk_data_pointer[0], &chunk_data_pointer[n], back_inserter(rv));
      return 0;
    }

    rv.resize(n);

    std::vector<unsigned long long> view_start(rank_temp), view_end(rank_temp);
    bool out_of_border = false;
    for (int ii = 0; ii < rank_temp; ii++)
    {
      view_start[ii] = my_location[ii] + start_offset[ii];
      view_end[ii] = my_location[ii] + end_offset[ii];
      if (view_end[ii] > (chunk_dim_size[ii] - 1))
      {
        out_of_border = true;
      }
    }
    //ArrayViewAccess(rv.data(), chunk_data_pointer, chunk_dim_size, view_start, view_end, ARRAY_VIEW_READ, sizeof(T));

    //PrintVector("view_start = ", view_start);
    //PrintVector("view_end = ", view_end);
    //PrintVector("chunk_dim_size = ", chunk_dim_size);

    //we may go to use the () operator
    if (out_of_border)
    {
      ReadHoodBorder(rv, start_offset, end_offset);
      return 0;
    }

    ArrayViewAccessP<T>(rv.data(), chunk_data_pointer, chunk_dim_size, view_start, view_end, ARRAY_VIEW_READ);
    return 0;
  }
  /**
   * @brief read neighborhood
   * 
   * @param start_offset 
   * @param end_offset 
   * @return std::vector<T> 
   */
  inline std::vector<T> ReadNeighbors(std::vector<int> &start_offset, std::vector<int> &end_offset) const
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

    std::vector<unsigned long long> view_start(rank_temp), view_end(rank_temp);
    bool out_of_border = false;
    for (int ii = 0; ii < rank_temp; ii++)
    {
      view_start[ii] = my_location[ii] + start_offset[ii];
      view_end[ii] = my_location[ii] + end_offset[ii];
      if (view_end[ii] > (chunk_dim_size[ii] - 1))
      {
        out_of_border = true;
      }
    }
    //ArrayViewAccess(rv.data(), chunk_data_pointer, chunk_dim_size, view_start, view_end, ARRAY_VIEW_READ, sizeof(T));

    //PrintVector("view_start = ", view_start);
    //PrintVector("view_end = ", view_end);
    //PrintVector("chunk_dim_size = ", chunk_dim_size);

    //we may go to use the () operator
    if (out_of_border)
    {
      ReadHoodBorder(rv, start_offset, end_offset);
      return rv;
    }

    ArrayViewAccessP<T>(rv.data(), chunk_data_pointer, chunk_dim_size, view_start, view_end, ARRAY_VIEW_READ);
    return rv;

    //ArrayViewAccess();

    //inline int ArrayViewAccess(void *view_buffer, void *array_buffer, std::vector<unsigned long long> &array_size, std::vector<unsigned long long> &start, std::vector<unsigned long long> &end, int read_write_code, int element_size);
    //size_t offset, rv_offset = 0;
    //std::vector<size_t> coordinate;
    //coordinate.resize(rank_temp);

    /*
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
    }*/

    /*
    std::vector<int> coordinate_iterate(start_offset.begin(), start_offset.end());

    for (size_t ii = 0; ii < n; ii++)
    {
      //PrintVector("ArrayIterator_c: ", c);
      for (int j = 0; j < rank_temp; j++)
      {
        coordinate[j] = my_location[j] + coordinate_iterate[j];
        if (coordinate[j] >= chunk_dim_size[j]) //Check boundary :: Be careful with size overflow
          coordinate[j] = chunk_dim_size[j] - 1;
      }

      ROW_MAJOR_ORDER_MACRO(chunk_dim_size, rank_temp, coordinate, offset);
      assert(offset <= chunk_data_size);
      rv[rv_offset] = chunk_data_pointer[offset];
      rv_offset = rv_offset + 1;

      ITERATOR_MACRO(coordinate_iterate, start_offset, end_offset);
    }*/

    return rv;
  }

  /**
   * @brief WriteHood
   * 
   * @param start_offset 
   * @param end_offset 
   * @param data 
   * @return int 
   */
  int WriteNeighbors(std::vector<int> &start_offset, std::vector<int> &end_offset, std::vector<T> &data) const
  {
    int rank_temp = start_offset.size();
    /*std::vector<size_t> start_offset_size_t, end_offset_size_t;
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
    }*/

    std::vector<unsigned long long> view_start(rank_temp), view_end(rank_temp);
    for (int ii = 0; ii < rank_temp; ii++)
    {
      view_start[ii] = my_location[ii] + start_offset[ii];
      view_end[ii] = my_location[ii] + end_offset[ii];
    }
    //ArrayViewAccess(data.data(), chunk_data_pointer, chunk_dim_size, view_start, view_end, ARRAY_VIEW_WRITE, sizeof(T));
    ArrayViewAccessP<T>(data.data(), chunk_data_pointer, chunk_dim_size, view_start, view_end, ARRAY_VIEW_WRITE);
    /*
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
    }*/

    return 0;
  }

  /**
   * @brief Get the value object
   * 
   * @return T 
   */
  T get_value()
  {
    return value;
  }

  /**
   * @brief Set the value object
   * 
   * @param value_p 
   */
  void set_value(const T value_p)
  {
    value = value_p;
  }

  /**
   * @brief Get the Value object
   * 
   * @return T 
   */
  T GetValue()
  {
    return value;
  }

  int GetValue(T &value_p)
  {
    value_p = value;
    return 0;
  }

  /**
   * @brief Set the value object
   * 
   * @param value_p 
   */
  int SetValue(const T value_p)
  {
    value = value_p;
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

  inline void SetLocation(unsigned long long my_offset, std::vector<unsigned long long> &my_coordinate, std::vector<unsigned long long> &my_location_no_ol_p, std::vector<unsigned long long> &chunk_dim_size_no_ol_p, std::vector<long long> ol_origin_offset_p, std::vector<unsigned long long> current_chunk_ol_size)
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
      my_location_no_ol[i] = my_location_no_ol_p[i];
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
   *        This is the used one in 
   * @param my_offset  within the local chunk
   * @param my_coordinate within the local chunk
   * @param my_location_no_ol_p 
   * @param chunk_dim_size_no_ol_p chunk size withiout considering overlap
   * @param ol_origin_offset_p  chunk offset from the global (0, 0)
   * @param current_chunk_ol_size overlap size
   * @param global_coordinate  current cell's global coodinate
   * @param global_coordinate_lm  linearized global_coordinate
   */
  void inline SetLocation(const unsigned long long &my_offset, const std::vector<unsigned long long> &my_coordinate, const std::vector<unsigned long long> &my_location_no_ol_p, const std::vector<unsigned long long> &chunk_dim_size_no_ol_p, const std::vector<long long> &ol_origin_offset_p, const std::vector<unsigned long long> &current_chunk_ol_size, const std::vector<unsigned long long> &global_coordinate_p, const unsigned long long &global_coordinate_lineared_p)
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
    //int rank = my_coordinate.size();
    my_location = my_coordinate;
    //chunk_dim_size = current_chunk_ol_size;

    //Disable below code out
    //used by get_local_neighbors_count_at_left
    //chunk_data_size_no_ol = 1;
    //for (const auto &e : chunk_dim_size_no_ol_p)
    // chunk_data_size_no_ol *= e;
    //chunk_data_size_no_ol = chunk_data_size_no_ol - 1; //start from 0
    //used by get_local_neighbors_count_at_left
    //ROW_MAJOR_ORDER_MACRO(chunk_dim_size_no_ol_p, chunk_dim_size_no_ol_p.size(), my_location_no_ol_p, my_offset_no_ol);
    //my_offset_no_ol = my_offset_no_ol - 1;

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

  std::vector<unsigned long long> GetGlobalCoordinate() const
  {
    return global_coordinate;
  }

  int GetGlobalIndex(std::vector<unsigned long long> &index_p) const
  {
    index_p = global_coordinate;
    return 0;
  }

  int GetLocalIndex(std::vector<unsigned long long> &index_p) const
  {
    //PrintVector("my_location_no_ol = ", my_location);
    index_p = my_location;
    return 0;
  }

  int GetTrailRunResult(std::vector<int> &overlap_size_p)
  {
    overlap_size_p.resize(trail_run_overlap_size.size());
    if (trail_run_flag == 1)
    {
      for (int i = 0; i < trail_run_overlap_size.size(); i++)
        overlap_size_p[i] = trail_run_overlap_size[i];
    }
    return 0;
  }

  /*
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
  }*/

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

  //void SetApplyPadding(T padding_value_p)
  //{
  //  padding_value_set_flag = 1;
  //  padding_value = padding_value_p;
  //}

  /**
   * @brief Set the Output Vector Flag object
   * 
   * @param is_output_vector_flag_p : true or false
   */
  void SetOutputVectorFlag(const bool is_output_vector_flag_p)
  {
    is_output_vector_flag = is_output_vector_flag_p;
  }

  /**
   * @brief Get the Output Vector Flag object
   * 
   * @return true 
   * @return false 
   */
  bool GetOutputVectorFlag()
  {
    return is_output_vector_flag;
  }

  void SetOutputVectorFlatDirection(OutputVectorFlatDirection output_vector_flat_direction_p)
  {
    output_vector_flat_direction = output_vector_flat_direction_p;
  }

  OutputVectorFlatDirection GetOutputVectorFlatDirection()
  {
    return output_vector_flat_direction;
  }

  /**
   * @brief Set the Padding object
   * 
   * @param padding_value_p 
   */
  void SetPadding(T padding_value_p)
  {
    has_padding_value_flag = true;
    padding_value = padding_value_p;
  }

  /**
   * @brief Get the Padding object
   * 
   * @return T 
   */
  T GetPadding()
  {
    return padding_value;
  }

  /**
   * @brief Set the Shape of the Stencil object
   * 
   * @param shape_p shape vector
   */
  int SetShape(const std::vector<size_t> &shape_p)
  {
    is_output_vector_flag = true;
    output_vector_shape = shape_p;
    return 0;
  }

  /**
   * @brief Get the Output Vector Shape object
   * 
   * @return std::vector<size_t> 
   */
  int GetShape(std::vector<size_t> &shape_p)
  {
    if (!is_output_vector_flag)
    {
      AU_EXIT("The shape of output vector is not set in UDF !");
    }
    shape_p = output_vector_shape;
    return 0;
  }

  /**
   * @brief Get the Max Offset Upper
   * 
   * @return std::vector<int> , the maximum offset at upper side (including)
   */
  int GetOffsetUpper(std::vector<int> &max_offset) const
  {
    //PrintVector("GetOffsetUpper.chunk_dim_size = ", chunk_dim_size);
    //PrintVector("GetOffsetUpper.my_location = ", my_location);
    int rank = chunk_dim_size.size();
    for (int i = 0; i < rank; i++)
    {
      max_offset.push_back(chunk_dim_size[i] - my_location[i] - 1);
    }
    return 0;
  }

  /**
   * @brief Get the Max Offset lower
   * 
   * @return std::vector<int> , the maximum offset at lower side (including)
   */
  int GetOffsetLower(std::vector<int> &max_offset) const
  {
    int rank = chunk_dim_size.size();

    for (int i = 0; i < rank; i++)
    {
      max_offset.push_back(my_location[i]);
    }
    return 0;
  }

  unsigned long long GetChunkID() const
  {
    return chunk_id;
  }

  int GetChunkID(unsigned long long &chunk_id_p) const
  {
    chunk_id_p = chunk_id;
    return 0;
  }

  void SetChunkID(unsigned long long chunk_id_p)
  {
    chunk_id = chunk_id_p;
  }

  inline int SetTagMap(std::map<std::string, std::string> &stencil_tag_map_p)
  {
    is_stencil_tag = true;
    stencil_tag_map = stencil_tag_map_p;

    return 0;
  }

  inline int GetTagMap(std::map<std::string, std::string> &stencil_tag_map_p) const
  {
    stencil_tag_map_p = stencil_tag_map;
    return 0;
  }

  inline bool HasTagMap() const
  {
    return is_stencil_tag;
  }

  /**
   * @brief Get the size of the current chunk which base cell is located
   *        This may be different from one run to another
   * 
   * @param chunk_size, the size of the current chunk 
   * @return int 
   */
  int GetCurrentChunkSize(std::vector<unsigned long long> &chunk_size) const
  {
    //int rank = chunk_dim_size.size();
    chunk_size = chunk_dim_size;
    return 0;
  }

  int GetArraySize(std::vector<unsigned long long> &array_size) const
  {
    array_size = global_data_size;
    return 0;
  }

  int SetArraySize(std::vector<unsigned long long> &array_size) const
  {
    global_data_size = array_size;
    return 0;
  }
};

#endif
