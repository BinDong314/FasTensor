AU_HOME=/Users/dbin/work/arrayudf-git-svn-test-on-bitbucket/build
HDF5_DIR=/Users/dbin/work/soft/hdf5-1.8.18/build/
CCC=mpicxx

CCC=mpicxx

.PHONY:all
all:translate-new translate

translate-new :translate-new.cpp 
	$(CCC)   -o  translate-new   translate-new.cpp    -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++17 -lArrayUDF -g3  -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

translate :translate.cpp 
	$(CCC)   -o  translate   translate.cpp    -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++17 -lArrayUDF -O3  -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

clean:
	rm translate-new translate
