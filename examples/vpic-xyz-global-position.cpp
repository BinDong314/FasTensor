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


Array<int> *np_local;
Array<float> *x0v;
Array<float> *nx;
Array<float> *dx;
Array<float> *y0v;
Array<float> *ny;
Array<float> *dy;
Array<float> *z0v;
Array<float> *nz;
Array<float> *dz;


#define DOMAIN_NUM 2048

inline int find_domain_index(unsigned long long particle_offset){
  int domain_index = 0;
  int np_local_acc = 0;
  for(int i = 0; i < DOMAIN_NUM; i++){
    domain_index = i;
    if (particle_offset > np_local_acc &&  particle_offset < (np_local_acc + np_local->operator()(i) ) ){
      break;
    }
    np_local_acc = np_local_acc + np_local->operator()(i);
  }
  return domain_index;
}

#define FIND_DOMAIN_INDEX_MACRO(particle_offset_m, domain_index_m){ \
  unsigned long long np_local_acc = 0;  \
  int  np_local_acc_temp; \
  for(int i = 0; i < DOMAIN_NUM; i++){ \
    domain_index_m = i; \
    np_local_acc_temp = np_local->operator()(i); \
    if (particle_offset_m > np_local_acc &&  particle_offset_m < (np_local_acc + np_local_acc_temp ) ){ \
      break; \
    } \
    np_local_acc = np_local_acc + np_local_acc_temp; \
  } \
}

int prev_domain_index = 0;
unsigned long long prev_np_local_acc =  0 ;
int              prev_np_local_acc_temp = 0;
inline float Domain_UDF(const Stencil<float> &p){
  unsigned long long particle_offset = p.get_id();
  if(particle_offset >= prev_np_local_acc && particle_offset < (prev_np_local_acc + prev_np_local_acc_temp))
    return (float) prev_domain_index;
  
  int   domain_index;
  unsigned long long np_local_acc = 0;  
  int np_local_acc_temp;
  for(int i = prev_domain_index; i < DOMAIN_NUM; i++){ 
    domain_index = i;                         
    np_local_acc_temp = np_local->operator()(i); 
    if (particle_offset >= np_local_acc &&  particle_offset < (np_local_acc + np_local_acc_temp ) ){
      break; 
    } 
    np_local_acc = np_local_acc + np_local_acc_temp; 
  }
  
  prev_domain_index = domain_index;    
  prev_np_local_acc = np_local_acc;
  prev_np_local_acc_temp = np_local_acc_temp;
  
  return (float) (domain_index) ;   
}


int nx_v = 0;
int x_udf_prev_domain_index = -1;
float dx_v = 0;
float x0_v = 0;

//x =  ( i%(nx[0] +2)   + (dX-1) /2.0 ) * dx[0] + x0[0]  = ...
inline float X_UDF(const Stencil<Particle> &p) 
{
  Particle pt = p(0);
  int   domain_index = (int) (pt.domain_id);
  if(x_udf_prev_domain_index != domain_index){
    nx_v = (int) (nx->operator()(domain_index));
    dx_v = dx->operator()(domain_index);
    x0_v = x0v->operator()(domain_index);
    x_udf_prev_domain_index = domain_index;
  }
  //if(p.get_id() < 10)
  //printf("domain_index＝%d, nx_v = %d , dx_v= %f, x0_v = %f, pd.dX=%f, x0_v=%f \n ", domain_index, nx_v, dx_v, x0_v, pt.dX, x0_v);
  //x_v = 
  return  ( ((int)(pt.i)) % ( nx_v + 2) + (pt.dX - 1)/2.0 ) * dx_v + x0_v;
}

inline float Y_UDF(const Stencil<Particle> &p) 
{
  Particle pt = p(0);
  float y_v, dy_v, y0_v;
  int   domain_index;
  //domain_index = find_domain_index(p.get_id());
  domain_index = (int) (pt.domain_id);
  int ny_v = ny->operator()(domain_index);
  dy_v = dy->operator()(domain_index);
  y0_v = y0v->operator()(domain_index);
  int nx_v = ny->operator()(domain_index);

  y_v = ( (int)(pt.i / (nx_v + 2))%(ny_v + 2) + (pt.dY - 1)/2.0 ) * dy_v + y0_v;
  return  y_v;
}

inline float Z_UDF(const Stencil<Particle> &p) 
{
  Particle pt = p(0);
  float z_v, dz_v, z0_v;
  int   domain_index;
  //domain_index = find_domain_index(p.get_id());
  domain_index = (int) (pt.domain_id);
  int nz_v = nz->operator()(domain_index);
  dz_v = dz->operator()(domain_index);
  z0_v = z0v->operator()(domain_index);
  int ny_v = ny->operator()(domain_index);
  int nx_v = nx->operator()(domain_index);
  
  z_v = ( (float)pt.i / ((nx_v + 2) * (ny_v + 2)) + (pt.dZ - 1)/2.0 ) * dz_v + z0_v;
  return  z_v;
}


//UDF One: create new labels
inline float I_UDF(const Stencil<int> &c){
  float i_v_float;
  i_v_float  =  (float)(c(0));
  return i_v_float;
}


int main(int argc, char *argv[])
{
  int cache_flag = 0, backward_flag = 0, mirrors_flag = 0, replace_flag = 0, copt, writeback_flag = 0, convert_flag = 0;
  int c_size = 12288, o_size = 1;
  int f_value = 0;
  
  
  // create an instance of the myArray class 
  // 23488064664
  char i_file[1024]="/global/cscratch1/sd/dbin/de-test-all-osts/vpic-small-mag2/particle/T.2/";
  char t_file[1024]="/global/cscratch1/sd/dbin/de-test-all-osts/vpic-small-mag2/particle/T.2/grid_metadata.h5p";
  char o_file[1024]="/global/cscratch1/sd/dbin/de-test-all-osts/vpic-small-mag2/particle/T.2/eparticle-new.h5p";
  char o_file_x[1024]="/global/cscratch1/sd/dbin/de-test-all-osts/vpic-small-mag2/particle/T.2/eparticle-new-x.h5p";
  char o_file_y[1024]="/global/cscratch1/sd/dbin/de-test-all-osts/vpic-small-mag2/particle/T.2/eparticle-new-y.h5p";
  char o_file_z[1024]="/global/cscratch1/sd/dbin/de-test-all-osts/vpic-small-mag2/particle/T.2/eparticle-new-z.h5p";
  char o_file_domain_id[1024]="/global/cscratch1/sd/dbin/de-test-all-osts/vpic-small-mag2/particle/T.2/eparticle-new-domain-id.h5p";


  
  char  group[1024]="/Step#2";
  char dataset[1024]="/Step#2/dX";

  std::vector<int> chunk_size(1) ;
  std::vector<int> overlap_size(1);
  chunk_size[0]   = 80000000; 
  overlap_size[0] = 0; 

  while ((copt = getopt (argc, argv, "cbmrwo:i:g:d:t:s:h:f:e")) != -1)
    switch (copt)
    {
      case 'e':
        convert_flag = 1;
        break;
      case 's':
        c_size =  atoi(optarg);
        break;	
      case 'h':
        o_size = atoi(optarg);
        break;	
      case 'f':
        f_value = atoi(optarg);
        break;	
      case 'c':
        cache_flag = 1;
        break;
      case 'b':
        backward_flag = 1;
        break;
      case 'm':
        mirrors_flag = 1;
        break;
      case 'r':
        replace_flag = 1;
        break;
      case 'w':
        writeback_flag = 1;
        break;
      case 'o':
	memset(o_file, 0, sizeof(o_file));
        strcpy(o_file, optarg);
	break;
      case 'i':
	memset(i_file, 0, sizeof(i_file));
	strcpy(i_file, optarg);
	break;
      case 'g':
        memset(group, 0, sizeof(group));
        strcpy(group, optarg);
	break;
      case 'd':
	memset(dataset, 0, sizeof(dataset));
        strcpy(dataset, optarg);
	break;
      default:
        break;
    }
  
  MPI_Init(&argc, &argv);

  if(convert_flag == 1){
    //Array<int, float> *i0 = new Array<int, float>(AU_NVS,  AU_HDF5, i_file, group,   "i", chunk_size, overlap_size);
    //Array<float> *i1 = new Array<float>(AU_COMPUTED,  AU_HDF5, o_file, group,   "if", chunk_size, overlap_size);
    //i0->Apply(I_UDF, i1);
    //delete i0;
    //delete i1;

    np_local = new Array<int>(AU_NVS, AU_HDF5, t_file, group, "np_local", AU_PRELOAD);
    Array<float> *ii1 = new Array<float>(AU_NVS,  AU_HDF5, i_file, group,   "if", chunk_size, overlap_size);
    Array<float> *ii2 = new Array<float>(AU_COMPUTED,  AU_HDF5, o_file_domain_id, group,   "domain_id", chunk_size, overlap_size);
    ii1->Apply(Domain_UDF, ii2);
    delete ii1;
    delete ii2;
    delete np_local;
    return 0;
  }
  
  
  //Orginal data set
  Array<float> *dX = new Array<float>(AU_NVS,  AU_HDF5, i_file, group, "dX", chunk_size, overlap_size);
  //Orginal data set
  Array<float> *dY = new Array<float>(AU_NVS,  AU_HDF5,  i_file, group, "dY",  chunk_size, overlap_size);
  //Orginal data set
  Array<float> *dZ = new Array<float>(AU_NVS,  AU_HDF5, i_file, group,  "dZ", chunk_size, overlap_size);
  //Orginal data set
  Array<float> *i = new Array<float>(AU_NVS,  AU_HDF5, i_file, group,   "if", chunk_size, overlap_size);
  Array<float> *domain_id = new Array<float>(AU_NVS,  AU_HDF5, i_file, group,   "domain_id", chunk_size, overlap_size);

  
  Array<Particle, float> *P = new Array<Particle, float>(AU_VIRTUAL);
  P->SetAttributes(dX, dY, dZ, i, domain_id); //Todo: change to pushback()

  //np_local = new Array<int>(AU_NVS, AU_HDF5, t_file, group, "np_local", AU_PRELOAD);
  x0v       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "x0", AU_PRELOAD);
  nx       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "nx", AU_PRELOAD);
  dx       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "dx", AU_PRELOAD);
  y0v       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "y0", AU_PRELOAD);
  ny       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "ny", AU_PRELOAD);
  dy       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "dy", AU_PRELOAD);
  z0v       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "z0", AU_PRELOAD);
  nz       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "nz", AU_PRELOAD);
  dz       = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "dz", AU_PRELOAD);
  

  
  //Results data sets
  Array<float> *X = new Array<float>(AU_COMPUTED, AU_HDF5, o_file_x, group, "x", chunk_size, overlap_size);
  //Results data sets
  Array<float> *Y = new Array<float>(AU_COMPUTED, AU_HDF5, o_file_y, group, "y", chunk_size, overlap_size);
  //Results data sets
  Array<float> *Z = new Array<float>(AU_COMPUTED, AU_HDF5, o_file_z, group, "z", chunk_size, overlap_size);

  
  P->Apply(X_UDF, X);  
  P->Apply(Y_UDF, Y);  
  P->Apply(Z_UDF, Z);  
  
  MPI_Finalize();

  //Clear
  delete dX;
  delete dY;
  delete dZ;
  delete i;
  delete domain_id;
  delete X;
  //delete Y;
  //delete Z;
  delete P;

  //delete np_local;
  delete x0v;
  delete nx;
  delete dx;
  delete y0v;
  delete ny;
  delete dy;
  delete z0v;
  delete nz;
  delete dz;

  return 0; 
}
