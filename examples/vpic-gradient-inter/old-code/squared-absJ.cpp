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

void create_xdmf_file(char *xml_output_file, char *dimensions,  char *origin_dxdydz_dimensions, char * origin,  char *dxdydz, char * attribute_name, char *hdf5_file_dataset);


float local_sum = 0;
unsigned long local_n = 0;
//Define operation for each vector (JX/JY/JZ)
inline float squared_absJ_UDF(const Stencil<float> &hp) 
{
  //printf(" jx,jy,jz = (%f, %f, %f) \n", hpt.jx,  hpt.jy,  hpt.jz);
  local_sum = local_sum +  hp(0) * hp(0);
  local_n++;
}


int main(int argc, char *argv[])
{

  char  i_file[1024]="/Users/dbin/work/vpic-hdf5/vpic/build/testdir/hydro_hdf5/T.50/hydro_ion_50_absJ.h5";  
  char  group[1024]   ="/Timestep_50";
  char  dsetabsJ[1024]="/Timestep_50/absJ";

  //Split array into chunks for parallel processing
  std::vector<int> chunk_size(3) ;
  std::vector<int> overlap_size(3);
  chunk_size[0]   = 32;   chunk_size[1]   = 1;   chunk_size[2]   = 32;  
  overlap_size[0] =  0;   overlap_size[1] = 0;   overlap_size[2] = 0;


  MPI_Init(&argc, &argv);
  int mpi_size, mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);


  
  //Orginal data sets
  Array<float> *ABSJ = new Array<float>(AU_NVS,  AU_HDF5, i_file, group,  dsetabsJ, chunk_size, overlap_size);
  
  //Run the code 
  ABSJ->Apply(squared_absJ_UDF, NULL);  
  
  float global_sum;
  MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
  
  unsigned long global_n;
  MPI_Reduce(&local_n, &global_n, 1, MPI_UNSIGNED_LONG , MPI_SUM, 0, MPI_COMM_WORLD);
  if(mpi_rank == 0){
    global_sum = global_sum / (float)global_n;
    global_sum =sqrt(global_sum); 
    printf("squared_absJ = %f \n", global_sum);
  }




  //Clear
  delete ABSJ;

  MPI_Finalize();

  return 0; 
}









