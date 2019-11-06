/*
 *ArrayUDF Copyright (c) 2017, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy).  All rights reserved.

 *If you have questions about your rights to use or distribute this software, please contact Berkeley Lab's Innovation & Partnerships Office at  IPO@lbl.gov.

* NOTICE. This Software was developed under funding from the U.S. Department of Energy and the U.S. Government consequently retains certain rights. As such, the U.S. Government has been granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software to reproduce, distribute copies to the public, prepare derivative works, and perform publicly and display publicly, and to permit other to do so. 
 */
/**
 *
 * Email questions to {dbin, kwu, sbyna}@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include "au.h"

using namespace std;
using namespace AU;

#define VEC_SIZE 8

/**
 * @brief UDF to support output of vector
 * 
 * @param iStencil : input stencil
 * @return Stencil<std::vector<float>> : output stencil
 */
inline Stencil<std::vector<float>> udf_vector(const Stencil<float> &iStencil)
{

    Stencil<std::vector<float>> oStencil;
    std::vector<float> temp_vec(VEC_SIZE);

    for (int i = 0; i < VEC_SIZE; i++)
    {
        temp_vec[i] = iStencil(0, i) + 2.0;
    }
    oStencil = temp_vec;
    return oStencil;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {4, 16};
    std::vector<int> overlap_size = {0, 0};

    //Orginal data set
    Array<float> *A = new Array<float>("EP_HDF5:./testf-16x16.h5p:/testg/testd", chunk_size, overlap_size);

    //Results data to be stored in a file
    Array<float> *B = new Array<float>("EP_HDF5:./testf-16x16-f2.h5p:/testg/testd");

    //Set the direction to flat vector of output
    A->SetVectorDirection(AU_FLAT_OUTPUT_ROW);

    //Skip paramter, run the udf_vector on the first cell of each row
    std::vector<int> strip_size = {1, 16};
    A->SetApplyStrip(strip_size);

    //Run udf_vector
    A->Apply(udf_vector, B);

    //Clear
    delete A;
    delete B;

    AU_Finalize();

    return 0;
}
