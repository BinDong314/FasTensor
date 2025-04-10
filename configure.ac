#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ArrayUDF], [0.0.1], [dbin@lbl.gov])
AC_CONFIG_SRCDIR([src/ft_array.h])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([subdir-objects])

AC_CONFIG_MACRO_DIR([m4])

LT_INIT 

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

AC_CONFIG_MACRO_DIRS([m4])
# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])
# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC


AC_SUBST(ARRAYUDF_CFLAGS)
AC_SUBST(ARRAYUDF_CPPFLAGS)
AC_SUBST(ARRAYUDF_CXXFLAGS)
AC_SUBST(ARRAYUDF_LIBS)
AC_SUBST(ARRAYUDF_LDFLAGS)

AC_ARG_WITH([hdf5],
[AS_HELP_STRING([--with-hdf5=DIR],
[use DIR as the root directory for HDF5 include and lib])],
[HDF5_HOME="$withval"],
[AC_MSG_ERROR([HDF5 must be specified using --with-hdf5 option])]
)

AC_MSG_CHECKING([for HDF5 at $HDF5_HOME])
if test -d "$HDF5_HOME"   && test -d "$HDF5_HOME/include"  && test -d "$HDF5_HOME/lib"; then
ARRAYUDF_CFLAGS="$ARRAYUDF_CFLAGS       -I$HDF5_HOME/include"
ARRAYUDF_CPPFLAGS="$ARRAYUDF_CPPFLAGS   -I$HDF5_HOME/include"
ARRAYUDF_LIBS="$ARRAYUDF_LIBS           -L$HDF5_HOME/lib -lhdf5 -lhdf5_hl -lz"
AC_MSG_RESULT([yes])	    
else
AC_MSG_ERROR([no: HDF5 directory not found!])
fi


# Handle --with-rabbitmq option
AC_ARG_WITH([rabbitmq],
    [AS_HELP_STRING([--with-rabbitmq=DIR],
    [use DIR as the root directory for RabbitMQ-C include and lib])],
    [RABBITMQ_HOME="$withval"],  # Set the value of RABBITMQ_HOME if specified
    [RABBITMQ_HOME=""])          # If not specified, set to an empty string

# Check for RabbitMQ only if specified
if test -n "$RABBITMQ_HOME"; then
    AC_MSG_CHECKING([for RabbitMQ at $RABBITMQ_HOME])
    if test -d "$RABBITMQ_HOME" && test -d "$RABBITMQ_HOME/include" && test -d "$RABBITMQ_HOME/lib"; then
        ARRAYUDF_CFLAGS="$ARRAYUDF_CFLAGS -I$RABBITMQ_HOME/include -DHAS_RABBITMQ_END_POINT"
        ARRAYUDF_CPPFLAGS="$ARRAYUDF_CPPFLAGS -I$RABBITMQ_HOME/include -DHAS_RABBITMQ_END_POINT"
        ARRAYUDF_LIBS="$ARRAYUDF_LIBS -L$RABBITMQ_HOME/lib -lrabbitmq"
        AC_MSG_RESULT([yes])
    else
        AC_MSG_ERROR([RabbitMQ directory not found in $RABBITMQ_HOME!])
    fi
else
    AC_MSG_RESULT([RabbitMQ not requested])
fi

# Check if we need RabbitMQ REST API support
AC_ARG_ENABLE([rabbitmq-restapi],
  [AS_HELP_STRING([--enable-rabbitmq-restapi], [Enable RabbitMQ REST API support])],
  [enable_rabbitmq_restapi=$enableval], [enable_rabbitmq_restapi=no])

if test "x$enable_rabbitmq_restapi" = "xyes"; then
    AC_MSG_NOTICE([RabbitMQ REST API support enabled])

    # Check if libcurl is installed
    AC_CHECK_PROG([CURL], [curl], [yes], [no])

    if test "x$CURL" = "xno"; then
        AC_MSG_NOTICE([libcurl not found. Attempting to download and install libcurl...])
        # Attempt to download and install libcurl
        ./scripts/download_curl.sh
    else
        AC_MSG_NOTICE([libcurl found.])
    fi

    # Check if nlohmann/json.hpp exists in src directory
    AC_CHECK_FILE([src/nlohmann/json.hpp], [], [
        AC_MSG_NOTICE([nlohmann/json.hpp not found. Downloading...])
        ./scripts/download_json.sh
    ])

    # Check if OpenSSL headers are available
    AC_CHECK_HEADER([openssl/ssl.h], [AC_MSG_RESULT([found])], [
        AC_MSG_RESULT([not found])
        AC_MSG_NOTICE([OpenSSL not found. Running installation script...])

        # Run the OpenSSL installation script
        ./scripts/download_openssl.sh

        # Check again after running the script
        #AC_CHECK_HEADER([openssl/ssl.h], [AC_MSG_RESULT([found])], [
        #    AC_MSG_ERROR([OpenSSL is still not found after running the installation script.])
        #])
    ])

    # Find OpenSSL libraries and include paths
    AC_CHECK_LIB([ssl], [SSL_library_init])
    AC_CHECK_LIB([crypto], [CRYPTO_num_locks])

    OPENSSL_CFLAGS="-I$(brew --prefix openssl)/include"
    OPENSSL_LIBS="-L$(brew --prefix openssl)/lib -lssl -lcrypto"

    #If not using Homebrew on macOS, use default paths
    #if test "$OS" != "Darwin"; then
    #    OPENSSL_CFLAGS="-I/usr/include/openssl"
    #    OPENSSL_LIBS="-L/usr/lib -lssl -lcrypto"
    #fi

    AC_SUBST([OPENSSL_CFLAGS])
    AC_SUBST([OPENSSL_LIBS])

    ARRAYUDF_CFLAGS="$ARRAYUDF_CFLAGS $OPENSSL_CFLAGS"
    ARRAYUDF_CPPFLAGS="$ARRAYUDF_CPPFLAGS $OPENSSL_CFLAGS"
    ARRAYUDF_LIBS="$ARRAYUDF_LIBS $OPENSSL_LIBS -lcurl"

    ARRAYUDF_CFLAGS="$ARRAYUDF_CFLAGS   -DHAS_RABBITMQ_RESTAPI_END_POINT"
    ARRAYUDF_CPPFLAGS="$ARRAYUDF_CPPFLAGS -DHAS_RABBITMQ_RESTAPI_END_POINT"
fi

# Handle --with-adios option
AC_ARG_WITH([adios],
[AS_HELP_STRING([--with-adios=DIR],
[use DIR as the root directory for ADIOS include and lib])],
[ADIOS_HOME="$withval"],
[with_adios=no]
)

if test "x$with_adios" != xno; then
  AC_MSG_CHECKING([for ADIOS at $ADIOS_HOME])
  if test -d "$ADIOS_HOME"   && test -d "$ADIOS_HOME/include"  && test -d "$ADIOS_HOME/lib"; then
  ARRAYUDF_CFLAGS="$ARRAYUDF_CFLAGS      -DADIOS2_USE_MPI -isystem $ADIOS_HOME/include -DHAS_ADIOS_END_POINT -I$ADIOS_HOME/include -DADIOS2_USE_MPI"
  ARRAYUDF_CPPFLAGS="$ARRAYUDF_CPPFLAGS  -DADIOS2_USE_MPI -isystem $ADIOS_HOME/include -DHAS_ADIOS_END_POINT -I$ADIOS_HOME/include -DADIOS2_USE_MPI "
  ARRAYUDF_LIBS="$ARRAYUDF_LIBS  $ADIOS_HOME/lib/libadios2_c_mpi.2.5.0.dylib $ADIOS_HOME/lib/libadios2_c.2.5.0.dylib  -DHAS_ADIOS_END_POINT -L$ADIOS_HOME/lib -DADIOS2_USE_MPI"
  AC_MSG_RESULT([yes !])	    
  else
  AC_MSG_ERROR([no: ADIOS directory not found!])
  fi
fi

AC_ARG_WITH([pnetcdf],
[AS_HELP_STRING([--with-pnetcdf=DIR],
[use DIR as the root directory for PnetCDF include and lib])],
[PNETCDF_HOME="$withval"],
[with_pnetcdf=no]
)

if test "x$with_pnetcdf" != xno; then
  AC_MSG_CHECKING([for PnetCDF at $ADIOS_HOME])
  if test -d "$PNETCDF_HOME"   && test -d "$PNETCDF_HOME/include"  && test -d "$PNETCDF_HOME/lib"; then
  ARRAYUDF_CFLAGS="$ARRAYUDF_CFLAGS       -DHAS_PNETCDF_END_POINT  -I$PNETCDF_HOME/include "
  ARRAYUDF_CPPFLAGS="$ARRAYUDF_CPPFLAGS  -DHAS_PNETCDF_END_POINT  -I$PNETCDF_HOME/include "
  ARRAYUDF_LIBS="$ARRAYUDF_LIBS        -DHAS_PNETCDF_END_POINT  -L$PNETCDF_HOME/lib -lpnetcdf"
  AC_MSG_RESULT([yes])	    
  else
  AC_MSG_ERROR([no: PnetCDF directory not found!])
  fi
fi

AC_ARG_WITH([dash],
[AS_HELP_STRING([--with-dash=DIR],
[use DIR as the root directory for DASH include and lib])],
[DASH_HOME="$withval"],
[with_dash=no]
)

if test "x$with_dash" != xno; then
  AC_MSG_CHECKING([for DASH at $DASH_HOME])
  if test -d "$DASH_HOME"   && test -d "$DASH_HOME/include"  && test -d "$DASH_HOME/lib"; then
  ARRAYUDF_CFLAGS="$ARRAYUDF_CFLAGS    -DHAS_DASH_ENDPOINT   -I$DASH_HOME/include -std=c++17 -DDASH_ENABLE_HDF5 -DDASH_MPI_IMPL_ID='mpich'"
  ARRAYUDF_CPPFLAGS="$ARRAYUDF_CPPFLAGS -DHAS_DASH_ENDPOINT  -I$DASH_HOME/include -std=c++17 -DDASH_ENABLE_HDF5 -DDASH_MPI_IMPL_ID='mpich'"
  ARRAYUDF_LIBS="$ARRAYUDF_LIBS        -DHAS_DASH_ENDPOINT   -L$DASH_HOME/lib -ldash-mpi -ldart-mpi -ldart-base -lpthread -std=c++14 -DDASH_ENABLE_HDF5 -DDASH_MPI_IMPL_ID='mpich'"
  AC_MSG_RESULT([yes])	    
  else
  AC_MSG_ERROR([no: DASH directory not found!])
  fi
fi

AC_SUBST(AU_OLDCXX_ENABLED)
AU_OLDCXX_ENABLED=false
AC_ARG_ENABLE([oldcxx], 
[AC_HELP_STRING([--enable-oldcxx],[enable oladcxx(not to use c++17])],
[AU_OLDCXX_ENABLED=true],
[AU_OLDCXX_ENABLED=false])
AM_CONDITIONAL([BUILD_OLDCXX], [test x$AU_OLDCXX_ENABLED = xtrue])


AC_MSG_CHECKING([compile oldcxx])
if test "$AU_OLDCXX_ENABLED" = "true" ; then
ARRAYUDF_CXXFLAGS="$ARRAYUDF_CXXFLAGS    -std=c++11"
AC_MSG_RESULT([yes])	    
else
ARRAYUDF_CXXFLAGS="$ARRAYUDF_CXXFLAGS    -std=c++17"
AC_MSG_RESULT([no])
fi


AC_SUBST(AU_DEBUG_ENABLED)
AU_DEBUG_ENABLED=false
AC_ARG_ENABLE([debug], 
[AC_HELP_STRING([--enable-debug],[enable debug (-g3) ])],
[AU_DEBUG_ENABLED=true],
[AU_DEBUG_ENABLED=false])


AC_MSG_CHECKING([enable debug])
if test "$AU_DEBUG_ENABLED" = "true" ; then
ARRAYUDF_CXXFLAGS="$ARRAYUDF_CXXFLAGS -g3"
CFLAGS+=" -g3"
CXXFLAGS+=" -g3"
AC_MSG_RESULT([yes])	    
else
ARRAYUDF_CXXFLAGS="$ARRAYUDF_CXXFLAGS -O3"
CFLAGS+=" -O3"
CXXFLAGS+=" -O3"
AC_MSG_RESULT([no])
fi



#/Users/dbin/work/soft/boost_1_64_0/build/include
#Python interface 
AC_SUBST(ARRAY_UDF_PYTHON_ENABLED)
ARRAY_UDF_PYTHON_ENABLED=false
AC_ARG_ENABLE([python], 
[AC_HELP_STRING([--enable-python],[enable python model for ArrayUDF])],
[ARRAY_UDF_PYTHON_ENABLED=true],
[ARRAY_UDF_PYTHON_ENABLED=false])
AM_CONDITIONAL([BUILD_PYTHON], [test x$ARRAY_UDF_PYTHON_ENABLED = xtrue])




AC_SUBST(AUPY_CFLAGS)
AC_SUBST(AUPY_CPPFLAGS)
AC_SUBST(AUPY_LIBS)

if test x$ARRAY_UDF_PYTHON_ENABLED = xtrue; then
VALID_PYTHON=
AM_PATH_PYTHON(3.4,[PYTHON_FOUND=true],[PYTHON_FOUND=false])
if test "x$PYTHON_FOUND" = xtrue ; then
   PYTHON_CPPFLAGS=
   PYTHON_LDFLAGS=
   AX_PYTHON_DEVEL
   if test "x$PYTHON" = x ; then
         VALID_PYTHON=no
   else
         VALID_PYTHON=yes
   fi
fi

if test "x$VALID_PYTHON" = xyes ; then
  AC_MSG_NOTICE([Building with Python bindings:])
  AC_MSG_NOTICE([$PYTHON_CPPFLAGS])
  AC_MSG_NOTICE([$PYTHON_LDFLAGS])
  AC_MSG_NOTICE(${PYTHON_LDFLAGS})
fi

# Find swig executable
# http://www.gnu.org/software/autoconf-archive/ax_pkg_swig.html
#AX_PKG_SWIG([], [], AC_MSG_ERROR([SWIG is required to build.]))
# Get SWIGFLAGS, CPPFLAGS, etc., for Python generation by swig
# http://www.gnu.org/software/autoconf-archive/ax_swig_python.html
#AX_SWIG_PYTHON
AC_ARG_WITH([swig],
  [AS_HELP_STRING([--with-swig=DIR],
  [use DIR as the root directory for swig's include and lib ])],
  [SWIG_HOME="$withval"],
  [AC_MSG_ERROR([SWIG should be specified using --with-swig option])])

  AC_MSG_CHECKING([for swig at $SWIG_HOME])
  	if test -d "$SWIG_HOME"   && test -d "$SWIG_HOME/bin"; then
	    AC_ARG_VAR([SWIGCC], [swig compiler command])
	    AC_CHECK_PROG(SWIGCC, [$SWIG_HOME/bin/swig], [$SWIG_HOME/bin/swig], [$SWIG_HOME/bin/swig])
	    AC_MSG_RESULT([yes])	    
	else
           AC_MSG_ERROR([no: SWIG directory not found!])
       fi
fi






AC_ARG_ENABLE(openmp,[AS_HELP_STRING([--enable-openmp], [enable OpenMP])])
AC_MSG_CHECKING([whether to enable OpenMP])
AS_IF([test "x${enable_openmp}" = "xyes" ], AC_MSG_RESULT([yes]), AC_MSG_RESULT([no]))
AM_CONDITIONAL(OPENMP, test $enable_openmp=true)


##AC_ARG_ENABLE(openmp,
##AS_HELP_STRING([--enable-openmp],
##              [enable openmp, default: no]),
##[case "${enableval}" in
##             yes) openmp=true;;
##             no)  openmp=false;;
##             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-openmp]) ;;
##esac],
##[openmp=false])
##AM_CONDITIONAL([OPENMP], [test $openmp=true])
#echo "enable_openmp=$enable_openmp"
#echo "OPENMP=$OPENMP"

dnl AC_ARG_WITH([python],
dnl   [AS_HELP_STRING([--with-python=DIR],
dnl   [use DIR as the root directory for pyhon's include and lib ])],
dnl   [PYTHON_HOME="$withval"],
dnl   [AC_MSG_ERROR([PYTHON should be specified using --with-python option])])

dnl   AC_MSG_CHECKING([for python at $PYTHON_HOME])
dnl   	if test -d "$PYTHON_HOME"   && test -d "$PYTHON_HOME/include"  && test -d "$PYTHON_HOME/lib"; then
dnl 	    AUPY_CFLAGS="$AUPY_CFLAGS     -I$PYTHON_HOME/include"
dnl 	    AUPY_CPPFLAGS="$AUPY_CFLAGS   -I$PYTHON_HOME/include"
dnl 	    AUPY_LIBS="$AUPY_CFLAGS       -L$PYTHON_HOME/lib  -lpython3.4m  -ldl -framework CoreFoundation"
dnl 	else
dnl            AC_MSG_ERROR([no: Python directory not found!])
dnl        fi

##http://hpc.pnl.gov/globalarrays/
##AC_ARG_WITH([ga],
##[AS_HELP_STRING([--with-ga=DIR],
##[use DIR as the root directory for Global Arrays (GA) Toolkit])],
##[GA_HOME="$withval"],
##[AC_MSG_ERROR([GA must be specified using --with-ga option])]
##)

##AC_MSG_CHECKING([for GA at $GA_HOME])
##if test -d "$GA_HOME"   && test -d "$GA_HOME/include"  && test -d "$GA_HOME/lib"; then
##ARRAYUDF_CFLAGS="$ARRAYUDF_CFLAGS       -I$GA_HOME/include"
##ARRAYUDF_CPPFLAGS="$ARRAYUDF_CPPFLAGS   -I$GA_HOME/include"
##ARRAYUDF_LIBS="$ARRAYUDF_LIBS         -L$GA_HOME/lib -lga -framework Accelerate -larmci"
##AC_MSG_RESULT([yes])	    
##else
##AC_MSG_ERROR([no: Global Arrays (GA) Toolkit directory not found!])
##fi

#AC_ARG_WITH([boost],
#[AS_HELP_STRING([--with-boost=DIR],
#[use DIR as the root directory for C++ Boost])],
#[BOOST_HOME="$withval"],
#[AC_MSG_ERROR([C++ Boost must be specified using --with-boost option])]
#)

#AC_MSG_CHECKING([for Boost at $BOOST_HOME])
#if test -d "$BOOST_HOME"   && test -d "$BOOST_HOME/include" ; then
#ARRAYUDF_CFLAGS="$ARRAYUDF_CFLAGS       -I$BOOST_HOME/include"
#ARRAYUDF_CPPFLAGS="$ARRAYUDF_CPPFLAGS   -I$BOOST_HOME/include"
#AC_MSG_RESULT([yes])	    
#else
#AC_MSG_ERROR([no: C++ Boost not found!])
#fi


AC_CONFIG_FILES([Makefile
src/Makefile
examples/Makefile])
AC_OUTPUT
