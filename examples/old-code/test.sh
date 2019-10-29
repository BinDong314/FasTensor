#!/bin/bash

rm testf-10x10.h5p testf-10x10-func3.h5p testf-10x10-func2.h5p testf-10x10-func1.h5p

echo "Create test files ...."
mpirun -n 1 ./fake-hdf5-data -f testf-10x10.h5p -g /testg -d /testg/testd -n 2 -s 10,10 -t 1


echo "Run UDF on files ..."
mpirun -n 2 ./udf-example
