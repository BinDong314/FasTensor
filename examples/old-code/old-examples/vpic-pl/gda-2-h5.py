import h5py
import numpy as np
import multiprocessing as mp
import ctypes as c
import sys
import math
from time import time

#Measure the time it takes to run this script
startTime = time()

#Define the input and output particle data file
outFileName =  "bx_1.h5p"

#Field data settings
bxFile  = "bx_1.gda"
bshape  = (512, 512, 256)


#Load the complete field data into memory. This quite a bit of data (~36G total) but on a large memory node this is ok. 
print("Loading gda file")
bx =  np.fromfile( bxFile , dtype='float32' ).reshape( ( bshape[2] , bshape[1] , bshape[0] ) ).transpose().astype(np.double)
print(bx.shape)
print(type(bx[0]))
print(bx[0,0,0])


print("Allocate output data file")

outfile = h5py.File(outFileName, 'a' )
outGroup = outfile.get("/").require_group("Step#4")
vpar_out = outGroup.require_dataset(name="bx" , shape=bx.shape, dtype='float32' )

vpar_out[0:bx.shape[0], 0:bx.shape[1], 0:bx.shape[2]] = bx
#infile.close()
outfile.flush()
outfile.close()

