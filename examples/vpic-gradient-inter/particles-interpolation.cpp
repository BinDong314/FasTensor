
#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include <math.h> /* ceil  and floor*/
#include <cstring>

#include "array_udf.h"

//Some help function
void print_help();
void get_metadata(char *file, char *group, int op, std::vector<float> &values);

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

std::vector<float> total_cells{64, 1, 64}; //Two domains
std::vector<float> dxdydz_per_cell{11.7188, 375, 4.88281};
std::vector<float> xyz_shitf{0, 187.5, 156}; //|x0| |y0| |z0|

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
  char i_file_metadata[1024] = "./test-file/grid_metadata_electron_50.h5";

  char group[64] = "/Timestep_50";
  char o_file[1024] = "./test-file/electron_50.h5";

  int c_size = 3277, o_size = 0;
  int copt;

  while ((copt = getopt(argc, argv, "hm:f:p:g:r:c:o:")) != -1)
    switch (copt)
    {
    case 'f':
      memset(i_file_field, 0, sizeof(i_file_field));
      strcpy(i_file_field, optarg);
      break;
    case 'p':
      memset(i_file_particle, 0, sizeof(i_file_particle));
      strcpy(i_file_particle, optarg);
      break;
    case 'm':
      memset(i_file_metadata, 0, sizeof(i_file_metadata));
      strcpy(i_file_metadata, optarg);
      break;
    case 'g':
      memset(group, 0, sizeof(group));
      strcpy(group, optarg);
      break;
    case 'r':
      memset(o_file, 0, sizeof(o_file));
      strcpy(o_file, optarg);
      break;
    case 'c':
      c_size = atoi(optarg);
      break;
    case 'o':
      o_size = atoi(optarg);
      break;
    case 'h':
      print_help();
      exit(0);
    default:
      break;
    }

  std::vector<int> chunk_size(1);
  std::vector<int> overlap_size(1);
  chunk_size[0] = c_size;
  overlap_size[0] = o_size;

  MPI_Init(&argc, &argv);

  //  1: dxdydz   2: |x0| |y0| |z0|
  get_metadata(i_file_metadata, group, 1, dxdydz_per_cell);
  get_metadata(i_file_metadata, group, 2, xyz_shitf);

  //printf("dxdydz_per_cell = (%f, %f, %f) \n", dxdydz_per_cell[0], dxdydz_per_cell[1], dxdydz_per_cell[2]);
  //printf("xyz_shitf       = (%f, %f, %f) \n", xyz_shitf[0], xyz_shitf[1], xyz_shitf[2]);

  //Pre load all filed vairables.
  //We can load one by one too
  EX = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, "ex", AU_PRELOAD);
  std::vector<unsigned long long> temp_v = EX->GetDimSize();
  for (int i = 0; i < 3; i++)
    total_cells[i] = temp_v[i];

  //printf("total_cells       = (%f, %f, %f) \n", total_cells[0], total_cells[1], total_cells[2]);

  EY = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, "ey", AU_PRELOAD);
  EZ = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, "ez", AU_PRELOAD);
  BX = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, "cbx", AU_PRELOAD);
  BY = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, "cby", AU_PRELOAD);
  BZ = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, "cbz", AU_PRELOAD);

  //printf("f000 = %.10f, f001 = %.10f, f002 = %.10f,  f62_0_0 = %f \n",  F->operator()(0, 0, 0), F->operator()(0, 0, 1),  F->operator()(0, 0, 2), F->operator()(62, 0, 0));

  //Orginal data set
  Array<float> *X = new Array<float>(AU_NVS, AU_HDF5, i_file_particle, group, "x", chunk_size, overlap_size);
  Array<float> *Y = new Array<float>(AU_NVS, AU_HDF5, i_file_particle, group, "y", chunk_size, overlap_size);
  Array<float> *Z = new Array<float>(AU_NVS, AU_HDF5, i_file_particle, group, "z", chunk_size, overlap_size);

  Array<Particle, float> *P = new Array<Particle, float>(AU_VIRTUAL);
  P->SetAttributes(X, Y, Z); //Todo: change to pushback()

  //Particle p=P->operator()(1);
  //std::cout <<  "P : x = " << p.x << ", y = " << p.y << ", z = " << p.z << std::endl;

  //Results data sets
  Array<float> *R_EX = new Array<float>(AU_COMPUTED, AU_HDF5, o_file, group, "ex", chunk_size, overlap_size);
  P->Apply(InterEX, R_EX);

  Array<float> *R_EY = new Array<float>(AU_COMPUTED, AU_HDF5, o_file, group, "ey", chunk_size, overlap_size);
  P->Apply(InterEY, R_EY);

  Array<float> *R_EZ = new Array<float>(AU_COMPUTED, AU_HDF5, o_file, group, "ez", chunk_size, overlap_size);
  P->Apply(InterEZ, R_EZ);

  Array<float> *R_BX = new Array<float>(AU_COMPUTED, AU_HDF5, o_file, group, "bx", chunk_size, overlap_size);
  P->Apply(InterBX, R_BX);

  Array<float> *R_BY = new Array<float>(AU_COMPUTED, AU_HDF5, o_file, group, "by", chunk_size, overlap_size);
  P->Apply(InterBY, R_BY);

  Array<float> *R_BZ = new Array<float>(AU_COMPUTED, AU_HDF5, o_file, group, "bz", chunk_size, overlap_size);
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

void print_help()
{
  const char *msg = "Usage: %s [OPTION] \n\
      	  -h help \n\
          -p particle file \n\
          -f field file  \n\
          -m metadata (grid) file \n\
          -r result file \n\
          -g group(or step) name \n\
          -c chunk size string (1D) \n\
          -o overlap (ghost zone) size (1D). \n\
          Example: mpirun -n 2 ./particles-interpolation  -f ./test-file/fields_50.h5 -p ./test-file/electron_50.h5  -m ./test-file/grid_metadata_electron_50.h5 -r ./test-file/fields_50.h5 -g /Timestep_50 -c 3277\n";

  fprintf(stdout, msg, "particles-interpolation");
}

//1: dxdydz   2: |x0| |y0| |z0|
void get_metadata(char *file, char *group, int op, std::vector<float> &values)
{

  Array<float> *t1, *t2, *t3;
  switch (op)
  {
  case 1:
    t1 = new Array<float>(AU_NVS, AU_HDF5, file, group, "dx", AU_PRELOAD);
    t2 = new Array<float>(AU_NVS, AU_HDF5, file, group, "dy", AU_PRELOAD);
    t3 = new Array<float>(AU_NVS, AU_HDF5, file, group, "dz", AU_PRELOAD);
    break;
  case 2:
    t1 = new Array<float>(AU_NVS, AU_HDF5, file, group, "x0", AU_PRELOAD);
    t2 = new Array<float>(AU_NVS, AU_HDF5, file, group, "y0", AU_PRELOAD);
    t3 = new Array<float>(AU_NVS, AU_HDF5, file, group, "z0", AU_PRELOAD);
    break;
  default:
    printf("Not supported op in %s: %d \n", __FILE__, __LINE__);
    exit(-1);
  }

  if (op == 2)
  {
    values[0] = abs(t1->operator()(0));
    values[1] = abs(t2->operator()(0));
    values[2] = abs(t3->operator()(0));
  }
  else
  {
    values[0] = t1->operator()(0);
    values[1] = t2->operator()(0);
    values[2] = t3->operator()(0);
  }
}
