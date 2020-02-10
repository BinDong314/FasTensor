#ifndef ARRAYUDF_MPI_H
#define ARRAYUDF_MPI_H

#include <ctime>
#include <iostream>

#define ENABLE_MPI 1

#ifdef ENABLE_MPI
#include "mpi.h"
#define MPI_COMM_TYPE MPI_Comm
#define AU_WTIME_TYPE double
#define AU_WTIME MPI_Wtime()
#define MPI_COMM_WORLD_DEFAULT MPI_COMM_WORLD
#define MPI_INIT(argc, argv, au_mpi_comm_global, au_mpi_rank_global, au_mpi_size_global) \
    {                                                                                    \
        int mpi_init_flag = 0;                                                           \
        MPI_Initialized(&mpi_init_flag);                                                 \
        if (!mpi_init_flag)                                                              \
        {                                                                                \
            MPI_Init(&argc, &argv);                                                      \
        }                                                                                \
        MPI_Comm_rank(au_mpi_comm_global, &au_mpi_rank_global);                          \
        MPI_Comm_size(au_mpi_comm_global, &au_mpi_size_global);                          \
    }

#define MPI_FINALIZE()                     \
    {                                      \
        int mpi_finalize_flag = 0;         \
        MPI_Finalized(&mpi_finalize_flag); \
        if (!mpi_finalize_flag)            \
        {                                  \
            MPI_Finalize();                \
        }                                  \
    }
#else
#define MPI_COMM_TYPE int
#define AU_WTIME_TYPE time_t
#define AU_WTIME time(NULL)
#define MPI_COMM_WORLD_DEFAULT 0
#define MPI_INIT(argc, argv, au_mpi_comm_global, au_mpi_rank_global, au_mpi_size_global) \
    {                                                                                    \
        au_mpi_rank_global = 0;                                                          \
        au_mpi_size_global = 1;                                                          \
    }
#define MPI_FINALIZE() \
    {                  \
    }

#endif
#endif