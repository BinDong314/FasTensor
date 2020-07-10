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

#ifndef ARRAY_UDF_H
#define ARRAY_UDF_H

#include <assert.h>
#include "au_endpoint.h"
#include "au_stencil.h"
#include "au_type.h"
#include "au_array.h"
#include "au_mpi.h"
#include <libdash.h>

extern int au_mpi_size_global;
extern int au_mpi_rank_global;
extern int au_size;
extern int au_rank;

extern MPI_COMM_TYPE au_mpi_comm_global;
void AU_Init(int argc, char *argv[], MPI_COMM_TYPE au_mpi_comm = MPI_COMM_WORLD_DEFAULT);

void AU_Finalize();

#endif
