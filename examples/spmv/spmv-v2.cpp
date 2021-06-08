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

typedef struct SparseMatrixElementType
{
    AU_UDT_INIT(SparseMatrixElementType)
    double i;
    double j;
    double v;
} SparseMatrixElement;

FT::Array<double> *Y;
std::vector<double> VX;

//UDF One: duplicate the original data
inline Stencil<double> udf_spmv_v2(const Stencil<SparseMatrixElement> &iStencil)
{
    //if (iStencil(0).i % 100 == 0)
    //    std::cout << " iStencil = (" << iStencil(0).i << ", " << iStencil(0).j << ", " << iStencil(0).v << " )\n";
    //VY[iStencil(0).i] += iStencil(0).v * VX[iStencil(0).j];
    int row_index = iStencil(0).i;
    double current_b = Y->GetValue(row_index);

    current_b += iStencil(0).v * VX[iStencil(0).j];

    Y->SetValue(current_b, row_index);
    return 0;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    FT_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {0};
    std::vector<int> overlap_size = {0};

    //Y =  A * X

    //Set up sparse matrix with COO format from three files
    //Partition the data based on chunk_size
    Array<SparseMatrixElement> *A = new Array<SparseMatrixElement>();
    A->PushBackAttribute<double>("EP_HDF5:./matrix.h5:/i");
    A->PushBackAttribute<double>("EP_HDF5:./matrix.h5:/j");
    A->PushBackAttribute<double>("EP_HDF5:./matrix.h5:/v");
    std::vector<unsigned long long> array_size;
    A->GetArraySize(array_size);
    chunk_size[0] = array_size[0] / ft_size;
    A->SetChunkSize(chunk_size);
    A->SetOverlapSize(overlap_size);

    //Find out the size of array, which is a string attribute on /matrix.h5;
    //Use the size to set the size for Y
    std::string array_size_str;
    std::string array_size_str_name = "ArraySize";
    A->GetTag(array_size_str_name, array_size_str);
    std::istringstream array_size_str_stream(array_size_str);
    int array_size_m;
    array_size_str_stream >> array_size_m;
    std::cout << "ArraySize =  " << array_size_str << " ,  array_size_m = " << array_size_m << "\n";

    //Set up the vector X
    //Read X into memory here on each process
    //Problem:  X has lots of duplicated across nodes
    //        Partition X in some way !
    FT::Array<double> *X = new Array<double>("EP_HDF5:./vector.h5:/v");
    X->GetArraySize(array_size);
    std::vector<unsigned long long> start, end;
    start.push_back(0);
    end.push_back(array_size[0] - 1);
    X->ReadArray(start, end, VX);

    //Set up the Y, with size from A (numbere of rows)
    //Problem: Y has lots of duplicated across nodes
    //         Partition Y in some way !
    std::vector<unsigned long long> y_array_size;
    y_array_size.push_back(array_size_m); //PrintVector("y_array_size = ", y_array_size);
    Y = new Array<double>("EP_MEMORY", y_array_size);
    //Duplicate Y on each process, with 0 values
    Y->Clone(0);

    //Run
    A->Transform(udf_spmv_v2);

    //Report the cost of UDF, I/O, etc,
    A->ReportCost();

    //Merge Y on all processes
    Y->Merge(AU_SUM);

    //Save Y into file
    if (!ft_rank)
    {
        Y->Backup("EP_HDF5:./y-v2.h5:/v");
    }
    //Clear
    delete A;
    delete X;
    delete Y;

    FT_Finalize();

    return 0;
}
