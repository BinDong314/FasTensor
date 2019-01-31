
#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include <math.h> /* ceil  and floor*/
#include <cstring>
#include "array_udf.h"

using namespace std;

//Help functions
void print_help();

struct Particle
{
    float x;
    float y;
    float z;
    friend std::ostream &operator<<(std::ostream &os, Particle &p) //test
    {
        os << '(' << p.x << ',' << p.y << ',' << p.z << ')';
        return os;
    }
    friend bool operator==(Particle &lp, Particle &rp)
    {
        return (lp.x == rp.y && lp.y == rp.y && lp.z == rp.z);
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

#define DELATA 0.01

Array<float> *BX, *BY, *BZ;
//Will be updated by get_metadata() with provided metadata file
std::vector<float> total_cells{64, 1, 64}; //Two domains
std::vector<float> dxdydz_per_cell{11.7188, 375, 4.88281};
std::vector<float> xyz_shitf{0, 187.5, 156};      //|x0| |y0| |z0|
std::vector<float> space_x0y0z0{0, -187.5, -156}; //|x0| |y0| |z0|

//Eq 44 in Section 7.7
Particle MagFieldInter(const Stencil<Particle> &hp)
{
    int i, j, k;
    Particle pt = hp(0);
    float x = pt.x, y = pt.y, z = pt.z;

    //Adjust x, y z here to be its cell index
    i = floor((x + xyz_shitf[0]) / dxdydz_per_cell[0]);
    j = floor((y + xyz_shitf[1]) / dxdydz_per_cell[1]);
    k = floor((z + xyz_shitf[2]) / dxdydz_per_cell[2]);

    //Check if xyz1 is beyond the boundary of the volume.
    //This may be the case for particles located at the boundary of the volume.
    if (i < 0)
        i = 0;
    if (j < 0)
        j = 0;
    if (k < 0)
        k = 0;

    if (i >= total_cells[0])
        i = total_cells[0] - 1;

    if (j >= total_cells[1])
        j = total_cells[1] - 1;

    if (k >= total_cells[2])
        k = total_cells[2] - 1;

    float bx, by, bz;
    bx = BX->operator()(i, j, k);
    by = BY->operator()(i, j, k);
    bz = BZ->operator()(i, j, k);

    Particle r;
    r.x = x + (bx / sqrt(bx * bx + by * by + bz * bz)) * DELATA;
    r.y = y + (by / sqrt(bx * bx + by * by + bz * bz)) * DELATA;
    r.z = z + (bz / sqrt(bx * bx + by * by + bz * bz)) * DELATA;

    //printf(" xyz = (%f, %f, %f), bxbybz = (%f, %f, %f), r.xyz = (%f, %f, %f)\n", x, y, z, bx, by, bz, r.x, r.y, r.z);

    return r;
}

int main(int argc, char *argv[])
{
    char i_file_field[1024] = "./test-file/fields_50.h5";
    char group[64] = "/Timestep_50";

    int mpi_size, mpi_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    BX = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, "cbx", AU_PRELOAD);
    BY = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, "cby", AU_PRELOAD);
    BZ = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, "cbz", AU_PRELOAD);

    std::vector<Particle> x0y0z0(1);
    float mpi_ratio = (mpi_rank + 1.0) / (mpi_size + 1.0);
    //std::cout << " mpi_ratio = " << mpi_ratio << std::endl;

    x0y0z0[0].x = space_x0y0z0[0] + mpi_ratio * total_cells[0] * dxdydz_per_cell[0];
    x0y0z0[0].y = space_x0y0z0[1] + mpi_ratio * total_cells[1] * dxdydz_per_cell[1];
    x0y0z0[0].z = space_x0y0z0[2] + mpi_ratio * total_cells[2] * dxdydz_per_cell[2];

    Array<Particle> *P = new Array<Particle>(x0y0z0);

    std::vector<Particle> xyz_trace;
    xyz_trace.push_back(x0y0z0[0]);

    std::vector<Particle> R;
    int max_step = 1000;
    for (int current_step = 0; current_step < max_step; current_step++)
    {
        P->ApplyV(MagFieldInter, R);
        xyz_trace.push_back(R[0]);
        P->UpdateCPPVector(R);
    }

    sleep(mpi_rank);
    for (int i = 0; i < 5; i++)
        std::cout << ",   " << xyz_trace[i];
    std::cout << " ... ... ";
    for (int i = max_step - 6; i < max_step; i++)
        std::cout << ",   " << xyz_trace[i];
    std::cout << std::endl;
    //Clear
    delete P;
    delete BX;
    delete BY;
    delete BZ;

    MPI_Finalize();

    return 0;
}

void print_help()
{
    const char *msg = "Usage: %s [OPTION] \n\
      	  -h help \n\
          -p particle file \n\
          -r result file \n\
          -g group(or step) name \n\
          -c chunk size (1D) \n\
          -o overlap (ghost zone) size (1D). \n\
          Example: mpirun -n 2 ./parallel-eb-at-particle  -p ./test-file/electron_50.h5 -r ./test-file/electron_50.h5 -g /Timestep_50 -c 3277\n";

    fprintf(stdout, msg, "parallel-eb-at-particle");
}