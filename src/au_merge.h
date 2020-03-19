/**
 *ArrayUDF Copyright (c) 2017, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy).  All rights reserved.
 *
 *If you have questions about your rights to use or distribute this software, please contact Berkeley Lab's Innovation & Partnerships Office at  IPO@lbl.gov.
 *
 * NOTICE. This Software was developed under funding from the U.S. Department of Energy and the U.S. Government consequently retains certain rights. As such, the U.S. Government has been granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software to reproduce, distribute copies to the public, prepare derivative works, and perform publicly and display publicly, and to permit other to do so. 
 *
 */

/**
 *
 * Email questions to dbin@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#ifndef AU_MERGE_H

#define AU_MERGE_H

//AuMergeType follows the defination within MPI
//See below for more information
typedef int AU_Op;

#define AU_MAX (AU_Op)(0x58000001)
#define AU_MIN (AU_Op)(0x58000002)
#define AU_SUM (AU_Op)(0x58000003)
#define AU_PROD (AU_Op)(0x58000004)
#define AU_LAND (AU_Op)(0x58000005)
#define AU_BAND (AU_Op)(0x58000006)
#define AU_LOR (AU_Op)(0x58000007)
#define AU_BOR (AU_Op)(0x58000008)
#define AU_LXOR (AU_Op)(0x58000009)
#define AU_BXOR (AU_Op)(0x5800000a)
#define AU_MINLOC (AU_Op)(0x5800000b)
#define AU_MAXLOC (AU_Op)(0x5800000c)
#define AU_REPLACE (AU_Op)(0x5800000d)
#define AU_NO_OP (AU_Op)(0x5800000e)

/*
https://github.com/pmodels/mpich/blob/master/src/include/mpi.h.in
typedef int MPI_Op;

#define MPI_MAX (MPI_Op)(0x58000001)
#define MPI_MIN (MPI_Op)(0x58000002)
#define MPI_SUM (MPI_Op)(0x58000003)
#define MPI_PROD (MPI_Op)(0x58000004)
#define MPI_LAND (MPI_Op)(0x58000005)
#define MPI_BAND (MPI_Op)(0x58000006)
#define MPI_LOR (MPI_Op)(0x58000007)
#define MPI_BOR (MPI_Op)(0x58000008)
#define MPI_LXOR (MPI_Op)(0x58000009)
#define MPI_BXOR (MPI_Op)(0x5800000a)
#define MPI_MINLOC (MPI_Op)(0x5800000b)
#define MPI_MAXLOC (MPI_Op)(0x5800000c)
#define MPI_REPLACE (MPI_Op)(0x5800000d)
#define MPI_NO_OP (MPI_Op)(0x5800000e)
*/

#endif