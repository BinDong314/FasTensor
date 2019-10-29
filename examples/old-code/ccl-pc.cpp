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
#include "array_udf_array.h"
#include "mpi.h"

using namespace std;

//Find the minimum label around c (including c)
inline int find_min_label_help(const Stencil<int> &c, int flag);

//UDF One: create new labels

inline int newLabelUDF(const Stencil<int> &c){
  if(c(0,0) != 0){
    return  c.get_id()+1; //"+1" to avoid the first one, whose id is 0
  }else{
    return 0;
  }
}

//UDF Two: forward sync labels.
int convergeFlag = 1;
inline int fsyncLabelUDF(const Stencil<int> &c){
  if(c(0,0) == 0)
    return 0;
  
  int min_label=find_min_label_help(c, 1); //1: forward flag
  if (c(0,0) != min_label)
    convergeFlag = 0;
  return min_label;
}

//UDF Two: backward sync labels.
inline int bsyncLabelUDF(const Stencil<int> &c){
  if (c(0,0) == 0)
    return 0;
  int min_label=find_min_label_help(c, 0); //0: backward flag
  if (c(0,0) != min_label)
    convergeFlag = 0;
  return min_label;
}

int main(int argc, char *argv[])
{
  //Init the MPICH
  MPI_Init(&argc, &argv);
  
  // set up the chunk size and the overlap size
  std::vector<int> chunk_size;   chunk_size.push_back(500);   chunk_size.push_back(500);
  std::vector<int> overlap_size; overlap_size.push_back(1); overlap_size.push_back(1);

  //Orginal data set
  Array<int> *A = new Array<int>("testf-img-cp.h5p", "/testg", "/testg/testd", chunk_size, overlap_size);
  
  //Results data sets
  Array<int> *B = new Array<int>("testf-img-cp-B.h5p", "/testg", "/testg/testd");
  
  //Results data sets
  Array<int> *C = new Array<int>("testf-img-cp-C.h5p", "/testg", "/testg/testd");

  //Assign new lables to each points
  A->Apply(newLabelUDF, B);  //B existing on disk after this step

  delete A;
  int ite = 0;

  int mpi_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  double t1, t2; 
  t1 = MPI_Wtime();
  do{
    convergeFlag = 1;
    B->Apply(fsyncLabelUDF, C);
    C->Apply(bsyncLabelUDF, B);
    ite++;
    printf("\n %d  iterations at rankb %d, convergeFlag=%d \n ", ite, mpi_rank, convergeFlag);
    int convergeFlag_global;
    MPI_Allreduce(&convergeFlag, &convergeFlag_global, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    convergeFlag = convergeFlag_global;
  }while(convergeFlag == 0); 
  
  t2 = MPI_Wtime(); 
  printf( "Elapsed time is %f\n", t2 - t1 );
  printf("Coverge after %d  iterations at rankb %d \n ", ite, mpi_rank);
  
  //Clear
  delete B;
  delete C;
  
  MPI_Finalize();
  
  return 0; 
}


inline int find_min_label_help(const Stencil<int> &c, int flag){
  if (c(0, 0) == 0)
    return 0;

  int min_label = c(0, 0);

  if (flag == 1){
    //Go through forward (including myself)
    if (c(-1,-1)  && min_label > c(-1,-1))   min_label = c(-1,-1);
    if (c(-1, 0)  && min_label > c(-1, 0))   min_label = c(-1,0);
    if (c(-1, 1)  && min_label > c(-1, 1))   min_label = c(-1,1);
    if (c(0, -1)  && min_label > c(0, -1))   min_label = c(0,-1);
    //if (c(0,  0)  && min_label > c(0,  0))   min_label = c(0,0);
  }else{
    //Go through backward
    if (c(1,-1)  && min_label > c(1,-1))    min_label = c(1,-1);
    if (c(1, 0)  && min_label > c(1, 0))    min_label = c(1,0);
    if (c(1, 1)  && min_label > c(1, 1))    min_label = c(1,1);
    if (c(0, 1)  && min_label > c(0, 1))    min_label = c(0,1);
    //if (min_label > c(0, 0))    min_label = c(0,0);
  }
  return min_label;
}
