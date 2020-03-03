/*
 *ArrayUDF Copyright (c) 2017, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy).  All rights reserved.

 *If you have questions about your rights to use or distribute this software, please contact Berkeley Lab's Innovation & Partnerships Office at  IPO@lbl.gov.

* NOTICE. This Software was developed under funding from the U.S. Department of Energy and the U.S. Government consequently retains certain rights. As such, the U.S. Government has been granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software to reproduce, distribute copies to the public, prepare derivative works, and perform publicly and display publicly, and to permit other to do so. 
 */
/**
 *
 * Email questions to dbin@lbl.gov
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

std::vector<unsigned long long> H_size = {4};
Array<float> *H;

//UDF One: duplicate the original data
inline Stencil<float> udf_hist(const Stencil<float> &iStencil)
{
    if (iStencil(0) < 40)
    {
        H->SetValue(H->GetValue(0) + 1, 0);
    }
    else if (iStencil(0) >= 40 && iStencil(0) < 80)
    {
        H->SetValue(H->GetValue(1) + 1, 1);
    }
    else if (iStencil(0) >= 80 && iStencil(0) < 120)
    {
        H->SetValue(H->GetValue(2) + 1, 2);
    }
    else if (iStencil(0) >= 120)
    {
        H->SetValue(H->GetValue(3) + 1, 3);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {32};
    std::vector<int> overlap_size = {0};

    H = new Array<float>("EP_MEMORY", H_size);
    //Input data
    Array<float> *A = new Array<float>("EP_HDF5:./test-data/testf-16-reduce.h5:/testg/testd", chunk_size, overlap_size);

    //Initialize H
    for (int i = 0; i < 4; i++)
    {
        H->SetValue(0, i);
    }

    std::cout << "Enable Local Mirros: " << std::endl;
    //Result data
    H->Clone(0);

    //Run
    A->Apply(udf_hist);

    H->Merge(AU_SUM);

    H->Nonvolatile("EP_HDF5:./test-data/testf-16-reduce-hist.h5:/testg/testd");

    //Clear
    delete A;
    delete H;

    AU_Finalize();

    return 0;
}
