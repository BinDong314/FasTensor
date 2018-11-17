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
  float dX;
  float dY;
  float dZ;
  float i;
  float domain_id;
  friend std::ostream& operator<<(std::ostream& os, Particle &p)   //test 
  {  
    os << p.dX << '/' << p.dY << '/' << p.dZ << '/'  << p.i << '/' << p.domain_id ;  
    return os;  
  }
  friend bool operator == (Particle &lp, Particle& rp)
  {
    return  (lp.dX == rp.dX && lp.dY == rp.dY && lp.dZ == rp.dZ  && lp.i == rp.i);
  }
  void set_value(int index, float value){
    switch(index){
      case 0:
        dX = value;
        break;
      case 1:
        dY =value;
        break;
      case 2:
        dZ = value;
        break;
      case 3:
        i = value;
        break;
      case 4:
        domain_id = value;
        break;
      default:
        printf("The object only has three variables !\n");
        break;
    }
  }
};


Array<int>   *np_local;
Array<float> *x0v;
Array<float> *dx;
Array<float> *y0v;
Array<float> *dy;
Array<float> *z0v;
Array<float> *dz;

Array<int> *nx;
Array<int> *ny;
Array<int> *nz;


//This is intilized by np-get
int np_total_domains;



inline float Domain_UDF(const Stencil<float> &p){
  unsigned long long particle_offset = p.get_id();
  int   domain_index;
  unsigned long long np_local_acc = 0;  
  for(domain_index = 0; domain_index < np_total_domains; domain_index++){ 
    if (particle_offset >= np_local_acc  &&  particle_offset < (np_local_acc + np_local->operator()(domain_index))){
      break; 
    } 
    np_local_acc = np_local_acc + np_local->operator()(domain_index); 
  }
  return (float) domain_index ;   
}



//x =  ( i%(nx[0] +2)   + (dX-1) /2.0 ) * dx[0] + x0[0]  = ...
inline float X_UDF(const Stencil<Particle> &p) 
{
  Particle pt = p(0);
  int    domain_index = (int) (pt.domain_id);
  int    nx_v = nx->operator()(domain_index);
  float  dx_v = dx->operator()(domain_index);
  float  x0_v = x0v->operator()(domain_index);

  return  ( domain_index % ( nx_v + 2) + (pt.dX - 1)/2.0 ) * dx_v + x0_v;
}

inline float Y_UDF(const Stencil<Particle> &p) 
{
  Particle pt = p(0);
  int domain_index = (int) (pt.domain_id);
  int ny_v = ny->operator()(domain_index);
  int nx_v = ny->operator()(domain_index);
  float dy_v     = dy->operator()(domain_index);
  float y0_v     = y0v->operator()(domain_index);
  return  ((domain_index / (nx_v + 2))%(ny_v + 2) + (pt.dY - 1)/2.0 ) * dy_v + y0_v;
}

inline float Z_UDF(const Stencil<Particle> &p) 
{
  Particle pt = p(0);
  int domain_index = (int) (pt.domain_id);
  float dz_v = dz->operator()(domain_index);
  float z0_v = z0v->operator()(domain_index);
  
  int nx_v = nx->operator()(domain_index);
  int ny_v = ny->operator()(domain_index);
  int nz_v = nz->operator()(domain_index);
  printf("domain_index = %d, dz_v = %f, z0_v = %f, nx_v = %d, ny_v = %d, nz_v = %d, pt.dZ = %f \n " , domain_index,  dz_v, dz_v, z0_v, nx_v, ny_v, nz_v, pt.dZ);
  return (domain_index / ((nx_v + 2) * (ny_v + 2)) + (pt.dZ - 1)/2.0 ) * dz_v + z0_v;
}


//UDF One: create new labels
inline float I_UDF(const Stencil<int> &c){
  float i_v_float;
  i_v_float  =  (float)(c(0));
  return i_v_float;
}


int main(int argc, char *argv[])
{
  int convert_flag = 1, copt;
  int c_size = 3277, o_size = 0;  
  
  char p_file[1024]="/Users/dbin/work/vpic-hdf5/vpic/build/testdir/particle/T.50/electron_50.h5part";
  char m_file[1024]="/Users/dbin/work/vpic-hdf5/vpic/build/testdir/particle/T.50/grid_metadata_electron_50.h5part";
  char o_file[1024]="/Users/dbin/work/vpic-hdf5/vpic/build/testdir/particle/T.50/electron_50_arrayudf.h5part";

  
  char  group[1024]="/Step#50";



  while ((copt = getopt (argc, argv, "cs:h:p:m:g:m:o:")) != -1)
    switch (copt)
    {
      case 'c':
        convert_flag = 1;
        break;
      case 's':
        c_size =  atoi(optarg);
        break;	
      case 'h':
        o_size = atoi(optarg);
        break;	
      case 'p':
	       memset(p_file, 0, sizeof(p_file));
         strcpy(p_file, optarg);
	       break;
      case 'm':
    	   memset(m_file, 0, sizeof(m_file));
	       strcpy(m_file, optarg);
	       break;
      case 'g':
        memset(group, 0, sizeof(group));
        strcpy(group, optarg);
	       break;
      case 'o':
         memset(o_file, 0, sizeof(o_file));
         strcpy(o_file, optarg);
         break;
      default:
        break;
    }
  
  std::vector<int> chunk_size(1) ;
  std::vector<int> overlap_size(1);
  chunk_size[0]   = c_size; 
  overlap_size[0] = o_size; 


  MPI_Init(&argc, &argv);

  if(convert_flag == 1){
    Array<int, float> *iINT = new Array<int, float>(AU_NVS,    AU_HDF5,  p_file, group,   "i", chunk_size, overlap_size);
    Array<float>      *iFLOAT = new Array<float>(AU_COMPUTED,  AU_HDF5,  p_file, group,   "if", chunk_size, overlap_size);
    iINT->Apply(I_UDF, iFLOAT);
    delete iINT;
   

    np_local = new Array<int>(AU_NVS, AU_HDF5, m_file, group,  "np_local", AU_PRELOAD);
    np_total_domains = np_local->GetDims()[0];
    printf("Totoal domains = %d \n", np_total_domains);
    Array<float> *domainID = new Array<float>(AU_COMPUTED,  AU_HDF5,   p_file, group, "domain_id", chunk_size, overlap_size);
    iFLOAT->Apply(Domain_UDF, domainID);
    
    delete np_local;
    delete iFLOAT;
    delete domainID;
    //return 0;
  }
  
  
  //Orginal data set
  Array<float> *dX        = new Array<float>(AU_NVS,  AU_HDF5, p_file, group,   "dX", chunk_size, overlap_size);
  //Orginal data set
  Array<float> *dY        = new Array<float>(AU_NVS,  AU_HDF5, p_file, group,   "dY",  chunk_size, overlap_size);
  //Orginal data set
  Array<float> *dZ        = new Array<float>(AU_NVS,  AU_HDF5, p_file, group,   "dZ", chunk_size, overlap_size);
  //Orginal data set
  Array<float> *i         = new Array<float>(AU_NVS,  AU_HDF5, p_file, group,   "if", chunk_size, overlap_size);
  Array<float> *domain_id = new Array<float>(AU_NVS,  AU_HDF5, p_file, group,   "domain_id", chunk_size, overlap_size);

  
  Array<Particle, float> *P = new Array<Particle, float>(AU_VIRTUAL);
  P->SetAttributes(dX, dY, dZ, i, domain_id); //Todo: change to pushback()

  //np_local = new Array<int>(AU_NVS, AU_HDF5, t_file, group, "np_local", AU_PRELOAD);
  x0v       = new Array<float>(AU_NVS, AU_HDF5,  m_file, group, "x0", AU_PRELOAD);
  y0v       = new Array<float>(AU_NVS, AU_HDF5,  m_file, group, "y0", AU_PRELOAD);
  z0v       = new Array<float>(AU_NVS, AU_HDF5,  m_file, group, "z0", AU_PRELOAD);

  nx        = new Array<int>(AU_NVS, AU_HDF5,  m_file, group, "nx", AU_PRELOAD);
  ny        = new Array<int>(AU_NVS, AU_HDF5,  m_file, group, "ny", AU_PRELOAD);
  nz        = new Array<int>(AU_NVS, AU_HDF5,  m_file, group, "nz", AU_PRELOAD);

  dx        = new Array<float>(AU_NVS, AU_HDF5,  m_file, group, "dx", AU_PRELOAD);
  dy        = new Array<float>(AU_NVS, AU_HDF5,  m_file, group, "dy", AU_PRELOAD);
  dz        = new Array<float>(AU_NVS, AU_HDF5,  m_file, group, "dz", AU_PRELOAD);
  

  
  //Results data sets
  Array<float> *X = new Array<float>(AU_COMPUTED, AU_HDF5, p_file, group, "x", chunk_size, overlap_size);
  Array<float> *Y = new Array<float>(AU_COMPUTED, AU_HDF5, p_file, group, "y", chunk_size, overlap_size);
  Array<float> *Z = new Array<float>(AU_COMPUTED, AU_HDF5, p_file, group, "z", chunk_size, overlap_size);

  
  P->Apply(X_UDF, X);  
  P->Apply(Y_UDF, Y);  
  P->Apply(Z_UDF, Z);  
  
  

  //Clear
  delete dX;
  delete dY;
  delete dZ;
  delete i;
  delete domain_id;
  delete X;
  delete Y;
  delete Z;
  delete P;

  delete x0v;
  delete nx;
  delete dx;
  delete y0v;
  delete ny;
  delete dy;
  delete z0v;
  delete nz;
  delete dz;

  MPI_Finalize();
  return 0; 
}
