MPI_PATH=/Users/dbin/work/soft/mpich-3.2/build/
INC_DIR=-I$(MPI_PATH)/include -I./
LIB_DIR=-L$(MPI_PATH)/lib 
LIB=-lmpich
all::test
test:iarray.cpp iarray-helper.h iarray.h test.cpp
	g++ -o test iarray.cpp test.cpp --std=c++0x  $(INC_DIR) $(LIB_DIR) $(LIB)
clean:
	rm test 
