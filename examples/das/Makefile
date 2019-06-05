

#Bin's Mac
#AU_HOME=../../build
#HDF5_DIR=/Users/dbin/work/soft/hdf5-dev/build
#FFTW_DIR=/opt/local/
#CCC=mpicxx
#OTHER_FLAGS=-I/opt/local/include/eigen2/ -L/opt/local/lib -lboost_program_options-mt

#Snowbear
AU_HOME=../../build
HDF5_DIR=/usr/local/software/sl-6.x86_64/modules/hdf5/1.11.3-gcc-p
FFTW_DIR=/global/software/sl-6.x86_64/modules/gcc/4.8.5/fftw/3.3.4-gcc
CCC=mpicxx
OTHER_FLAGS=-I./fft-header/  -lboost_program_options-mt -std=c++11 -std=gnu++11


#Cori at NERSC
#AU_HOME=../../build
#HDF5_DIR=/usr/common/software/hdf5-parallel/1.10.2/intel
#FFTW_DIR=
#CCC=CC

.PHONY:all
all:h5-vds-merge tdms2h5 das-fft-full


h5-vds-merge:h5-vds-merge.c  
	$(CCC) -o h5-vds-merge h5-vds-merge.c -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5  -lhdf5_hl -lz $(OTHER_FLAGS)

tdms2h5:tdms2h5.c
	$(CCC) -o tdms2h5 tdms2h5.c  -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 -lhdf5_hl -lz 


das-fft-full:das-fft-full.cpp
	$(CCC) -o das-fft-full fft-header/resample.cpp fft/kiss_fft.c das-fft-full.cpp  -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3 -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5  -lhdf5_hl -lz  -I$(FFTW_DIR)/include -L$(FFTW_DIR)/lib -lfftw3 $(OTHER_FLAGS)

clean:
	rm das-correlation h5-vds-merge tdms2h5 das-fft das-fft-full
