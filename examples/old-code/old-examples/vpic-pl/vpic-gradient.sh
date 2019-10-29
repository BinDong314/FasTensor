#!/bin/bash -l
#SBATCH -p debug
#SBATCH -N 4
##SBATCH --qos=premium
#SBATCH -t 00:30:00
##SBATCH -C haswell
#SBATCH -L SCRATCH
#SBATCH -o vpic-gradient.%j.output
#SBATCH -e vpic-gradient.%j.errput

srun -n 128 -N 4 ./vpic-gradient
