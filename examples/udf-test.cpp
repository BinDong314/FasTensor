/*
 *ArrayUDF Copyright (c) 2017, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy).  All rights reserved.

 *If you have questions about your rights to use or distribute this software, please contact Berkeley Lab's Innovation & Partnerships Office at  IPO@lbl.gov.

* NOTICE. This Software was developed under funding from the U.S. Department of Energy and the U.S. Government consequently retains certain rights. As such, the U.S. Government has been granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software to reproduce, distribute copies to the public, prepare derivative works, and perform publicly and display publicly, and to permit other to do so. 
 */
/**
 *
 * Email questions to {dbin, kwu, sbyna}@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include "array_udf.h"

using namespace std;

//UDF One: duplicate the original data
inline float myfunc0(const Stencil<float> &c){
  return c(0);
}


//UDF One: duplicate the original data
inline float myfunc1(const Stencil<float> &c){
  return c(0)/2;
}

//UDF Two: replace one with the sum of left-top corner
//         and right-bottem corner 
inline float myfunc2(const Stencil<float> &c){
  return c(0)/8;
}


int main(int argc, char *argv[])
{
  //Init the MPICH, etc.
  AU_Init(argc, argv);

  // set up the chunk size and the overlap size
  std::vector<int> chunk_size; chunk_size.push_back(5000); chunk_size.push_back(5000);
  std::vector<int> overlap_size; overlap_size.push_back(0); overlap_size.push_back(0);

  //Orginal data set
  Array<float> *A = new Array<float>(AU_NVS,  AU_HDF5, "./testf-hist-data.h5p", "/testg", "/testg/testd", chunk_size, overlap_size);
  
  //Results data sets
  Array<float> *B = new Array<float>(AU_COMPUTED, AU_HDF5, "./testf-inter-xyz-1.h5p", "/testg", "/testg/y", chunk_size, overlap_size);
  A->Apply(myfunc1, B); delete B;
  
  Array<float> *C = new Array<float>(AU_COMPUTED, AU_HDF5, "./testf-inter-xyz-2.h5p", "/testg", "/testg/z", chunk_size, overlap_size);
  A->Apply(myfunc2, C); delete C;
  
  Array<float> *D = new Array<float>(AU_COMPUTED, AU_HDF5, "./testf-inter-xyz.h5p", "/testg", "/testg/x", chunk_size, overlap_size);

  //Apply myfunc1, myfunc2, myfunc3 to A, seperately.
  //Results are stored onto B, C , D, accordingly 
  A->Apply(myfunc0, D);
  
  //Clear
  delete A;
  delete D;
  
  AU_Finalize();
  
  return 0; 
}
