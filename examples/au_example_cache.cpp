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

//UDF One: duplicate the original data
inline Stencil<float> udf_cache1(const Stencil<float> &iStencil)
{
    Stencil<float> oStencil;
    oStencil = iStencil(0, 0) * 2.0;
    return oStencil;
}

//UDF One: duplicate the original data
inline Stencil<float> udf_cache2(const Stencil<float> &iStencil)
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
    std::vector<int> chunk_size = {4, 16};
    std::vector<int> overlap_size = {1, 1};

    //Input data
    Array<float> *A = new Array<float>("EP_HDF5:./test-data/testf-16x16-cache.h5:/testg/testd", chunk_size, overlap_size);

    //Intermediate data
    Array<float> *B = new Array<float>("EP_MEMORY");

    //Intermediate data
    Array<float> *C = new Array<float>("EP_HDF5:./test-data/testf-16x16-cache-output.h5:/testg/testd");

    //Run
    A->Apply(udf_cache1, B);

    //Dump intermediate data
    B->Nonvolatile("EP_HDF5:./test-data/testf-16x16-cache-intermediate.h5:/testg/testd");

    //Run analysis again
    B->Apply(udf_cache2, C);

    //Clear
    delete A;
    delete B;
    delete C;

    AU_Finalize();

    return 0;
}
