#!/bin/bash -l

module load openmpi/3.1.2-gcc
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/soft/hdf5/build/lib/

export INPUT="/bearbin-data3/home/dbin/data/westSac-merged.h5p"
export FOLDER="/bearbin-data3/home/dbin/data/westSac-merged-udf.h5p"

#120000, 11648
rm  $FOLDER
mpirun -n 4  ./das-correlation -i $INPUT -o ${FOLDER} -g / -d /DataByChannelTime -n 2 -c 30000,32 -t 0,20 -s 1000,1 -e 500 -w 300 -l 10
