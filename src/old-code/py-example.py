from ArrayUDF import *
from mpi4py import MPI

cs=IntVector(2)
os=IntVector(2)
cs[0]=5
cs[1]=5
os[0]=1
os[1]=1


#UDF function
def f1(cell):
    return O2f(cell,0, 0)*2+O2f(cell,0, 1)

#Run UDF f1 on testf.h5p and store results on testf-f1.h5p
A=Arrayf("./testf.h5p", "/testg", "/testg/testd", cs, os)
B=Arrayf("./testf-f1.h5p", "/testg", "/testg/testd")
PyApply(f1, A, B)
