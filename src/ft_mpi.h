/*
****************************

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy). 
All rights reserved.

If you have questions about your rights to use or distribute this software,
please contact Berkeley Lab's Intellectual Property Office at
IPO@lbl.gov.

NOTICE.  This Software was developed under funding from the U.S. Department
of Energy and the U.S. Government consequently retains certain rights.  As
such, the U.S. Government has been granted for itself and others acting on
its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the
Software to reproduce, distribute copies to the public, prepare derivative 
works, and perform publicly and display publicly, and to permit others to do so.


****************************


*** License Agreement ***

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy). 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

(1) Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

(2) Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

(3) Neither the name of the University of California, Lawrence Berkeley
National Laboratory, U.S. Dept. of Energy nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

You are under no obligation whatsoever to provide any bug fixes, patches,
or upgrades to the features, functionality or performance of the source
code ("Enhancements") to anyone; however, if you choose to make your
Enhancements available either publicly, or directly to Lawrence Berkeley
National Laboratory, without imposing a separate written license agreement
for such Enhancements, then you hereby grant the following license: a
non-exclusive, royalty-free perpetual license to install, use, modify,
prepare derivative works, incorporate into other computer software,
distribute, and sublicense such enhancements or derivative works thereof,
in binary and source code form.
*/

/**
 *
 * Author: Bin Dong dbin@lbl.gov
 * Web: https://crd.lbl.gov/bin-dong
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

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
#define MPI_INFO_NULL_DEFAULT MPI_INFO_NULL
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
    else if (std::is_same<T, std::complex<double>>::value)
    {
        return MPI_CXX_DOUBLE_COMPLEX;
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
#define AU_Bcast(data_bffer_p, count_p, datatype_p, root_p, comm_p)   \
    {                                                                 \
        MPI_Bcast(data_bffer_p, count_p, datatype_p, root_p, comm_p); \
    }

/**
 * @brief get max/min/sum of local_value 
 * 
 * @tparam T 
 * @param local_value : input value
 * @param stats_vector : stats result: max, min, sum
 */
template <typename T>
inline void MPIReduceStats(const T local_value, std::vector<T> &stats_vector)
{
    MPI_Datatype mpi_type = InferMPIType<T>();
    T value_max, value_min, value_sum;
    MPI_Allreduce(&local_value, &value_max, 1, mpi_type, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(&local_value, &value_min, 1, mpi_type, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&local_value, &value_sum, 1, mpi_type, MPI_SUM, MPI_COMM_WORLD);

    stats_vector.resize(3);
    stats_vector[0] = value_max;
    stats_vector[1] = value_min;
    stats_vector[2] = value_sum;
}

#else
#define MPI_COMM_TYPE int
#define AU_WTIME_TYPE time_t
#define AU_WTIME time(NULL)
#define MPI_COMM_WORLD_DEFAULT 0
#define MPI_INFO_NULL_DEFAULT 0
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

/**
 * @brief get max/min/sum of local_value 
 * 
 * @tparam T 
 * @param local_value : input value
 * @param stats_vector : stats result
 */
template <typename T>
inline void MPIReduceStats(const T local_value, std::vector<T> &stats_vector)
{
    stats_vector.resize(3);
    stats_vector[0] = local_value;
    stats_vector[1] = local_value;
    stats_vector[2] = local_value;
}

template <typename T>
inline int InferMPIType()
{
    AU_EXIT("No MPI used to compile wihtout MPI compiled !");
}

inline int InferMPIMergeOp(std::string &opt_str)
{
    AU_EXIT("Should not be here wihtout MPI compiled  !");
}

#define AU_Reduce(local_buffer_p, reduced_buffer_p, size, type, op, root, comm) \
    {                                                                           \
        AU_EXIT("Should not be here wihtout MPI compiled !");                   \
    }

#define AU_Bcast(data_bffer_p, count_p, datatype_p, root_p, comm_p) \
    {                                                               \
        AU_EXIT("Should not be here wihtout MPI compiled !");       \
    }
#endif

#endif