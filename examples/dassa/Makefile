

#Bin's Mac
AU_DIR=/Users/dbin/work/test-fasttensor/fasttensor/build
#AU_DIR=/Users/dbin/work/FastTensor/build
HDF5_DIR=/Users/dbin/work/test-fasttensor/hdf5-1.12.0/build
FFTW_DIR=/opt/local
DASH_DIR=/Users/dbin/opt/dash-0.4.0
EIGEN3_DIR=/Users/dbin/work/test-fasttensor/fasttensor/examples/dassa/Tools3rd

CCC=mpicxx
OTHER_FLAGS=-O0 -g  -std=c++17 DasLib3rd.cpp

AU_FLAG=-I$(AU_DIR)/include -L$(AU_DIR)/lib -lArrayUDF
HDF5_FLAG=-I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5  -lhdf5_hl -lz
FFTW_FLAG=-I$(FFTW_DIR)/include -L$(FFTW_DIR)/lib -lfftw3 
DASH_FLAG=-I$(DASH_DIR)/include -L$(DASH_DIR)/lib -ldash-mpi -ldart-mpi -ldart-base -lpthread -DDASH_ENABLE_HDF5 -DDASH_MPI_IMPL_ID='mpich'
EIGEN3_FLAG=-I$(EIGEN3_DIR)
ALL_FLAGS=$(AU_FLAG) $(HDF5_FLAG) $(FFTW_FLAG) $(OTHER_FLAGS) $(DASH_FLAG) $(EIGEN3_FLAG)

.PHONY:all
all:au_example_stack

au_example_stack:au_example_stack.cpp
	$(CCC) -o au_example_stack au_example_stack.cpp $(ALL_FLAGS)

clean:
	rm au_example_stack
