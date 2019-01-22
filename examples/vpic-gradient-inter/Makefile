AU_HOME=/Users/dbin/work/arrayudf-git-svn-test-on-bitbucket/build
HDF5_DIR=/Users/dbin/work/soft/hdf5-1.10.3/build/


.PHONY:all
all:element-wise vpic-xyz-global-position particles-interpolation

element-wise:element-wise.cpp  
	mpic++ -o  element-wise          element-wise.cpp       -g     -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF  -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

vpic-xyz-global-position:vpic-xyz-global-position.cpp
	mpic++ -o vpic-xyz-global-position  vpic-xyz-global-position.cpp  -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3 -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

particles-interpolation:particles-interpolation.cpp
	mpic++ -o particles-interpolation  particles-interpolation.cpp  -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3 -I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5 

clean:
	rm element-wise vpic-xyz-global-position particles-interpolation
