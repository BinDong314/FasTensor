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

/**
 * @brief
 *   2 processes (p1, p2) and 5 chunks (c1, c2, c3, c4, c5)
 *   Results of different methods:
 *   - CHUNK_SCHEDULING_RR Round Robin:
 *                p1 <- c1 c3 c5
 *                p3 <- c2 c4
 *  - CHUNK_SCHEDULING_CR Contiguous Range:
 *                p1 <- c1 c2 c3
 *                p3 <- c4 c5
 */
#ifndef FT_CHUNK_SCHEDULING_H
#define FT_CHUNK_SCHEDULING_H
typedef enum ChunkSchedulingMethodEnum
{
    CHUNK_SCHEDULING_RR = 0, // Round Robin (default)
    CHUNK_SCHEDULING_CR = 1, // Contiguous Range
} ChunkSchedulingMethod;

/**
 * @brief
 *
 * @param data_total_chunks_p
 * @param ft_rank_p
 * @param ft_size_p
 * @return true
 * @return false
 *  5 0 2 -> 3, 5 1 2 -> 2,
 */
inline int CRMyMaxChunks(unsigned long long data_total_chunks_p, int ft_rank_p, int ft_size_p)
{
    if (data_total_chunks_p % ft_size_p == 0)
    {
        return data_total_chunks_p / ft_size_p;
    }
    else
    {
        if (ft_rank_p < (data_total_chunks_p % ft_size_p))
        {
            return data_total_chunks_p / ft_size_p + 1;
        }
        else
        {
            return data_total_chunks_p / ft_size_p;
        }
    }
}

/**
 * @brief
 *
 * @param data_total_chunks_p
 * @param ft_rank_p
 * @param ft_size_p
 * @return true
 * @return false
 *  5 0 2 -> 0, 5 1 2 -> 2,
 */
inline int CRMyStartChunk(unsigned long long data_total_chunks_p, int ft_rank_p, int ft_size_p)
{
    if (data_total_chunks_p % ft_size_p == 0)
    {
        return data_total_chunks_p / ft_size_p * ft_rank_p;
    }
    else
    {
        // unsigned long long temp_max = data_total_chunks_p / ft_size_p;
        // unsigned long long temp_left_over = data_total_chunks_p % ft_size_p;
        if (ft_rank_p < (data_total_chunks_p % ft_size_p))
        {
            return (data_total_chunks_p / ft_size_p + 1) * ft_rank_p;
        }
        else
        {
            return (data_total_chunks_p / ft_size_p) * ft_rank_p + data_total_chunks_p % ft_size_p;
        }
    }
}

inline int CRMyLastChunks(unsigned long long data_total_chunks_p, int ft_rank_p, int ft_size_p)
{
    int start_chunk = CRMyStartChunk(data_total_chunks_p, ft_rank_p, ft_size_p);
    std::cout << "CRMyStartChunk(" << data_total_chunks_p << ", " << ft_rank_p << ", " << ft_size_p << ") =" << start_chunk << "\n";
    int max_chunks = CRMyMaxChunks(data_total_chunks_p, ft_rank_p, ft_size_p);
    std::cout << "CRMyMaxChunks(" << data_total_chunks_p << ", " << ft_rank_p << ", " << ft_size_p << ") =" << max_chunks << "\n";
    return CRMyStartChunk(data_total_chunks_p, ft_rank_p, ft_size_p) + CRMyMaxChunks(data_total_chunks_p, ft_rank_p, ft_size_p);
}
#endif