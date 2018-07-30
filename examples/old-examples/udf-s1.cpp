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

extern int single_step_flag;
extern int trail_run_flag;
extern int row_major_chunk_flag;
extern int arraystoreloadflat;
extern int handop_flag;

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

int num_of_cells = 5;
inline float myfunc1(const SDSArrayCell<float> &c){
  return (4*c(1,0) - c(-1,0)  - c(0,1)  - c(0,-1) - c(1,0));
  //float result = 0;
  //for (int i = 0; i < num_of_cells; i++)
  //	  result = result + c(i,i);
  //return result;
}


int main(int argc, char *argv[])
{
    // create an instance of the myArray class 
  std::vector<int> chunk_size(2); 
  std::vector<int> overlap_size(2);
  //
  //argv[1] : chunk size[0]
  //argv[2] : chunk size[1]
  //argv[3] : overlap_size
  //argv[4] : file name
  //argv[5] : output file name
  
  //argv[6] : single  step flag: 1 0
  //argv[7] : trail-run flag: 1 0
  //argv[8] : auto row-major chunking: 1, 0
  //argv[9] : # of UDF cells
  //argv[10]: compare with array store
  //argv[11]: use hand-optimized code
  char filename[245];
  char filename_output[245];
  if (argc > 5){
	  chunk_size[0]   = atoi(argv[1]); 
	  chunk_size[1]   = atoi(argv[2]);
	  overlap_size[0] = atoi(argv[3]); overlap_size[1] = atoi(argv[3]);
	  sprintf(filename, "%s", argv[4]);	
	  sprintf(filename_output, "%s", argv[5]);
	  single_step_flag = atoi(argv[6]);
	  trail_run_flag = atoi(argv[7]);
	  row_major_chunk_flag=atoi(argv[8]); 
	  num_of_cells  = atoi(argv[9]); 
	  arraystoreloadflat = atoi(argv[10]);
          handop_flag  = atoi(argv[11]);
  }else{
	  chunk_size[0]   = 2000; chunk_size[1] = 2000;
	  overlap_size[0] = 10   ; overlap_size[1] = 10;
	  sprintf(filename, "%s", "/scratch3/scratchdirs/dbin/udf/fake-2d-tiny.h5p");	
	  sprintf(filename_output, "%s", "/scratch3/scratchdirs/dbin/udf/fake-2d-tiny.h5p");
  }
  MPI_Init(&argc, &argv);
  //SDSArray<float> *A = new SDSArray<float>("/global/cscratch1/sd/dbin/ost-248/udf/fake-2d-tiny.h5p", "/testg", "/testg/testd", chunk_size, overlap_size);
  
  SDSArray<float> *A = new SDSArray<float>(filename, "/testg", "/testg/testd", chunk_size, overlap_size);
  
  //rank, dims, chunk_size, overlap_size
  SDSArray<float> *B = new SDSArray<float>(filename_output, "/testg", "/testg/testd");
  
  A->Apply(myfunc1, B);
  
  delete A;
  delete B;
  MPI_Finalize();
  
  return 0; 
}
