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

//Define a compund data strcuture 
struct HydroPoint{
  float jx;
  float jy;
  float jz;
  //Some help functions.
  friend std::ostream& operator<<(std::ostream& os, HydroPoint &p)   //test 
  {  
    os << p.jx << '/' << p.jy << '/' << p.jz  << std::endl;  
    return os;  
  }
    friend bool operator == (HydroPoint &lp, HydroPoint& rp)
  {
    return  (lp.jx == rp.jx && lp.jy == rp.jy && lp.jz == rp.jz);
  }

  void set_value(int index, float value){
    switch(index){
      case 0:
        jx = value;
        break;
      case 1:
        jy =value;
        break;
      case 2:
        jz = value;
        break;
      default:
        printf("The object only has three variables !\n");
        break;
    }
  }
};


//Define operation for each vector (JX/JY/JZ)
inline float absJ_UDF(const Stencil<HydroPoint> &hp) 
{
  HydroPoint hpt = hp(0,0,0);
  //printf(" jx,jy,jz = (%f, %f, %f) \n", hpt.jx,  hpt.jy,  hpt.jz);
  return sqrt(hpt.jx * hpt.jx + hpt.jy * hpt.jy + hpt.jz * hpt.jz);
}


int main(int argc, char *argv[])
{

  char i_file[1024]="/Users/dbin/work/vpic-hdf5/vpic/build/testdir/hydro_hdf5/T.50/hydro_ion_50.h5";
  char o_file[1024]="/Users/dbin/work/vpic-hdf5/vpic/build/testdir/hydro_hdf5/T.50/hydro_ion_50_m.h5";
  
  char  group[1024]="/Timestep_50";
  char  dsetX[1024]="/Timestep_50/jx";
  char  dsetY[1024]="/Timestep_50/jy";
  char  dsetZ[1024]="/Timestep_50/jz";
  char  dsetM[1024]="/Timestep_50/absJ";

  //Split array into chunks for parallel processing
  std::vector<int> chunk_size(3) ;
  std::vector<int> overlap_size(3);
  chunk_size[0]   = 32;   chunk_size[1]   = 1;   chunk_size[2]   = 32;  
  overlap_size[0] =  0;   overlap_size[1] = 0;   overlap_size[2] = 0;


   MPI_Init(&argc, &argv);


  
  //Orginal data sets
  Array<float> *JX = new Array<float>(AU_NVS,  AU_HDF5, i_file, group,  dsetX, chunk_size, overlap_size);
  Array<float> *JY = new Array<float>(AU_NVS,  AU_HDF5, i_file, group,  dsetY,  chunk_size, overlap_size);
  Array<float> *JZ = new Array<float>(AU_NVS,  AU_HDF5, i_file, group,  dsetZ, chunk_size, overlap_size);
 
  
  //Create a compound data strucutre to include JX/JY/JZ
  Array<HydroPoint, float> *JXYZ = new Array<HydroPoint, float>(AU_VIRTUAL);
  JXYZ->SetAttributes(JX, JY, JZ); 


  //Output array
  Array<float> *M = new Array<float>(AU_COMPUTED, AU_HDF5, o_file, group, dsetM, chunk_size, overlap_size);

  //Run the code 
  JXYZ->Apply(absJ_UDF, M);  
  

  //Clear
  delete JX;
  delete JY;
  delete JZ;
  delete JXYZ;
  delete M;

  MPI_Finalize();

  return 0; 
}
