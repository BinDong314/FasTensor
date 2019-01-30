AU_HOME=/Users/dbin/work/arrayudf-git-svn-test-on-bitbucket/build
HDF5_DIR=/Users/dbin/work/soft/hdf5-1.8.18/build/
CCC=mpicxx

.PHONY:all
all:field-element-wise-op particle-global-xyz particle-interpolation parallel-eb-at-particle

field-element-wise-op:field-element-wise-op.cpp 
	$(CCC) -o  field-element-wise-op   field-element-wise-op.cpp     -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3  -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

particle-global-xyz:particle-global-xyz.cpp
	$(CCC) -o particle-global-xyz  particle-global-xyz.cpp  -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3 -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

particle-interpolation:particle-interpolation.cpp
	$(CCC) -o particle-interpolation  particle-interpolation.cpp  -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3 -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

parallel-eb-at-particle: parallel-eb-at-particle.cpp
	$(CCC) -o parallel-eb-at-particle parallel-eb-at-particle.cpp -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3 -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 
clean:
	rm field-element-wise-op particle-global-xyz particle-interpolation parallel-eb-at-particle
