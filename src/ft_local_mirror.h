

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

#ifndef AU_LOCAL_MIRROR_H
#define AU_LOCAL_MIRROR_H

#define LOCAL_MIRROR_READ_FLAG 0
#define LOCAL_MIRROR_WRITE_FLAG 1

#include <iostream> // std::cout
#include <sstream>  // std::istringstream
#include <string>   // std::string

#include "ft_utility.h"
#include "ft_mpi.h"

//see au.h for its definations
extern int ft_size;
extern int ft_rank;

template <typename T>
void *CreateLocalMirrorHelp(std::string init_value_str, size_t local_mirror_size)
{
    T *local_mirror_buffer_typed = (T *)malloc(local_mirror_size * sizeof(T));
    if (local_mirror_buffer_typed == NULL)
    {
        AU_EXIT("Not enough memory to crate local mirror.");
        return NULL;
    }

    //Set it to zeror by default
    std::memset(local_mirror_buffer_typed, 0, local_mirror_size * sizeof(T));

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

    std::vector<unsigned long long> read_count;
    unsigned long long element_count = 1;
    read_count.resize(mirror_size.size());
    for (int i = 0; i < mirror_size.size(); i++)
    {
        read_count[i] = end[i] - start[i] + 1;
        element_count = element_count * read_count[i];
    }

    if (read_count == mirror_size)
    {
        //std::cout << "Access local whole ! \n";
        if (read_write_code == LOCAL_MIRROR_READ_FLAG)
        {
            std::memcpy(data, local_mirror_buffer, element_count * sizeof(T));
        }
        else
        {
            std::memcpy(local_mirror_buffer, data, element_count * sizeof(T));
        }
        return 0;
    }
    // else
    // {
    //     PrintVector("read_count: ", read_count);
    //     PrintVector("mirror_size: ", mirror_size);
    //     //std::cout << "Not Access local whole ! \n";
    // }

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
                data_typed_offset++;
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
                    data_typed_offset++;
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
    if (!ft_rank)
    {
        reduced_mirror_buffer = (T *)malloc(local_mirror_size * sizeof(T));
    }
    AU_MPI_Datatype merge_data_type = InferMPIType<T>();
    AU_MPI_Op merge_op = InferMPIMergeOp(opt_str);

    AU_Reduce(local_mirror_buffer, reduced_mirror_buffer, local_mirror_size, merge_data_type, merge_op, 0, MPI_COMM_WORLD_DEFAULT);

    if (!ft_rank)
    {
        std::memcpy(local_mirror_buffer, reduced_mirror_buffer, local_mirror_size * sizeof(T));
    }

    return reduced_mirror_buffer;
}

template <typename T>
void BcastHelp(void *local_mirror_buffer, unsigned long long &local_mirror_size)
{
    AU_MPI_Datatype data_type = InferMPIType<T>();

    AU_Bcast(local_mirror_buffer, local_mirror_size, data_type, 0, MPI_COMM_WORLD_DEFAULT);
}

#endif