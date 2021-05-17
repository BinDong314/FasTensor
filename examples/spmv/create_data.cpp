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

#define MATRIX_M 1024
#define MATRIX_N MATRIX_M
#define N_SPARSE_ELEMENTS (MATRIX_M * 3 - 2)

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    //Input data
    Array<double> *AI = new Array<double>("EP_HDF5:./matrix.h5:/i");
    Array<double> *AJ = new Array<double>("EP_HDF5:./matrix.h5:/j");
    Array<double> *AV = new Array<double>("EP_HDF5:./matrix.h5:/v");

    std::vector<unsigned long long> data_size;

    std::vector<unsigned long long> start_addr, end_addr;
    /**
     * Write data point
     * 
     */
    std::vector<double> IV, JV, VV, XV;
    for (int i = 0; i < MATRIX_M; i++)
    {
        if (i - 1 >= 0)
        {
            IV.push_back(i);
            JV.push_back(i - 1);
            VV.push_back(1);
        }

        IV.push_back(i);
        JV.push_back(i);
        VV.push_back(-2);

        if (i + 1 < MATRIX_N)
        {
            IV.push_back(i);
            JV.push_back(i + 1);
            VV.push_back(1);
        }
    }

    data_size.push_back(IV.size());
    //PrintVector("data_size =", data_size);
    AI->SetArraySize(data_size);
    AJ->SetArraySize(data_size);
    AV->SetArraySize(data_size);

    start_addr.push_back(0);
    end_addr.push_back(IV.size() - 1);

    AI->WriteArray(start_addr, end_addr, IV);
    AJ->WriteArray(start_addr, end_addr, JV);
    AV->WriteArray(start_addr, end_addr, VV);

    //Input data
    Array<double> *AX = new Array<double>("EP_HDF5:./vector.h5:/v");
    for (int i = 0; i < MATRIX_N; i++)
    {
        XV.push_back(i + 0.1);
    }

    data_size.clear();
    start_addr.clear();
    end_addr.clear();

    data_size.push_back(MATRIX_N);
    AX->SetArraySize(data_size);

    start_addr.push_back(0);
    end_addr.push_back(MATRIX_N - 1);

    AX->WriteArray(start_addr, end_addr, XV);

    std::string array_size_str;
    array_size_str = std::to_string(MATRIX_M) + " " + std::to_string(MATRIX_N);
    AI->SetTag("ArraySize", array_size_str);
    AJ->SetTag("ArraySize", array_size_str);
    AV->SetTag("ArraySize", array_size_str);

    delete AI;
    delete AJ;
    delete AV;
    delete AX;

    AU_Finalize();

    return 0;
}
