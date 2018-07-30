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


#include "array_udf_stencil.h"


//This file is only used when PyArrayUDF is defined.
//  Define the offset function to support Python interface
//  We define these functions for 1D, 2D, 3D data seperately
//  The template is used to support different types.
//  See ArrayUDF.i for the mapped functioin names in Python.
//  The mapped functions can be
//    O1f -> for float data type
//    O1i -> for int data type

//Offset function for 1D data
template<typename T>
T O1(void *stencil, int i1){
  Stencil<T> *stencil_pointer =  (Stencil<T> *)(stencil);
  return stencil_pointer->operator()(i1);
}

//Offset function for 2D data
template<typename T>
T O2(void *stencil, int i1, int i2){
  Stencil<T> *stencil_pointer =  (Stencil<T> *)(stencil);
  return stencil_pointer->operator()(i1, i2);
}

//Offset function for 3D data
template<typename T>
T O3(void *stencil, int i1, int i2, int i3){
  Stencil<T> *stencil_pointer =  (Stencil<T> *)(stencil);
  return stencil_pointer->operator()(i1, i2, i3);
}

//Offset function for 4D data
template<typename T>
T O3(void *stencil, int i1, int i2, int i3, int i4){
  Stencil<T> *stencil_pointer =  (Stencil<T> *)(stencil);
  return stencil_pointer->operator()(i1, i2, i3, i4);
}


