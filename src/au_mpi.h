#ifndef ARRAYUDF_MPI_H
#define ARRAYUDF_MPI_H

#include <ctime>
#include <iostream>
#include "au_merge.h"

#define ENABLE_MPI 1

#ifdef ENABLE_MPI
#include "mpi.h"
#define MPI_COMM_TYPE MPI_Comm
#define AU_WTIME_TYPE double
#define AU_WTIME MPI_Wtime()
#define MPI_COMM_WORLD_DEFAULT MPI_COMM_WORLD
#define AU_MPI_Datatype MPI_Datatype
#define AU_MPI_Op MPI_Op
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

template <typename T>
inline MPI_Datatype InferMPIType()
{
    if (std::is_same<T, int>::value)
    {
        return MPI_INT;
    }
    else if (std::is_same<T, short>::value)
    {
        return MPI_SHORT;
    }
    else if (std::is_same<T, long>::value)
    {
        return MPI_LONG;
    }
    else if (std::is_same<T, long long>::value)
    {
        return MPI_LONG_LONG;
    }
    else if (std::is_same<T, unsigned int>::value)
    {
        return MPI_UNSIGNED;
    }
    else if (std::is_same<T, unsigned short>::value)
    {
        return MPI_UNSIGNED_SHORT;
    }
    else if (std::is_same<T, unsigned long>::value)
    {
        return MPI_UNSIGNED_LONG;
    }
    else if (std::is_same<T, unsigned long long>::value)
    {
        return MPI_UNSIGNED_LONG_LONG;
    }
    else if (std::is_same<T, float>::value)
    {
        return MPI_FLOAT;
    }
    else if (std::is_same<T, double>::value)
    {
        return MPI_DOUBLE;
    }
    else
    {
        AU_EXIT("Not supporte MPI data type");
    }

    return MPI_INT;
}

inline MPI_Op InferMPIMergeOp(std::string &opt_str)
{
    AU_Op op_int;
    std::istringstream reader(opt_str);
    reader >> op_int;

    switch (op_int)
    {
    case AU_MAX:
        return MPI_MAX;
        break;
    case AU_MIN:
        return MPI_MIN;
        break;
    case AU_SUM:
        return MPI_SUM;
        break;
    case AU_PROD:
        return MPI_PROD;
        break;
    case AU_LAND:
        return MPI_LAND;
        break;
    case AU_BAND:
        return MPI_BAND;
        break;
    case AU_LOR:
        return MPI_LOR;
        break;
    case AU_BOR:
        return MPI_BOR;
        break;
    case AU_LXOR:
        return MPI_LXOR;
        break;
    case AU_BXOR:
        return MPI_BXOR;
        break;
    case AU_MINLOC:
        return MPI_MINLOC;
        break;
    case AU_MAXLOC:
        return MPI_MAXLOC;
        break;
    case AU_REPLACE:
        return MPI_REPLACE;
        break;
    default:
        AU_EXIT("Not supported type in MPI_Reduce");
        break;
    }

    return 0;
}

#define AU_Reduce(local_buffer_p, reduced_buffer_p, size, type, op, root, comm)   \
    {                                                                             \
        MPI_Reduce(local_buffer_p, reduced_buffer_p, size, type, op, root, comm); \
    }

#else
#define MPI_COMM_TYPE int
#define AU_WTIME_TYPE time_t
#define AU_WTIME time(NULL)
#define MPI_COMM_WORLD_DEFAULT 0

#define AU_MPI_Datatype int
#define AU_MPI_Op int

#define MPI_INIT(argc, argv, au_mpi_comm_global, au_mpi_rank_global, au_mpi_size_global) \
    {                                                                                    \
        au_mpi_rank_global = 0;                                                          \
        au_mpi_size_global = 1;                                                          \
    }
#define MPI_FINALIZE() \
    {                  \
    }

template <typename T>
inline int InferMPIType()
{
    AU_EXIT("No MPI used to compile!");
}

inline int InferMPIMergeOp(std::string &opt_str)
{
    AU_EXIT("Should not be here!");
}

#define AU_Reduce(local_buffer_p, reduced_buffer_p, size, type, op, root, comm) \
    {                                                                           \
        AU_EXIT("Should not be here!")                                          \
    }

#endif

#endif