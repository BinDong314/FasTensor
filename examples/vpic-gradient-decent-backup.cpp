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
  float i;
  friend std::ostream& operator<<(std::ostream& os, Particle &p)   //test 
  {  
    os << p.x << '/' << p.y << '/' << p.z << '/' << p.i;  
    return os;  
  }
  friend bool operator == (Particle &lp, Particle& rp)
  {
    return  (lp.x == rp.x && lp.y == rp.y && lp.z == rp.z  && lp.i == rp.i);
  }
};


Array<float> *np_local;
Array<float> *x0;
Array<float> *nx;
Array<float> *dx;
Array<float> *y0;
Array<float> *ny;
Array<float> *dy;
Array<float> *z0;
Array<float> *nz;
Array<float> *dz;


//
//To degug here, with single process
// Why bx000~bx111 are zero? 
// Also, the openMP seems doesn't work here
// 
inline float X_UDF(const Stencil<Particle> &p) 
{
  float result;
  return  result;
}

inline float Y_UDF(const Stencil<Particle> &p) 
{
  float result;
  return  result;
}

inline float Z_UDF(const Stencil<Particle> &p) 
{
  float result;
  return  result;
}



int main(int argc, char *argv[])
{
    // create an instance of the myArray class 
    // 123, 463, 188960
  char i_file[1024]="/global/cscratch1/sd/dbin/de-test-all-osts/vpic-small-mag2/data1/bx_1.h5p";
  char o_file[1024]="/global/cscratch1/sd/dbin/de-test-all-osts/vpic-small-mag2/data1/bx_1_grad.h5p";
 char  group[1024]="/Step#4";
  char dataset[1024]="/Step#4/bx";

  std::vector<int> chunk_size(3) ; // = {41,463,1181};
  std::vector<int> overlap_size(3);// = {1,1,1};
  chunk_size[0]   = 256;  chunk_size[1] = 256; chunk_size[2]   = 256;
  overlap_size[0] = 1; overlap_size[1] =   1;  overlap_size[2] = 1;
  MPI_Init(&argc, &argv);

  //Array<float> *A = new Array<float>(AU_NVS,  AU_HDF5, i_file, group, dataset, chunk_size, overlap_size);
  //Array<float> *BX = new Array<float>(AU_NVS,  AU_HDF5, i_file, group, dataset, AU_PRELOAD);
  //BX->EnableApplyWriteback(); 
  //BX->Apply(GradientUDF, NULL);
  //std::cout <<  "GX Gradient at 1000: " << BX->operator()(1, 1, 1) << std::endl;
  //BX->ReportTime(); 
  //delete BX;
  
  //Orginal data set
  Array<float> *dX = new Array<float>(AU_NVS,  AU_HDF5, i_file, group, "/Step#0/dX", chunk_size, overlap_size);
  //Orginal data set
  Array<float> *dY = new Array<float>(AU_NVS,  AU_HDF5,  i_file, group, "/Step#0/dY",  chunk_size, overlap_size);
  //Orginal data set
  Array<float> *dZ = new Array<float>(AU_NVS,  AU_HDF5, i_file, group,   "/Step#0/dZ", chunk_size, overlap_size);
  //Orginal data set
  Array<float> *i = new Array<float>(AU_NVS,  AU_HDF5, i_file, group,   "/Step#0/i", chunk_size, overlap_size);

  Array<Particle, float> *P = new Array<Particle, float>(AU_VIRTUAL);
  P->SetAttributes(X, Y, Z, i); //Todo: change to pushback()

  np_local = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "/Step#4/np_local", AU_PRELOAD);
  x0       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "/Step#4/x0", AU_PRELOAD);
  nx       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "/Step#4/nx", AU_PRELOAD);
  dx       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "/Step#4/dx", AU_PRELOAD);
  y0       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "/Step#4/y0", AU_PRELOAD);
  ny       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "/Step#4/ny", AU_PRELOAD);
  dy       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "/Step#4/dy", AU_PRELOAD);
  z0       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "/Step#4/z0", AU_PRELOAD);
  nz       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "/Step#4/nz", AU_PRELOAD);
  dz       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "/Step#4/dz", AU_PRELOAD);
  

  
  //Results data sets
  Array<float> *X = new Array<float>(AU_COMPUTED, AU_HDF5, i_file, group, "/Step#0/x", chunk_size, overlap_size);
  //Results data sets
  Array<float> *Y = new Array<float>(AU_COMPUTED, AU_HDF5, i_file, group, "/Step#0/y", chunk_size, overlap_size);
  //Results data sets
  Array<float> *Z = new Array<float>(AU_COMPUTED, AU_HDF5, i_file, group, "/Step#0/z", chunk_size, overlap_size);

  
  P->Apply(X_UDF, X);  
  P->Apply(Y_UDF, Y);  
  P->Apply(Z_UDF, Z);  
  
  MPI_Finalize();

  //Clear
  delete dX;
  delete dY;
  delete dZ;
  delete i;
  delete X;
  delete Y;
  delete Z;
  delete P;

  return 0; 
}
