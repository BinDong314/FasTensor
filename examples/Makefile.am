AUTOMAKE_OPTIONS=gnu

AM_CPPFLAGS = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/third_party  @ARRAYUDF_CPPFLAGS@   -ldl 
AM_CFLAGS   = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/third_party  @ARRAYUDF_CFLAGS@    -ldl 
AM_CXXFLAGS = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/third_party  @ARRAYUDF_CXXFLAGS@    -ldl  
AM_LDFLAGS  = @ARRAYUDF_LIBS@  -all-static  -ldl 


#bin_PROGRAMS= ccl-pc-new  fake-hdf5-data udf-example-new udf-example conv2d ft-example
bin_PROGRAMS= ft-example fake-hdf5-data


fake_hdf5_data_SOURCES=fake-HDF5-data.c
fake_hdf5_data_DEPENDENCIES=../src/libArrayUDF.la
fake_hdf5_data_LDADD=../src/libArrayUDF.la

ft_example_SOURCES=ft-example.cpp
ft_example_DEPENDENCIES=../src/libArrayUDF.la
ft_example_LDADD=../src/libArrayUDF.la

udf_example_SOURCES=udf-example.cpp
udf_example_DEPENDENCIES=../src/libArrayUDF.la
udf_example_LDADD=../src/libArrayUDF.la

udf_example_new_SOURCES=udf-example-new.cpp
udf_example_new_DEPENDENCIES=../src/libArrayUDF.la
udf_example_new_LDADD=../src/libArrayUDF.la

ccl_pc_new_SOURCES=ccl-pc-new.cpp
ccl_pc_new_DEPENDENCIES=../src/libArrayUDF.la
ccl_pc_new_LDADD=../src/libArrayUDF.la

conv2d_SOURCES=conv-2d.cpp
conv2d_DEPENDENCIES=../src/libArrayUDF.la
conv2d_LDADD=../src/libArrayUDF.la





