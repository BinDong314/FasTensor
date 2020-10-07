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
