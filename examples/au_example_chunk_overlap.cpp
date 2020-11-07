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
inline Stencil<float> udf_hello_world(const Stencil<float> &iStencil)
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

    std::string endpoint_id = "EP_HDF5:./test-data/testf-16x16-chunk-overlap.h5:/testg/testd";
    //Input data
    Array<float> *A = new Array<float>();

    A->SetEndpoint(endpoint_id);
    A->SetChunkSize(chunk_size);
    A->SetOverlapSize(overlap_size);
    //Result data
    Array<float> *B = new Array<float>("EP_HDF5:./test-data/testf-16x16-chunk-overlap-output.h5:/testg/testd");

    //Run
    A->Apply(udf_hello_world, B);

    std::vector<int> chunk_size_get;
    A->GetChunkSize(chunk_size_get);

    std::vector<int> overlap_size_get;
    A->GetOverlapSize(overlap_size_get);

    std::string endpoint_id_get;
    A->GetEndpoint(endpoint_id_get);
    int ret = 0;
    if (chunk_size_get != chunk_size)
    {
        AU_INFO("Failed: got wrong chunk size");
        ret = -1;
    }

    if (overlap_size_get != overlap_size)
    {
        AU_INFO("Failed: got wrong overlap size");
        ret = -1;
    }

    if (endpoint_id_get != endpoint_id)
    {
        AU_INFO("Failed: got wrong endpoint id");
        ret = -1;
    }

    std::vector<float> readback;
    std::vector<unsigned long long> read_start = {0, 0}, read_end = {15, 15};
    B->ReadArray(read_start, read_end, readback);
    PrintVector("readback", readback);
    //Clear
    delete A;
    delete B;

    AU_Finalize();

    return ret;
}
