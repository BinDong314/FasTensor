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

//UDF One: duplicate the original data
inline float myfunc1(const Stencil<float> &c)
{
  return c(0, 0);
}

//UDF Two: replace one with the sum of left-top corner
//         and right-bottem corner
inline float myfunc2(const Stencil<float> &c)
{
  return c(-1, -1) + c(1, 1);
}

//UDF Three: five point stencil
inline float myfunc3(const Stencil<float> &c)
{
  return (c(0, 0) + c(0, -1) + c(0, 1) + c(1, 0) + c(-1, 0)) * 5.0;
}

int main(int argc, char *argv[])
{
  //Init the MPICH
  MPI_Init(&argc, &argv);

  // set up the chunk size and the overlap size
  std::vector<int> chunk_size;
  chunk_size.push_back(5);
  chunk_size.push_back(5);
  std::vector<int> overlap_size;
  overlap_size.push_back(1);
  overlap_size.push_back(1);

  //Orginal data set
  AU::Array<float> *A = new AU::Array<float>("testf-10x10.h5p", "/testg", "/testg/testd", chunk_size, overlap_size);

  //Results data sets
  AU::Array<float> *B = new AU::Array<float>("testf-10x10-func1.h5p", "/testg", "/testg/testd");
  AU::Array<float> *C = new AU::Array<float>("testf-10x10-func2.h5p", "/testg", "/testg/testd");
  AU::Array<float> *D = new AU::Array<float>("testf-10x10-func3.h5p", "/testg", "/testg/testd");

  //Apply myfunc1, myfunc2, myfunc3 to A, seperately.
  //Results are stored onto B, C , D, accordingly
  A->Apply(myfunc1, B);
  A->Apply(myfunc2, C);
  A->Apply(myfunc3, D);

  //Clear
  delete A;
  delete B;
  delete C;
  delete D;

  MPI_Finalize();

  return 0;
}