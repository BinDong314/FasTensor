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

//Find the minimum label around c (including c)
inline float find_min_label_help(const Stencil<float> &c, int flag);

//UDF One: create new labels
int Label=1;
inline int newLabelUDF(const Stencil<float> &c){
  if(c(0,0) != 0){
    return Label++;
  }else{
    return 0;
  }
}

int convergeFlag=1;

inline int allsyncLabelUDF(const Stencil<int> &c){
  if(c(0,0) == 0) return 0;
  //int min_label=find_min_label_help(c, 1); //1: forward flag
  int min_label = c(0, 0);
  if(c(-1, -1) != 0  && c(-1, -1) < min_label) min_label= c(-1,-1);
  if(c(-1,  0) != 0  && c(-1,  0) < min_label) min_label= c(-1, 0);
  if(c(-1,  1) != 0  && c(-1,  1) < min_label) min_label=c(-1, 1);
  if(c(0,  -1) != 0  && c(0,  -1) < min_label) min_label=c(0, -1);
  if(c(0,  1)  != 0  && c(0,  1) < min_label) min_label=c(0,  1);
  if(c(1,  0)  != 0  && c(1,  0) < min_label) min_label=c(1,  0);
  if(c(1,  1)  != 0  && c(1,  1) < min_label) min_label=c(1,  1);
  if(c(1, -1)  != 0  && c(1, -1) < min_label) min_label=c(1, -1);
  
  if (c(0,0) != min_label) convergeFlag = 0;
  return min_label;
}

inline int fsyncLabelUDF(const Stencil<int> &c){
  if(c(0,0) == 0) return 0;
  //int min_label=find_min_label_help(c, 1); //1: forward flag
  int min_label = c(0, 0);
  if(c(-1, -1) != 0  && c(-1, -1) < min_label) min_label= c(-1,-1);
  if(c(-1,  0) != 0  && c(-1,  0) < min_label) min_label= c(-1, 0);
  if(c(-1,  1) != 0  && c(-1,  1) < min_label) min_label=c(-1, 1);
  if(c(0,  -1) != 0  && c(0,  -1) < min_label) min_label=c(0, -1);
  
  if (c(0,0) != min_label) convergeFlag = 0;
  return min_label;
}

//UDF Two: backward sync labels.
inline int bsyncLabelUDF(const Stencil<int> &c){
  if (c(0,0) == 0) return 0;
  //int min_label=find_min_label_help(c, 0); //0: backward flag
  int min_label = c(0, 0);
  if(c(0,  1)!= 0  && c(0,  1) < min_label) min_label=c(0,  1);
  if(c(1,  0)!= 0  && c(1,  0) < min_label) min_label=c(1,  0);
  if(c(1,  1)!= 0  && c(1,  1) < min_label) min_label=c(1,  1);
  if(c(1, -1)!= 0  && c(1, -1) < min_label) min_label=c(1, -1);
  if (c(0,0) != min_label) convergeFlag = 0;
  return min_label;
}


int main(int argc, char *argv[])
{
 /*/m1248/bin-data/data-backup/BISICLES/plot.Ant.Steady-500m-newSMB-1km.000000.2d_flat.h5" {
	GROUP "/"
	DATASET "flatten_data" {DATATYPE  H5T_IEEE_F64LE{DATASPACE  SIMPLE { ( 12288, 12288 ) / ( 12288, 12288 ) }*/
  //Init the MPICH
  //MPI_Init(&argc, &argv);
  //AU_Init();
  AU_Init(argc, argv);
  int mpi_size, mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);


  // set up the chunk size and the overlap size
  std::vector<int> chunk_size; chunk_size.push_back(12288); chunk_size.push_back(12288);
  std::vector<int> overlap_size; overlap_size.push_back(0); overlap_size.push_back(0);

  //Orginal data set
  Array<float, int> *A = new Array<float, int>(AU_NVS,  AU_HDF5,"/project/projectdirs/m1248/bin-data/data-backup/BISICLES/plot.Ant.Steady-500m-newSMB-1km.000000.2d_flat.h5", "/", "/flatten_data", chunk_size, overlap_size);
  
  //Results data sets
  Array<int> *B = new Array<int>(AU_COMPUTED, AU_HDF5,"/project/projectdirs/m1248/bin-data/data-backup/BISICLES/plot.Ant.Steady-500m-newSMB-1km.000000.2d_flat_sq_B.h5", "/testg", "/testg/testd", chunk_size, overlap_size);
  B->EnableCache();

  //Results data sets
  Array<int> *C = new Array<int>(AU_COMPUTED, AU_HDF5,"/project/projectdirs/m1248/bin-data/data-backup/BISICLES/plot.Ant.Steady-500m-newSMB-1km.000000.2d_flat_sq_C.h5", "/testg", "/testg/testd", chunk_size, overlap_size);
  C->EnableCache();
  
  double t1, t2, t3, t4; 
  //Assign new lables to each points
  t1 = MPI_Wtime();
  A->Apply(newLabelUDF, B);  //B existing on disk after this step
  t2 = MPI_Wtime(); 
  if(mpi_rank == 0) {printf( "Elapsed time for assigning labels:  %f. Total labels = %lld \n", t2 - t1, Label);fflush(stdout);}
  A->ReportTime(); 
  delete A;
  int ite = 0;
  do{
    convergeFlag = 1;
    B->Apply(fsyncLabelUDF, C); 
    C->Apply(bsyncLabelUDF, B);
    ite++;
    if(ite%2 == 0 && mpi_rank == 0) { t4 = MPI_Wtime(); printf("Ite = %d, Elapsed time: %f \n", ite, t4-t1); fflush(stdout);}
    //if(ite == 2) break;
  }while(convergeFlag == 0); 

  B->ReportTime();
  C->ReportTime();

  printf("Coverge after %d  iterations \n ", ite);
  t2 = MPI_Wtime();


  if(mpi_rank == 0) {printf( "Elapsed time before nonvolatile is %f\n", t2 - t1 );  fflush(stdout);}

 
  B->Nonvolatile();
  C->Nonvolatile();
  
  
  //Clear
  delete B;
  delete C;
  
  //MPI_Finalize();
   AU_Finalize(); 
  return 0; 
}


inline float find_min_label_help(const Stencil<float> &c, int flag){
  if (c(0, 0) == 0)
    return 0;

  float min_label = c(0, 0);

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
