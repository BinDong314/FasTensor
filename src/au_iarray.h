

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

#ifndef AU_IARRAY_H
#define AU_IARRAY_H

#include <vector>
#include "au_type.h"

/**
 * @brief array of local data
 * 
 */
struct iArrayLocal
{
    int rank;
    std::vector<size_t> count;      //Size for each dim
    std::vector<size_t> offset;     //Starting point for each dim
    std::vector<size_t> row_stride; // rowStride = count
    void *local_array_data;

    iArrayLocal(std::vector<int> &count_p, std::vector<int> &row_stride_p)
    {
        count = count_p;
        row_stride = row_stride_p;
        rank = count_p.size();
        offset.resize(rank);
        for (int i = 0; i < rank; i++)
            offset[i] = 0;
    }

    iArrayLocal(std::vector<int> &count_p, std::vector<int> row_stride_p, std::vector<int> offset_p)
    {
        count = count_p;
        row_stride = row_stride_p;
        offset = offset_p;
        rank = count_p.size();
    }

    iArrayLocal()
    {
    }
};

/**
 * @brief global array
 * 
 */
struct iArray
{
    //for portability, they are au_mpi_size_global, au_mpi_rank_global
    int mpi_size;
    int mpi_rank;

    //Global array infomation
    //Same across all process
    int array_rank;
    AuEndpointDataType array_data_element_type;
    std::vector<unsigned long long> g_dimension;
    std::vector<int> g_ghost_size;
    std::vector<int> g_chunk_size; //g_chunk_size is different from chunK_size of Array in au_array.h
                                   //g_chunk_size is the size of local cached data
                                   //we only supported regular chunk, i.e., each rank has continious data
                                   //  and sum(g_dimension/g_chunk_size) = mpi_size

    //Local array location within the global array
    //l_start and l_count is the for the data without ghost_size
    //l_offset is the place to start the
    std::vector<unsigned long long> l_start;
    std::vector<unsigned long long> l_count;
    std::vector<unsigned long long> l_start_ghost; //l_start - g_ghost_size
    std::vector<unsigned long long> l_count_ghost; //l_start + lount + g_ghost_size

    //local_array contains data from l_start_ghost to l_count_ghost
    iArrayLocal *local_array;

    iArray(std::vector<unsigned long long> &g_dimension_p, std::vector<int> &g_ghost_size_p, std::vector<int> &g_chunk_size_p, int mpi_size_p, int mpi_rank_p)
    {
        g_dimension = g_dimension_p;
        g_ghost_size = g_ghost_size_p;
        g_chunk_size = g_chunk_size_p;
        mpi_size = mpi_size_p;
        mpi_rank = mpi_rank_p;
        array_rank = g_dimension.size();
    }
};

#endif
