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
#include "getopt.h"

using namespace std;

//UDF One: create new labels
inline int newLabelUDF(const Stencil<float> &c)
{
  if (c(0, 0) != 0)
  {
    return c.get_id() + 1; //"+1" to avoid the first one, whose id is 0
  }
  else
  {
    return 0;
  }
}

//UDF Two: forward sync labels.
int convergeFlag = 1;
int convergeFlag_global;

inline int fsyncLabelUDF(const Stencil<int> &c)
{
  if (c(0, 0) == 0)
    return 0;
  //int min_label=find_min_label_help(c, 1); //1: forward flag
  int min_label = c(0, 0);
  if (c(-1, -1) != 0 && c(-1, -1) < min_label)
    min_label = c(-1, -1);
  if (c(-1, 0) != 0 && c(-1, 0) < min_label)
    min_label = c(-1, 0);
  if (c(-1, 1) != 0 && c(-1, 1) < min_label)
    min_label = c(-1, 1);
  if (c(0, -1) != 0 && c(0, -1) < min_label)
    min_label = c(0, -1);

  if (c(0, 0) != min_label)
  {
    convergeFlag = 0;
  }
  return min_label;
}

//UDF Two: backward sync labels.
inline int bsyncLabelUDF(const Stencil<int> &c)
{
  if (c(0, 0) == 0)
    return 0;
  //int min_label=find_min_label_help(c, 0); //0: backward flag
  int min_label = c(0, 0);
  if (c(0, 1) != 0 && c(0, 1) < min_label)
    min_label = c(0, 1);
  if (c(1, 0) != 0 && c(1, 0) < min_label)
    min_label = c(1, 0);
  if (c(1, 1) != 0 && c(1, 1) < min_label)
    min_label = c(1, 1);
  if (c(1, -1) != 0 && c(1, -1) < min_label)
    min_label = c(1, -1);
  if (c(0, 0) != min_label)
    convergeFlag = 0;
  return min_label;
}

//UDF Two: backward sync labels.
inline int allcheckUDF(const Stencil<int> &c)
{
  if (c(0, 0) == 0)
    return 0;
  //int min_label=find_min_label_help(c, 0); //0: backward flag
  int min_label = c(0, 0);
  if (c(0, 1) != 0 && c(0, 1) < min_label)
    min_label = c(0, 1);
  if (c(1, 0) != 0 && c(1, 0) < min_label)
    min_label = c(1, 0);
  if (c(1, 1) != 0 && c(1, 1) < min_label)
    min_label = c(1, 1);
  if (c(1, -1) != 0 && c(1, -1) < min_label)
    min_label = c(1, -1);
  if (c(-1, -1) != 0 && c(-1, -1) < min_label)
    min_label = c(-1, -1);
  if (c(-1, 0) != 0 && c(-1, 0) < min_label)
    min_label = c(-1, 0);
  if (c(-1, 1) != 0 && c(-1, 1) < min_label)
    min_label = c(-1, 1);
  if (c(0, -1) != 0 && c(0, -1) < min_label)
    min_label = c(0, -1);
  if (c(0, 0) != min_label)
    convergeFlag = 0;
  return min_label;
}

int main(int argc, char *argv[])
{

  int cache_flag = 0, backward_flag = 0, mirrors_flag = 0, replace_flag = 0, copt, writeback_flag = 0, all_check_flag = 0;
  int c_size = 12288, o_size = 1;
  char i_file[1024] = "/Users/dbin/work/data/plot.Ant.Steady-500m-newSMB-1km.000000.2d_flat.h5";
  char o_file[1024] = "/Users/dbin/work/data/plot.Ant.Steady-500m-newSMB-1km.000000.2d_flat-ccl.h5";
  char group[1024] = "/";
  char dataset[1024] = "/flatten_data";
  int f_value = 0;

  while ((copt = getopt(argc, argv, "lcbmrwo:i:g:d:t:s:h:f:")) != -1)
    switch (copt)
    {
    case 's':
      c_size = atoi(optarg);
      break;
    case 'h':
      o_size = atoi(optarg);
      break;
    case 'f':
      f_value = atoi(optarg);
      break;
    case 'c':
      cache_flag = 1;
      break;
    case 'b':
      backward_flag = 1;
      break;
    case 'm':
      mirrors_flag = 1;
      break;
    case 'r':
      replace_flag = 1;
      break;
    case 'w':
      writeback_flag = 1;
      break;
    case 'o':
      memset(o_file, 0, sizeof(o_file));
      strcpy(o_file, optarg);
      break;
      //case 't':
      //memset(t_file, 0, sizeof(t_file));
      //strcpy(t_file, optarg);
      //break;
    case 'i':
      memset(i_file, 0, sizeof(i_file));
      strcpy(i_file, optarg);
      break;
    case 'g':
      memset(group, 0, sizeof(group));
      strcpy(group, optarg);
      break;
    case 'd':
      memset(dataset, 0, sizeof(dataset));
      strcpy(dataset, optarg);
      break;
    case 'l':
      all_check_flag = 1;
      break;
    default:
      break;
    }

  //Init the MPICH
  AU_Init(argc, argv);

  int mpi_size, mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  // set up the chunk size and the overlap size
  std::vector<int> chunk_size;
  chunk_size.push_back(c_size);
  chunk_size.push_back(c_size);
  std::vector<int> overlap_size;
  overlap_size.push_back(o_size);
  overlap_size.push_back(o_size);
  std::vector<int> mirrors;
  mirrors.push_back(f_value);

  //Orginal data set
  Array<float, int> *A = new Array<float, int>(AU_NVS, AU_HDF5, i_file, group, dataset, chunk_size, overlap_size);

  //Results data sets
  Array<int> *B = new Array<int>(AU_COMPUTED, AU_HDF5, o_file, "/ccl", "/ccl/labels", chunk_size, overlap_size);
  if (cache_flag == 1)
    B->EnableCache();
  if (mirrors_flag == 1)
    B->SetMirrorValue(mirrors);
  if (replace_flag == 1)
    B->EnableApplyReplace();
  if (writeback_flag == 1)
    B->EnableApplyWriteback();

  double t1, t2, t3, t4;
  //Assign new lables to each points
  t1 = MPI_Wtime();
  A->Apply(newLabelUDF, B); //B existing in memory after this step
  t2 = MPI_Wtime();
  if (mpi_rank == 0)
  {
    printf("Elapsed time for assigning labels:  %f\n", t2 - t1);
    fflush(stdout);
  }
  A->ReportTime();
  delete A;

  int ite = 0;
  t1 = MPI_Wtime();
  if (all_check_flag == 0)
  {
    do
    {
      convergeFlag = 1;
      B->SetApplyDirection(0);
      B->Apply(fsyncLabelUDF);
      B->SetApplyDirection(1);
      B->Apply(bsyncLabelUDF);
      ite++;
      MPI_Allreduce(&convergeFlag, &convergeFlag_global, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    } while (convergeFlag_global == 0);
  }
  else
  {
    //B->EnableCache();
    //B->EnableApplyWriteback();
    do
    {
      convergeFlag = 1;
      B->SetApplyDirection(0);
      B->Apply(allcheckUDF);
      ite++;
      if (ite % 50 == 0)
        printf("Ite = %d \n", ite);
      MPI_Allreduce(&convergeFlag, &convergeFlag_global, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    } while (convergeFlag_global == 0);
  }
  t2 = MPI_Wtime();
  if (mpi_rank == 0)
  {
    printf("Coverge after %d  iterations. Elapsed time for nonvolatile is %f\n", ite, t2 - t1);
    fflush(stdout);
  }

  B->Nonvolatile();
  t3 = MPI_Wtime();
  if (mpi_rank == 0)
  {
    printf("Elapsed time for nonvolatile is %f\n", t3 - t2);
    fflush(stdout);
  }

  B->ReportTime();

  //Clear
  delete B;

  AU_Finalize();

  return 0;
}
