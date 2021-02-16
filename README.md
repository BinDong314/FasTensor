This package contains the initial prototype of ArrayUDF from the project SDS (Scientific Data Services) framework. See copyright at the end. 

Please report any problem you've encountered in using this package to anyone of the following.

Bin Dong: dbin@lbl.gov 

Further reference for the ArrayUDF can be found in our paper 

"ArrayUDF: User-Defined Scientific Data Analysis on Arrays, HPDC ’17, June 26–30, 2017, Washington, DC, USA."

A high-level overview of the ArrayUDF can be found at doc/ArrayUDF-HPDC-2017.pdf


Please jump to the section 4 for instructions for Python interface.


1, Required software packages:

   Note:

          1), Install MPICH2 before HDF5 to enable the parallel I/O in HDF5

          2), add mpicc/mpicxx/mpirun from MPICH2 installation to your PATH

          3), add h5dump from HDF5 installation to your PATH

	  
   -- MPICH2 from http://www.mpich.org/ 

      (tested version: http://www.mpich.org/static/downloads/3.2/mpich-3.2.tar.gz)


      
   -- HDF5 from https://www.hdfgroup.org/

      (tested version: https://support.hdfgroup.org/ftp/HDF5/current18/src/hdf5-1.8.18.tar)


2, Install ArrayUDF

   Step 1), ./configure  --with-hdf5=HDF5_INSTALLATION_DIR

      Note: 
 
      1) you might need to run "chmod a+x ./autogen.sh; ./autogench".
            
      2) you might need to add configure command with 

          "CC= HDF5_INSTALLATION_DIR/bin/mpicc CXX=CC= HDF5_INSTALLATION_DIR/bin/mpicxx"
 
   Step 2), make 


3, Run and verify the examples:

   1), cd examples

   2), create a simple 2D (10 x 10) float array

       e.g., 

       mpirun -n 1 ./fake-hdf5-data -f testf-10x10.h5p -g /testg -d /testg/testd -n 2 -s 10,10 -t 1
       
       the file testf-10x10.h5p has a group "testg" and a dataset "testd" under the group.

   3), execute the UDF example 
      
       e.g., mpirun -n 2 udf-example
      
       The source file "udf-example.cpp" actually has three UDF functions.
      
       Please take a look the "myfunc1", "myfunc2", and "myfunc3", and their commends for detailed explanations.
       
   4), check the result 
      
       The result file "testf-10x10-func1.h5p" by applying "myfunc1" has the same content as the file "testf-10x10.h5p".
       =====================
       >> h5dump testf-10x10-func1.h5p
       HDF5 "testf-10x10-func1.h5p" {
       GROUP "/" {
       GROUP "testg" {
       DATASET "testd" {
         DATATYPE  H5T_IEEE_F32LE
         DATASPACE  SIMPLE { ( 10, 10 ) / ( 10, 10 ) }
         DATA {
         (0,0): 0.81, 0.81, 0.81, 0.81, 0.81, 0.81, 0.81, 0.81, 
         ... ...
       =====================


       The result file "testf-10x10-func2.h5p" by applying "myfunc2" contains the summary of two points of the file "testf-10x10.h5p".
       =====================
       >> h5dump testf-10x10-func2.h5p
       HDF5 "testf-10x10-func2.h5p" {
       GROUP "/" {
       GROUP "testg" {
       DATASET "testd" {
         DATATYPE  H5T_IEEE_F32LE
         DATASPACE  SIMPLE { ( 10, 10 ) / ( 10, 10 ) }
         DATA {
         (0,0): 1.62, 1.62, 1.62, 1.62, 1.62, 1.62, 1.62, 1.62, 
	 ... ...
       =====================

       The result file "testf-10x10-func3.h5p" by applying "myfunc3" contains the 5-point stencil results of the file  "testf-10x10.h5p".
       =====================
       HDF5 "testf-10x10-func3.h5p" {
       GROUP "/" {
       GROUP "testg" {
       DATASET "testd" {
         DATATYPE  H5T_IEEE_F32LE
         DATASPACE  SIMPLE { ( 10, 10 ) / ( 10, 10 ) }
         DATA {
         (0,0): 20.25, 20.25, 20.25, 20.25, 20.25, 20.25, 20.25, 20.25,
         ... ...
       =====================



4, Python interface

   Following the following steps to build python interface.

0) Required software packages

      Excepting the HDF5 and MPICH2, the python interface for ArrayUDF
      also needs SWIG and Python.
      
      ---For SWIG,   see http://www.swig.org/ for dowload and installation.

      	     	     Our system was tested with swig-3.0.10.
		     
      ---For Python, its version must be > 3.0

      	     	     Our system was tested with python3.4 and python3.5
		     
   1) configure
       
       ./configure --prefix=$PWD/build
   
                   --with-hdf5=HDF5_INSTALLATION_DIR
		   
		   --enable-python

		   --with-swig=SWIG_INSTALLATION_DIR
		   
       Note:  Assume the compiled code will be installed under $PWD/build
              The SWIG is installed under SWIG_INSTALLATION_DIR
              you might need to add configure with "CC= HDF5_INSTALLATION_DIR/bin/mpicc
              CXX=CC= HDF5_INSTALLATION_DIR/bin/mpicxx"
	      
   2)  compile and install

       make & make install
   
   3) Test the interface:
      
       > cd $PWD/build/lib/python3.5/site-packages/
   
        Note: Assume the version of python used is 3.5
   
      > cp ../../../../examples/py-example.py ./
      
      > cp ../../../../examples/testf.h5p     ./
      
         Note: See above 3 to see how to create testf.h5p.
	 
      > python3.5 py-example.py
      
      The py-example.py apply following UDF f1 on file testf.h5p.
      
      ========================
      def f1(s):
          return O2f(s, 0, 0)*2+O2f(s, 0,  1)
      ========================

      The result files are :
      =======================
      HDF5 "testf-f1.h5p" {
      GROUP "/" {
         GROUP "testg" {
      	 DATASET "testd" {
         	 DATATYPE  H5T_IEEE_F32LE
                 DATASPACE  SIMPLE { ( 10, 10 ) / ( 10, 10 ) }
           DATA {
            (0,0): 2.43, 2.43, 2.43, 2.43, 2.43, 2.43, 2.43, 2.43, 2.43, 2.43,
            (1,0): 2.43, 2.43, 2.43, 2.43, 2.43, 2.43, 2.43, 2.43, 2.43, 2.43,
            (2,0): 2.43, 2.43, 2.43, 2.43, 2.43, 2.43, 2.43, 2.43, 2.43, 2.43,
            .........
      ======================



*** Copyright Notice ***
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