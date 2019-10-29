AUTOMAKE_OPTIONS=gnu

AM_CPPFLAGS = @ARRAYUDF_CPPFLAGS@  -I./third_part
AM_CFLAGS = @ARRAYUDF_CFLAGS@  -I./third_part
AM_CXXFLAGS = @ARRAYUDF_CXXFLAGS@    -I./third_part 
AM_LDFLAGS = @ARRAYUDF_LIBS@   -all-static     -I./third_part

lib_LTLIBRARIES=libArrayUDF.la
libArrayUDF_la_SOURCES=au_array.h au_endpoint.h	au_stencil.h au_attribute.h	au_endpoint_hdf5.h	au_utility.h
libArrayUDF_la_LDFLAGS=-version-info 0:0:0  -all-static
include_HEADERS=au_array.h	au_endpoint.h au_stencil.h au_attribute.h au_endpoint_hdf5.h au_utility.h

#include_HEADERS
#pkginclude_HEADERS
#nobase_pkginclude_HEADERS=array_udf_array.h array_udf_stencil.h array_udf_io.h utility.h iarray/g/iarray-helper.h iarray/g/iarray.h array_udf.h   array_udf_h5.h   array_udf_attribute.h  array_udf_iarray.h  array_udf_pyoffset.h
#BUILT_SOURCES = $(builddir)/ArrayUDF.py $(builddir)/ArrayUDF_wrap.c:ArrayUDF.i
#	$(SWIG) $(AX_SWIG_PYTHON_OPT) -o $@ $<

#python_PYTHON = $(builddir)/ArrayUDF.py
#pyexec_LTLIBRARIES 	

if BUILD_PYTHON
##CXXFLAGS+="-DPY_ARRAYUDF"
## This is the Python module interfacing to libexample
# example.py and example_wrap.c are created by SWIG from src/example.i
# Put these in the build tree
BUILT_SOURCES = ArrayUDF.py ArrayUDF_wrap.cxx
# tell Make how to create the generated SWIG files
ArrayUDF.py ArrayUDF_wrap.cxx: ArrayUDF.i
	$(SWIGCC) -c++ -python ArrayUDF.i 

# Ask autotools to install the generated Python file
python_PYTHON = ArrayUDF.py

# This is the Python module
pyexec_LTLIBRARIES = _ArrayUDF.la
# It is composed of the generated C file
_ArrayUDF_la_SOURCES = ArrayUDF_wrap.cxx array_udf_array.h array_udf_stencil.h array_udf_io.h array_udf_pyoffset.h utility.h
# -module tells libtool we don't need a 'lib' prefix
_ArrayUDF_la_LDFLAGS = -module -avoid-version ${PYTHON_LDFLAGS}  -shared $(PYTHON_LIBS) @ARRAYUDF_LIBS@ 
_ArrayUDF_la_CXXFLAGS= -DPY_ARRAYUDF=1 $(PYTHON_CPPFLAGS) $(PYTHON_CFLAGS)   @ARRAYUDF_CPPFLAGS@ 
_ArrayUDF_la_CFLAGS  = -DPY_ARRAYUDF=1 $(PYTHON_CPPFLAGS) $(PYTHON_CFLAGS)   @ARRAYUDF_CFLAGS@


clean-local:
	rm ArrayUDF.py ArrayUDF_wrap.cxx
endif


