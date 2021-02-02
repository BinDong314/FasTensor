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

inline Stencil<vector<float>> udf_tran(const Stencil<float> &iStencil)
{
    std::vector<int> start = {0, 0}, end = {15, 11};
    std::vector<float> data_ori, data_tra(12 * 16);
    iStencil.ReadNeighbors(start, end, data_ori);
    transpose_data_2D(data_ori.data(), data_tra.data(), 16, 12);
    std::vector<size_t> shape = {12, 16};
    return Stencil<vector<float>>(data_tra, shape);
}

int main(int argc, char *argv[]) // start here
{
    FT_Init(argc, argv);
    std::vector<int> chunk_size = {16, 12};
    std::vector<int> ss = {16, 12};
    Array<float> *A = new Array<float>("EP_HDF5:tutorial.h5:/dat", chunk_size);
    A->SetStride(ss);
    Array<float> *B = new Array<float>("EP_HDF5:tutorial_tran.h5:/dat");
    A->Transform(udf_tran, B);
    A->CreateVisFile();
    B->CreateVisFile();
    delete A;
    delete B;
    FT_Finalize();
    return 0;
}
