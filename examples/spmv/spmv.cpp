/*
****************************

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy). 
All rights reserved.

If you have questions about your rights to use or distribute this software,
please contact Berkeley Lab's Intellectual Property Office at
IPO@lbl.gov.

NOTICE.  This Software was developed under funding from the U.S. Department
of Energy and the U.S. Government consequently retains certain rights.  As
such, the U.S. Government has been granted for itself and others acting on
its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the
Software to reproduce, distribute copies to the public, prepare derivative 
works, and perform publicly and display publicly, and to permit others to do so.


****************************


*** License Agreement ***

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy). 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

(1) Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

(2) Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

(3) Neither the name of the University of California, Lawrence Berkeley
National Laboratory, U.S. Dept. of Energy nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

You are under no obligation whatsoever to provide any bug fixes, patches,
or upgrades to the features, functionality or performance of the source
code ("Enhancements") to anyone; however, if you choose to make your
Enhancements available either publicly, or directly to Lawrence Berkeley
National Laboratory, without imposing a separate written license agreement
for such Enhancements, then you hereby grant the following license: a
non-exclusive, royalty-free perpetual license to install, use, modify,
prepare derivative works, incorporate into other computer software,
distribute, and sublicense such enhancements or derivative works thereof,
in binary and source code form.
*/

/**
 *
 * Author: Bin Dong dbin@lbl.gov
 * Web: https://crd.lbl.gov/bin-dong
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
std::vector<double> VI, VJ, VV;

//UDF One: duplicate the original data
inline Stencil<std::vector<double>> udf_spmv(const Stencil<double> &iStencil)
{

    std::vector<double> result_vector, BV;

    //Get vector b
    std::vector<unsigned long long> current_chunk_size;
    iStencil.GetCurrentChunkSize(current_chunk_size);
    std::vector<int> start_address, end_address;
    start_address.push_back(0);
    end_address.push_back(current_chunk_size[0] - 1);
    iStencil.ReadNeighbors(start_address, end_address, BV);

    std::vector<unsigned long long> B_start_index_global;
    iStencil.GetGlobalIndex(B_start_index_global);
    std::vector<unsigned long long> B_start_index_local;
    iStencil.GetLocalIndex(B_start_index_local);

    int result_start_index = B_start_index_global[0];
    B_start_index_global[0] = B_start_index_global[0] - B_start_index_local[0];

    int result_vector_size_local = 0, result_vector_size_min = VI[0], result_vector_size_max = VI[0];
    for (int i = 0; i < VI.size(); i++)
    {
        if (VI[i] < result_vector_size_min)
            result_vector_size_min = VI[i];
        if (VI[i] > result_vector_size_max)
            result_vector_size_max = VI[i];
    }
    result_vector_size_local = result_vector_size_max - result_vector_size_min + 1;
    result_vector.resize(result_vector_size_local, 0);
    //std::cout << 1 * 0.1 - 2 * 1.1 + 1 * 2.1 << "\n";
    // float result = 0
    // result =  result + 1 * 0.1
    // result =  result - 2 * 1.1
    // result =  result + 1 * 2.1
    // result is not 0

    int result_index = 0, b_index;
    for (int i = 0; i < VI.size(); i++)
    {
        result_index = VI[i] - result_start_index;
        b_index = VJ[i] - B_start_index_global[0];

        if (b_index < BV.size())
        {
            result_vector[result_index] = result_vector[result_index] + VV[i] * BV[b_index];
        }
        else
        {
            std::cout << "Error: i = " << i << ", (VI VJ VV)=  (" << VI[i] << ", " << VJ[i] << " , " << VV[i] << "), B_start_index_global[0] = " << B_start_index_global[0] << ", b_index =" << b_index << ", BV.size() = " << BV.size() << "\n";
            exit(-1);
        }
    }

    //PrintVector("result_vector = ", result_vector);
    Stencil<std::vector<double>> oStencil;
    std::vector<size_t> vector_shape(1);
    vector_shape[0] = result_vector.size();
    oStencil = result_vector;
    oStencil.SetShape(vector_shape);
    return oStencil;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    FT_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {1535};
    std::vector<int> overlap_size = {0};

    //Input Matrix
    //  Problem: partition a single row on different processes
    //           -> should we keep overlap_size == 0
    //           -> if not, how to decide the overlap_size ==0
    FT::Array<double> *AI = new Array<double>("EP_HDF5:./matrix.h5:/i", chunk_size, overlap_size);
    FT::Array<double> *AJ = new Array<double>("EP_HDF5:./matrix.h5:/j", chunk_size, overlap_size);
    FT::Array<double> *AV = new Array<double>("EP_HDF5:./matrix.h5:/v", chunk_size, overlap_size);

    //Problem: Restore AI/AJ/AV into memory by chunk
    std::vector<unsigned long long> start, end;
    start.push_back(ft_rank * 1535);
    end.push_back(ft_rank * 1535 + 1535 - 1);
    AI->ReadArray(start, end, VI);
    AJ->ReadArray(start, end, VJ);
    AV->ReadArray(start, end, VV);

    //Problem:
    //  1) overlap_size is not-predefined
    std::vector<int> chunk_size_v = {512};
    std::vector<int> overlap_size_v = {1};

    FT::Array<double> *X = new Array<double>("EP_HDF5:./vector.h5:/v", chunk_size_v, overlap_size_v);

    //Y =  A * X
    FT::Array<double> *Y = new Array<double>("EP_HDF5:./y.h5:/v", chunk_size_v, overlap_size_v);

    X->EnableApplyStride(chunk_size_v);

    //Run
    X->Transform(udf_spmv, Y);

    //Report the cost of UDF, I/O, etc,
    X->ReportCost();

    //Clear
    delete AI;
    delete AJ;
    delete AV;
    delete X;
    delete Y;

    FT_Finalize();

    return 0;
}
