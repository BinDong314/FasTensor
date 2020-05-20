AUTOMAKE_OPTIONS=gnu

AM_CPPFLAGS = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/third_part  @ARRAYUDF_CPPFLAGS@   -ldl 
AM_CFLAGS   = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/third_part  @ARRAYUDF_CFLAGS@    -ldl 
AM_CXXFLAGS = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/third_part  @ARRAYUDF_CXXFLAGS@    -ldl 
AM_LDFLAGS  = -Wl,--start-group @ARRAYUDF_LIBS@  -all-static  -ldl 


bin_PROGRAMS=   au_test_data_generator \
                au_example_hello_world \
                au_example_vector au_example_array \
                au_example_vds \
                au_example_1f1p \
                au_example_cache \
                au_example_reduce \
                au_example_reduce2D_1f1p 
                

LDADD=../src/libArrayUDF.la
DEPENDENCIES=../src/libArrayUDF.la

au_test_data_generator_SOURCES = au_test_data_generator.cpp
au_example_hello_world_SOURCES=au_example_hello_world.cpp
au_example_vector_SOURCES=au_example_vector.cpp
au_example_array_SOURCES=au_example_array.cpp
au_example_vds_SOURCES=au_example_vds.cpp
au_example_1f1p_SOURCES=au_example_1f1p.cpp
au_example_cache_SOURCES=au_example_cache.cpp
au_example_reduce_SOURCES=au_example_reduce.cpp
au_example_reduce2D_1f1p_SOURCES=au_example_reduce2D_1f1p.cpp

