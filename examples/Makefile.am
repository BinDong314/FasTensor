AUTOMAKE_OPTIONS=gnu

AM_CPPFLAGS = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/third_party  @ARRAYUDF_CPPFLAGS@   -ldl 
AM_CFLAGS   = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/third_party  @ARRAYUDF_CFLAGS@    -ldl 
AM_CXXFLAGS = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/third_party  @ARRAYUDF_CXXFLAGS@    -ldl  
AM_LDFLAGS  = @ARRAYUDF_LIBS@  -all-static  -ldl 


bin_PROGRAMS=  au_test_data_generator au_example_basic au_example_vector au_example_array


au_test_data_generator_SOURCES = au_test_data_generator.cpp
au_test_data_generator_DEPENDENCIES=../src/libArrayUDF.la
au_test_data_generator_LDADD=../src/libArrayUDF.la


au_example_basic_SOURCES=au_example_basic.cpp
au_example_basic_DEPENDENCIES=../src/libArrayUDF.la
au_example_basic_LDADD=../src/libArrayUDF.la


au_example_vector_SOURCES=au_example_vector.cpp
au_example_vector_DEPENDENCIES=../src/libArrayUDF.la
au_example_vector_LDADD=../src/libArrayUDF.la


au_example_array_SOURCES=au_example_array.cpp
au_example_array_DEPENDENCIES=../src/libArrayUDF.la
au_example_array_LDADD=../src/libArrayUDF.la





