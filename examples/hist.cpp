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

//Results data sets
Array<int> *I;  //containing global shared data, i.e., interval 
std::vector<long long int> Hist_Count; // contain local results for each interval 

//Auto pointer 
inline int hist_help(float value){
  int interval_index = 0;
  while(interval_index < I->GetDims()[0]){
    if(I->operator()(interval_index) < value ){
      interval_index++;
    }else{
      break;
    }
  }
  return interval_index;
}

inline float hist_udf(const Stencil<float> &x){
  unsigned long long max_offset_to_left;
  //max_offset_to_left = x.get_local_neighbors_count_at_left();
  //std::cout << "Size of my chunk : " << max_offset_to_left << std::endl;
  //for(int i=0; i < max_offset_to_left; i++){
    Hist_Count[hist_help(x(0))]++;
    //printf("%d, value %f, index %d \n", i,  x(i), hist_help(x(i)));
    //}
  return 0;
}

int main(int argc, char *argv[])
{
  //Init the MPICH
  AU_Init(argc, argv);
  
  // set up the chunk size and the overlap size
  std::vector<int> chunk_size;    chunk_size.push_back(5000); 
  std::vector<int> overlap_size;  overlap_size.push_back(0);
  std::vector<int> striping_size; striping_size.push_back(5000);
  std::vector<long long int> Final_Hist_Count; // contain global results for each interval 

  //Orginal data set
  Array<float> *X = new Array<float>(AU_NVS,  AU_HDF5, "testf-hist-data.h5p", "/testg", "/testg/testd", chunk_size, overlap_size);

  Array<float> *Y = new Array<float>(AU_NVS,  AU_HDF5, "testf-hist-data.h5p", "/testg", "/testg/testd", chunk_size, overlap_size);

  I = new Array<int>(AU_NVS, AU_HDF5, "testf-hist-bin.h5p", "/testg", "/testg/testd", AU_PRELOAD);
  //std::cout << "I at 5:" << I->operator()(5) << std::endl;
  //std::cout << "# of Bins: "  << I->GetDims()[0] << std::endl;
  
  Hist_Count.resize(I->GetDims()[0]); //I->GetDims()[0] is the number of Interval
  Final_Hist_Count.resize(I->GetDims()[0]);
  std::fill(Hist_Count.begin(), Hist_Count.end(), 0);

  X->SetApplyStripSize(striping_size);
  X->Apply(hist_udf, NULL);  

  MPI_Reduce(&Hist_Count[0], &Final_Hist_Count[0], Hist_Count.size(), MPI_LONG_LONG_INT,  MPI_SUM, 0, MPI_COMM_WORLD);
  
  printf("     Range     Count \n ");
  printf("    ---- %d :   %lld  \n ", I->operator()(0), Final_Hist_Count[0]);
  for(int i = 1; i < Hist_Count.size()-1; i++){
    printf("    %d - %d :   %lld  \n ", I->operator()(i-1), I->operator()(i), Final_Hist_Count[i]);
  }
  printf("   %d ---- :   %lld  \n ", I->operator()(Hist_Count.size()-1), Final_Hist_Count[Hist_Count.size()-1]);
  
  
  //Clear
  delete X;
  delete I;
  //delete T;
  
  AU_Finalize();

   
  return 0; 
}


