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
#include "array_udf.h"

using namespace std;

//UDF One: duplicate the original data
inline Stencil<float> myfunc1(const Stencil<float> &iStencil)
{
    Stencil<float> oStencil;
    oStencil = iStencil(0, 0) + 2.0;
    return oStencil;
}

#define VEC_SIZE 16
//UDF One: duplicate the original data
inline Stencil<std::vector<float>> myfunc2(const Stencil<float> &iStencil)
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

inline Stencil<float> myfunc3(const Stencil<float> &iStencil)
{

    Stencil<float> oStencil;
    if (iStencil(0) > 100.0)
    {
        oStencil = iStencil(0);
    }
    return oStencil;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {4, 16};
    std::vector<int> overlap_size = {1, 1};

    //Orginal data set
    Array<float> *A = new Array<float>("EP_HDF5:./testf-16x16.h5p:/testg:/testg/testd", chunk_size, overlap_size);

    //Results data 1 to be stored in a file
    Array<float> *B = new Array<float>("EP_HDF5:./testf-16x16-f1.h5p:/testg:/testg/testd");
    //A->Apply(myfunc1, B);
    delete B;

    //Results data 2 to be stored in a file
    Array<float> *C = new Array<float>("AU_HDF5:./testf-16x16-f2.h5p:/testg:/testg/testd");
    std::vector<int> strip_size = {1, 16};
    //A->SetOutputVector(VEC_SIZE, 1);
    //A->SetApplyStripSize(strip_size);
    //A->Apply(myfunc2, C);

    //Clear
    delete A;
    delete C;

    AU_Finalize();

    return 0;
}
