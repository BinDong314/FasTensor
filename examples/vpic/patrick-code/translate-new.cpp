// overload_array.cpp
// overloading the c++ array subscript operator []
//http://neondataskills.org/HDF5/TimeSeries-Data-In-HDF5-Using-R/

#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include <math.h> /* ceil  and floor*/
#include <cstring>
#include <optional> //Need c++17

#include "array_udf.h"

const float mi_me = 1836.2;

using namespace std;

//Define a compund data strcuture
struct CompoundPoint
{
    //Add this to expose to ArrayUDF
    AU_UDT_INIT(CompoundPoint)
    //Field dump
    float ex, ey, ez;
    float bx, by, bz;
    // densities
    float e1n, e2n, i1n, i2n;
    //electron current
    float e1jx, e1jy, e1jz;
    float e2jx, e2jy, e2jz;
    //ion current vector
    float i1jx, i1jy, i1jz;
    float i2jx, i2jy, i2jz;
    // electron pressure
    float e1pxx, e1pyy, e1pzz, e1pxy, e1pxz, e1pyz;
    float e2pxx, e2pyy, e2pzz, e2pxy, e2pxz, e2pyz;
    float e1pyx, e1pzx, e1pzy, e2pyx, e2pzx, e2pzy;
    // ion pressure
    float i1pxx, i1pyy, i1pzz, i1pxy, i1pxz, i1pyz;
    float i2pxx, i2pyy, i2pzz, i2pxy, i2pxz, i2pyz;
    float i1pyx, i1pzx, i1pzy, i2pyx, i2pzx, i2pzy;
};

struct OutputCompoundPoint
{
    //Add this to expose to ArrayUDF
    AU_UDT_INIT(OutputCompoundPoint)
    float ex, ey, ez;
    float Eidealx, Eidealy, Eidealz;
    float Enonidealx, Enonidealy, Enonidealz;
    float Dideal;
    float Dnonideal;
    float bx, by, bz;
    float ne;
    float ni;
    float rho;
    float emixing;
    float vex, vey, vez;
    float ux, uy, uz;
    float uex, uey, uez;
    float uix, uiy, uiz;
    float pexx, peyy, pezz, pexy, pexz, peyz, peyx, pezx, pezy;
    float pixx, piyy, pizz, pixy, pixz, piyz, piyx, pizx, pizy;
    float absB;
    float jx, jy, jz;
    float absJ;
};

inline OutputCompoundPoint extract_All_UDF(const Stencil<CompoundPoint> &hp)
{
    CompoundPoint hpt = hp(0, 0, 0);
    OutputCompoundPoint op;

    float by = hpt.by;
    float bz = hpt.bz;
    float bx = hpt.bx;
    float ex = hpt.ex;
    float ey = hpt.ey;
    float ez = hpt.ez;

    op.ex = ex;
    op.ey = ey;
    op.ez = ez;

    {

        float ne = fabs(hpt.e1n + hpt.e2n);
        float ni = hpt.i1n + hpt.i2n;
        float jey = hpt.e1jy + hpt.e2jy;
        float jiy = hpt.i1jy + hpt.i2jy;
        float jez = hpt.e1jz + hpt.e2jz;
        float jiz = hpt.i1jz + hpt.i2jz;
        float uy = 0.;
        float uz = 0.;
        if (ne + ni > 0.)
        {
            uy = (jey + jiy * mi_me) / (ne + ni * mi_me);
            uz = (jez + jiz * mi_me) / (ne + ni * mi_me);
        }
        op.Eidealx = by * uz - bz * uy;
    }

    {
        float ne = fabs(hpt.e1n + hpt.e2n);
        float ni = hpt.i1n + hpt.i2n;
        float jex = hpt.e1jx + hpt.e2jx;
        float jix = hpt.i1jx + hpt.i2jx;
        float jez = hpt.e1jz + hpt.e2jz;
        float jiz = hpt.i1jz + hpt.i2jz;
        float ux = 0.;
        float uz = 0.;
        if (ne + ni > 0.)
        {
            ux = (jex + jix * mi_me) / (ne + ni * mi_me);
            uz = (jez + jiz * mi_me) / (ne + ni * mi_me);
        }

        op.Eidealy = bz * ux - bx * uz;
    }
    {
        float ne = fabs(hpt.e1n + hpt.e2n);
        float ni = hpt.i1n + hpt.i2n;
        float jex = hpt.e1jx + hpt.e2jx;
        float jix = hpt.i1jx + hpt.i2jx;
        float jey = hpt.e1jy + hpt.e2jy;
        float jiy = hpt.i1jy + hpt.i2jy;
        float ux = 0.;
        float uy = 0.;
        if (ne + ni > 0.)
        {
            ux = (jex + jix * mi_me) / (ne + ni * mi_me);
            uy = (jey + jiy * mi_me) / (ne + ni * mi_me);
        }
        op.Eidealz = bx * uy - by * ux;
    }

    {
        float ne = fabs(hpt.e1n + hpt.e2n);
        float ni = hpt.i1n + hpt.i2n;
        float jey = hpt.e1jy + hpt.e2jy;
        float jiy = hpt.i1jy + hpt.i2jy;
        float jez = hpt.e1jz + hpt.e2jz;
        float jiz = hpt.i1jz + hpt.i2jz;
        float uy = 0.;
        float uz = 0.;
        if (ne + ni > 0.)
        {
            uy = (jey + jiy * mi_me) / (ne + ni * mi_me);
            uz = (jez + jiz * mi_me) / (ne + ni * mi_me);
        }
        op.Enonidealx = ex - (by * uz - bz * uy);
    }

    {
        float ne = fabs(hpt.e1n + hpt.e2n);
        float ni = hpt.i1n + hpt.i2n;
        float jex = hpt.e1jx + hpt.e2jx;
        float jix = hpt.i1jx + hpt.i2jx;
        float jez = hpt.e1jz + hpt.e2jz;
        float jiz = hpt.i1jz + hpt.i2jz;
        float ux = 0.;
        float uz = 0.;
        if (ne + ni > 0.)
        {
            ux = (jex + jix * mi_me) / (ne + ni * mi_me);
            uz = (jez + jiz * mi_me) / (ne + ni * mi_me);
        }
        op.Enonidealy = ey - (bz * ux - bx * uz);
    }

    {
        float ne = fabs(hpt.e1n + hpt.e2n);
        float ni = hpt.i1n + hpt.i2n;
        float jex = hpt.e1jx + hpt.e2jx;
        float jix = hpt.i1jx + hpt.i2jx;
        float jey = hpt.e1jy + hpt.e2jy;
        float jiy = hpt.i1jy + hpt.i2jy;
        float ux = 0.;
        float uy = 0.;
        if (ne + ni > 0.)
        {
            ux = (jex + jix * mi_me) / (ne + ni * mi_me);
            uy = (jey + jiy * mi_me) / (ne + ni * mi_me);
        }
        op.Enonidealz = ez - (bx * uy - by * ux);
    }

    {
        float jex = hpt.e1jx + hpt.e2jx;
        float jey = hpt.e1jy + hpt.e2jy;
        float jez = hpt.e1jz + hpt.e2jz;
        float jix = hpt.i1jx + hpt.i2jx;
        float jiy = hpt.i1jy + hpt.i2jy;
        float jiz = hpt.i1jz + hpt.i2jz;
        float jx = jex + jix;
        float jy = jey + jiy;
        float jz = jez + jiz;
        float ne = fabs(hpt.e1n + hpt.e2n);
        float ni = hpt.i1n + hpt.i2n;
        float ux = 0.;
        float uy = 0.;
        float uz = 0.;
        if (ne + ni > 0.)
        {
            ux = (jex + jix * mi_me) / (ne + ni * mi_me);
            uy = (jey + jiy * mi_me) / (ne + ni * mi_me);
            uz = (jez + jiz * mi_me) / (ne + ni * mi_me);
        }
        float eidealx = by * uz - bz * uy;
        float eidealy = bz * ux - bx * uz;
        float eidealz = bx * uy - by * ux;
        op.Dideal = jx * eidealx + jy * eidealy + jz * eidealz;
    }

    {
        float jex = hpt.e1jx + hpt.e2jx;
        float jey = hpt.e1jy + hpt.e2jy;
        float jez = hpt.e1jz + hpt.e2jz;
        float jix = hpt.i1jx + hpt.i2jx;
        float jiy = hpt.i1jy + hpt.i2jy;
        float jiz = hpt.i1jz + hpt.i2jz;
        float jx = jex + jix;
        float jy = jey + jiy;
        float jz = jez + jiz;
        float ne = fabs(hpt.e1n + hpt.e2n);
        float ni = hpt.i1n + hpt.i2n;
        float ux = 0.;
        float uy = 0.;
        float uz = 0.;
        if (ne + ni > 0.)
        {
            ux = (jex + jix * mi_me) / (ne + ni * mi_me);
            uy = (jey + jiy * mi_me) / (ne + ni * mi_me);
            uz = (jez + jiz * mi_me) / (ne + ni * mi_me);
        }
        float enonidealx = ex - (by * uz - bz * uy);
        float enonidealy = ey - (bz * ux - bx * uz);
        float enonidealz = ez - (bx * uy - by * ux);
        op.Dnonideal = jx * enonidealx + jy * enonidealy + jz * enonidealz;
    }
    op.bx = hpt.bx;
    op.by = hpt.by;
    op.bz = hpt.bz;

    op.ne = hpt.e1n + hpt.e2n;
    op.ni = hpt.i1n + hpt.i2n;

    {
        float ne = fabs(hpt.e1n + hpt.e2n);
        float ni = hpt.i1n + hpt.i2n;
        op.rho = ni - ne;
    }

    op.emixing = hpt.e1n / (hpt.e1n + hpt.e2n);

    {
        float ne = fabs(hpt.e1n + hpt.e2n);
        float jex = hpt.e1jx + hpt.e2jx;
        float jey = hpt.e1jy + hpt.e2jy;
        float jez = hpt.e1jz + hpt.e2jz;
        float uex = 0.;
        float uey = 0.;
        float uez = 0.;
        if (ne > 0.)
        {
            uex = jex / ne;
            uey = jey / ne;
            uez = jez / ne;
        }
        float gamma = sqrt(1. + uex * uex + uey * uey + uez * uez);
        op.vex = uex / gamma;
    }

    {
        float ne = fabs(hpt.e1n + hpt.e2n);
        float jex = hpt.e1jx + hpt.e2jx;
        float jey = hpt.e1jy + hpt.e2jy;
        float jez = hpt.e1jz + hpt.e2jz;
        float uex = 0.;
        float uey = 0.;
        float uez = 0.;
        if (ne > 0.)
        {
            uex = jex / ne;
            uey = jey / ne;
            uez = jez / ne;
        }
        float gamma = sqrt(1. + uex * uex + uey * uey + uez * uez);
        op.vey = uey / gamma;
    }

    {
        float ne = fabs(hpt.e1n + hpt.e2n);
        float jex = hpt.e1jx + hpt.e2jx;
        float jey = hpt.e1jy + hpt.e2jy;
        float jez = hpt.e1jz + hpt.e2jz;
        float uex = 0.;
        float uey = 0.;
        float uez = 0.;
        if (ne > 0.)
        {
            uex = jex / ne;
            uey = jey / ne;
            uez = jez / ne;
        }
        float gamma = sqrt(1. + uex * uex + uey * uey + uez * uez);
        op.vez = uez / gamma;
    }

    {
        float ne = fabs(hpt.e1n + hpt.e2n);
        float ni = hpt.i1n + hpt.i2n;
        float jex = hpt.e1jx + hpt.e2jx;
        float jix = hpt.i1jx + hpt.i2jx;
        if (ne + ni == 0.)
        {
            op.ux = 0.;
        }
        else
        {
            op.ux = (jex + jix * mi_me) / (ne + ni * mi_me);
        }
    }

    {
        float ne = fabs(hpt.e1n + hpt.e2n);
        float ni = hpt.i1n + hpt.i2n;
        float jey = hpt.e1jy + hpt.e2jy;
        float jiy = hpt.i1jy + hpt.i2jy;
        if (ne + ni == 0.)
        {
            op.uy = 0.;
        }
        else
        {
            op.uy = (jey + jiy * mi_me) / (ne + ni * mi_me);
        }
    }

    {
        float ne = fabs(hpt.e1n + hpt.e2n);
        float ni = hpt.i1n + hpt.i2n;
        float jez = hpt.e1jz + hpt.e2jz;
        float jiz = hpt.i1jz + hpt.i2jz;
        if (ne + ni == 0.)
        {
            op.uz = 0.;
        }
        else
        {
            op.uz = (jez + jiz * mi_me) / (ne + ni * mi_me);
        }
    }

    {
        float rho = hpt.e1n + hpt.e2n;
        if (rho == 0.)
        {
            op.uex = 0.;
        }
        else
        {
            op.uex = (hpt.e1jx + hpt.e2jx) / rho;
        }
    }

    {
        float rho = hpt.e1n + hpt.e2n;
        if (rho == 0.)
        {
            op.uey = 0.;
        }
        else
        {
            op.uey = (hpt.e1jy + hpt.e2jy) / rho;
        }
    }

    {
        float rho = hpt.e1n + hpt.e2n;
        if (rho == 0.)
        {
            op.uez = 0.;
        }
        else
        {
            op.uez = (hpt.e1jz + hpt.e2jz) / rho;
        }
    }

    {
        float rho = hpt.i1n + hpt.i2n;
        if (rho == 0.)
        {
            op.uix = 0.;
        }
        else
        {
            op.uix = (hpt.i1jx + hpt.i2jx) / rho;
        }
    }

    {
        float rho = hpt.i1n + hpt.i2n;
        if (rho == 0.)
        {
            op.uiy = 0.;
        }
        else
        {
            op.uiy = (hpt.i1jy + hpt.i2jy) / rho;
        }
    }

    {
        float rho = hpt.i1n + hpt.i2n;
        if (rho == 0.)
        {
            op.uiz = 0.;
        }
        else
        {
            op.uiz = (hpt.i1jz + hpt.i2jz) / rho;
        }
    }

    op.pexx = hpt.e1pxx + hpt.e2pxx;
    op.peyy = hpt.e1pyy + hpt.e2pyy;
    op.pezz = hpt.e1pzz + hpt.e2pzz;
    op.pexy = hpt.e1pxy + hpt.e2pxy;
    op.pexz = hpt.e1pxz + hpt.e2pxz;
    op.peyz = hpt.e1pyz + hpt.e2pyz;
    op.peyx = hpt.e1pyx + hpt.e2pyx;
    op.pezx = hpt.e1pzx + hpt.e2pzx;
    op.pezy = hpt.e1pzy + hpt.e2pzy;

    op.pixx = hpt.e1pxx + hpt.e2pxx;
    op.piyy = hpt.e1pyy + hpt.e2pyy;
    op.pizz = hpt.e1pzz + hpt.e2pzz;
    op.pixy = hpt.e1pxy + hpt.e2pxy;
    op.pixz = hpt.e1pxz + hpt.e2pxz;
    op.piyz = hpt.e1pyz + hpt.e2pyz;
    op.piyx = hpt.e1pyx + hpt.e2pyx;
    op.pizx = hpt.e1pzx + hpt.e2pzx;
    op.pizy = hpt.e1pzy + hpt.e2pzy;

    {
        op.absB = sqrt(bx * bx + by * by + bz * bz);
    }

    op.jx = hpt.e1jx + hpt.e2jx + hpt.i1jx + hpt.i2jx;
    op.jy = hpt.e1jy + hpt.e2jy + hpt.i1jy + hpt.i2jy;
    op.jz = hpt.e1jz + hpt.e2jz + hpt.i1jz + hpt.i2jz;

    {
        float jx = hpt.e1jx + hpt.e2jx + hpt.i1jx + hpt.i2jx;
        float jy = hpt.e1jy + hpt.e2jy + hpt.i1jy + hpt.i2jy;
        float jz = hpt.e1jz + hpt.e2jz + hpt.i1jz + hpt.i2jz;
        op.absJ = sqrt(jx * jx + jy * jy + jz * jz);
    }

    return op;
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

int main(int argc, char *argv[])
{

    MPI_Init(&argc, &argv);
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    int mpi_size;
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    const std::string basedir = "/Users/dbin/work/arrayudf-git-svn-test-on-bitbucket/examples/vpic/patrick-code/picA_006_hdf5_1step/";
    const std::string field_dir = basedir + "field_hdf5/";
    const std::string hydro_dir = basedir + "hydro_hdf5/";
    const std::string o_dir = basedir + "data/";

    //const int timestep = 1000;
    for (int timestep = 1000; timestep <= 1001; timestep += 1000)
    {
        std::string timestepdir = "T." + to_string(timestep) + "/";
        std::string i_file_field = field_dir + timestepdir + "fields_" + to_string(timestep) + ".h5";
        std::string i_file_electron1 = hydro_dir + timestepdir + "electron1_" + to_string(timestep) + ".h5";
        std::string i_file_ion1 = hydro_dir + timestepdir + "ion1_" + to_string(timestep) + ".h5";
        std::string i_file_electron2 = hydro_dir + timestepdir + "electron2_" + to_string(timestep) + ".h5";
        std::string i_file_ion2 = hydro_dir + timestepdir + "ion2_" + to_string(timestep) + ".h5";
        //const std::string o_file          = hydro_dir+timestepdir+"processed_"+to_string(timestep)+".h5";

        const std::string group = "/Timestep_" + to_string(timestep) + "/";

        //Split array into chunks for parallel processing
        if (mpi_rank == 0)
        {
            std::cout << "currentDateTime()=" << currentDateTime() << std::endl;
        }

        //Create a compound data structure to include JX/JY/JZ/....
        Array<CompoundPoint, float> *XYZ = new Array<CompoundPoint, float>(AU_VIRTUAL);

        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_field, group, "ex", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_field, group, "ey", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_field, group, "ez", 0);
        
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_field, group, "cbx", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_field, group, "cby", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_field, group, "cbz", 0);

        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron1, group, "rho", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron2, group, "rho", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion1, group, "rho", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion2, group, "rho", 0);

        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron1, group, "jz", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron1, group, "jy", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron1, group, "jz", 0);

        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron2, group, "jz", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron2, group, "jy", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron2, group, "jz", 0);

        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion1, group, "jz", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion1, group, "jy", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion1, group, "jz", 0);

        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion2, group, "jz", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion2, group, "jy", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion2, group, "jz", 0);

        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron1, group, "px", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron1, group, "py", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron1, group, "pz", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron1, group, "px", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron1, group, "py", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron1, group, "pz", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron1, group, "px", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron1, group, "py", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron1, group, "pz", 0);

        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron2, group, "px", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron2, group, "py", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron2, group, "pz", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron2, group, "px", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron2, group, "py", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron2, group, "pz", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron2, group, "px", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron2, group, "py", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_electron2, group, "pz", 0);

        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion1, group, "px", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion1, group, "py", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion1, group, "pz", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion1, group, "px", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion1, group, "py", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion1, group, "pz", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion1, group, "px", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion1, group, "py", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion1, group, "pz", 0);

        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion2, group, "px", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion2, group, "py", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion2, group, "pz", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion2, group, "px", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion2, group, "py", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion2, group, "pz", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion2, group, "px", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion2, group, "py", 0);
        XYZ->PushBackAttribute(AU_NVS, AU_HDF5, i_file_ion2, group, "pz", 0);

        Array<OutputCompoundPoint, float> *XYZ_OUTPUT = new Array<OutputCompoundPoint, float>(AU_VIRTUAL);

        std::string o_file = o_dir + timestepdir + "xyz_au_output_" + to_string(timestep) + ".h5";
        mkdir((o_dir + timestepdir).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "ex", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "ey", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "ez", 0);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "Eidealx", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "Eidealy", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "Eidealz", 0);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "Enonidealx", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "Enonidealy", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "Enonidealz", 0);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "Didea", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "Dnonideal", 0);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "bx", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "by", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "bz", 0);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "ne", 0);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "ni", 0);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "rho", 0);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "emixing", 0);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "vex", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "vey", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "vez", 0);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "ux", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "uy", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "uz", 0);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "uex", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "uey", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "uez", 0);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "uix", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "uiy", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "uiz", 0);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "pexx", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "peyy", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "pezz", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "pexy", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "pexz", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "peyz", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "peyx", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "pezx", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "pezy", 0);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "pixx", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "piyy", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "pizz", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "pixy", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "pixz", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "piyz", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "piyx", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "pizx", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "pizy", 0);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "absB", 0);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "jx", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "jy", 0);
        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "jz", 0);

        XYZ_OUTPUT->PushBackAttribute(AU_COMPUTED, AU_HDF5, o_file, group, "absJ", 0);

        std::vector<int> strip_size = {4, 1, 5};
        XYZ->SetApplyStripSize(strip_size);
        XYZ->Apply(extract_All_UDF, XYZ_OUTPUT);

        XYZ->ReportTime();

        delete XYZ;
        delete XYZ_OUTPUT;
    }

    MPI_Finalize();

    return 0;
}
