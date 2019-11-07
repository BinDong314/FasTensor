#include "au.h"

int au_mpi_size_global;
int au_mpi_rank_global;

MPI_COMM_TYPE au_mpi_comm_global = MPI_COMM_WORLD_DEFAULT;

void AU_Init(int argc, char *argv[], MPI_COMM_TYPE au_mpi_comm_user)
{

    au_mpi_comm_global = au_mpi_comm_user;
    MPI_INIT(argc, argv, au_mpi_comm_global, au_mpi_rank_global, au_mpi_size_global);
}

void AU_Finalize()
{
    MPI_FINALIZE();
}