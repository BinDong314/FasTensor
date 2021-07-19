#Bin's Mac
AU_DIR=/Users/dbin/work/fastensor/build
HDF5_DIR=/Users/dbin/work/soft/hdf5-1.12.0/build
DASH_DIR=/Users/dbin/work/soft/dash/build/install


#Lawrencium
#AU_DIR=/clusterfs/bear/BinDong_DAS_Data/fasttensor/build
#DASH_DIR=/clusterfs/bear/BinDong_DAS_Data/fasttensor/tools3rd/dash/build/opt/dash-0.4.0
#EIGEN3_DIR=${PWD}/Tools3rd/


SHELL:=/bin/bash

CCC=mpicxx
OTHER_FLAGS=-g3  -std=c++17 -ffast-math

AU_FLAG=-I$(AU_DIR)/include -L$(AU_DIR)/lib -lFastensor
HDF5_FLAG=-I$(HDF5_DIR)/include -L$(HDF5_DIR)/lib -lhdf5  -lhdf5_hl -lz
DASH_FLAG=-I$(DASH_DIR)/include -L$(DASH_DIR)/lib -ldash-mpi -ldart-mpi -ldart-base -lpthread -DDASH_ENABLE_HDF5 -DDASH_MPI_IMPL_ID='mpich' -DHAS_DASH_ENDPOINT
ALL_FLAGS= $(OTHER_FLAGS) $(AU_FLAG) $(HDF5_FLAG)  $(DASH_FLAG) 

.PHONY:all
all:create_data spmv spmv-v2 spmv-stencil create_data_3d spmv-hpcg-3d spmv-hpcg-3d-read spmv-hpcg-3d-read-cache

create_data:create_data.cpp
	$(CCC) -o create_data create_data.cpp $(ALL_FLAGS)

spmv:spmv.cpp
	$(CCC) -o spmv spmv.cpp $(ALL_FLAGS)

spmv-v2:spmv-v2.cpp
	$(CCC) -o spmv-v2 spmv-v2.cpp $(ALL_FLAGS)

spmv-stencil:spmv-stencil.cpp
	$(CCC) -o spmv-stencil spmv-stencil.cpp $(ALL_FLAGS)

create_data_3d:create_data_3d.cpp
	$(CCC) -o create_data_3d create_data_3d.cpp $(ALL_FLAGS)

spmv-hpcg-3d:spmv-hpcg-3d.cpp
	$(CCC) -o spmv-hpcg-3d spmv-hpcg-3d.cpp $(ALL_FLAGS)

spmv-hpcg-3d-read:spmv-hpcg-3d-read.cpp
	$(CCC) -o spmv-hpcg-3d-read spmv-hpcg-3d-read.cpp $(ALL_FLAGS)


spmv-hpcg-3d-read-cache:spmv-hpcg-3d-read-cache.cpp
	$(CCC) -o spmv-hpcg-3d-read-cache spmv-hpcg-3d-read-cache.cpp $(ALL_FLAGS)

clean:
	rm -rf create_data create_data.dSYM spmv spmv.dSYM spmv-v2 spmv-v2.dSYM  spmv-stencil spmv-stencil.dSYM create_data_3d create_data_3d.dSYM spmv-hpcg-3d spmv-hpcg-3d.dSYM spmv-hpcg-3d-read spmv-hpcg-3d-read.dSYM spmv-hpcg-3d-read-cache spmv-hpcg-3d-read-cache.dSYM

