
#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include <math.h> /* ceil  and floor*/
#include <cstring>

#include "array_udf.h"

using namespace std;

struct Particle
{
    float ex;
    float ey;
    float ez;
    float bx;
    float by;
    float bz;

    friend std::ostream &operator<<(std::ostream &os, Particle &p) //test
    {
        os << p.ex << '/' << p.ey << '/' << p.ez << '/' << p.bx << '/' << p.by << '/' << p.bz;
        return os;
    }
    friend bool operator==(Particle &lp, Particle &rp)
    {
        return (lp.ex == rp.ex && lp.ey == rp.ey && lp.ez == rp.ez && lp.bx == rp.bx && lp.by == rp.by && lp.bz == rp.bz);
    }
    void set_value(int index, float value)
    {
        switch (index)
        {
        case 0:
            ex = value;
            break;
        case 1:
            ey = value;
            break;
        case 2:
            ez = value;
            break;
        case 3:
            bx = value;
            break;
        case 4:
            by = value;
            break;
        case 5:
            bz = value;
            break;
        default:
            printf("The object only has six variables !\n");
            break;
        }
    }
};

//Eq 43 in Section 7.7
inline float ParallelEB(const Stencil<Particle> &hp)
{
    float ex_p = hp(0).ex;
    float ey_p = hp(0).ey;
    float ez_p = hp(0).ez;

    float bx_p = hp(0).bx;
    float by_p = hp(0).by;
    float bz_p = hp(0).bz;

    return (ex_p * bx_p + ey_p * by_p + ez_p * bz_p) / sqrt(bx_p * bx_p + by_p * by_p + bz_p * bz_p);
}

int main(int argc, char *argv[])
{
    char i_file_particle[1024] = "./electron_50.h5";
    char group[64] = "/Timestep_50";

    int c_size = 3277, o_size = 0;

    std::vector<int> chunk_size(1);
    std::vector<int> overlap_size(1);
    chunk_size[0] = c_size;
    overlap_size[0] = o_size;

    MPI_Init(&argc, &argv);

    //Orginal data set
    Array<float> *EX = new Array<float>(AU_NVS, AU_HDF5, i_file_particle, group, "ex", chunk_size, overlap_size);
    Array<float> *EY = new Array<float>(AU_NVS, AU_HDF5, i_file_particle, group, "ey", chunk_size, overlap_size);
    Array<float> *EZ = new Array<float>(AU_NVS, AU_HDF5, i_file_particle, group, "ez", chunk_size, overlap_size);
    Array<float> *BX = new Array<float>(AU_NVS, AU_HDF5, i_file_particle, group, "bx", chunk_size, overlap_size);
    Array<float> *BY = new Array<float>(AU_NVS, AU_HDF5, i_file_particle, group, "by", chunk_size, overlap_size);
    Array<float> *BZ = new Array<float>(AU_NVS, AU_HDF5, i_file_particle, group, "bz", chunk_size, overlap_size);

    Array<Particle, float> *P = new Array<Particle, float>(AU_VIRTUAL);
    P->SetAttributes(EX, EY, EZ, BX, BY, BZ); //Todo: change to pushback()

    //Particle p=P->operator()(1);
    //std::cout <<  "P : x = " << p.x << ", y = " << p.y << ", z = " << p.z << std::endl;

    //Results data sets
    Array<float> *R = new Array<float>(AU_COMPUTED, AU_HDF5, i_file_particle, group, "eparallel", chunk_size, overlap_size);
    P->Apply(ParallelEB, R);

    //Clear
    delete P;
    delete EX;
    delete EY;
    delete EZ;
    delete BX;
    delete BY;
    delete BZ;
    delete R;

    MPI_Finalize();

    return 0;
}
