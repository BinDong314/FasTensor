AUTOMAKE_OPTIONS=gnu

AM_CPPFLAGS = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/third_part  @ARRAYUDF_CPPFLAGS@   -ldl 
AM_CFLAGS   = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/third_part  @ARRAYUDF_CFLAGS@    -ldl 
AM_CXXFLAGS = -I$(top_srcdir)/src/ -I$(top_srcdir)/src/third_part  @ARRAYUDF_CXXFLAGS@    -ldl 

AM_LDFLAGS  = @ARRAYUDF_LIBS@  -all-static  -ldl 
#AM_LDFLAGS  = -Wl,--start-group @ARRAYUDF_LIBS@  -all-static  -ldl 

LDADD=../src/libFastensor.la @ARRAYUDF_LIBS@  -ldl
DEPENDENCIES=../src/libFastensor.la

bin_PROGRAMS=   ft_test_data_generator \
                ft_example_hello_world \
                ft_example_vector \
                ft_example_array \
                ft_example_vds \
                ft_example_1f1p \
                ft_example_reduce \
                ft_example_reduce2D_1f1p \
                ft_example_adios \
                ft_example_pnetcdf \
                ft_example_simple \
                ft_example_kf1p \
                ft_example_vector_2d \
                ft_example_padding \
                ft_example_tag \
                ft_example_chunk_overlap \
                ft_example_cache \
                ft_example_cmpd \
                ft_example_base \
                ft_example_streaming


               



ft_test_data_generator_SOURCES = ft_test_data_generator.cpp
ft_example_hello_world_SOURCES=ft_example_hello_world.cpp
ft_example_vector_SOURCES=ft_example_vector.cpp
ft_example_array_SOURCES=ft_example_array.cpp
ft_example_vds_SOURCES=ft_example_vds.cpp
ft_example_1f1p_SOURCES=ft_example_1f1p.cpp
ft_example_cache_SOURCES=ft_example_cache.cpp
ft_example_reduce_SOURCES=ft_example_reduce.cpp
ft_example_reduce2D_1f1p_SOURCES=ft_example_reduce2D_1f1p.cpp
ft_example_adios_SOURCES=ft_example_adios.cpp
ft_example_pnetcdf_SOURCES=ft_example_pnetcdf.cpp
ft_example_simple_SOURCES=ft_example_simple.cpp
ft_example_kf1p_SOURCES=ft_example_kf1p.cpp
ft_example_vector_2d_SOURCES=ft_example_vector_2d.cpp
ft_example_padding_SOURCES=ft_example_padding.cpp
ft_example_tag_SOURCES=ft_example_tag.cpp
ft_example_chunk_overlap_SOURCES=ft_example_chunk_overlap.cpp
ft_example_cmpd_SOURCES=ft_example_cmpd.cpp
ft_example_base_SOURCES=ft_example_base.cpp
ft_example_streaming_SOURCES=ft_example_streaming.cpp
