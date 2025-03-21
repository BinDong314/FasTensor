
Moved this FasTensor to https://github.com/BinDong314/FasTensor.git


This package contains the FastTensor, a new version of ArrayUDF from the project SDS (Scientific Data Services) framework at LBNL. This project is supported by the DOE. Please see copyright at the end. 


FasTensor, formerly known as ArrayUDF, is a generic parallel programming model for big data analyses with any user-defined functions (UDF). These functions may express data analysis operations from traditional database (DB) systems to advanced machine learning pipelines. FasTensor exploits the structural-locality in the multidimensional arrays to automate file operations, data partitioning, communication, parallel execution, and common data management operations.

FasTensor has the same idea as the MapReduce and Apache Spark to reduce the programming efforts. But, FasTensor is orders of magnitude faster than them because it is directly defined and executed on the multidimensional array, as shown below. Even comparing with highly optimized code in TensorFlow, FasTensor can achieve up to 13X performance speedup in executing expensive steps in CNN. FasTensor can scale up to over 10,000 CPU cores on supercomputers efficiently.




Please report any problem you've encountered in using this package to  Bin Dong: dbin@lbl.gov

1, Required software packages:

```
   Note:
          1), Install MPICH2 before HDF5 to enable the parallel I/O in HDF5
          2), add mpicc/mpicxx/mpirun from MPICH2 installation to your PATH
          3), add h5dump from HDF5 installation to your PATH
	  
   -- MPICH2 from http://www.mpich.org/ 
      (tested version: http://www.mpich.org/static/downloads/3.2/mpich-3.2.tar.gz)
   -- HDF5 from https://www.hdfgroup.org/
       https://www.hdfgroup.org/downloads/hdf5/source-code/

  Other Optional packages
   -- ADIOS https://github.com/ornladios/ADIOS2
```

1.1, HDF5

```
 > cd tools3rd
 > tar zxvf hdf5-1.12.0.tar.gz
 > cd hdf5-1.12.0
 > ./autogen.sh
 > ./configure --enable-parallel --prefix=$PWD/build CC=mpicc
 >  make install
```

1.2 RabbitMQ-C

```
 >  git clone https://github.com/alanxz/rabbitmq-c
 >  cd rabbitmq-c
 >	mkdir build && cd build
 >	cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/install -DOPENSSL_ROOT_DIR=$(brew --prefix openssl)
 >	cmake --build . --target install
```

2, FastTensor

```
 > ./configure --prefix=$PWD/build --with-hdf5=../hdf5-1.12.0/build/build/ CXX=mpic++ CC=mpicc 
```


3, Note on Lawrencium at LBNL


> module load gcc/7.4.0 hdf5/1.10.5-gcc-p
> ./autogen.sh
> ./configure --prefix=/clusterfs/bear/BinDong_DAS_Data/fasttensor/build --wit
h-hdf5=/global/software/sl-7.x86_64/modules/gcc/7.4.0/hdf5/1.10.5-gcc-p CXX=mpic
++ --with-dash=/clusterfs/bear/BinDong_DAS_Data/fasttensor/tools3rd/dash/build/o
pt/dash-0.4.0/


4, Note on Cori at NERSC

export HDF5_BASE=/opt/cray/pe/hdf5-parallel/1.10.5.2/GNU/8.2/
vim CMakeExt/HDF5.cmake
set (HDF5_LINKER_FLAGS "-lhdf5_hl -lhdf5 -ldl -lm -lz")




5, DASH (It is not used now)
   -- DASH from  https://github.com/dash-project/dash/   
```
 > cd tools3rd
 > tar zxvf dash.tar.gz
   (or git clone https://github.com/dash-project/dash.git )
 > vim build.minimal.sh
   Change 
      -DENABLE_HDF5=OFF 
   To   
      -DENABLE_HDF5=ON
 
   Change 
     -DINSTALL_PREFIX=$HOME/opt/dash-0.4.0/
   To
     -DINSTALL_PREFIX=$PWD/install

   Your may also need to setup HDF5_ROOT to HDF5 installation directory if DASH can not find HDF5
 > ./build.minimal.sh
 > make -C ./build install 

```

Error issue with DASH/C++ compiler:

```
/Users/dbin/opt/dash-0.4.0//include/cpp17/cstddef.h:12:12: error:
      redefinition of 'byte'
enum class byte : unsigned char {};
```

You can resolve it by 

```
  cp dash/cstddef.h ~/opt/dash-0.4.0/include/cpp17/
```




7, Copyright 

****************************

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy). 
All rights reserved.

If you have questions about your rights to use or distribute this software,
please contact Berkeley Lab's Intellectual Property Office at
IPO@lbl.gov.

NOTICE.  This Software was developed under funding from the U.S. Department
of Energy and the U.S. Government consequently retains certain rights.  As
such, the U.S. Government has been granted for itself and others acting on
its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the
Software to reproduce, distribute copies to the public, prepare derivative 
works, and perform publicly and display publicly, and to permit others to do so.


****************************


*** License Agreement ***

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy). 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

(1) Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

(2) Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

(3) Neither the name of the University of California, Lawrence Berkeley
National Laboratory, U.S. Dept. of Energy nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

You are under no obligation whatsoever to provide any bug fixes, patches,
or upgrades to the features, functionality or performance of the source
code ("Enhancements") to anyone; however, if you choose to make your
Enhancements available either publicly, or directly to Lawrence Berkeley
National Laboratory, without imposing a separate written license agreement
for such Enhancements, then you hereby grant the following license: a
non-exclusive, royalty-free perpetual license to install, use, modify,
prepare derivative works, incorporate into other computer software,
distribute, and sublicense such enhancements or derivative works thereof,
in binary and source code form.
