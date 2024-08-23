# Include paths
AM_CPPFLAGS = $(DOCOPT_CPPFLAGS)

# Library paths
AM_LDFLAGS = $(DOCOPT_LDFLAGS)
bin_PROGRAMS = ftstream

ftstream_SOURCES = ft_stream_gen.cpp dataset_types.cpp dataset_utils.cpp fastflush.cpp


# Libraries to link against. 
# @TODO: add libtool support, so we don't need to manually add the libdocopt library
ftstream_LDADD = $(top_srcdir)/src/third-party/docopt/build/lib/libdocopt.a

# C++ flags
AM_CXXFLAGS = -std=c++17
