AUTOMAKE_OPTIONS=gnu

AM_CPPFLAGS = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/third_party  @ARRAYUDF_CPPFLAGS@   -ldl 
AM_CFLAGS   = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/third_party  @ARRAYUDF_CFLAGS@    -ldl 
AM_CXXFLAGS = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/third_party  @ARRAYUDF_CXXFLAGS@    -ldl  
AM_LDFLAGS  = @ARRAYUDF_LIBS@  -all-static  -ldl 


bin_PROGRAMS= au_example au_test_data.cpp


au_test_data_SOURCES=au_test_data.cpp
au_test_data_DEPENDENCIES=../src/libArrayUDF.la
au_test_data_LDADD=../src/libArrayUDF.la

au_example_SOURCES=au_example.cpp
au_example_DEPENDENCIES=../src/libArrayUDF.la
au_example_LDADD=../src/libArrayUDF.la






