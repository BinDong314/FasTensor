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

struct InputStruct
{
    int i;
    int j;
    int k;
};

struct OutputStruct
{
    int i;
    int j;
    int k;
    float ave;
};

//UDF One: duplicate the original data
inline Stencil<OutputStruct> udf_vds(const Stencil<InputStruct> &iStencil)
{
    OutputStruct ovds;
    Stencil<OutputStruct> oStencil;

    ovds.i = iStencil(0, 0).i;
    ovds.j = iStencil(0, 0).j;
    ovds.k = iStencil(0, 0).k;
    ovds.ave = (iStencil(0, 0).i + iStencil(0, 0).j + iStencil(0, 0).k) / 3.0;

    oStencil = ovds;
    return oStencil;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {4, 16};
    std::vector<int> overlap_size = {1, 1};

    //Input data
    Array<InputStruct> *A = new Array<InputStruct>(chunk_size, overlap_size);
    A->PushBackAttribute<int>("EP_HDF5:./test-data/testf-16x16-vds.h5:/testg/x");
    A->PushBackAttribute<int>("EP_HDF5:./test-data/testf-16x16-vds.h5:/testg/y");
    A->PushBackAttribute<int>("EP_HDF5:./test-data/testf-16x16-vds.h5:/testg/z");

    A->PrintEndpointInfo();

    //Result data
    Array<OutputStruct> *B = new Array<OutputStruct>();
    B->PushBackAttribute<int>("EP_HDF5:./test-data/testf-16x16-vds-output.h5:/testg/x");
    B->PushBackAttribute<int>("EP_HDF5:./test-data/testf-16x16-vds-output.h5:/testg/y");
    B->PushBackAttribute<int>("EP_HDF5:./test-data/testf-16x16-vds-output.h5:/testg/z");
    B->PushBackAttribute<float>("EP_HDF5:./test-data/testf-16x16-vds-output.h5:/testg/ave");

    B->PrintEndpointInfo();

    //Run
    A->Apply(udf_vds, B);

    //Clear
    delete A;
    delete B;

    AU_Finalize();

    return 0;
}
