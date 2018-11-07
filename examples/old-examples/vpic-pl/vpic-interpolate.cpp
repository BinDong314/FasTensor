// overload_array.cpp
// overloading the c++ array subscript operator []
//http://neondataskills.org/HDF5/TimeSeries-Data-In-HDF5-Using-R/

#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include <math.h>       /* ceil  and floor*/
#include <cstring>

#include "array_udf.h"


using namespace std;



struct Particle{
  float x;
  float y;
  float z;
  friend std::ostream& operator<<(std::ostream& os, Particle &p)   //test 
  {  
    os << p.x << '/' << p.y << '/' << p.z ;  
    return os;  
  }
  friend bool operator == (Particle &lp, Particle& rp)
  {
    return  (lp.x == rp.x && lp.y == rp.y && lp.z == rp.z);
  }
  void set_value(int index, float value){
    switch(index){
      case 0:
        x = value;
        break;
      case 1:
        y =value;
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


const float bshape[3]      = {2000, 1980,  1000};
const float borigin[3]     = {0,    -165,  -66};
const float originShift[3] = {0 ,    165,   66};
const float bsize[3]       = {330,   330,  132};
const float bcellSize[3]   = {0.16508, 0.16675, 0.13213}; //bsize/(bshape-1.0)

Array<float> *M; 
//
//To degug here, with single process
// Why bx000~bx111 are zero? 
// Also, the openMP seems doesn't work here
// 
inline float TriDInterUDF(const Stencil<Particle> &p) 
{
  Particle pt = p(0);
  //printf("Particle x: %f, y: %f, z: %f \n", pt.x, pt.y, pt.z);
  int      x[2], y[2], z[2];
  float    xw, yw, zw;
  //Adjust x, y z here, based on bshape, borigin, orginshift, bsize
  x[0]  = floor((pt.x + originShift[0]) / bcellSize[0]) ;
  y[0]  = floor((pt.y + originShift[1]) / bcellSize[1]) ;
  z[0]  = floor((pt.z + originShift[2]) / bcellSize[2]) ;
  x[1]  = x[0] + 1;
  y[1]  = y[0] + 1;
  z[1]  = z[0] + 1;
  
  //Check if xyz1 is beyond the boundary of the volume. This may be the case for particles located at the boundary of the volume.
  if(x[1] >= bshape[0]){
    x[1] = bshape[0] -  1;
    x[0] = bshape[0] -  2;
  }
  if(y[1] >= bshape[1]){
    y[1] = bshape[1] -  1;
    y[0] = bshape[1] -  2;
  }
  if(z[1] >= bshape[2]){
    z[1] = bshape[2] -  1;
    z[0] = bshape[2] -  2;
  }

  if(x[0] < 0){
    x[1] = 1;
    x[0] = 0;
  }
  if(y[0] < 0){
    y[1] = 1;
    y[0] = 0;
  }
  if(z[0] < 0){
    z[1] = 1;
    z[0] = 0;
  }
  
  xw = abs( pt.x - (x[0]*bcellSize[0] - originShift[0]) ) / bcellSize[0];
  yw = abs( pt.y - (y[0]*bcellSize[1] - originShift[1]) ) / bcellSize[1];
  zw = abs( pt.z - (z[0]*bcellSize[2] - originShift[2]) ) / bcellSize[2];
  
  float bx000 = M->operator()(x[0], y[0], z[0]);
  float bx100 = M->operator()(x[1], y[0], z[0]);
  float bx010 = M->operator()(x[0], y[1], z[0]);
  float bx001 = M->operator()(x[0], y[0], z[1]);
  float bx101 = M->operator()(x[1], y[0], z[1]);
  float bx011 = M->operator()(x[0], y[1], z[1]);
  float bx110 = M->operator()(x[1], y[1], z[0]);
  float bx111 = M->operator()(x[1], y[1], z[1]);

  return  bx000*(1.0-xw)*(1.0-yw)*(1.0-zw) + bx100*xw*(1.0-yw)*(1.0-zw) + bx010*(1.0-xw)*yw*(1.0-zw) +  bx001*(1.0-xw)*(1.0-yw)*zw + bx101*xw*(1.0-yw)*zw + bx011*(1.0-xw)*yw*zw + bx110*xw*yw*(1.0-zw) + bx111*xw*yw*zw;
}

int main(int argc, char *argv[])
{
    // create an instance of the myArray class 
    // 123, 463, 188960
  char i_file_bx[1024]=      "/global/cscratch1/sd/dbin/vpic-fanguo/T.10400/field_10400_gradient.h5p";
  char i_file_particle[1024]="/global/cscratch1/sd/dbin/vpic-fanguo/T.10400/electron_tracer_output_xyz.h5p";
  //char o_file_particle[1024]="/global/cscratch1/sd/dbin/vpic-fanguo/T.10400/electron_tracer_output_xyz_gradient.h5p";

 char  group[1024]="/Step#10400";
  char dataset[1024]="/Step#10400/bx";

  std::vector<int> chunk_size(1) ; 
  std::vector<int> overlap_size(1);
  chunk_size[0]   = 10000000;  
  overlap_size[0] = 0;

  
  MPI_Init(&argc, &argv);
  M = new Array<float>(AU_NVS,  AU_HDF5, i_file_bx, "/", "/bx", AU_PRELOAD);
  //Orginal data set
  Array<float> *X = new Array<float>(AU_NVS,  AU_HDF5, i_file_particle, group,  "x", chunk_size, overlap_size);
  //Orginal data set
  Array<float> *Y = new Array<float>(AU_NVS,  AU_HDF5,  i_file_particle, group, "y",  chunk_size, overlap_size);
  //Orginal data set
  Array<float> *Z = new Array<float>(AU_NVS,  AU_HDF5, i_file_particle, group,   "z", chunk_size, overlap_size);

  Array<Particle, float> *P = new Array<Particle, float>(AU_VIRTUAL);
  P->SetAttributes(X, Y, Z); //Todo: change to pushback()

  //Particle p=P->operator()(1);
  //std::cout <<  "P : x = " << p.x << ", y = " << p.y << ", z = " << p.z << std::endl;

  //Results data sets
  Array<float> *PN = new Array<float>(AU_COMPUTED, AU_HDF5, i_file_particle, group, "bx_grad", chunk_size, overlap_size);
  //PN->EnableCache();
  P->Apply(TriDInterUDF, PN);
  P->ReportTime();
  //PN->Nonvolatile();
  //Clear
  delete X;
  delete Y;
  delete Z;
  delete P;
  delete M;
  delete PN;
  MPI_Finalize();
  
  return 0; 
}
