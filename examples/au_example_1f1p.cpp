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

//UDF One: duplicate the original data
inline Stencil<float> udf_1f1p(const Stencil<float> &iStencil)
{
    Stencil<float> oStencil;
    oStencil = iStencil(0, 0) * 2.0;
    return oStencil;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {16, 16};
    std::vector<int> overlap_size = {0, 0};

    //Input data
    Array<float> *A = new Array<float>("EP_DIR:EP_HDF5:./test-data/test_1f1p_dir:/testg/testd", chunk_size, overlap_size);

    //Result data
    Array<float> *B = new Array<float>("EP_DIR:EP_HDF5:./test-data/test_1f1p_dir_output:/testg/testd");

    //Run
    A->Apply(udf_1f1p, B);

    //Clear
    delete A;
    delete B;

    AU_Finalize();

    return 0;
}
