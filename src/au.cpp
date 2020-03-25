#include "au.h"

int au_mpi_size_global;
int au_mpi_rank_global;

int au_size;
int au_rank;

MPI_COMM_TYPE au_mpi_comm_global = MPI_COMM_WORLD_DEFAULT;

void AU_Init(int argc, char *argv[], MPI_COMM_TYPE au_mpi_comm_user)
{
    au_mpi_comm_global = au_mpi_comm_user;
    dash::init(&argc, &argv);
    //size_t team_size = dash::Team::All().size();
    //std::cout << "DASH team size : " << team_size << "\n";
    MPI_INIT(argc, argv, au_mpi_comm_global, au_mpi_rank_global, au_mpi_size_global);
    au_size = au_mpi_size_global;
    au_rank = au_mpi_rank_global;
    std::cout << "au_mpi_size_global =  " << au_mpi_size_global << ", au_mpi_rank_global = " << au_mpi_rank_global << "\n";
}

void AU_Finalize()
{
    dash::finalize();
    MPI_FINALIZE();
}