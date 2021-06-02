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
 * @example ft_example_vector_2d.cpp
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
#include "ft.h"

using namespace std;
using namespace FT;

//Chunk size
#define INPUT_CHK_ROWS 4
#define INPUT_CHK_COLS 4

//Size of the output vector per chunk
#define OUTPUT_VEC_ROWS 2
#define OUTPUT_VEC_COLS 2

/**
 * @brief UDF to support output of 2D vector
 * 
 * @param iStencil : input stencil
 * @return Stencil<std::vector<float>> : output stencil
 */
inline Stencil<std::vector<float>> udf_vector_2d(const Stencil<float> &iStencil)
{

    Stencil<std::vector<float>> oStencil;
    std::vector<float> temp_vec(OUTPUT_VEC_ROWS * OUTPUT_VEC_COLS);

    //each output element is sum of a OUTPUT_VEC_ROWS by OUTPUT_VEC_COLS
    //e.g. input 4 by 4 chunks is
    //      A[0][0] A[0][1] A[0][2] A[0][3]
    //      A[1][0] A[1][1] A[1][2] A[1][3]
    //      A[2][0] A[2][1] A[2][2] A[2][3]
    //      A[3][0] A[3][1] A[3][2] A[3][3]
    //
    // The output B is
    //      B[0][0] =  A[0][0] +  A[0][1] + A[1][0] + A[1][1]
    //      B[0][1] =  A[0][2] +  A[0][3] + A[1][2] + A[1][3]
    //      .....
    for (int i = 0; i < OUTPUT_VEC_ROWS; i++)
    {
        for (int j = 0; j < OUTPUT_VEC_COLS; j++)
        {
            temp_vec[i * OUTPUT_VEC_COLS + j] = iStencil(i * OUTPUT_VEC_ROWS, j * OUTPUT_VEC_COLS) + iStencil(i * OUTPUT_VEC_ROWS, j * OUTPUT_VEC_COLS + 1) + iStencil(i * OUTPUT_VEC_ROWS + 1, j * OUTPUT_VEC_COLS) + iStencil(i * OUTPUT_VEC_ROWS + 1, j * OUTPUT_VEC_COLS + 1);
        }
    }
    oStencil = temp_vec;

    PrintVector("temp_vec = ", temp_vec);
    std::vector<size_t> vector_shape(2);
    vector_shape[0] = OUTPUT_VEC_ROWS;
    vector_shape[1] = OUTPUT_VEC_COLS;
    oStencil.SetShape(vector_shape);
    return oStencil;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {4, 4};
    std::vector<int> overlap_size = {0, 0};

    //Orginal data set
    Array<float> *A = new Array<float>("EP_HDF5:./test-data/testf-16x16-vector-2d.h5:/testg/testd", chunk_size, overlap_size);

    //Results data to be stored in a file
    Array<float> *B = new Array<float>("EP_HDF5:./test-data/testf-16x16-vector-2d-output.h5:/testg/testd");

    //Set the direction to flat vector of output
    //A->SetVectorDirection(AU_FLAT_OUTPUT_ROW);

    //Skip paramter, run the udf_vector on the first cell of each 4 by 4 chunk
    std::vector<int> skip_size = {4, 4};
    A->EnableApplyStride(skip_size);

    //Run udf_vector
    A->Apply(udf_vector_2d, B);

    //std::vector<float> AV = A->ReadArray(std::vector<unsigned long long> (0,0), std::vector<unsigned long long> (16,16));
    //std::vector<float> BV = B->ReadArray();

    //Clear
    delete A;
    delete B;

    AU_Finalize();

    return 0;
}
