AUTOMAKE_OPTIONS=gnu

AM_CPPFLAGS = @ARRAYUDF_CPPFLAGS@  -I./third_part
AM_CFLAGS = @ARRAYUDF_CFLAGS@  -I./third_part
AM_CXXFLAGS = @ARRAYUDF_CXXFLAGS@    -I./third_part 
AM_LDFLAGS = @ARRAYUDF_LIBS@   -all-static     -I./third_part

lib_LTLIBRARIES=libArrayUDF.la
libArrayUDF_la_SOURCES=au.h au.cpp au_array.h au_array.cpp au_endpoint.h au_endpoint.cpp au_stencil.h au_type.h au_endpoint_hdf5.h au_endpoint_hdf5.cpp au_endpoint_factory.h au_utility.cpp au_utility.h 
libArrayUDF_la_LDFLAGS=-version-info 0:0:0  -all-static
include_HEADERS=au_array.h	au_endpoint.h au_stencil.h au_type.h au_endpoint_hdf5.h au_utility.h



