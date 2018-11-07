AU_HOME=/Users/dbin/work/arrayudf-git-dev/build
HDF5_DIR=/Users/dbin/work/soft/hdf5-1.10.3/build/


.PHONY:all
all:absJ 

absJ:absJ.cpp  
	mpic++ -o  absJ absJ.cpp  -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3 -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

clean:
	rm absJ
