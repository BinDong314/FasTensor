AUTOMAKE_OPTIONS=gnu

AM_CPPFLAGS = @ARRAYUDF_CPPFLAGS@  -I./third_part -lcurl -lgssapi_krb5
AM_CFLAGS = @ARRAYUDF_CFLAGS@  -I./third_part -lcurl -lgssapi_krb5
AM_CXXFLAGS = @ARRAYUDF_CXXFLAGS@    -I./third_part  -lcurl -lgssapi_krb5
AM_LDFLAGS = @ARRAYUDF_LIBS@     -I./third_part -lcurl -lgssapi_krb5

lib_LTLIBRARIES=libFastensor.la
libFastensor_la_SOURCES=ft.h ft.cpp \
                       ft_array.h ft_array.cpp \
                       ft_endpoint.h ft_endpoint.cpp \
                       ft_stencil.h \
                       ft_type.h ft_type.cpp \
                       ft_endpoint_hdf5.h ft_endpoint_hdf5.cpp \
                       ft_endpoint_factory.h \
                       ft_utility.cpp ft_utility.h \
                       ft_endpoint_dir.cpp ft_endpoint_dir.h \
                       ft_endpoint_memory.cpp ft_endpoint_memory.h \
                       ft_endpoint_adios.h ft_endpoint_adios.cpp \
                       ft_endpoint_pnetcdf.h ft_endpoint_pnetcdf.cpp \
                       ft_endpoint_tdms.h  ft_endpoint_tdms.cpp \
                       ft_endpoint_binary.h ft_endpoint_binary.cpp \
                       ft_endpoint_csv.h ft_endpoint_csv.cpp \
                       ft_endpoint_dir_stream.h ft_endpoint_dir_stream.cpp \
                       ft_endpoint_rabbitmq.h ft_endpoint_rabbitmq.cpp \
                       ft_array_iterator.h ft_array_iterator.cpp \
                       ft_array_view_access.h ft_array_view_access.cpp \
                       ft_endpoint_rabbitmq_restapi.h ft_endpoint_rabbitmq_restapi.cpp \
                       ft_array_transpose.h \
                       ft_utility_macro.h \
                       ft_output_vector.h ft_output_vector.cpp \
                       ft_xdmf.h \
                       ft_xdmf.cpp \
                       ft_chunk_scheduling.h

libFastensor_la_LDFLAGS=-version-info 0:0:0
include_HEADERS=ft_array.h	ft_endpoint.h ft_stencil.h ft_type.h ft_endpoint_hdf5.h \
                ft_utility.h \
                third_part/cista.h\
                third_part/cista_au_extended_ptr_tuple.h \
                third_part/cista_au_extended_tuple.h \
                third_part/cista_au_extended_arity.h \
                ft_endpoint_dir.h \
                ft_endpoint_memory.h ft.h ft_endpoint_factory.h ft_local_mirror.h  ft_mpi.h ft_merge.h \
                ft_array_iterator.h \
                ft_endpoint_adios.h \
                ft_endpoint_pnetcdf.h \
                ft_endpoint_tdms.h \
                ft_endpoint_binary.h \
                ft_array_view_access.h \
                ft_array_transpose.h \
                ft_utility_macro.h \
                ft_output_vector.h \
                ft_endpoint_csv.h \
                ft_xdmf.h \
                ft_vis.h \
                ft_chunk_scheduling.h \
                ft_endpoint_rabbitmq.h \
                ft_endpoint_dir_stream.h \
                ft_endpoint_rabbitmq_restapi.h




