/*
 *ArrayUDF Copyright (c) 2017, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy).  All rights reserved.

 *If you have questions about your rights to use or distribute this software, please contact Berkeley Lab's Innovation & Partnerships Office at  IPO@lbl.gov.

* NOTICE. This Software was developed under funding from the U.S. Department of Energy and the U.S. Government consequently retains certain rights. As such, the U.S. Government has been granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software to reproduce, distribute copies to the public, prepare derivative works, and perform publicly and display publicly, and to permit other to do so. 
 */
/**
 *
 * Email questions to {dbin, kwu, sbyna}@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include <math.h>       /* ceil  and floor*/
#include <cmath>

#include "array_udf.h"
#define abs(x) ((x)<0 ? -(x) : (x))


using namespace std;


struct Particle{
  float x;
  float y;
  float z;
  friend std::ostream& operator<<(std::ostream& os, Particle &p)   //test 
  {  
    os << p.x << '/' << p.y << '/' << p.z;  
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

float bshape[3]      = {2000, 2000,  800};
float borigin[3]     = {0,    -165,  -66};
float originShift[3] = {0 ,    165,   66};
float bsize[3]       = {330,   330,  132};
float bcellSize[3]   = {0.1650825, 0.1650825, 0.16520650}; //bsize/(bshape-1.0)

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
  float      x[2], y[2], z[2], xw, yw, zw;
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
  }
  if(y[1] >= bshape[1]){
    y[1] = bshape[1] -  1;
  }
  if(z[1] >= bshape[2]){
    z[1] = bshape[2] -  1;
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
  
  //float temp1 = M->operator()(0,0,0);
  float result;
  result = bx000*(1.0-xw)*(1.0-yw)*(1.0-zw) + bx100*xw*(1.0-yw)*(1.0-zw) + bx010*(1.0-xw)*yw*(1.0-zw) +  bx001*(1.0-xw)*(1.0-yw)*zw + bx101*xw*(1.0-yw)*zw + bx011*(1.0-xw)*yw*zw + bx110*xw*yw*(1.0-zw) + bx111*xw*yw*zw;
  return  result;
  //return pt.x;
  //printf("Particle x: %f, y: %f, z: %f \n", std::get<0>(p),  std::get<2>(p),  std::get<2>(p));
  //return  std::get<0>(p)+std::get<2>(p)+std::get<2>(p);
}


int main(int argc, char *argv[])
{

  int cache_flag = 0, backward_flag = 0, mirrors_flag = 0, replace_flag = 0, copt, writeback_flag = 0;
  int c_size = 12470411, o_size = 0;
  char i_file[1024]="/global/cscratch1/sd/dbin/de-test-all-osts/eparticle_T22860_1.5_filter.h5p";
  char o_file[1024]="/global/cscratch1/sd/dbin/de-test-all-osts/eparticle_T22860_1.5_filter_inter_result.h5p";
  char t_file[1024]= "/global/cscratch1/sd/dbin/de-test-all-osts/bx_22860.h5p";
   //"/global/cscratch1/sd/dbin/de-test-all-osts/bx_22860.h5p"; bx-small-test.h5p
  char  group[1024]="/Step#0";
  char dataset[1024]="/p";
  int f_value = 0;

  while ((copt = getopt (argc, argv, "cbmrwo:i:g:d:t:s:h:f:")) != -1)
    switch (copt)
    {
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
      case 't':
        memset(t_file, 0, sizeof(t_file));
        strcpy(t_file, optarg);
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



  //Init the MPICH
  AU_Init(argc, argv);

 //  std::cout << offsetof(Particle, x) << std::endl;
 // std::cout << offsetof(Particle, y) << std::endl;
 // std::cout << offsetof(Particle, z) << std::endl;
 
 //DATASPACE  SIMPLE { ( 623520550 ) 
 //1213075*514
  // set up the chunk size and the overlap size
  //Please run me with 10 cores. 
  std::vector<int> chunk_size;    chunk_size.push_back(c_size); 
  std::vector<int> overlap_size;  overlap_size.push_back(o_size);


  //Orginal data set
  Array<float> *X = new Array<float>(AU_NVS,  AU_HDF5, i_file, group, "/Step#0/x", chunk_size, overlap_size);
  //Orginal data set
  Array<float> *Y = new Array<float>(AU_NVS,  AU_HDF5,  i_file, group, "/Step#0/y",  chunk_size, overlap_size);
  //Orginal data set
  Array<float> *Z = new Array<float>(AU_NVS,  AU_HDF5, i_file, group,   "/Step#0/z", chunk_size, overlap_size);


  M = new Array<float>(AU_NVS, AU_HDF5, t_file, group, "/Step#0/bx", AU_PRELOAD);

  Array<Particle, float> *P = new Array<Particle, float>(AU_VIRTUAL);
  P->SetAttributes(X, Y, Z); //Todo: change to pushback()

  //Particle p=P->operator()(1);
  //std::cout <<  "P : x = " << p.x << ", y = " << p.y << ", z = " << p.z << std::endl;
  
  //Results data sets
  Array<float> *PN = new Array<float>(AU_COMPUTED, AU_HDF5, o_file, group, dataset, chunk_size, overlap_size);
     
  P->Apply(TriDInterUDF, PN);  
  
  //Clear
  delete X;
  delete Y;
  delete Z;
  delete P;
  delete M;
  delete PN;
    

  AU_Finalize();

   
  return 0; 
}


