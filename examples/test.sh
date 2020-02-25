#!/bin/bash

echo "Prepare test data ..."
##Clean data
mkdir ./test-data > /dev/null 2>&1
rm ./test-data/*.h5 

##Create  test data
./au_test_data_generator -f ./test-data/testf-16x16-hello-world.h5 -g /testg -d /testg/testd -n 2 -s 16,16 -t 1 > /dev/null 2>&1
./au_test_data_generator -f ./test-data/testf-16x16-vector.h5 -g /testg -d /testg/testd -n 2 -s 16,16 -t 1 > /dev/null 2>&1
./au_test_data_generator -f ./test-data/testf-16x16-array.h5 -g /testg -d /testg/testd -n 2 -s 16,16 -t 1 > /dev/null 2>&1


./au_test_data_generator -f ./test-data/testf-16x16-vds-x.h5 -g /testg -d /testg/x -n 2 -s 16,16 -t 0 -i 1 > /dev/null 2>&1
./au_test_data_generator -f ./test-data/testf-16x16-vds-y.h5 -g /testg -d /testg/y -n 2 -s 16,16 -t 0 -i 101 > /dev/null 2>&1
./au_test_data_generator -f ./test-data/testf-16x16-vds-z.h5 -g /testg -d /testg/z -n 2 -s 16,16 -t 0 -i 1001 > /dev/null 2>&1

h5copy -i ./test-data/testf-16x16-vds-x.h5 -o ./test-data/testf-16x16-vds.h5 -s  /testg/x  -d /testg/x -p > /dev/null 2>&1
h5copy -i ./test-data/testf-16x16-vds-y.h5 -o ./test-data/testf-16x16-vds.h5 -s  /testg/y  -d /testg/y  > /dev/null 2>&1
h5copy -i ./test-data/testf-16x16-vds-z.h5 -o ./test-data/testf-16x16-vds.h5 -s  /testg/z  -d /testg/z > /dev/null 2>&1
rm ./test-data/testf-16x16-vds-x.h5 > /dev/null 2>&1
rm ./test-data/testf-16x16-vds-y.h5 > /dev/null 2>&1
rm ./test-data/testf-16x16-vds-z.h5 > /dev/null 2>&1


mkdir ./test-data/test_1f1p_dir > /dev/null 2>&1
./au_test_data_generator -f ./test-data/test_1f1p_dir/testf-16x16-1f1p-1.h5 -g /testg -d /testg/testd -n 2 -s 16,16 -t 1 > /dev/null 2>&1
./au_test_data_generator -f ./test-data/test_1f1p_dir/testf-16x16-1f1p-2.h5 -g /testg -d /testg/testd -n 2 -s 16,16 -t 1 > /dev/null 2>&1
./au_test_data_generator -f ./test-data/test_1f1p_dir/testf-16x16-1f1p-3.h5 -g /testg -d /testg/testd -n 2 -s 16,16 -t 1 > /dev/null 2>&1
./au_test_data_generator -f ./test-data/test_1f1p_dir/testf-16x16-1f1p-4.h5 -g /testg -d /testg/testd -n 2 -s 16,16 -t 1 > /dev/null 2>&1
mkdir ./test-data/test_1f1p_dir_output > /dev/null 2>&1

./au_test_data_generator -f ./test-data/testf-16x16-cache.h5 -g /testg -d /testg/testd -n 2 -s 16,16 -t 1 > /dev/null 2>&1


./au_test_data_generator -f ./test-data/testf-16-reduce.h5 -g /testg -d /testg/testd -n 1 -s 128 -t 1 > /dev/null 2>&1

echo "Start to run test  ..."

# To test of running a command
# $1 command name
# $2 output data of the command (HDF5 format)
# $3 correct output of the command (HDF5 format)
function run_command(){
    $1 > /dev/null 2>&1
    if [ $? -eq 0 ]
    then
        echo "Test run $1  ... [PASSED]"
    else
        echo "Test run $1  ... [FAILED]" >&2
    fi

    if [ "$#" -gt 1 ]; then
        h5diff ./test-data/$2 ./test-data-good/$2 > /dev/null 2>&1
        if [ $? -eq 0 ]
        then
            echo "Checked output of $1 [PASSED]"
        else
            echo "Checked output of $2 [FAILED]" >&2
        fi    
    fi
}

# To test of running au_example_1f1p
# $1 command name
# $2 output data  of the command (HDF5 format)
function run_command_1f1p(){
    $1 > /dev/null 2>&1
    if [ $? -eq 0 ]
    then
        echo "Test run $1  ... [PASSED]"
    else
        echo "Test run $1  ... [FAILED]" >&2
    fi

    for fileindex in {1..4}
    do
        h5diff ./test-data/test_1f1p_dir_output/testf-16x16-1f1p-$fileindex.h5 ./test-data-good/test_1f1p_dir_output/testf-16x16-1f1p-$fileindex.h5 > /dev/null 2>&1
        if [ $? -eq 0 ]
        then
            echo "Checked output [$fileindex] of au_example_1f1p [PASSED]"
        else
            echo "Checked output [$fileindex] of au_example_1f1p [FAILED]" >&2
        fi    
    done
}

## Run the test code
run_command ./au_example_hello_world testf-16x16-hello-world-output.h5 
run_command ./au_example_vector testf-16x16-vector-output.h5 
run_command ./au_example_array
run_command ./au_example_vds testf-16x16-vds-output.h5
run_command_1f1p ./au_example_1f1p 

run_command  ./au_example_cache testf-16x16-cache-output.h5