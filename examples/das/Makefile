#AU_HOME=/bearbin-data3/home/dbin/soft/arrayudf-test-new/build
#HDF5_DIR=/bearbin-data3/home/dbin/soft/hdf5/build


.PHONY:all
all:das-correlation h5-vds-merge

das-correlation:das-correlation.cpp  
	mpiCC -o das-correlation das-correlation.cpp -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3 -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

h5-vds-merge:h5-vds-merge.c  
	mpiCC -o h5-vds-merge h5-vds-merge.c -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3 -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 


clean:
	rm das-correlation h5-vds-merge
