#! /bin/bash
libtoolize --force
aclocal -I m4 --install
autoheader
automake --force-missing --add-missing --copy 
autoconf


##CC=/Users/dbin/work/soft/mpich-3.2/build/bin/mpicc CXX=/Users/dbin/work/soft/mpich-3.2/build/bin/mpicxx ./configure  --with-hdf5=/Users/dbin/work/soft/hdf5-1.8.18/build --with-ga=/Users/dbin/work/soft/ga-5.6.1/build --prefix=$PWD/build --enable-debug --with-boost=/Users/dbin/work/soft/boost_1_64_0/build/
