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

int rows_chunk = 11648;
int cols_chunk = 60000;

//UDF One: duplicate the original data
inline Stencil<double> udf_kf1p(const Stencil<short> &iStencil)
{
    Stencil<double> oStencil;
    double oDouble = iStencil(0, 0) * 1.0;
    oStencil = oDouble;
    return oStencil;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    // set up the chunk size and the overlap size
    // 11648, 30000 for each dataset
    std::vector<int> chunk_size(2);
    chunk_size[0] = rows_chunk;
    chunk_size[1] = cols_chunk;
    std::vector<int> overlap_size = {0, 0};

    //Input data
    Array<short> *A = new Array<short>("EP_DIR:EP_TDMS:/Users/dbin/work/arrayudf-git-svn-test-on-bitbucket/examples/das/tdms-dir", chunk_size, overlap_size);
    std::vector<std::string> aug_merge_index, aug_dir_sub_cmd, aug_input_search_rgx;

    aug_merge_index.push_back("1");
    aug_dir_sub_cmd.push_back("BINARY_ENABLE_TRANSPOSE_ON_READ");
    aug_input_search_rgx.push_back("^(.*)[135]\\.tdms$");

    A->EndpointControl(DIR_MERGE_INDEX, aug_merge_index);
    //A->EndpointControl(DIR_SUB_CMD_ARG, aug_dir_sub_cmd1); //Not needed
    A->EndpointControl(DIR_SUB_CMD_ARG, aug_dir_sub_cmd);
    A->EndpointControl(DIR_INPUT_SEARCH_RGX, aug_input_search_rgx);

    //Result data

    //Store into a single file
    //Array<double> *B = new Array<double>("EP_HDF5:./tdms-dir-dec/test.h5:/DataCT");

    //Store into multiple file
    Array<double> *B = new Array<double>("EP_DIR:EP_HDF5:./tdms-dir-dec/:/DataCT");

    std::vector<std::string> aug_output_replace_arg;
    aug_output_replace_arg.push_back("^(.*)\\.tdms$");
    aug_output_replace_arg.push_back("$1.h5");
    B->EndpointControl(DIR_MERGE_INDEX, aug_merge_index);
    B->EndpointControl(DIR_OUPUT_REPLACE_RGX, aug_output_replace_arg);
    //Run
    A->Apply(udf_kf1p, B);

    //Clear
    delete A;
    delete B;

    AU_Finalize();

    return 0;
}
