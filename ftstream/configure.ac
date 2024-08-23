AC_PREREQ([2.69])
AC_INIT([FTSTREAM], [1.0], [no-email])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_AUX_DIR([build-aux])

# Initialize Automake
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

# Check for C++ compiler
AC_PROG_CXX

# Dr. Bin has confirmed we will be using C++17 standard
AX_CXX_COMPILE_STDCXX([17], [noext], [mandatory])

# Check for required programs
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_CHECK_PROG([CMAKE], [cmake], [cmake])
AS_IF([test x"$CMAKE" = x],
      [AC_MSG_ERROR([CMake is required to build Docopt])])

#====================================================================#
#                           Add HDF5                                 #
#====================================================================#
AC_ARG_WITH([hdf5],
  [AS_HELP_STRING([--with-hdf5=DIR],
  [use DIR as the root directory for HDF5 include and lib])],
  [HDF5_HOME="$withval"],
  [AC_MSG_ERROR([HDF5 must be specified using --with-hdf5 option])])
AC_MSG_CHECKING([for HDF5 at $HDF5_HOME])
AS_IF([test -d "$HDF5_HOME" && test -d "$HDF5_HOME/include" && test -d "$HDF5_HOME/lib"],
  [
    CPPFLAGS="$CPPFLAGS -I$HDF5_HOME/include"
    LDFLAGS="$LDFLAGS -L$HDF5_HOME/lib"
    AC_MSG_RESULT([yes])
  ],
  [
    AC_MSG_RESULT([no])
    AC_MSG_ERROR([HDF5 directory structure not found!])
  ])
AC_CHECK_HEADER([hdf5.h], [], 
  [AC_MSG_ERROR([HDF5 header not found in $HDF5_HOME/include])])
AC_CHECK_LIB([hdf5], [H5Fcreate], [], 
  [AC_MSG_ERROR([HDF5 library not found in $HDF5_HOME/lib])])
# Add HDF5 to LIBS
#LIBS="$LIBS -lhdf5"

#====================================================================#
#                     Build and Add Docopt                           #
#====================================================================#
# Set up Docopt build
AC_CONFIG_COMMANDS([docopt],
[
  cd ${srcdir}/src/third-party/docopt
  $CMAKE .
  $MAKE
  $MAKE install
],
[
  srcdir=$srcdir
  abs_top_builddir=$abs_top_builddir
  CMAKE=$CMAKE
])

# Add Docopt to compilation flags
DOCOPT_CPPFLAGS="-I\${abs_top_builddir}/src/third-party/docopt/build/include"
DOCOPT_LDFLAGS="-L\${abs_top_builddir}/src/third-party/docopt/build/lib"
AC_SUBST([DOCOPT_CPPFLAGS])
AC_SUBST([DOCOPT_LDFLAGS])

# Generate config.h
AC_CONFIG_HEADERS([config.h])

# Output files
AC_CONFIG_FILES([
    Makefile
    src/Makefile
])

AC_OUTPUT
