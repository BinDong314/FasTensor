
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

struct Field
{
    AU_UDT_INIT(Field)
    float ejx, ejy, ejz;
    float ijx, ijy, ijz;
};

struct Diagnostics
{
    AU_UDT_INIT(Diagnostics)
    float jx, jy, jz;
    float absJ;
};

inline Stencil<Diagnostics> udf_vpic_diag(const Stencil<Field> &iStencil)
{
    Diagnostics dia;
    //Total Current
    dia.jx = iStencil(0, 0, 0).ejx + iStencil(0, 0, 0).ijx;
    dia.jy = iStencil(0, 0, 0).ejy + iStencil(0, 0, 0).ijy;
    dia.jz = iStencil(0, 0, 0).ejz + iStencil(0, 0, 0).ijz;
    //Absolute Value of Current
    dia.absJ = sqrtf(dia.jx * dia.jx + dia.jy * dia.jy + dia.jz * dia.jz);
    return Stencil<Diagnostics>(dia);
}

int main(int argc, char *argv[])
{
    AU_Init(argc, argv);

    std::vector<int> cs = {4, 4, 4};

    FT::Array<Field> *A = new FT::Array<Field>(cs);
    A->AppendAttribute<float>("EP_HDF5:./hydro_electron.h5:/Timestep_1/jx");
    A->AppendAttribute<float>("EP_HDF5:./hydro_electron.h5:/Timestep_1/jy");
    A->AppendAttribute<float>("EP_HDF5:./hydro_electron.h5:/Timestep_1/jz");
    A->AppendAttribute<float>("EP_HDF5:./hydro_ion.h5:/Timestep_1/jx");
    A->AppendAttribute<float>("EP_HDF5:./hydro_ion.h5:/Timestep_1/jy");
    A->AppendAttribute<float>("EP_HDF5:./hydro_ion.h5:/Timestep_1/jz");

    FT::Array<Diagnostics> *B = new FT::Array<Diagnostics>();
    B->AppendAttribute<float>("EP_HDF5:./vpic_diag.h5:/Timestep_1/jx");
    B->AppendAttribute<float>("EP_HDF5:./vpic_diag.h5:/Timestep_1/jy");
    B->AppendAttribute<float>("EP_HDF5:./vpic_diag.h5:/Timestep_1/jz");
    B->AppendAttribute<float>("EP_HDF5:./vpic_diag.h5:/Timestep_1/absJ");

    A->Apply(udf_vpic_diag, B);

    delete A;
    delete B;
    AU_Finalize();
    return 0;
}
