#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ArrayUDF], [0.0.1], [dbin@lbl.gov])
AC_CONFIG_SRCDIR([src/array_udf_array.h])
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

AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug],
               [enable debugging, default: no]),
[case "${enableval}" in
             yes) debug=true;;
             no)  debug=false;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[debug=false])
AM_CONDITIONAL(DEBUG, test $debug=true)
echo "debug=$debug"
#echo "DEBUG=$DEBUG"

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


AC_CONFIG_FILES([Makefile
src/Makefile
examples/Makefile])
AC_OUTPUT
