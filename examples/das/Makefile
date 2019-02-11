AU_HOME=/Users/dbin/work/arrayudf-git-svn-test-on-bitbucket/build
HDF5_DIR=/Users/dbin/work/soft/hdf5-1.8.18/build/


.PHONY:all
all:das-correlation h5-vds-merge tdms2h5 das-fft

das-correlation:das-correlation.cpp  
	mpicxx -o das-correlation das-correlation.cpp -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3 -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

h5-vds-merge:h5-vds-merge.c  
	mpicc -o h5-vds-merge h5-vds-merge.c -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5  

tdms2h5:tdms2h5.c
	#echo $(HDF5_DIR)
	mpicc -o tdms2h5 tdms2h5.c  -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

das-fft:das-fft.cpp
	mpicxx -o das-fft fft/kiss_fft.c das-fft.cpp -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3 -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

clean:
	rm das-correlation h5-vds-merge tdms2h5 das-fft
