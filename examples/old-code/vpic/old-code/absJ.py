import h5py
import numpy as np


filename='/Users/dbin/work/vpic-hdf5/vpic/build/testdir/hydro_hdf5/T.50/hydro_ion_50.h5'
file    = h5py.File(filename, 'r')   # 'r' means
jx = file['/Timestep_50/jx'][()]
jy = file['/Timestep_50/jy'][()]
jz = file['/Timestep_50/jz'][()]

print(jx.shape)

print("jx[0, 0, 0], jx[0, 0, 1]", jx[0, 0, 0], jx[0, 0, 1])
print("jy[0, 0, 0], jy[0, 0, 1]", jy[0, 0, 0], jy[0, 0, 1])
print("jz[0, 0, 0], jz[0, 0, 1]", jz[0, 0, 0], jz[0, 0, 1])

absJ=np.sqrt(jx**2 + jy**2 + jz**2)

print("absJ[0, 0, 0], absJ[0, 0, 1]", absJ[0, 0, 0], absJ[0, 0, 1])

outputfilename='/Users/dbin/work/vpic-hdf5/vpic/build/testdir/hydro_hdf5/T.50/hydro_ion_50_m_py.h5'
outputf = h5py.File(outputfilename,'w')
group   = outputf.create_group("/Timestep_50")
dset    = group.create_dataset("absJ", absJ.shape, data=absJ)

