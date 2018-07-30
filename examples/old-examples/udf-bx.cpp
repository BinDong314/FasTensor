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
  //return (c(0,0,0)+c(0,0,1)+c(0,1, 0)+c(0,1,1)+c(1,0,0)+c(1,0,1)+c(1,1,0)+c(1,1,1))*2.0;
  return c(0,0,0)+c(0,0,1)+c(0,1, 0)+c(0,1,1)+c(1,0,0)+c(1,0,1)+c(1,1,0)+c(1,1,1);
}


int main(int argc, char *argv[])
{
    // create an instance of the myArray class 
  std::vector<int> chunk_size(3); // = {250,250,100};
  std::vector<int> overlap_size(3);// = {1,1,1};
  //2000, 2000, 800
  chunk_size[0]   = 200; chunk_size[1] = 200; chunk_size[2] = 200;
  overlap_size[0] = 1  ; overlap_size[1] = 1; overlap_size[2] = 1;
  MPI_Init(&argc, &argv);

  SDSArray<float> *A = new SDSArray<float>("/scratch3/scratchdirs/dbin/udf/bx_22860.h5p", "/Step#0", "/Step#0/bx", chunk_size, overlap_size);
  //rank, dims, chunk_size, overlap_size
  SDSArray<float> *B = new SDSArray<float>("/scratch3/scratchdirs/dbin/udf/bx_22860-udf.h5p", "/Step#0", "/Step#0/bx");
  
  A->Apply(myfunc1, B);
  
  delete A;
  delete B;
  MPI_Finalize();
  
  return 0; 
}
