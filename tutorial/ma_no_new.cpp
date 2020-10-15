/**
 *
 * Author:  Bin Dong, dbin@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#include "ft.h"
using namespace std;
using namespace FT;
inline Stencil<float> udf_ma(const Stencil<float> &iStencil);

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    FT_Init(argc, argv);
    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {4, 4};
    std::vector<int> overlap_size = {0, 1};
    //Input data
    Array<float> A("EP_HDF5:tutorial.h5:/dat", chunk_size, overlap_size);
    //Result data
    Array<float> B("EP_HDF5:tutorial_ma_new.h5:/dat");
    //Run
    A.Transform(udf_ma, B);
    FT_Finalize();
    return 0;
}
inline Stencil<float> udf_ma(const Stencil<float> &iStencil)
{
    Stencil<float> oStencil;
    oStencil = (iStencil(0, -1) + iStencil(0, 0) + iStencil(0, 1)) / 3.0;
    return oStencil;
}