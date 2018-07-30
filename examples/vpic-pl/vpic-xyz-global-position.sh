#!/bin/bash -l
#SBATCH -p debug
#SBATCH -N 1
##SBATCH --qos=premium
#SBATCH -t 00:30:00
##SBATCH -C haswell
#SBATCH -L SCRATCH
#SBATCH -o vpic-xyz-global-position.%j.output
#SBATCH -e vpic-xyz-global-position.%j.errput

srun -n 32 -N 1 ./vpic-xyz-global-position  
