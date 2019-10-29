#!/bin/bash -l
#SBATCH -p debug
#SBATCH -N 20
##SBATCH --qos=premium
#SBATCH -t 00:30:00
#SBATCH -C haswell
#SBATCH -L SCRATCH
#SBATCH -o das-correlation-large.%j.output
#SBATCH -e das-correlation-large.%j.errput

rm /global/cscratch1/sd/dbin/de-test-all-osts/DAS/data_earthquake-arrayudf.h5
srun -n 500 -N 20 ./das-correlation -i /global/cscratch1/sd/dbin/de-test-all-osts/DAS/data_earthquake.h5 -o /global/cscratch1/sd/dbin/de-test-all-osts/DAS/data_earthquake-arrayudf.h5  -g / -d /dat -n 2 -c 180001,20 -t 0,11  -e 500 -w 50 -l 10
