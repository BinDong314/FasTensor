/**
 *ArrayUDF Copyright (c) 2017, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy).  All rights reserved.
 *
 *If you have questions about your rights to use or distribute this software, please contact Berkeley Lab's Innovation & Partnerships Office at  IPO@lbl.gov.
 *
 * NOTICE. This Software was developed under funding from the U.S. Department of Energy and the U.S. Government consequently retains certain rights. As such, the U.S. Government has been granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software to reproduce, distribute copies to the public, prepare derivative works, and perform publicly and display publicly, and to permit other to do so. 
 *
 */

/**
 *
 * Email questions to {dbin, kwu, sbyna}@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#ifndef ARRAY_UDF
#define ARRAY_UDF

//#define GACXX 1

//#ifdef GACXX
//#include "ga++.h"
//#endif

//#include "ga.h"
#include <assert.h>
#include "mpi.h"
#include "array_udf_io.h"
#include "array_udf_stencil.h"
#include "utility.h"
#include "array_udf_attribute.h"
#include "array_udf_array.h"

//#ifndef HAVE_OMP
//#define HAVE_OMP 1
//#endif

#if defined(_OPENMP)
#include <omp.h>
#endif

using namespace AU;

void AU_Init(int argc, char *argv[])
{

  //#ifdef GACXX
  //int heap=300000, stack=300000;
  //GA::Initialize(argc, argv, heap, stack, MT_F_REAL, 0);
  //#else
  //int heap=30000, stack=30000;
  MPI_Init(&argc, &argv);
  //GA_Initialize();
  ///if(!MA_init(C_DBL, stack, heap)) GA_Error("MA_init failed",stack+heap);
  //if(!MA_init(C_INT, stack, heap)) GA_Error("MA_init failed",stack+heap);
  //#endif
  int mpi_rank, mpi_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

#ifdef ENABLE_OPENMP
  if (mpi_rank == 0)
  {
    int nthreads = omp_get_num_threads();
    printf("OpenMPI threds' number = %d \n", nthreads);
  }
#endif
}

void AU_Finalize()
{
  //#ifdef GACXX
  //GA::Terminate();
  //#else
  //GA_Terminate();
  MPI_Finalize();
  //#endif
}

#endif
