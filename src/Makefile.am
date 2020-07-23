AUTOMAKE_OPTIONS=gnu

AM_CPPFLAGS = @ARRAYUDF_CPPFLAGS@  -I./third_part
AM_CFLAGS = @ARRAYUDF_CFLAGS@  -I./third_part
AM_CXXFLAGS = @ARRAYUDF_CXXFLAGS@    -I./third_part  
AM_LDFLAGS = @ARRAYUDF_LIBS@   -all-static     -I./third_part

lib_LTLIBRARIES=libArrayUDF.la
libArrayUDF_la_SOURCES=au.h au.cpp \
                       au_array.h au_array.cpp \
                       au_endpoint.h au_endpoint.cpp \
                       au_stencil.h \
                       au_type.h au_type.cpp \
                       au_endpoint_hdf5.h au_endpoint_hdf5.cpp \
                       au_endpoint_factory.h \
                       au_utility.cpp au_utility.h \
                       au_endpoint_dir.cpp au_endpoint_dir.h \
                       au_endpoint_memory.cpp au_endpoint_memory.h \
                       au_endpoint_adios.h au_endpoint_adios.cpp \
                       au_endpoint_pnetcdf.h au_endpoint_pnetcdf.cpp \
                       au_endpoint_tdms.h  au_endpoint_tdms.cpp \
                       au_endpoint_binary.h au_endpoint_binary.cpp \
                       au_array_iterator.h au_array_iterator.cpp \
                       au_array_view_access.h au_array_view_access.cpp 

libArrayUDF_la_LDFLAGS=-version-info 0:0:0  -all-static
include_HEADERS=au_array.h	au_endpoint.h au_stencil.h au_type.h au_endpoint_hdf5.h \
                au_utility.h third_part/cista.h third_part/cista_au_extended_ptr_tuple.h \
                third_part/cista_au_extended_tuple.h third_part/cista_au_extended_arity.h au_endpoint_dir.h \
                au_endpoint_memory.h au.h au_endpoint_factory.h au_local_mirror.h  au_mpi.h au_merge.h \
                au_array_iterator.h \
                au_endpoint_adios.h \
                au_endpoint_pnetcdf.h \
                au_endpoint_tdms.h \
                au_endpoint_binary.h \
                au_array_view_access.h \
                au_array_iterator.h



