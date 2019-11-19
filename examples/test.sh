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


mkdir ./test-data/test_1f1p_dir
./au_test_data_generator -f ./test-data/test_1f1p_dir/testf-16x16-1f1p-1.h5 -g /testg -d /testg/testd -n 2 -s 16,16 -t 1
./au_test_data_generator -f ./test-data/test_1f1p_dir/testf-16x16-1f1p-2.h5 -g /testg -d /testg/testd -n 2 -s 16,16 -t 1
./au_test_data_generator -f ./test-data/test_1f1p_dir/testf-16x16-1f1p-3.h5 -g /testg -d /testg/testd -n 2 -s 16,16 -t 1
./au_test_data_generator -f ./test-data/test_1f1p_dir/testf-16x16-1f1p-4.h5 -g /testg -d /testg/testd -n 2 -s 16,16 -t 1
mkdir ./test-data/test_1f1p_dir_output

# To test of running a command
# $1 command name
# $2 output data of the command (HDF5 format)
# $3 correct output of the command (HDF5 format)
function run_command(){
    $1
    if [ $? -eq 0 ]
    then
        echo "Test run $1  ... [PASSED]"
    else
        echo "Test run $1 ... [FAILED]" >&2
    fi

    if [ "$#" -gt 1 ]; then
        h5diff ./test-data/$2 ./test-data-good/$2
        if [ $? -eq 0 ]
        then
            echo "Checked output of $1 [PASSED]"
        else
            echo "Checked output of $2 [FALSE]" >&2
        fi    
    fi
}




## Run the test code
run_command ./au_example_hello_world testf-16x16-hello-world-output.h5 
run_command ./au_example_vector testf-16x16-vector-output.h5 
run_command ./au_example_array
run_command ./au_example_vds testf-16x16-vds-output.h5
