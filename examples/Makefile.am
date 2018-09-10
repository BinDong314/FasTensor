AUTOMAKE_OPTIONS=gnu

AM_CPPFLAGS = -I$(top_srcdir)/src/  @ARRAYUDF_CPPFLAGS@  -std=c++11 -std=c++0x
AM_CFLAGS   = -I$(top_srcdir)/src/ @ARRAYUDF_CFLAGS@  -std=c++11 -std=c++0x
AM_CXXFLAGS = -I$(top_srcdir)/src/ @ARRAYUDF_CFLAGS@  -std=c++11 -std=c++0x
AM_LDFLAGS  = @ARRAYUDF_LIBS@  -all-static -std=c++11 -std=c++0x

#-fopenmp -openmp -DENABLE_OPENMP -g -O0 -std=c++11 -std=c++0x
#bin_PROGRAMS=conv  fake-hdf5-data udf-example-new inter-xyz conv
#bin_PROGRAMS= fake-hdf5-data udf-example udf-example-new ccl-pc-new  inter hist  udf-test 
#bin_PROGRAMS= fake-hdf5-data ccl-pc-new conv inter-xyz conv-micro-test vpic-gradient-decent vpic-xyz-global-position
bin_PROGRAMS=  ccl-pc-new fake-hdf5-data udf-example-new


vpic_xyz_global_position_SOURCES=vpic-xyz-global-position.cpp
vpic_xyz_global_position_DEPENDENCIES=../src/libArrayUDF.la
vpic_xyz_global_position_LDADD=../src/libArrayUDF.la

vpic_gradient_decent_SOURCES=vpic-gradient-decent.cpp
vpic_gradient_decent_DEPENDENCIES=../src/libArrayUDF.la
vpic_gradient_decent_LDADD=../src/libArrayUDF.la

fake_hdf5_data_SOURCES=fake-HDF5-data.c
fake_hdf5_data_DEPENDENCIES=../src/libArrayUDF.la
fake_hdf5_data_LDADD=../src/libArrayUDF.la

#udf_example_SOURCES=udf-example.cpp
#udf_example_DEPENDENCIES=../src/libArrayUDF.la
#udf_example_LDADD=../src/libArrayUDF.la

udf_example_new_SOURCES=udf-example-new.cpp
udf_example_new_DEPENDENCIES=../src/libArrayUDF.la
udf_example_new_LDADD=../src/libArrayUDF.la


#ccl_sq_SOURCES=ccl-sq.cpp
#ccl_sq_DEPENDENCIES=../src/libArrayUDF.la
#ccl_sq_LDADD=../src/libArrayUDF.la

#ccl_sq_new_SOURCES=ccl-sq-new.cpp
#ccl_sq_new_DEPENDENCIES=../src/libArrayUDF.la
#ccl_sq_new_LDADD=../src/libArrayUDF.la


#ccl_pc_SOURCES=ccl-pc.cpp
#ccl_pc_DEPENDENCIES=../src/libArrayUDF.la
#ccl_pc_LDADD=../src/libArrayUDF.la


ccl_pc_new_SOURCES=ccl-pc-new.cpp
ccl_pc_new_DEPENDENCIES=../src/libArrayUDF.la
ccl_pc_new_LDADD=../src/libArrayUDF.la


#inter_SOURCES=inter.cpp
#inter_DEPENDENCIES=../src/libArrayUDF.la
#inter_LDADD=../src/libArrayUDF.la


inter_xyz_SOURCES=inter-xyz.cpp
inter_xyz_DEPENDENCIES=../src/libArrayUDF.la
inter_xyz_LDADD=../src/libArrayUDF.la


#hist_SOURCES=hist.cpp
#hist_DEPENDENCIES=../src/libArrayUDF.la
#hist_LDADD=../src/libArrayUDF.la


conv_SOURCES=conv.cpp
conv_DEPENDENCIES=../src/libArrayUDF.la
conv_LDADD=../src/libArrayUDF.la

conv_micro_test_SOURCES=conv-micro-test.cpp
conv_micro_test_DEPENDENCIES=../src/libArrayUDF.la
conv_micro_test_LDADD=../src/libArrayUDF.la


#udf_test_SOURCES=udf-test.cpp
#udf_test_DEPENDENCIES=../src/libArrayUDF.la
#udf_test_LDADD=../src/libArrayUDF.la



