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

#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include "ft.h"

using namespace std;
using namespace FT;

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    std::vector<unsigned long long> A_size = {8, 8};
    //Input data
    Array<float> *A = new Array<float>("EP_PNETCDF:./test-data/pnetcdf_test.nc:testv", A_size);

    /**
     * Write some data
     */
    std::cout << "Set value, via (...) operator: \n";
    for (int i = 0; i < A_size[0]; i++)
    {
        for (int j = 0; j < A_size[1]; j++)
        {
            A->SetValue(i + j + 0.001, i, j);
        }
    }
    std::cout << " \n\n";

    std::cout << "Read all cells, via GetValue() method: \n";
    //Get the data by point, same as above operation
    for (int i = 0; i < A_size[0]; i++)
    {
        for (int j = 0; j < A_size[1]; j++)
        {
            std::cout << A->GetValue(i, j) << " , ";
        }
        std::cout << " \n";
    }
    std::cout << " \n\n";
    /**
     * Read  vector
     */
    std::vector<unsigned long long> start = {0, 0};
    std::vector<unsigned long long> end = {5, 5};
    std::vector<float> datav(6 * 6);
    A->ReadEndpoint(start, end, static_cast<void *>(datav.data()));

    std::cout << "Read vector of cells [0,0]-[5,5], via ReadEndpoint() method: \n";
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            std::cout << datav[i * 6 + j] << ", ";
        }
        std::cout << " \n";
    }
    std::cout << " \n";

    /**
     * Update/write data vector
     * 
     */
    //Let's update the data and write & read-back
    /*  for (int i = 0; i < 6 * 6; i++)
    {
        datav[i] = datav[i] - 1.1;
    }
    A->WriteEndpoint(start, end, static_cast<void *>(datav.data()));

    std::vector<float> datav_updated(6 * 6);
    A->ReadEndpoint(start, end, static_cast<void *>(datav_updated.data()));
    std::cout << "Read vector of cells [0,0]-[5,5], after WriteEndpoint() /w each cell - 1.1: \n";
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            std::cout << datav_updated[i * 6 + j] << ", ";
        }
        std::cout << " \n";
    }
    std::cout << " \n\n";

*/
    delete A;

    AU_Finalize();

    return 0;
}
