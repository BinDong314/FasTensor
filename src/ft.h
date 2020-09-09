

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
