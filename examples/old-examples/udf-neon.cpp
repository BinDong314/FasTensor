// overload_array.cpp
// overloading the c++ array subscript operator []
//http://neondataskills.org/HDF5/TimeSeries-Data-In-HDF5-Using-R/
#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include "sds_udf_cell.h"
#include "sds_udf_array.h"
#include "mpi.h"

using namespace std;

/*
 * User's functions 
 * Support
 * S3D data: 
 * Climate data: 
 * VPIC: 
 * OpenMSI:  
 * Satalite Data: 
 */ 

inline float myfunc1(const SDSArrayCell<float> &c){
	  return (c(0,0,0)+c(-1,0,0)+c(-2,0, 0)+c(-3,0,0))/4;
}


int main(int argc, char *argv[])
{
    // create an instance of the myArray class 
  std::vector<int> chunk_size(3);
  std::vector<int> overlap_size(3); 
  //1617 4320 8640
  chunk_size[0] = 147; chunk_size[1]=864; chunk_size[2] = 192;	  
  overlap_size[0] = 4; overlap_size[1] = 4; overlap_size[2] = 4;

  MPI_Init(&argc, &argv);

  SDSArray<float> *A = new SDSArray<float>("/scratch3/scratchdirs/dbin/udf/cortadv5_WeeklySST_zyx.h5p",    "/", "/WeeklySST", chunk_size, overlap_size);
  //rank, dims, chunk_size, overlap_size
  SDSArray<float> *B = new SDSArray<float>("/scratch3/scratchdirs/dbin/udf/cortadv5_WeeklySST_zyx-udf.h5p", "/", "/WeeklySST");
  
  A->Apply(myfunc1, B);
  
  delete A;
  delete B;
  MPI_Finalize();
  
  return 0; 
}
