AU_HOME=/Users/dbin/work/arrayudf-git-svn-test-on-bitbucket/build
HDF5_DIR=/Users/dbin/work/soft/hdf5-1.8.18/build/
CCC=mpicxx

.PHONY:all
all:element-wise global-xyz particles-interpolation parallel-eb-at-particle

element-wise:element-wise.cpp  
	$(CCC) -o  element-wise   element-wise.cpp    -g   -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF  -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

global-xyz:global-xyz.cpp
	$(CCC) -o global-xyz  global-xyz.cpp  -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3 -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

particles-interpolation:particles-interpolation.cpp
	$(CCC) -o particles-interpolation  particles-interpolation.cpp  -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3 -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

parallel-eb-at-particle: parallel-eb-at-particle.cpp
	$(CCC) -o parallel-eb-at-particle parallel-eb-at-particle.cpp -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3 -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 
clean:
	rm element-wise global-xyz particles-interpolation parallel-eb-at-particle
