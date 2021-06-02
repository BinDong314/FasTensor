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
 * @example ft_example_stack.cpp
 * @author Bin Dong (dbin@lbl.gov)
 * @brief This example show how to output a vector in FasTensor .  
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
#include <math.h>
#include "au.h"
#include "DasLib.h"

using namespace std;
using namespace AU;

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

#define LTS 14999 //length of time series
#define CHS 201   //channels

double t_start = -59.9920000000000, t_end = 59.9920000000000, sample_rate = 0.00800000000000000;

std::vector<unsigned long long> H_size = {CHS, LTS};
Array<double> *H;
Array<double> semblanceSum_denom;

inline Stencil<double> stack_udf(const Stencil<double> &iStencil)
{
    std::vector<double> ts(LTS);
    for (int i = 0; i < LTS; i++)
    {
        ts[i] = iStencil(0, i);
    }
    //Remove the media
    double median = DasLib::Median(ts);
    for (int i = 0; i < LTS; i++)
    {
        ts[i] = ts[i] - median;
    }

    //Subset
    std::vector<double> ts_sub;
    ts_sub = TimeSubset(t_start, t_end, -59, 59, sample_rate);
    /*
    bool flag = CausalityFlagging(ts_sub, 0.05, 3.0, 10, t_start, t_end, sample_rate);
    if (flag = flase)
    {
        //Flipud(ts_sub);
    }*/

    std::vector<double> semblance_denom(LTS_new);
    std::vector<std::complex> coherency;

    for (int i = 0; i < LTS_new; i++)
    {
        semblance_denom[i] = ts_sub[i] * ts_sub[i];
    }

    coherency = instanPhaseEstimator(ts_sub);

    int temp_index;
    std::vector<unsigned long long> temp_coord = iStencil.GetCoordinate();
    temp_index = temp_coord[0];
    for (int i = 0; i < LTS; i++)
    {
        H->SetValue(H->GetValue(temp_index, i) + ts[i], temp_index, i);
    }

    //std::cout << "finish one file, temp_index = " << temp_index << std::endl;
    return 0;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {201, 14999};
    std::vector<int> overlap_size = {0, 0};

    H = new Array<double>("EP_MEMORY", H_size);

    //Input data, 8 by 8
    Array<double> *A = new Array<double>("EP_DIR:EP_HDF5:/Users/dbin/work/arrayudf-git-svn-test-on-bitbucket/examples/das/stacking_files/xcorr_examples_h5:/xcoor", chunk_size, overlap_size);

    //Result data
    H->Clone(0);

    std::vector<int> skip_size = {1, 14999};
    A->EnableApplyStride(skip_size);

    //Run
    A->Apply(stack_udf);

    H->Merge(AU_SUM);

    H->Nonvolatile("EP_HDF5:/Users/dbin/work/arrayudf-git-svn-test-on-bitbucket/examples/das/stacking_files/xcorr_examples_h5_stack.h5:/xcoorstack");

    //Clear
    delete A;
    delete H;

    AU_Finalize();

    return 0;
}
