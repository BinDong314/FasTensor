
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

#ifndef AU_LOCAL_MIRROR_H
#define AU_LOCAL_MIRROR_H

#define LOCAL_MIRROR_READ_FLAG 0
#define LOCAL_MIRROR_WRITE_FLAG 1

#include <iostream> // std::cout
#include <sstream>  // std::istringstream
#include <string>   // std::string

#include "au_utility.h"
#include "au_mpi.h"

//see au.h for its definations
extern int au_mpi_size_global;
extern int au_mpi_rank_global;

template <typename T>
void *CreateLocalMirrorHelp(std::string init_value_str, size_t local_mirror_size)
{
    T *local_mirror_buffer_typed = (T *)malloc(local_mirror_size * sizeof(float));
    if (local_mirror_buffer_typed == NULL)
    {
        AU_EXIT("Not enough memory to crate local mirror.");
        return NULL;
    }

    if (init_value_str != "")
    {
        std::istringstream reader(init_value_str);
        T init_val;
        reader >> init_val;
        for (int i = 0; i < local_mirror_size; i++)
        {
            local_mirror_buffer_typed[i] = init_val;
        }
    }
    return (void *)local_mirror_buffer_typed;
}

template <typename T>
int AccessLocalMirrorHelp(void *local_mirror_buffer, std::vector<unsigned long long> &mirror_size,
                          std::vector<unsigned long long> &start, std::vector<unsigned long long> &end,
                          void *data, int read_write_code)
{
    T *local_mirror_buffer_typed = (T *)local_mirror_buffer;
    T *data_typed = (T *)data;
    switch (mirror_size.size())
    {
    case 1:
    {
        unsigned long long data_typed_offset = 0;
        for (unsigned long long i = start[0]; i <= end[0]; i++)
        {
            if (read_write_code == LOCAL_MIRROR_READ_FLAG)
            {
                data_typed[data_typed_offset] = local_mirror_buffer_typed[i];
            }
            else
            {
                local_mirror_buffer_typed[i] = data_typed[data_typed_offset];
            }
            data_typed_offset++;
        }
        break;
    }
    case 2:
    {
        unsigned long long offset;
        unsigned long long data_typed_offset = 0;
        std::vector<unsigned long long> coodinate_temp(2);
        for (unsigned long long i = start[0]; i <= end[0]; i++)
        {
            for (unsigned long long j = start[1]; j <= end[1]; j++)
            {
                coodinate_temp[0] = i;
                coodinate_temp[1] = j;
                ROW_MAJOR_ORDER_MACRO(mirror_size, mirror_size.size(), coodinate_temp, offset);
                //std::cout << "coodinate_temp =" << i << ", " << j << ", offset = " << offset << ", data_typed[offset] =" << data_typed[offset] << ", buffer_typed[offset]=" << local_mirror_buffer_typed[offset] << std::endl;
                if (read_write_code == LOCAL_MIRROR_READ_FLAG)
                {
                    data_typed[data_typed_offset] = local_mirror_buffer_typed[offset];
                }
                else
                {
                    local_mirror_buffer_typed[offset] = data_typed[data_typed_offset];
                }
            }
        }
        break;
    }
    break;
    case 3:
    {
        unsigned long long offset;
        unsigned long long data_typed_offset = 0;
        std::vector<unsigned long long> coodinate_temp(3);
        for (unsigned long long i = start[0]; i <= end[0]; i++)
        {
            for (unsigned long long j = start[1]; j <= end[1]; j++)
            {
                for (unsigned long long k = start[2]; k <= end[2]; k++)
                {
                    coodinate_temp[0] = i;
                    coodinate_temp[1] = j;
                    coodinate_temp[2] = k;
                    ROW_MAJOR_ORDER_MACRO(mirror_size, mirror_size.size(), coodinate_temp, offset);
                    if (read_write_code == LOCAL_MIRROR_READ_FLAG)
                    {
                        data_typed[data_typed_offset] = local_mirror_buffer_typed[offset];
                    }
                    else
                    {
                        local_mirror_buffer_typed[offset] = data_typed[data_typed_offset];
                    }
                }
            }
        }
        break;
    }
    default:
        break;
    }

    return 0;
}

template <typename T>
T *MergeMirrorsHelp(void *local_mirror_buffer, unsigned long long &local_mirror_size, std::string &opt_str)
{
    T *reduced_mirror_buffer = NULL;
    if (!au_mpi_rank_global)
    {
        reduced_mirror_buffer = (T *)malloc(local_mirror_size * sizeof(T));
    }
    AU_MPI_Datatype merge_data_type = InferMPIType<T>();
    AU_MPI_Op merge_op = InferMPIMergeOp(opt_str);

    AU_Reduce(local_mirror_buffer, reduced_mirror_buffer, local_mirror_size, merge_data_type, merge_op, 0, MPI_COMM_WORLD_DEFAULT);
    return reduced_mirror_buffer;
}

template <typename T>
void BcastHelp(void *local_mirror_buffer, unsigned long long &local_mirror_size)
{
    AU_MPI_Datatype data_type = InferMPIType<T>();

    AU_Bcast(local_mirror_buffer, local_mirror_size, data_type, 0, MPI_COMM_WORLD_DEFAULT);
}

#endif