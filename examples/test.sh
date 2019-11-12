#!/bin/bash

##Clean data
rm ./test-data/*.h5 

##Create  test data
./au_test_data_generator -f ./test-data/testf-16x16-hello-world.h5 -g /testg -d /testg/testd -n 2 -s 16,16 -t 1
./au_test_data_generator -f ./test-data/testf-16x16-vector.h5 -g /testg -d /testg/testd -n 2 -s 16,16 -t 1
./au_test_data_generator -f ./test-data/testf-16x16-array.h5 -g /testg -d /testg/testd -n 2 -s 16,16 -t 1


./au_test_data_generator -f ./test-data/testf-16x16-vds-x.h5 -g /testg -d /testg/x -n 2 -s 16,16 -t 0 -i 1
./au_test_data_generator -f ./test-data/testf-16x16-vds-y.h5 -g /testg -d /testg/y -n 2 -s 16,16 -t 0 -i 101
./au_test_data_generator -f ./test-data/testf-16x16-vds-z.h5 -g /testg -d /testg/z -n 2 -s 16,16 -t 0 -i 1001

h5copy -i ./test-data/testf-16x16-vds-x.h5 -o ./test-data/testf-16x16-vds.h5 -s  /testg/x  -d /testg/x -p
h5copy -i ./test-data/testf-16x16-vds-y.h5 -o ./test-data/testf-16x16-vds.h5 -s  /testg/y  -d /testg/y 
h5copy -i ./test-data/testf-16x16-vds-z.h5 -o ./test-data/testf-16x16-vds.h5 -s  /testg/z  -d /testg/z 
rm ./test-data/testf-16x16-vds-x.h5
rm ./test-data/testf-16x16-vds-y.h5
rm ./test-data/testf-16x16-vds-z.h5



## Run the test code
./au_example_hello_world
./au_example_vector
./au_example_array
