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

#ifndef AU_ARRAY_ITERATOR_H
#define AU_ARRAY_ITERATOR_H

#include <array>
#include <iostream>
#include <vector>

/**
 * @brief get the next ordinates_p from current ordinates_p 
 *        call 
 * @param ordinates_p, input and output for  current and next coordinate
 * @param start_p  is the start range, n-dimensional, including 
 * @param end_p is the end range, n-dimensional. including
 */
inline void
iterate(std::vector<int> &ordinates_p, const std::vector<int> start_p, const std::vector<int> end_p);

#define ITERATOR_MACRO(ordinates_p, start_p, end_p)                                         \
    {                                                                                       \
        int dimensions = start_p.size();                                                    \
        for (int dimension_index = dimensions - 1; dimension_index >= 0; dimension_index--) \
        {                                                                                   \
            if (ordinates_p[dimension_index] < end_p[dimension_index])                      \
            {                                                                               \
                ordinates_p[dimension_index]++;                                             \
                break;                                                                      \
            }                                                                               \
            ordinates_p[dimension_index] = start_p[dimension_index];                        \
        }                                                                                   \
    }

#endif

/*
//Example file of usage
#include <vector>
#include <iostream>

inline void
iterate(std::vector<int> &ordinates, const std::vector<int> minimums, const std::vector<int> maximums)
{
    int dimensions = minimums.size();
    // iterate over dimensions in reverse...
    for (int dimension = dimensions - 1; dimension >= 0; dimension--)
    {

        if (ordinates[dimension] < maximums[dimension])
        {
            // If this dimension can handle another increment... then done.
            ordinates[dimension]++;
            break;
        }

        // Otherwise, reset this dimension and bubble up to the next dimension to take a look
        ordinates[dimension] = minimums[dimension];
    }
}

int main()
{
    int N = 3;
    std::vector<int> min_c, max_c, ord_c;
    min_c.resize(N);
    max_c.resize(N);
    ord_c.resize(N);

    min_c[0] = 1;
    min_c[1] = 2;
    min_c[2] = 1;

    max_c[0] = 4;
    max_c[1] = 5;
    max_c[2] = 6;

    ord_c[0] = min_c[0];
    ord_c[1] = min_c[1];
    ord_c[2] = min_c[2];

    int maxIterations = 1;
    for (int i = 0; i < N; i++)
    {
        maxIterations = maxIterations * (max_c[i] - min_c[i] + 1);
    }

    for (int iteration = 0; iteration < maxIterations; iteration++)
    {
        for (int i = 0; i < N; i++)
        {
            std::cout << ord_c[i] << " , ";
        }
        std::cout << "\n";
        //iterate(n, ord, min, max)

        iterate(ord_c, min_c, max_c);
    }
}

*/