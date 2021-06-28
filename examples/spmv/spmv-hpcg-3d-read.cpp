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

std::vector<double> neighbors_vector_values;
std::vector<int> neighbors_vector_start = {-1, -1, -1}, neighbors_vector_end = {1, 1, 1};
double result;
//UDF One: duplicate the original data
inline int udf_hpcg_3d(const Stencil<double> &iStencil, Stencil<double> &oStencil)
{

    //double result = 0;
    //double result = -iStencil(-1, -1, -1) - iStencil(-1, -1, 0) - iStencil(-1, -1, 1) - iStencil(-1, 0, -1) - iStencil(-1, 0, 0) - iStencil(-1, 0, 1) - iStencil(-1, 1, -1) - iStencil(-1, 1, 0) - iStencil(-1, 1, 1) - iStencil(0, -1, -1) - iStencil(0, -1, 0) - iStencil(0, -1, 1) - iStencil(0, 0, -1) + 26 * iStencil(0, 0, 0) - iStencil(0, 0, 1) - iStencil(0, 1, -1) - iStencil(0, 1, 0) - iStencil(0, 1, 1) - iStencil(1, -1, -1) - iStencil(1, -1, 0) - iStencil(1, -1, 1) - iStencil(1, 0, -1) - iStencil(1, 0, 0) - iStencil(1, 0, 1) - iStencil(1, 1, -1) - iStencil(1, 1, 0) - iStencil(1, 1, 1);

    iStencil.ReadNeighbors(neighbors_vector_start, neighbors_vector_end, neighbors_vector_values);
    result = 26 * neighbors_vector_values[13] - neighbors_vector_values[0] - neighbors_vector_values[1] - neighbors_vector_values[2] - neighbors_vector_values[3] - neighbors_vector_values[4] - neighbors_vector_values[5] - neighbors_vector_values[6] - neighbors_vector_values[7] - neighbors_vector_values[8] - -neighbors_vector_values[9] - neighbors_vector_values[10] - neighbors_vector_values[11] - neighbors_vector_values[12] - neighbors_vector_values[14] - neighbors_vector_values[15] - neighbors_vector_values[16] - neighbors_vector_values[17] - neighbors_vector_values[18] - neighbors_vector_values[19] - neighbors_vector_values[20] - neighbors_vector_values[21] - neighbors_vector_values[22] - neighbors_vector_values[23] - neighbors_vector_values[24] - neighbors_vector_values[25] - neighbors_vector_values[26];
    oStencil = result;
    //return Stencil<double>(result);
    return 0;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    FT_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {512, 512, 256};
    std::vector<int> overlap_size = {1, 1, 1};

    //Input Matrix
    FT::Array<double> *A = new Array<double>("EP_HDF5:./matrix-3d.h5:/dat", chunk_size, overlap_size);

    //Y =  A * X
    //In this case, size of Y is equal to size of X
    FT::Array<double> *Y = new Array<double>("EP_HDF5:./y-3d.h5:/v");

    neighbors_vector_values.resize(27);

    //Run
    A->TransformTest(udf_hpcg_3d, Y);

    //Report the cost of UDF, I/O, etc,
    A->ReportCost();

    //Clear
    delete A;
    delete Y;

    FT_Finalize();

    return 0;
}
