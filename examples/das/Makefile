AU_HOME=../../build
HDF5_DIR=/opt/cray/pe/hdf5-parallel/1.10.1.1/GNU/5.1
FFTW_DIR=/opt/local/


.PHONY:all
all:das-fft

das-correlation:das-correlation.cpp  
	CC -o das-correlation das-correlation.cpp -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3 -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 -lhdf5_hl -lz

h5-vds-merge:h5-vds-merge.c  
	CC -o h5-vds-merge h5-vds-merge.c -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5  -lhdf5_hl -lz

tdms2h5:tdms2h5.c
	#echo $(HDF5_DIR)
	CC -o tdms2h5 tdms2h5.c  -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 -lhdf5_hl -lz 

das-fft:das-fft.cpp
	CC -o das-fft fft/kiss_fft.c das-fft.cpp  -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3 -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5  -lhdf5_hl -lz 

clean:
	rm das-correlation h5-vds-merge tdms2h5 das-fft
