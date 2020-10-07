/**
 *
 * Author:  Bin Dong, dbin@lbl.gov
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
