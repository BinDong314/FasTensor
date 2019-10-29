##12288/2   = 6144
##12288/4   = 3072
##12288/8   = 1536
##12288/16  = 768
srun  -n 16 ./ccl-pc-new -w -r -c -s 3072  -i /project/projectdirs/m1248/bin-data/data-backup/BISICLES/plot.Ant.Steady-500m-newSMB-1km.000000.2d_flat.h5   -t /project/projectdirs/m1248/bin-data/data-backup/BISICLES/plot.Ant.Steady-500m-newSMB-1km.000000.2d_flat_ccl_labels-memory.h5 

##mpirun -n 4 ./ccl-pc-new -w -r  -s 8 -h 1  -i testf.h5p -o testf-output.h5p -g /testg -d /testg/testd -t testf-temp.h5p 



