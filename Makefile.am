AUTOMAKE_OPTIONS = gnu
ACLOCAL_AMFLAGS=-I m4
#if DEBUG
#if TRUE
#ARRAYUDF_CPPFLAGS += -g -O0 -std=c++11 -std=c++0x
#ARRAYUDF_CFLAGS   += -g -O0 
#ARRAYUDF_CXXFLAGS += -g -O0 -std=c++11 -std=c++0x
#ARRAYUDF_LDFLAGS  += -g -O0 -std=c++11 -std=c++0x
#else
#AM_CPPFLAGS = -O2 -std=c++11 -std=c++0x
#AM_CFLAGS   = -O2 
#AM_CXXFLAGS = -O2 -std=c++11 -std=c++0x
#AM_LDFLAGS  = -O2 -std=c++11 -std=c++0x
#endif


if OPENMP
#if TRUE
ARRAYUDF_CPPFLAGS +=  -fopenmp -openmp -DENABLE_OPENMP
ARRAYUDF_CFLAGS   +=  -fopenmp -openmp -DENABLE_OPENMP
ARRAYUDF_CXXFLAGS +=  -fopenmp -openmp -DENABLE_OPENMP
ARRAYUDF_LDFLAGS  +=  -fopenmp -openmp -DENABLE_OPENMP
endif

#if BUILD_PYTHON
#SUBDIRS=src
#else
SUBDIRS=src examples
#endif

