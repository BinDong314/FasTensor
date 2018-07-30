
# ArrayUDF Copyright (c) 2017, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy).  All rights reserved.

# If you have questions about your rights to use or distribute this software, please contact Berkeley Lab's Innovation & Partnerships Office at  IPO@lbl.gov.

# NOTICE. This Software was developed under funding from the U.S. Department of Energy and the U.S. Government consequently retains certain rights. As such, the U.S. Government has been granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software to reproduce, distribute copies to the public, prepare derivative works, and perform publicly and display publicly, and to permit other to do so. 

# Email questions to {dbin, kwu, sbyna}@lbl.gov
# Scientific Data Management Research Group
# Lawrence Berkeley National Laboratory


from ArrayUDF import *
from mpi4py import MPI

cs=IntVector(2)
os=IntVector(2)
cs[0]=5
cs[1]=5
os[0]=1
os[1]=1


#UDF function
#s is Stencil typed pointer
#O2f offsets current Stencil s with certain integer number
#Here, "O" means offset, 
#      "2" means 2 dimenstional data, 
#      "f" means float data type 
#We also have O1, O3 for 1D and 3D data.
#We also have f, i, d for float, integer, and double 
#Overall, the corresponding c++ function is
#   float f1(Stencil<float> s){
#       return s(0,0)*2+s(0,1)
#   }
def f1(s):
    return O2f(s, 0, 0)*2+O2f(s, 0,  1)

#Run UDF f1 on testf.h5p and store results on testf-f1.h5p
A=Arrayf("./testf.h5p", "/testg", "/testg/testd", cs, os)
B=Arrayf("./testf-f1.h5p", "/testg", "/testg/testd")
PyApply(f1, A, B)
