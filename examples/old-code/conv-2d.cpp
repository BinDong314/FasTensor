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
#include <math.h> /* ceil  and floor*/
#include <cstring>

#include "array_udf.h"

using namespace std;

//http://cs231n.github.io/convolutional-networks/
inline int ConvUDF(const Stencil<int> &p)
{
  int temp = 1 * p(-1, -1) + 2 * p(-1, 0) + 1 * p(-1, 1) + 2 * p(0, -1) + 4 * p(0, 0) + 3 * p(0, 1) + 1 * p(1, -1) + 2 * p(1, 0) + 1 * p(1, 1);
}

int main(int argc, char *argv[])
{

  char i_file[1024] = "./testf.h5p";
  char group[1024] = "/testg";
  char dataset[1024] = "/testg/testd";

  //Init the MPICH
  AU_Init(argc, argv);

  //768, 1152
  // set up the chunk size and the overlap size
  //mpirun -n 4x
  std::vector<int> chunk_size;
  chunk_size.push_back(8192);
  chunk_size.push_back(8192);
  std::vector<int> overlap_size;
  overlap_size.push_back(0);
  overlap_size.push_back(0);

  //Orginal data set
  Array<int, int> *X = new Array<int, int>(AU_NVS, AU_HDF5, i_file, group, dataset, chunk_size, overlap_size);
  X->Apply(ConvUDF);
  X->ReportTime();

  //Clear
  delete X;

  AU_Finalize();

  return 0;
}
