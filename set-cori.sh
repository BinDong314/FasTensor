module swap intel/19.0.3.199 intel/19.1.3.304
module load cray-hdf5-parallel
./autogen.sh
./configure --prefix=/project/projectdirs/m1248/fasttensor/build --with-hdf5=/opt/cray/pe/hdf5-parallel/1.10.5.2/INTEL/19.0 CXX=CC
