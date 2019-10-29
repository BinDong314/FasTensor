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
#include <cstring>

#include "array_udf.h"


using namespace std;

#define FILTERS  8
#define FILTER_WEIGHT 2
#define FILTER_HEIGHT 2

//Weight array; each row is a filter
Array<float> *W; 

//http://cs231n.github.io/convolutional-networks/
inline std::vector<float> ConvUDF(const Stencil<float> &p){
  std::vector<float> test;
  float  temp_result;
  for(int k = 0; k < FILTERS; k++){
    temp_result = 0;
    for(int i = 0 ; i < FILTER_HEIGHT; i++){
      for(int j = 0; j < FILTER_WEIGHT; j++){
        temp_result = p(i, j) + temp_result;
      }
    }
    test.push_back(temp_result);
  }
  
  return  test;
}

//http://cs231n.github.io/convolutional-networks/
inline float ConvUDF2D(const Stencil<float> &p){
  return  p(0,0)*1+p(0,1)*2+p(1,0)*3+p(1,1)*4;
}



inline float ReluUDF(const Stencil<float> &p){
   if(p(0, 0, 0) > 0)
	   return p(0,0,0) + 1;
   return 0;			 
}

inline float ReluUDF2D(const Stencil<float> &p){
   if(p(0, 0) > 0)
	   return p(0,0);
   return 0;			 
}


int main(int argc, char *argv[])
{

  int cache_flag = 0, backward_flag = 0, mirrors_flag = 0, replace_flag = 0, copt, writeback_flag = 0, exchange_flag=0, row_major_chunking_flag = 0;
  int c_size=4096, o_size = 0;
  char i_file[1024]="/global/cscratch1/sd/dbin/de-test-all-osts/sc18/syn-conv-16G.h5p";
  char o_file[1024]="/global/cscratch1/sd/dbin/de-test-all-osts/sc18/syn-conv-16G-output.h5p";
  char t_file[1024]="/global/cscratch1/sd/dbin/de-test-all-osts/sc18/syn-conv-16G-inter.h5p";
  char  group[1024]="/testg";
  char dataset[1024]="/testg/testd";
  int f_value = 0;

  while ((copt = getopt (argc, argv, "cbmrwo:i:g:d:t:s:h:f:e:j")) != -1)
    switch (copt)
    {
      case 'j':
        row_major_chunking_flag = 1;
        break;	
      case 'e':
	exchange_flag = atoi(optarg);
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

  // set up the chunk size and the overlap size
  std::vector<int> chunk_size;    chunk_size.push_back(c_size);     chunk_size.push_back(c_size);
  std::vector<int> overlap_size;  overlap_size.push_back(o_size);   overlap_size.push_back(o_size);
  std::vector<int> striping_size; striping_size.push_back(2);  striping_size.push_back(2);

   if(exchange_flag == 0){
       //Orginal data set
       Array<float, std::vector<float>> *X = new Array<float, std::vector<float>>(AU_NVS,  AU_HDF5, i_file, group, dataset, chunk_size, overlap_size);
       Array<std::vector<float>> *Y = new Array<std::vector<float>>(AU_COMPUTED, AU_HDF5, o_file, group, dataset, chunk_size, overlap_size);

       if(cache_flag == 1) Y->EnableCache();
       X->SetApplyStripSize(striping_size);
  
       X->SetApplyFilters(FILTERS);

        X->Apply(ConvUDF, Y);  

  	X->ReportTime();
  	if(cache_flag == 1) {
    		//Y->Nonvolatile();
    		Array<float> *Y_ReGrided = Y->Regrid<float>();
    		if(writeback_flag == 1) Y_ReGrided->EnableApplyWriteback();
    		Y_ReGrided->Apply(ReluUDF, NULL);
    		Y_ReGrided->Nonvolatile();
    		Y_ReGrided->ReportTime();
    		delete Y_ReGrided;
  	}else{
    		std::vector<int> csr;  csr.push_back(c_size/2);   csr.push_back(c_size/2);  csr.push_back(FILTERS);
    		std::vector<int> osr;  osr.push_back(0);   osr.push_back(0);  csr.push_back(0);
    		Array<float> *YY = new Array<float>(AU_NVS, AU_HDF5, o_file, group, dataset, csr, osr);
    		Array<float> *ZZ = new Array<float>(AU_COMPUTED, AU_HDF5, t_file, group, dataset, csr, osr);
    		YY->Apply(ReluUDF, ZZ);
    		YY->ReportTime();
    		delete YY;
    		delete ZZ;	    
       } 
     delete X;
     delete Y;
  }else if (exchange_flag == 1){
    std::vector<int> csr;  csr.push_back(c_size);   csr.push_back(c_size); 
    std::vector<int> osr;  osr.push_back(o_size);   osr.push_back(o_size); 
    std::vector<float> mirrors; mirrors.push_back(f_value);

    Array<float> *XX = new Array<float>(AU_NVS, AU_HDF5, i_file, group, dataset, chunk_size, overlap_size);
    Array<float> *YY = new Array<float>(AU_COMPUTED, AU_HDF5, t_file, group, dataset, chunk_size, overlap_size);
    Array<float> *ZZ = new Array<float>(AU_COMPUTED, AU_HDF5, o_file, group, dataset, chunk_size, overlap_size);
    if(mirrors_flag == 1) XX->SetMirrorValue(mirrors);
    if(cache_flag   == 1) YY->EnableCache();
    if(cache_flag   == 1) ZZ->EnableCache();
    XX->Apply(ReluUDF2D, YY);
    if( backward_flag == 1){
	YY->SetApplyDirection(1);
    }
    if( mirrors_flag == 0 ) {
      YY->Apply(ReluUDF2D, ZZ);
    }
    XX->ReportTime();
    YY->ReportTime();
    
    delete YY;
    delete ZZ;	    
 
  }else{
    //c_size = 16384; o_size = 0;	  
    std::vector<int> csr;  csr.push_back(c_size);   csr.push_back(c_size); 
    std::vector<int> osr;  osr.push_back(o_size);   osr.push_back(o_size); 

    Array<float> *XX = new Array<float>(AU_NVS, AU_HDF5, i_file, group, dataset, csr, osr);
    Array<float> *YY = new Array<float>(AU_COMPUTED, AU_HDF5, t_file, group, dataset, csr, osr);
    if(row_major_chunking_flag) XX->EnableAutoRowMajorChunk();
    XX->Apply(ConvUDF2D, YY);
    XX->ReportTime();
    //YY->ReportTime();
    delete XX;
    delete YY;	    
  }
   AU_Finalize();
   
  return 0; 
}


