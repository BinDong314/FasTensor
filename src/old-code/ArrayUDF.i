/**
 *ArrayUDF Copyright (c) 2017, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy).  All rights reserved.
 *
 *If you have questions about your rights to use or distribute this software, please contact Berkeley Lab's Innovation & Partnerships Office at  IPO@lbl.gov.
 *
 * NOTICE. This Software was developed under funding from the U.S. Department of Energy and the U.S. Government consequently retains certain rights. As such, the U.S. Government has been granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software to reproduce, distribute copies to the public, prepare derivative works, and perform publicly and display publicly, and to permit other to do so. 
 *
 */

/**
 *
 * Email questions to {dbin, kwu, sbyna}@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */


%module ArrayUDF

%typemap(in) (float (*UDF)(void *)){
  $1 = (float (*)(void *))PyInt_AsLong($input);
#ifdef DEBUG
  printf("Convert UDF function type. \n");
#endif
}

%typemap(in) (void *){
  $1 = (void *)PyInt_AsLong($input);
#ifdef DEBUG
  printf("Convert Void * argument for Offset function. \n");
#endif
}

%typemap(in) (int){
  $1 = (int)PyInt_AsLong($input);
#ifdef DEBUG  
  printf("Convert Int argument for Offset function. \n");
#endif  
 }


%define PY_ARRAYUDF 1 /*Define micro to select Apply function in Array Class*/
%enddef

%{
#include "array_udf_pyoffset.h"
#include "array_udf_array.h"
#include "array_udf_stencil.h"
#include "array_udf_io.h"
#include "utility.h"
%}

%include "std_vector.i"

namespace std
{
%template(IntVector)    vector<int>;
%template(DoubleVector) vector<double>;
%template(FloatVector)  vector<float>;
}

%include "std_string.i"
%include "carrays.i"
%array_class(int, intArray);
%array_class(float, floatArray);

%include "cpointer.i"
%pointer_functions(int,   intp);
%pointer_functions(float, floatp);

%include "array_udf_array.h"
%include "array_udf_stencil.h"
%include "array_udf_io.h"
%include "utility.h"  
%include "array_udf_pyoffset.h"

%template(Datai) Data<int>;
%template(Dataf) Data<float>;
%template(Datad) Data<double>;

%template(Stencilf) Stencil<float>;
%template(Stencild) Stencil<double>;
%template(Stencili) Stencil<int>;

%template(Arrayf) Array<float>;
%template(Arrayd) Array<double>;
%template(Arrayi) Array<int>;

%template(O1f) O1<float>;
%template(O1i) O1<int>;
%template(O1d) O1<double>;

%template(O2f) O2<float>;
%template(O2i) O2<int>;
%template(O2d) O2<double>;

%template(O3f) O3<float>;
%template(O3i) O3<int>;
%template(O3d) O3<double>;




%pythoncode
%{
import ctypes
      
py_callback_typef = ctypes.CFUNCTYPE(ctypes.c_float, ctypes.c_void_p)

def PyApply(udf, A, B):
    f = py_callback_typef(udf)
    f_ptr = ctypes.cast(f, ctypes.c_void_p).value
    A.Apply(f_ptr, B)
%}

