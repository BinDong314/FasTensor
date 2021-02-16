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

#include "ft.h"
#include <numeric>

int c_ofst = 20, c_n = 2 * c_ofst + 1;
int ch_ofst = 2, w_ofst = 10;
inline Stencil<float> similarity_udf(const Stencil<short> &c)
{
    float max_corr1 = 0, max_corr2 = 0, temp_corr1, temp_corr2;
    float c_sqSum = 0, n_sqSum1, n_sqSum2, cn_Sum1, cn_Sum2;
    std::vector<float> cw(c_n);               //current window
    std::vector<float> nw1(c_n + 2 * w_ofst); //neighbor window 1
    std::vector<float> nw2(c_n + 2 * w_ofst); //neighbor window 2
    for (int i = -c_ofst; i <= c_ofst; i++)
    {
        cw[i + c_ofst] = c(0, i);
    }
    c_sqSum = sqrt(std::inner_product(cw.begin(), cw.end(), cw.begin(), 0));
    for (int j = -c_ofst - w_ofst; j <= c_ofst + w_ofst; j++)
    {
        nw1[j + c_ofst + w_ofst] = c(ch_ofst, j);
        nw2[j + c_ofst + w_ofst] = c(-ch_ofst, j);
    }
    for (int j = -w_ofst; j <= w_ofst; j++)
    {
        int nws = j + w_ofst;
        cn_Sum1 = inner_product(cw.begin(), cw.end(), nw1.begin() + nws, 0);
        cn_Sum2 = inner_product(cw.begin(), cw.end(), nw2.begin() + nws, 0);
        n_sqSum1 = sqrt(inner_product(nw1.begin() + nws, nw1.begin() + nws + c_n, nw1.begin() + nws, 0));
        n_sqSum2 = sqrt(inner_product(nw2.begin() + nws, nw2.begin() + nws + c_n, nw2.begin() + nws, 0));
        temp_corr1 = abs(cn_Sum1) / (c_sqSum * n_sqSum1);
        temp_corr2 = abs(cn_Sum2) / (c_sqSum * n_sqSum2);
        if (temp_corr1 > max_corr1)
            max_corr1 = temp_corr1;
        if (temp_corr2 > max_corr2)
            max_corr2 = temp_corr2;
    }
    return Stencil<float>((max_corr1 + max_corr2) / 2);
}
int main(int argc, char *argv[])
{
    std::vector<int> os = {50, 50};
    std::vector<int> cs = {11648, 30000};
    std::vector<int> ss = {1456, 10000};
    FT_Init(argc, argv);
    FT::Array<short> *IFILE = new FT::Array<short>("EP_HDF5:./das.h5:/DataCT", cs, os);
    FT::Array<float> *OFILE = new FT::Array<float>("EP_HDF5:./das_similarity.h5:/dat");
    IFILE->SetStride(ss);
    IFILE->Transform(similarity_udf, OFILE);
    IFILE->ReportTime();
    delete IFILE;
    delete OFILE;
    FT_Finalize();
    return 0;
}
