
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

struct InputStruct
{
    AU_UDT_INIT(InputStruct)
    int i;
    int j;
    int k;
};

struct OutputStruct
{
    AU_UDT_INIT(OutputStruct)
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

    std::cout << A->GetValue(0, 0) << "\n";

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
