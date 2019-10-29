// overload_array.cpp
// overloading the c++ array subscript operator []
//http://neondataskills.org/HDF5/TimeSeries-Data-In-HDF5-Using-R/

#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include <math.h>       /* ceil  and floor*/
#include <cstring>

#include "array_udf.h"


using namespace std;

inline float GradientUDF(const Stencil<float> &c){
  return (c(0,0,0)*7.0 - c(0,0,1) - c(0,0,-1) - c(0,1,0) - c(0,-1,0) - c(1,0,0) - c(-1,0,0));
}



int main(int argc, char *argv[])
{
    // create an instance of the myArray class 
    // 123, 463, 188960
  char i_file[1024]=      "/global/cscratch1/sd/dbin/vpic-fanguo/T.10400/field_10400.h5p";
  char o_file[1024]=      "/global/cscratch1/sd/dbin/vpic-fanguo/T.10400/field_10400_gradient.h5p";

  char  group[1024]="/";

  std::vector<int> chunk_size(3) ; 
  std::vector<int> overlap_size(3);
  chunk_size[0]   = 250; chunk_size[1]   = 495; chunk_size[2]   = 250;  
  overlap_size[0] = 1;   overlap_size[1] = 1  ; overlap_size[2] = 1;
  
  MPI_Init(&argc, &argv);
  //Array<float> *A = new Array<float>(AU_NVS,  AU_HDF5, i_file, group, dataset, chunk_size, overlap_size);
  Array<float> * IFILE = new Array<float>(AU_NVS,  AU_HDF5, i_file, group, "/bx", chunk_size, overlap_size);
  Array<float> * OFILE = new Array<float>(AU_COMPUTED,  AU_HDF5, o_file, group, "/bx", chunk_size, overlap_size);

  //rank, dims, chunk_size, overlap_size
  //std::cout <<  "Before Gradient at M(1,1,1): " << M->operator()(1, 1, 1) << std::endl;
  IFILE->Apply(GradientUDF, OFILE);
  //std::cout <<  "After  Gradient at M(1,1,1): " << M->operator()(1, 1, 1) << std::endl;
  IFILE->ReportTime();

  delete IFILE;
  delete OFILE;
  MPI_Finalize();
  
  return 0; 
}
