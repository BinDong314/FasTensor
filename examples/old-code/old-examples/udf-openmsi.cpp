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
  return (c(0,0,0)*7.0 - c(0,0,1) - c(0,0,-1) - c(0,1,0) - c(0,-1,0) - c(1,0,0) - c(-1,0,0));
}


int main(int argc, char *argv[])
{
    // create an instance of the myArray class 
    // 123, 463, 188960
  std::vector<int> chunk_size(3) ; // = {41,463,1181};
  std::vector<int> overlap_size(3);// = {1,1,1};
  chunk_size[0]   = 1181;  chunk_size[1] = 463; chunk_size[2]   = 41;
  overlap_size[0] = 1; overlap_size[1] =   1; overlap_size[2] = 1;
  MPI_Init(&argc, &argv);

  SDSArray<float> *A = new SDSArray<float>("/scratch3/scratchdirs/dbin/udf/OpenMSI_Potato_Eye-data0-ZYX.h5", "/entry_0", "/entry_0/data_0", chunk_size, overlap_size);
  //rank, dims, chunk_size, overlap_size
  SDSArray<float> *B = new SDSArray<float>("/scratch3/scratchdirs/dbin/udf/OpenMSI_Potato_Eye-data0-ZYX.h5", "/entry_0/", "/entry_0/data_0");
  
  A->Apply(myfunc1, B);
  
  delete A;
  delete B;
  MPI_Finalize();
  
  return 0; 
}
