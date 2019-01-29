
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

//Global variabe Field array
//It is duplicated at each mpi rank
Array<float> *EX, *EY, *EZ, *BX, *BY, *BZ;

const float total_cells[3] = {64, 1, 64}; //Two domains
const float cells_per_domain[3] = {32, 1, 64};
const float dxdydz_per_cell[3] = {11.7188, 375, 4.88281};
const float dxdydz_per_domain[3] = {375.0016, 375, 312.49984}; //cells_per_domain * dxdydz_per_cell
const float xyz_shitf[3] = {0, 187.5, 156};                    //x0

//Eq. 35 in Section 7.6
inline float InterEX(const Stencil<Particle> &p)
{
  Particle pt = p(0);
  //printf("Particle x: %f, y: %f, z: %f \n", pt.x, pt.y, pt.z);
  int i, j, k;

  //Adjust x, y z here to be positive
  i = floor((pt.x + xyz_shitf[0]) / dxdydz_per_cell[0]);
  j = floor((pt.y + xyz_shitf[1]) / dxdydz_per_cell[1]);
  k = floor((pt.z + xyz_shitf[2]) / dxdydz_per_cell[2]);

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

  float dx, dy, dz;
  dx = fmodf(pt.x, dxdydz_per_cell[0]) / dxdydz_per_cell[0];
  dy = fmodf(pt.y, dxdydz_per_cell[1]) / dxdydz_per_cell[1];
  dz = fmodf(pt.z, dxdydz_per_cell[2]) / dxdydz_per_cell[2];

  float ex000, ex010, ex001, ex011;
  ex000 = EX->operator()(i, j, k);
  ex010 = EX->operator()(i, j + 1, k);
  ex001 = EX->operator()(i, j, k + 1);
  ex011 = EX->operator()(i, j + 1, k + 1);
  //printf(" xyz = (%f, %f, %f), ijk = (%d, %d, %d), dx/dy/dz=(%f, %f, %f), ex000=%f, ex010=%f, ex001=%f, ex011=%f \n ", pt.x, pt.y, pt.z, i, j, k, dx, dy, dz, ex000, ex010, ex001, ex011);
  return ((1 - dy) * (1 - dz) * ex000 + (1 + dy) * (1 - dz) * ex010 + (1 - dy) * (1 + dz) * ex001 + (1 + dy) * (1 + dz) * ex011) / 4.0;
}

//Eq. 37 in Section 7.6
inline float InterEY(const Stencil<Particle> &p)
{
  Particle pt = p(0);
  //printf("Particle x: %f, y: %f, z: %f \n", pt.x, pt.y, pt.z);
  int i, j, k;

  //Adjust x, y z here to be positive
  i = floor((pt.x + xyz_shitf[0]) / dxdydz_per_cell[0]);
  j = floor((pt.y + xyz_shitf[1]) / dxdydz_per_cell[1]);
  k = floor((pt.z + xyz_shitf[2]) / dxdydz_per_cell[2]);

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

  float dx, dy, dz;
  dx = fmodf(pt.x, dxdydz_per_cell[0]) / dxdydz_per_cell[0];
  dy = fmodf(pt.y, dxdydz_per_cell[1]) / dxdydz_per_cell[1];
  dz = fmodf(pt.z, dxdydz_per_cell[2]) / dxdydz_per_cell[2];

  float ey000, ey010, ey001, ey011;
  ey000 = EY->operator()(i, j, k);
  ey010 = EY->operator()(i + 1, j, k);
  ey001 = EY->operator()(i, j, k + 1);
  ey011 = EY->operator()(i + 1, j, k + 1);
  //printf(" xyz = (%f, %f, %f), ijk = (%d, %d, %d), dx/dy/dz=(%f, %f, %f), ex000=%f, ex010=%f, ex001=%f, ex011=%f \n ", pt.x, pt.y, pt.z, i, j, k, dx, dy, dz, ex000, ex010, ex001, ex011);
  return ((1 - dx) * (1 - dz) * ey000 + (1 + dx) * (1 - dz) * ey010 + (1 - dx) * (1 + dz) * ey001 + (1 + dx) * (1 + dz) * ey011) / 4.0;
}

//Eq. 38 in Section 7.6
//The Eq. 38 has some format issue
inline float InterEZ(const Stencil<Particle> &p)
{
  Particle pt = p(0);
  //printf("Particle x: %f, y: %f, z: %f \n", pt.x, pt.y, pt.z);
  int i, j, k;

  //Adjust x, y z here to be positive
  i = floor((pt.x + xyz_shitf[0]) / dxdydz_per_cell[0]);
  j = floor((pt.y + xyz_shitf[1]) / dxdydz_per_cell[1]);
  k = floor((pt.z + xyz_shitf[2]) / dxdydz_per_cell[2]);

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

  float dx, dy, dz;
  dx = fmodf(pt.x, dxdydz_per_cell[0]) / dxdydz_per_cell[0];
  dy = fmodf(pt.y, dxdydz_per_cell[1]) / dxdydz_per_cell[1];
  dz = fmodf(pt.z, dxdydz_per_cell[2]) / dxdydz_per_cell[2];

  float ez000, ez010, ez001, ez011;
  ez000 = EZ->operator()(i, j, k);
  ez010 = EZ->operator()(i + 1, j, k);
  ez001 = EZ->operator()(i, j + 1, k);
  ez011 = EZ->operator()(i + 1, j + 1, k);
  //printf(" xyz = (%f, %f, %f), ijk = (%d, %d, %d), dx/dy/dz=(%f, %f, %f), ex000=%f, ex010=%f, ex001=%f, ex011=%f \n ", pt.x, pt.y, pt.z, i, j, k, dx, dy, dz, ex000, ex010, ex001, ex011);
  return ((1 - dx) * (1 - dy) * ez000 + (1 + dx) * (1 - dy) * ez010 + (1 - dx) * (1 + dy) * ez001 + (1 + dx) * (1 + dy) * ez011) / 4.0;
}

//Eq. 39 in Section 7.6
inline float InterBX(const Stencil<Particle> &p)
{
  Particle pt = p(0);
  //printf("Particle x: %f, y: %f, z: %f \n", pt.x, pt.y, pt.z);
  int i, j, k;

  //Adjust x, y z here to be positive
  i = floor((pt.x + xyz_shitf[0]) / dxdydz_per_cell[0]);
  j = floor((pt.y + xyz_shitf[1]) / dxdydz_per_cell[1]);
  k = floor((pt.z + xyz_shitf[2]) / dxdydz_per_cell[2]);

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

  float dx;
  dx = fmodf(pt.x, dxdydz_per_cell[0]) / dxdydz_per_cell[0];

  float bx000, bx100;
  bx000 = BX->operator()(i, j, k);
  bx100 = BX->operator()(i + 1, j, k);
  //printf(" xyz = (%f, %f, %f), ijk = (%d, %d, %d), dx/dy/dz=(%f, %f, %f), ex000=%f, ex010=%f, ex001=%f, ex011=%f \n ", pt.x, pt.y, pt.z, i, j, k, dx, dy, dz, ex000, ex010, ex001, ex011);
  return ((1 - dx) * bx000 + (1 + dx) * bx100) / 2.0;
}

//Eq. 40 in Section 7.6
inline float InterBY(const Stencil<Particle> &p)
{
  Particle pt = p(0);
  //printf("Particle x: %f, y: %f, z: %f \n", pt.x, pt.y, pt.z);
  int i, j, k;

  //Adjust x, y z here to be positive
  i = floor((pt.x + xyz_shitf[0]) / dxdydz_per_cell[0]);
  j = floor((pt.y + xyz_shitf[1]) / dxdydz_per_cell[1]);
  k = floor((pt.z + xyz_shitf[2]) / dxdydz_per_cell[2]);

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

  float dy;
  dy = fmodf(pt.y, dxdydz_per_cell[1]) / dxdydz_per_cell[1];

  float by000, by100;
  by000 = BY->operator()(i, j, k);
  by100 = BY->operator()(i, j + 1, k);
  //printf(" xyz = (%f, %f, %f), ijk = (%d, %d, %d), dx/dy/dz=(%f, %f, %f), ex000=%f, ex010=%f, ex001=%f, ex011=%f \n ", pt.x, pt.y, pt.z, i, j, k, dx, dy, dz, ex000, ex010, ex001, ex011);
  return ((1 - dy) * by000 + (1 + dy) * by100) / 2.0;
}

//Eq. 40 in Section 7.6
inline float InterBZ(const Stencil<Particle> &p)
{
  Particle pt = p(0);
  //printf("Particle x: %f, y: %f, z: %f \n", pt.x, pt.y, pt.z);
  int i, j, k;

  //Adjust x, y z here to be positive
  i = floor((pt.x + xyz_shitf[0]) / dxdydz_per_cell[0]);
  j = floor((pt.y + xyz_shitf[1]) / dxdydz_per_cell[1]);
  k = floor((pt.z + xyz_shitf[2]) / dxdydz_per_cell[2]);

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

  float dz;
  dz = fmodf(pt.z, dxdydz_per_cell[2]) / dxdydz_per_cell[2];

  float bz000, bz100;
  bz000 = BZ->operator()(i, j, k);
  bz100 = BZ->operator()(i, j, k + 1);
  //printf(" xyz = (%f, %f, %f), ijk = (%d, %d, %d), dx/dy/dz=(%f, %f, %f), ex000=%f, ex010=%f, ex001=%f, ex011=%f \n ", pt.x, pt.y, pt.z, i, j, k, dx, dy, dz, ex000, ex010, ex001, ex011);
  return ((1 - dz) * bz000 + (1 + dz) * bz100) / 2.0;
}

int main(int argc, char *argv[])
{
  char i_file_field[1024] = "./test-file/fields_50.h5";
  char i_file_particle[1024] = "./test-file/electron_50.h5";

  char group[64] = "/Timestep_50";
  char dsetEX[64] = "/Timestep_50/ex";
  char dsetEY[64] = "/Timestep_50/ey";
  char dsetEZ[64] = "/Timestep_50/ez";
  char dsetBX[64] = "/Timestep_50/cbx";
  char dsetBY[64] = "/Timestep_50/cby";
  char dsetBZ[64] = "/Timestep_50/cbz";

  int c_size = 3277, o_size = 0;

  std::vector<int> chunk_size(1);
  std::vector<int> overlap_size(1);
  chunk_size[0] = c_size;
  overlap_size[0] = o_size;

  MPI_Init(&argc, &argv);

  //Pre load all filed vairables.
  //We can load one by one too
  EX = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, dsetEX, AU_PRELOAD);
  EY = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, dsetEY, AU_PRELOAD);
  EZ = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, dsetEZ, AU_PRELOAD);
  BX = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, dsetBX, AU_PRELOAD);
  BY = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, dsetBY, AU_PRELOAD);
  BZ = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, dsetBZ, AU_PRELOAD);

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
  Array<float> *R_EX = new Array<float>(AU_COMPUTED, AU_HDF5, i_file_particle, group, "ex", chunk_size, overlap_size);
  P->Apply(InterEX, R_EX);

  Array<float> *R_EY = new Array<float>(AU_COMPUTED, AU_HDF5, i_file_particle, group, "ey", chunk_size, overlap_size);
  P->Apply(InterEY, R_EY);

  Array<float> *R_EZ = new Array<float>(AU_COMPUTED, AU_HDF5, i_file_particle, group, "ez", chunk_size, overlap_size);
  P->Apply(InterEZ, R_EZ);

  Array<float> *R_BX = new Array<float>(AU_COMPUTED, AU_HDF5, i_file_particle, group, "bx", chunk_size, overlap_size);
  P->Apply(InterBX, R_BX);

  Array<float> *R_BY = new Array<float>(AU_COMPUTED, AU_HDF5, i_file_particle, group, "by", chunk_size, overlap_size);
  P->Apply(InterBY, R_BY);

  Array<float> *R_BZ = new Array<float>(AU_COMPUTED, AU_HDF5, i_file_particle, group, "bz", chunk_size, overlap_size);
  P->Apply(InterBZ, R_BZ);

  //Clear
  delete X;
  delete Y;
  delete Z;
  delete P;
  delete EX;
  delete EY;
  delete EZ;
  delete BX;
  delete BY;
  delete BZ;
  delete R_EX;
  delete R_EY;
  delete R_EZ;
  delete R_BX;
  delete R_BY;
  delete R_BZ;

  MPI_Finalize();

  return 0;
}
