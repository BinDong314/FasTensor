/**
 *
 * Author:  Bin Dong, dbin@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include "ft.h"

using namespace std;
using namespace FT;

#define KERNELS 3
#define KERNEL_HEIGHT 2
#define KERNEL_WIDTH 2
float kernel_weight[KERNELS][KERNEL_HEIGHT][KERNEL_WIDTH] = {
    {{1, 2}, {3, 4}},
    {{2, 3}, {4, 5}},
    {{3, 4}, {5, 6}},
};
Stencil<vector<float>> UDF_Cov(const Stencil<float> &iStencil)
{
    float temp_sum = 0;
    vector<float> conv_vec;
    for (int k = 0; k < KERNELS; k++)
    {
        for (int i = 0; i < KERNEL_HEIGHT; i++)
        {
            for (int j = 0; j < KERNEL_WIDTH; j++)
            {
                temp_sum += iStencil(i, j) * kernel_weight[k][i][j];
            }
        }
        conv_vec.push_back(temp_sum);
        temp_sum = 0;
    }
    return Stencil<vector<float>>(conv_vec, std::vector<size_t>{1, 1, KERNELS});
}

Stencil<vector<float>> UDF_Cov2(const Stencil<float> &iStencil)
{
    vector<int> sro = {0, 0}, ero = {1, 1};
    //Read s(0,1)  s(0,1) s(1,0)  s(1,1)
    vector<float> val_vec = iStencil.ReadNeighbors(sro, ero);
    float temp_sum = 0;
    vector<float> conv_vec;
    for (int k = 0; k < KERNELS; k++)
    {
        for (int i = 0; i < KERNEL_HEIGHT; i++)
        {
            for (int j = 0; j < KERNEL_WIDTH; j++)
            {
                temp_sum += val_vec[i * KERNEL_WIDTH + j] * kernel_weight[k][i][j];
            }
        }
        conv_vec.push_back(temp_sum);
        temp_sum = 0;
    }
    return Stencil<vector<float>>(conv_vec, std::vector<size_t>{1, 1, KERNELS});
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    FT_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {4, 4};
    std::vector<int> overlap_size = {1, 1};

    //Input data
    Array<float> *A = new Array<float>("EP_HDF5:tutorial.h5:/data", chunk_size, overlap_size);

    //Result data
    Array<float> *B = new Array<float>("EP_HDF5:tutorial_conv.h5:/data");

    //Result data
    Array<float> *C = new Array<float>("EP_HDF5:tutorial_conv2.h5:/data");

    //Run
    A->Transform(UDF_Cov, B);
    A->ReportTime();

    //Run
    A->Transform(UDF_Cov2, C);
    A->ReportTime();

    //Clear
    delete A;
    delete B;
    delete C;

    FT_Finalize();

    return 0;
}

/*

  ReadPoint       ReadHood
 0.00475144      0.00299549
 0.00464177      0.00302005
 0.00485325      0.00286984
 0.00491452      0.00293207
 0.00488591      0.00290775
 0.00470757      0.00291348
 0.00506592      0.00296903
 0.00487113      0.00280595
 0.00486803      0.00293565

*/