
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
    float x;
    float y;
    float z;
    friend std::ostream &operator<<(std::ostream &os, Particle &p) //test
    {
        os << p.x << '/' << p.y << '/' << p.z;
        return os;
    }
    friend bool operator==(Particle &lp, Particle &rp)
    {
        return (lp.x == rp.x && lp.y == rp.y && lp.z == rp.z);
    }
    void set_value(int index, float value)
    {
        switch (index)
        {
        case 0:
            x = value;
            break;
        case 1:
            y = value;
            break;
        case 2:
            z = value;
            break;
        default:
            printf("The object only has three variables !\n");
            break;
        }
    }
};

const float bshape[3] = {32, 1, 64};
const float origin[3] = {0, -187.5, -156.25};
const float originShift[3] = {738.281, -187.5, 151.367};
const float bsize[3] = {739.281, 0, 302.617};
const float dxdydz[3] = {11.7188, 375, 4.88281}; //bsize/(bshape-1.0)

//Global variabe Field array
//It is duplicated at each mpi rank
Array<float> *F;

inline float TriInterUDF(const Stencil<Particle> &p)
{
    Particle pt = p(0);
    //printf("Particle x: %f, y: %f, z: %f \n", pt.x, pt.y, pt.z);
    int x[2], y[2], z[2];
    float xw, yw, zw;

    //Adjust x, y z here, based on bshape, borigin, orginshift, bsize
    x[0] = floor((pt.x + originShift[0]) / dxdydz[0]);
    y[0] = floor((pt.y + originShift[1]) / dxdydz[1]);
    z[0] = floor((pt.z + originShift[2]) / dxdydz[2]);
    x[1] = x[0] + 1;
    y[1] = y[0] + 1;
    z[1] = z[0] + 1;

    //Check if xyz1 is beyond the boundary of the volume.
    //This may be the case for particles located at the boundary of the volume.

    if (x[0] < 0)
        x[0] = 0;
    if (y[0] < 0)
        y[0] = 0;
    if (z[0] < 0)
        z[0] = 0;

    if (x[0] >= bshape[0])
        x[0] = bshape[0] - 1;
    ;
    if (y[0] >= bshape[1])
        y[0] = bshape[1] - 1;
    ;
    if (z[0] >= bshape[2])
        z[0] = bshape[2] - 1;
    ;

    if (x[1] >= bshape[0])
        x[1] = bshape[0] - 1;
    if (y[1] >= bshape[1])
        y[1] = bshape[1] - 1;
    if (z[1] >= bshape[2])
        z[1] = bshape[2] - 1;

    if (x[1] < 0)
        x[1] = 0;
    if (y[1] < 0)
        y[1] = 0;
    if (z[1] < 0)
        z[1] = 0;

    xw = abs(pt.x - (x[0] * dxdydz[0] - originShift[0])) / dxdydz[0];
    yw = abs(pt.y - (y[0] * dxdydz[1] - originShift[1])) / dxdydz[1];
    zw = abs(pt.z - (z[0] * dxdydz[2] - originShift[2])) / dxdydz[2];

    float bx000 = F->operator()(x[0], y[0], z[0]);
    float bx001 = F->operator()(x[0], y[0], z[1]);
    float bx010 = F->operator()(x[0], y[1], z[0]);
    float bx011 = F->operator()(x[0], y[1], z[1]);
    float bx100 = F->operator()(x[1], y[0], z[0]);
    float bx101 = F->operator()(x[1], y[0], z[1]);
    float bx110 = F->operator()(x[1], y[1], z[0]);
    float bx111 = F->operator()(x[1], y[1], z[1]);

    //printf("Particle xyz= (%f,  %f,  %f), x = (%d, %d), y = (%d, %d), z= (%d, %d),  bx 000...b111 = (%.15f, %.15f, %.15f , %.15f, %.15f,  %.15f, %.15f, %.15f) \n", pt.x, pt.y, pt.z, x[0], x[1], y[0], y[1], z[0], z[1], bx000, bx001, bx010, bx011, bx100, bx101, bx110, bx111);
    return bx000 * (1.0 - xw) * (1.0 - yw) * (1.0 - zw) + bx100 * xw * (1.0 - yw) * (1.0 - zw) + bx010 * (1.0 - xw) * yw * (1.0 - zw) + bx001 * (1.0 - xw) * (1.0 - yw) * zw + bx101 * xw * (1.0 - yw) * zw + bx011 * (1.0 - xw) * yw * zw + bx110 * xw * yw * (1.0 - zw) + bx111 * xw * yw * zw;
}

int main(int argc, char *argv[])
{
    char i_file_field[1024] = "./fields_50_field_element.h5";
    char i_file_particle[1024] = "./electron_50.h5";

    char group[64] = "/Timestep_50";
    char dsetF[64] = "/Timestep_50/Eparallel";

    int c_size = 3277, o_size = 0;

    std::vector<int> chunk_size(1);
    std::vector<int> overlap_size(1);
    chunk_size[0] = c_size;
    overlap_size[0] = o_size;

    MPI_Init(&argc, &argv);

    F = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, dsetF, AU_PRELOAD);
    //printf("f000 = %.10f, f001 = %.10f, f002 = %.10f,  f62_0_0 = %f \n",  F->operator()(0, 0, 0), F->operator()(0, 0, 1),  F->operator()(0, 0, 2), F->operator()(62, 0, 0));

    //Orginal data set
    Array<float> *X = new Array<float>(AU_NVS, AU_HDF5, i_file_particle, group, "x", chunk_size, overlap_size);
    //Orginal data set
    Array<float> *Y = new Array<float>(AU_NVS, AU_HDF5, i_file_particle, group, "y", chunk_size, overlap_size);
    //Orginal data set
    Array<float> *Z = new Array<float>(AU_NVS, AU_HDF5, i_file_particle, group, "z", chunk_size, overlap_size);

    Array<Particle, float> *P = new Array<Particle, float>(AU_VIRTUAL);
    P->SetAttributes(X, Y, Z); //Todo: change to pushback()

    //Particle p=P->operator()(1);
    //std::cout <<  "P : x = " << p.x << ", y = " << p.y << ", z = " << p.z << std::endl;

    //Results data sets
    Array<float> *PN = new Array<float>(AU_COMPUTED, AU_HDF5, i_file_particle, group, "eparallel", chunk_size, overlap_size);
    P->Apply(TriInterUDF, PN);
    //P->ReportTime();
    //PN->Nonvolatile();
    //Clear
    delete X;
    delete Y;
    delete Z;
    delete P;
    delete F;
    delete PN;

    MPI_Finalize();

    return 0;
}
