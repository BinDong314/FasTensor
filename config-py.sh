#!/bin/bash
./autogen.sh
./configure --prefix=/Users/dbin/work/arrayudf/build --with-hdf5=/Users/dbin/work/soft/hdf5-1.8.18/build/ CC=/Users/dbin/work/soft/mpich-3.2/build/bin/mpicc CXX=/Users/dbin/work/soft/mpich-3.2/build/bin/mpic++ --enable-python --with-swig=/Users/dbin/work/soft/swig-3.0.10/build/

#PYTHON=python3.4 LDFALGS="-L/opt/local/Library/Frameworks/Python.framework/Versions/3.4/lib/python3.4/"
#-L/opt/local/Library/Frameworks/Python.framework/Versions/3.4/lib/ -I/opt/local/Library/Frameworks/Python.framework/Versions/3.4/include/python3.4m" 
