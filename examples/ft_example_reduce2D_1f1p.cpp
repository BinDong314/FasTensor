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
 * @example ft_example_reduce2D_1f1p.cpp
 * @author Bin Dong (dbin@lbl.gov)
 * @brief This example show how to reduce 2D data  in FasTensor .  
 * @version 0.1
 * @date 2021-05-28
 * 
 * @copyright Copyright (c) 2021
 * 
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

/**
 * @brief Example to sum each row of A (8 x 8)
 *        Store result in H (8 x 2)
 *        H[0,0] = sum (A[0,0], A[0,1], A[0,2], A[0,3])  
 *        H[0,1] = sum (A[0,4], A[0,5], A[0,6], A[0,7])  
 *        H[1,0] = sum (A[1,0], A[1,1], A[1,2], A[1,3]) 
 *        ...  ...
 *       
 *        A points to  a list of files and each have size of 8 x 8.
 */

std::vector<unsigned long long> H_size = {8, 2};
Array<float> *H;

inline Stencil<float> udf_hist_2d(const Stencil<float> &iStencil)
{

    float temp_value = 0;
    int temp_index;
    std::vector<unsigned long long> temp_coord = iStencil.GetCoordinate();
    temp_index = temp_coord[0];

    for (int i = 0; i < 4; i++)
        temp_value += iStencil(0, i);
    H->SetValue(H->GetValue(temp_index, 0) + temp_value, temp_index, 0);
    //std::cout << "temp_index = " << temp_index << ", temp_value 1 =  " << temp_value << " H->GetValue(temp_index, 0) =" << H->GetValue(temp_index, 0) << std::endl;

    temp_value = 0;
    for (int i = 4; i < 8; i++)
        temp_value += iStencil(0, i);
    H->SetValue(H->GetValue(temp_index, 1) + temp_value, temp_index, 1);

    //std::cout << "temp_index = " << temp_index << ", temp_value 2 =  " << temp_value << std::endl;

    return 0;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {8, 8};
    std::vector<int> overlap_size = {0, 0};

    H = new Array<float>("EP_MEMORY", H_size);

    //Input data, 8 by 8
    Array<float> *A = new Array<float>("EP_DIR:EP_HDF5:./test-data/test_1f1p_dir_2d:/testg/testd", chunk_size, overlap_size);

    //Initialize H
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            H->SetValue(0, i, j);
        }
    }

    //Result data
    H->Clone();

    std::vector<int> skip_size = {1, 8};
    A->EnableApplyStride(skip_size);

    //Run
    A->Apply(udf_hist_2d);

    H->Merge(AU_SUM);

    H->Nonvolatile("EP_HDF5:./test-data/test_1f1p_dir_2d_result.h5:/testg/testd");

    //Clear
    delete A;
    delete H;

    AU_Finalize();

    return 0;
}
