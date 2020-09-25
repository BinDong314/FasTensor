#include "ft.h"

int au_mpi_size_global;
int au_mpi_rank_global;

int au_size;
int au_rank;

MPI_COMM_TYPE au_mpi_comm_global = MPI_COMM_WORLD_DEFAULT;

//std::vector<Endpoint *> endpoint_clean_vector;
std::map<Endpoint *, bool> endpoint_clean_vector;

void FT_Init(int argc, char *argv[], MPI_COMM_TYPE au_mpi_comm_user)
{
    au_mpi_comm_global = au_mpi_comm_user;
    dash::init(&argc, &argv);
    //size_t team_size = dash::Team::All().size();
    //std::cout << "DASH team size : " << team_size << "\n";
    MPI_INIT(argc, argv, au_mpi_comm_global, au_mpi_rank_global, au_mpi_size_global);
    au_size = au_mpi_size_global;
    au_rank = au_mpi_rank_global;
    if (!au_rank)
        std::cout << "RUN analysis with  [" << au_size << "] processes \n";
}

void FT_Finalize()
{

    /*
    if (endpoint_clean_vector.size() != 0)
    {
        for (int i = 0; i < endpoint_clean_vector.size(); i++)
        {
            if (endpoint_clean_vector[i] != NULL)
                delete endpoint_clean_vector[i];
        }
    }*/

    for (std::map<Endpoint *, bool>::iterator it = endpoint_clean_vector.begin(); it != endpoint_clean_vector.end(); ++it)
    {
        //std::cout << it->first << " => " << it->second << '\n';
        if (it->second == true)
        {
            delete it->first;
            std::cout << "call delete \n";
        }
        std::cout << "in finalize \n";
    }

    dash::finalize();
    MPI_FINALIZE();
}
