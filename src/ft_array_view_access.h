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

#ifndef AU_ARRAY_VIEW_ACCESS_H
#define AU_ARRAY_VIEW_ACCESS_H

#include <vector>
#include <cstdint>
#include <cstring>
#include "ft_array_iterator.h"
#include "ft_utility_macro.h"

#define ARRAY_VIEW_READ 0
#define ARRAY_VIEW_WRITE 1

/**
 * @brief Access a subset between v_bu (view buffer) and a_bu (array buffer)
 * 
 * @param v_bu: view buffer
 * @param v_st: start of view buffer
 * @param a_bu: array buffer
 * @param a_st: start of array buffer
 * @param count_n: element to access
 * @param rm:  flag to read or write, ARRAY_VIEW_READ and ARRAY_VIEW_WRITE
 */
#define VIEW_ACCESS_HELP_V(v_bu, v_st, a_bu, a_st, count_n, rw)                                 \
    {                                                                                           \
        if (rw == ARRAY_VIEW_READ)                                                              \
        {                                                                                       \
            std::memcpy();                                                                      \
            std::copy(a_bu.begin() + a_st, a_bu.begin() + a_st + count_n, v_bu.begin() + v_st); \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            std::copy(v_bu.begin() + v_st, v_bu.begin() + v_st + count_n, a_bu.begin() + a_st); \
        }                                                                                       \
    }
/**
 * @brief Access an view (subset) of an array 
 * 
 * @param view_buffer : pointer to the buffer for the view
 * @param array_buffer : pointer to the buffer for the array
 * @param array_size : the size for the array
 * @param start : the start address of the view
 * @param end : the end address of the view
 * @param read_write_code : mode to read or write, e.g., ARRAY_VIEW_READ , ARRAY_VIEW_WRITE 1
 * @param element_size : the size of each element data
 * @return int : non zero if works, otherwise failed
 */
template <class T>
inline int ArrayViewAccessV(std::vector<T> &view_v, std::vector<T> &array_v, std::vector<unsigned long long> &array_size, std::vector<unsigned long long> &start, std::vector<unsigned long long> &end, int read_write_code)
{
    std::vector<unsigned long long> view_size;
    unsigned long long element_count = 1;
    view_size.resize(array_size.size());
    for (int i = 0; i < array_size.size(); i++)
    {
        view_size[i] = end[i] - start[i] + 1;
        element_count = element_count * view_size[i];
    }

    view_v.resize(element_count);
    if (view_size == array_size)
    {
        VIEW_ACCESS_HELP(view_v, 0, array_v, 0, element_count, read_write_code);
        return 0;
    }

    if (view_size.size() == 1)
    {
        VIEW_ACCESS_HELP(view_v, 0, array_v, start[0], element_count, read_write_code);
        return 0;
    }

    unsigned long long array_buffer_offset = 0, view_buffer_offset = 0;

    if (view_size.size() == 2)
    {
        for (int i = 0; i < view_size[0]; i++)
        {
            array_buffer_offset = array_size[1] * (start[0] + i) + start[1];
            view_buffer_offset = view_size[1] * i;
            //Copy each row
            VIEW_ACCESS_HELP(view_v, view_buffer_offset, array_v, array_buffer_offset, view_size[1], read_write_code);
        }
        return 0;
    }

    //Generic version
    array_buffer_offset = 0, view_buffer_offset = 0;
    std::vector<unsigned long long> ord(start.begin(), start.end());
    for (unsigned long long i = 0; i < element_count; i++)
    {
        ROW_MAJOR_ORDER_MACRO(array_size, array_size.size(), ord, array_buffer_offset);
        MEMCPY_ACCESS_HELP(view_v, view_buffer_offset, array_v, array_buffer_offset, 1, read_write_code);
        view_buffer_offset++;
        ITERATOR_MACRO(ord, start, end); //update ord by increasing "1", row-major order
    }

    return 0;
}

//void* memcpy( void* dest, const void* src, std::size_t count );
//std::memcpy(data, local_mirror_buffer, element_count * sizeof(T));

#define VIEW_ACCESS_HELP_P(v_bu, v_st, a_bu, a_st, count_n, rw, type_size) \
    {                                                                      \
        if (rw == ARRAY_VIEW_READ)                                         \
        {                                                                  \
            std::memcpy(v_bu + v_st, a_bu + a_st, count_n * type_size);    \
        }                                                                  \
        else                                                               \
        {                                                                  \
            std::memcpy(a_bu + a_st, v_bu + v_st, count_n * type_size);    \
        }                                                                  \
    }

template <class T>
inline int ArrayViewAccessP(T *view_v, T *array_v, const std::vector<unsigned long long> array_size, const std::vector<unsigned long long> start, const std::vector<unsigned long long> end, const int read_write_code)
{
    std::vector<unsigned long long> view_size;
    unsigned long long element_count = 1;
    view_size.resize(array_size.size());
    for (int i = 0; i < array_size.size(); i++)
    {
        view_size[i] = end[i] - start[i] + 1;
        element_count = element_count * view_size[i];
    }

    //view_v.resize(element_count);
    if (view_size == array_size)
    {
        VIEW_ACCESS_HELP_P(view_v, 0, array_v, 0, element_count, read_write_code, sizeof(T));
        return 0;
    }

    if (view_size.size() == 1)
    {
        VIEW_ACCESS_HELP_P(view_v, 0, array_v, start[0], element_count, read_write_code, sizeof(T));
        return 0;
    }

    unsigned long long array_buffer_offset = 0, view_buffer_offset = 0;
    if (view_size.size() == 2)
    {
        //PrintVector("view_size =", view_size);
        //PrintVector("array_size =", array_size);

        for (int i = 0; i < view_size[0]; i++)
        {
            // PrintScalar("array_buffer_offset = ", array_buffer_offset);
            // PrintScalar("view_buffer_offset = ", view_buffer_offset);
            array_buffer_offset = array_size[1] * (start[0] + i) + start[1];
            view_buffer_offset = view_size[1] * i;
            //Copy each row
            VIEW_ACCESS_HELP_P(view_v, view_buffer_offset, array_v, array_buffer_offset, view_size[1], read_write_code, sizeof(T));
            //VIEW_ACCESS_HELP(view_v, view_size[1] * i, array_v, array_size[1] * (start[0] + i) + start[1], view_size[1], read_write_code);
        }
        return 0;
    }

    unsigned long long array_buffer_offset_2;
    if (view_size.size() == 3)
    {
        for (int i = 0; i < view_size[0]; i++)
        {
            array_buffer_offset = array_size[2] * array_size[1] * (start[0] + i);
            view_buffer_offset = view_size[2] * view_size[1] * (start[0] + i);
            for (int j = 0; j < view_size[1]; j++)
            {
                VIEW_ACCESS_HELP_P(view_v, view_buffer_offset + view_size[2] * (start[1] + j), array_v, array_buffer_offset + array_size[2] * (start[1] + j), view_size[2], read_write_code, sizeof(T));
            }
        }
        return 0;
    }

    //Generic version
    array_buffer_offset = 0, view_buffer_offset = 0;
    std::vector<unsigned long long> ord(start.begin(), start.end());
    for (unsigned long long i = 0; i < element_count; i++)
    {
        ROW_MAJOR_ORDER_MACRO(array_size, array_size.size(), ord, array_buffer_offset);
        VIEW_ACCESS_HELP_P(view_v, view_buffer_offset, array_v, array_buffer_offset, 1, read_write_code, sizeof(T));
        view_buffer_offset++;
        ITERATOR_MACRO(ord, start, end); //update ord by increasing "1", row-major order
    }

    return 0;
}
#endif