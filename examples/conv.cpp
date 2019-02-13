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

#define FILTERS 1
#define FILTER_WEIGHT 3
#define FILTER_HEIGHT 3

//Weight array; each row is a filter
Array<int> *W;

float kernel[8][2][2] = {
    {{1, 2}, {3, 4}},
    {{2, 3}, {4, 5}},
    {{3, 4}, {5, 6}},
    {{4, 5}, {6, 7}},
    {{5, 6}, {7, 8}},
    {{6, 7}, {8, 9}},
    {{7, 8}, {9, 10}},
    {{8, 9}, {10, 11}},
};

//http://cs231n.github.io/convolutional-networks/
inline std::vector<float> ConvUDF(const Stencil<float> &p)
{
  std::vector<float> test;
  float temp_result;
  for (int k = 0; k < FILTERS; k++)
  {
    temp_result = 0;
    for (int i = 0; i < FILTER_HEIGHT; i++)
    {
      for (int j = 0; j < FILTER_WEIGHT; j++)
      {
        temp_result = p(i, j) * kernel[k][i][j] + temp_result;
      }
    }
    test.push_back(temp_result);
  }

  return test;
}

inline float ReluUDF(const Stencil<float> &p)
{
  if (p(0, 0, 0) > 0)
    return p(0, 0, 0);
  return 0;
}

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
//
inline float PoolUDF(const Stencil<float> &p)
{
  return MAX(MAX(MAX(MAX(MAX(MAX(MAX(p(0, 0, 0), p(0, 0, 1)), p(0, 1, 0)), p(0, 1, 1)), p(1, 0, 0)), p(1, 0, 1)), p(1, 1, 0)), p(1, 1, 1));
}

int main(int argc, char *argv[])
{

  int cache_flag = 1, backward_flag = 0, mirrors_flag = 0, replace_flag = 0, copt, writeback_flag = 0;
  int c_size = 12288, o_size = 1;
  char i_file[1024] = "/Users/dbin/work/data/TMQ-sim.h5p";
  char o_file[1024] = "/Users/dbin/work/data/TMQ-sim-result.h5p";
  char t_file[1024] = "/Users/dbin/work/data/TMQ-sim-result-2.h5p";
  char group[1024] = "/TMQ";
  char dataset[1024] = "/TMQ/TMQ";
  int f_value = 0;

  while ((copt = getopt(argc, argv, "cbmrwo:i:g:d:t:s:h:f:")) != -1)
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
    case 't':
      memset(t_file, 0, sizeof(t_file));
      strcpy(t_file, optarg);
      break;
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
    default:
      break;
    }

  //Init the MPICH
  AU_Init(argc, argv);

  //768, 1152
  // set up the chunk size and the overlap size
  //mpirun -n 4x
  std::vector<int> chunk_size;
  chunk_size.push_back(384);
  chunk_size.push_back(576);
  std::vector<int> overlap_size;
  overlap_size.push_back(0);
  overlap_size.push_back(0);
  std::vector<int> striping_size;
  striping_size.push_back(2);
  striping_size.push_back(2);

  //Orginal data set
  Array<float, std::vector<float>> *X = new Array<float, std::vector<float>>(AU_NVS, AU_HDF5, i_file, group, dataset, chunk_size, overlap_size);

  Array<std::vector<float>> *Y = new Array<std::vector<float>>(AU_COMPUTED, AU_HDF5, o_file, group, dataset, chunk_size, overlap_size);

  if (cache_flag == 1)
    Y->EnableCache();

  X->SetApplyStripSize(striping_size);
  X->SetApplyFilters(FILTERS);

  X->Apply(ConvUDF, Y);
  X->ReportTime();

  if (cache_flag == 1)
  {
    double t_start = MPI_Wtime();
    //Y->Nonvolatile();
    Array<float> *Y_ReGrided = Y->Regrid<float>();
    //if(writeback_flag == 1)
    Y_ReGrided->EnableApplyWriteback();
    Y_ReGrided->Apply(ReluUDF, NULL);
    //Y_ReGrided->Nonvolatile();
    //char o_file2[1024]="/Users/dbin/work/data/TMQ-sim-result-2.h5p";
    //( 384, 576, 8 ) / ( 384, 576, 8 )
    //chunk size 192, 288, 4
    chunk_size.resize(3);
    chunk_size[0] = 192;
    chunk_size[1] = 288;
    chunk_size[2] = 8;
    overlap_size.resize(3);
    overlap_size[0] = 0;
    overlap_size[1] = 0;
    overlap_size[2] = 0;
    striping_size.resize(3);
    striping_size[0] = 2;
    striping_size[1] = 2;
    striping_size[2] = 2;
    Array<float> *Z = new Array<float>(AU_COMPUTED, AU_HDF5, t_file, group, dataset, chunk_size, overlap_size);
    Y_ReGrided->SetApplyStripSize(striping_size);
    Y_ReGrided->DisableApplyWriteback();
    Z->EnableCache();
    Y_ReGrided->Apply(PoolUDF, Z);
    double t_end = MPI_Wtime();
    std::cout << "Time of UDF: " << t_end - t_start << std::endl;
    Z->Nonvolatile();
    //dimension:   192, 288, 4
    //chunk size:  86  144 2
    /*
    Z->Apply(P0UDF, Z_P0);
    Z->Apply(P1UDF, Z_P1);
    Z->Apply(P2UDF, Z_P2);
    Z_P0.Regrid();
    Z_P1.Regrid();
    Z_P2.Regriad();
    Z_P0.Reduce(); Z_P1.Reduce(); Z_P2.Reduce();*/

    delete Z;
    delete Y_ReGrided;
  }

  //Clear
  delete X;
  delete Y;

  AU_Finalize();

  return 0;
}
