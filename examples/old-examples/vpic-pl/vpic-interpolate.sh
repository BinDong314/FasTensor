#!/bin/bash -l
#SBATCH -p debug
#SBATCH -N 2
##SBATCH --qos=premium
#SBATCH -t 00:30:00
##SBATCH -C haswell
#SBATCH -L SCRATCH
#SBATCH -o vpic-interpolate.%j.output
#SBATCH -e vpic-interpolate.%j.errput

srun -n 2 -N 2 ./vpic-interpolate
