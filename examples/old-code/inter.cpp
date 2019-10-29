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
#include <math.h>       /* ceil  and floor*/


using namespace std;

//Results data sets
Array<float> *Y; //(AU_NVS, AU_HDF5,"testf-inter-C.h5p", "/testg", "/testg/testd", AU_PRELOAD);

//Auto pointer 

inline float OneDimInterUDF(const Stencil<float> &x){
  //return 0;
  float temp1 = Y->operator()(floor(x(0)));
  float temp2=  Y->operator()(ceil(x(0)))-Y->operator()(floor(x(0)));
  float temp3=  (x(0)- floor(x(0)))/(ceil(x(0))- floor(x(0)));
  printf(" For x %f: Y.floor = %f, Y.ceil= %f \n", x(0), temp1,  Y->operator()(ceil(x(0))));
  return temp1 + temp2/temp3 ;
      
}


int main(int argc, char *argv[])
{
  //Init the MPICH
  AU_Init(argc, argv);
  
  // set up the chunk size and the overlap size
  std::vector<int> chunk_size;   chunk_size.push_back(5); 
  std::vector<int> overlap_size; overlap_size.push_back(1);

  //Orginal data set
  Array<float> *X = new Array<float>(AU_NVS,  AU_HDF5, "testf-inter-A.h5p", "/testg", "/testg/testd", chunk_size, overlap_size);

  //Results data sets
  Array<float> *YN = new Array<float>(AU_COMPUTED, AU_HDF5,"testf-inter-B.h5p", "/testg", "/testg/testd", chunk_size, overlap_size);

  
  Y = new Array<float>(AU_NVS, AU_HDF5,"testf-inter-C.h5p", "/testg", "/testg/testd", AU_PRELOAD);
  std::cout << "Y at 10:" << Y->operator()(10) << std::endl;

   
  X->Apply(OneDimInterUDF, YN);  

  //Clear
  delete X;
  delete YN;
  delete &Y;
  
  AU_Finalize();

   
  return 0; 
}


