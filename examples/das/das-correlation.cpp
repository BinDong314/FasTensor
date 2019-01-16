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

#define NAME_LENGTH 1024
void convert_str_vector(int n, char *str,  int *vector);
void  printf_help(char *cmd);

using namespace std;
#define SQRT_M(XV, NN,  sqSum_X){\
    sqSum_X = 0; \	
   for(int iiii=0; iiii<NN; iiii++){ \
       sqSum_X = sqSum_X + XV[iiii]*XV[iiii]; \
   } \
    sqSum_X = sqrt(sqSum_X); \
}

#define SQRT_SUM_M(CW, X1, X2, NN, sum_CWX1, sum_CWX2, sq_X1, sq_X2, neighbor_window_start){\
   sum_CWX1=0; \
   sum_CWX2=0; \
   sq_X1=0;  \
   sq_X2=0;  \	
   for(int i=0; i<NN; i++){ \
        sum_CWX1 = sum_CWX1 + CW[i]*X1[i+neighbor_window_start];\
	sum_CWX2 = sum_CWX2 + CW[i]*X2[i+neighbor_window_start];\
	sq_X1    = sq_X1 + X1[i+neighbor_window_start]*X1[i+neighbor_window_start]; \
	sq_X2    = sq_X2 + X2[i+neighbor_window_start]*X2[i+neighbor_window_start]; \
   }\
   sq_X1=sqrt(sq_X1);\
   sq_X2=sqrt(sq_X2);\
}


//#define CELL_OFFSET    500   //size of a window = 2*CELL_OFFSET+1
//#define CELL_TOTAL     2*CELL_OFFSET+1
//#define CHANNEL_OFFSET 10     //which channel to correlate
//#define WINDOW_OFFSET  50    //how many windows to look forward and backward
//float current_window[CELL_TOTAL];
//float neighbor_window1[CELL_TOTAL+2*WINDOW_OFFSET], neighbor_window2[CELL_TOTAL+2*WINDOW_OFFSET];

int    CELL_OFFSET = 500, CELL_TOTAL = 2*500+1, CHANNEL_OFFSET = 10, WINDOW_OFFSET = 50;
float *current_window=NULL, *neighbor_window1=NULL, *neighbor_window2=NULL;
int    neighbor_window_start;

inline float CorrelationUDF(const Stencil<float> &c){
  float max_correlation1 = 0, max_correlation2 = 0, temp_correlation1, temp_correlation2;
  float current_sqSum = 0, neighbor_sqSum1 = 0,neighbor_sqSum2 = 0, cn_Sum1 = 0, cn_Sum2 =0 ;
  for(int i = - CELL_OFFSET; i  <= CELL_OFFSET; i++){
      current_window[i+CELL_OFFSET] = c(i,0);
  }
  SQRT_M(current_window, CELL_TOTAL, current_sqSum);

  for(int j = - CELL_OFFSET - WINDOW_OFFSET; j  <= CELL_OFFSET+WINDOW_OFFSET; j++){
    	neighbor_window1[j+CELL_OFFSET+WINDOW_OFFSET] = c(j, CHANNEL_OFFSET);
	neighbor_window2[j+CELL_OFFSET+WINDOW_OFFSET] = c(j,-CHANNEL_OFFSET);
  }

  for(int j = - WINDOW_OFFSET; j  <= WINDOW_OFFSET; j++){
	  neighbor_window_start = j+WINDOW_OFFSET; 
	  SQRT_SUM_M(current_window, neighbor_window1, neighbor_window2, CELL_TOTAL, cn_Sum1, cn_Sum2, neighbor_sqSum1, neighbor_sqSum2, neighbor_window_start);
          temp_correlation1 = abs(cn_Sum1)/(current_sqSum * neighbor_sqSum1);
          temp_correlation2 = abs(cn_Sum2)/(current_sqSum * neighbor_sqSum2);
	  if(temp_correlation1 > max_correlation1) max_correlation1 = temp_correlation1;
          if(temp_correlation2 > max_correlation2) max_correlation2 = temp_correlation2;
  }

  return (max_correlation1+max_correlation2)/2;
}

int main(int argc, char *argv[])
{
  char  i_file[NAME_LENGTH]=      "/global/cscratch1/sd/dbin/de-test-all-osts/DAS/data_earthquake.h5";
  char  o_file[NAME_LENGTH]=      "/global/cscratch1/sd/dbin/de-test-all-osts/DAS/data_earthquake-udf.h5";
  char  group[NAME_LENGTH]="/";  //both input and output file share the same group and dataset name
  char  dataset[NAME_LENGTH]="/dat";
  char     ghost_size_str[NAME_LENGTH];
  char     chunk_size_str[NAME_LENGTH];
  char     strip_size_str[NAME_LENGTH];
  std::vector<int>     ghost_size;
  std::vector<int>     chunk_size;
  std::vector<int>     strip_size;
  int      array_ranks;
  int      strip_flag = 0;

  int copt; 
  while ((copt = getopt (argc, argv, "o:i:g:d:c:t:n:s:e:w:l:h")) != -1)
    switch (copt)
    {
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
      case 'c':
        strcpy(chunk_size_str, optarg);
        break;
      case 't':
        strcpy(ghost_size_str, optarg);
        break;
      case 's':
        strip_flag = 1;
        strcpy(strip_size_str, optarg);
        break;
      case 'n':
        array_ranks    = atoi(optarg);
        break;
      case 'e':
        CELL_OFFSET = atoi(optarg);
        CELL_TOTAL = 2*CELL_OFFSET+1;
        break;
      case 'w':
        WINDOW_OFFSET = atoi(optarg);
        break;
      case 'l':
        CHANNEL_OFFSET = atoi(optarg);
        break;
      case 'h':
        printf_help(argv[0]);
        break;	
      default:
        printf("Wrong option [%c] for %s \n",copt, argv[0]);
        printf_help(argv[0]);
        exit(-1);
        break;
    }

  chunk_size.resize(array_ranks); ghost_size.resize(array_ranks); strip_size.resize(array_ranks);
  convert_str_vector(array_ranks, ghost_size_str,   &(ghost_size[0]));
  convert_str_vector(array_ranks, chunk_size_str,   &(chunk_size[0]));
  if(strip_flag) convert_str_vector(array_ranks, strip_size_str,   &(strip_size[0]));

  current_window   =  (float *)malloc(sizeof(float) * CELL_TOTAL);
  neighbor_window1 =  (float *)malloc(sizeof(float) * (CELL_TOTAL + 2*WINDOW_OFFSET));
  neighbor_window2 =  (float *)malloc(sizeof(float) * (CELL_TOTAL + 2*WINDOW_OFFSET));
  
  //( 30000, 11648 )  11648/91= 128 chunks
  //std::vector<int> chunk_size;  chunk_size.resize(array_ranks);
  //std::vector<int> overlap_size; 
  //std::vector<int> striping_size;
  //chunk_size[0]   = 180001; chunk_size[1]    = 20; 
  //overlap_size[0] = 0;     overlap_size[1]   = 11 ; 
  //striping_size[0] =10;    striping_size[1]  = 1;

  MPI_Init(&argc, &argv);
  //Array<float> *A = new Array<float>(AU_NVS,  AU_HDF5, i_file, group, dataset, chunk_size, overlap_size);
  Array<int, float> * IFILE = new Array<int float>(AU_NVS,       AU_HDF5, i_file, group, dataset, chunk_size, ghost_size);
  Array<float>      * OFILE = new Array<float>(AU_COMPUTED,  AU_HDF5, o_file, group, dataset, chunk_size, ghost_size);
  
  if(strip_flag) IFILE->SetApplyStripSize(strip_size); 
  //rank, dims, chunk_size, overlap_siz
  IFILE->Apply(CorrelationUDF, OFILE);
  IFILE->ReportTime();

  delete IFILE;
  delete OFILE;
  MPI_Finalize();

  free(current_window);free(neighbor_window1);free(neighbor_window2);
  return 0; 
}



void convert_str_vector(int n, char *str,  int *vector){
  int   i;
  char *pch;
  char  temp[NAME_LENGTH];
  
  if (n == 1){
    vector[0] = atoi(str);
  }else{
    strcpy(temp, str);
    pch = strtok(temp, ",");
    
    i = 0;
    while(pch != NULL)
    {
      //printf("%s \n", pch);	   
      vector[i] = atoi(pch);
      pch=strtok(NULL, ",");
      i++;
    }
  }
  return;
}

void  printf_help(char *cmd){
  char *msg="Usage: %s [OPTION] \n\
      	  -h help (--help)\n\
          -i input file \n\
          -o output file \n\
          -g group name (path) \n\ 
          -d dataset name \n\
          -c chunk size (seperate by comma , ) \n\
          -t ghost szie (seperate by comma ,) \n\
          -n number of array dimension \n\
          -s strip size (seperate by comma ,) \n\
          -e cell offset  \n\
          -w window offset \n\
          -l channel offset  \n\
          Example: srun -n 128 -N 4 %s -i /global/cscratch1/sd/dbin/de-test-all-osts/DAS/data_earthquake.h5 -o /global/cscratch1/sd/dbin/de-test-all-osts/DAS/data_earthquake-arrayudf.h5  -g / -d /dat -n 2 -c 180001,20 -t 0,11  -e 500 -w 50 -l 10 \n";

  fprintf(stdout, msg, cmd, cmd);
}


// // function that returns correlation coefficient.
// //Not used
// inline float correlationV(float X[], float Y[], int n)
// {
//     float sum_XY = 0;
//     float squareSum_X = 0, squareSum_Y = 0;
//     for (int i = 0; i < n; i++)
//     {
//         // sum of X[i] * Y[i].
//         sum_XY = sum_XY + X[i] * Y[i];
//         // sum of square of array elements.
//         squareSum_X = squareSum_X + X[i] * X[i];
//         squareSum_Y = squareSum_Y + Y[i] * Y[i];
//     }
//     // use formula for calculating correlation coefficient.
//     float corr = abs(sum_XY) / (sqrt(squareSum_X) * sqrt(squareSum_Y));
//     return corr;
// }

