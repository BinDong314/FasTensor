AU_HOME=/Users/dbin/work/arrayudf-git-svn-test-on-bitbucket/build
HDF5_DIR=/Users/dbin/work/soft/hdf5-1.8.18/build/
CCC=mpicxx

.PHONY:all
all:particle-filter particle-binning
particle-filter:particle-filter.cpp 
	$(CCC) -o  particle-filter   particle-filter.cpp     -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++17 -lArrayUDF -O3  -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

particle-binning:particle-binning.cpp 
	$(CCC) -o  particle-binning   particle-binning.cpp     -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++17 -lArrayUDF -O3  -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

clean:
	rm particle-filter particle-binning
