/*
 *ArrayUDF Copyright (c) 2017, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy).  All rights reserved.

 *If you have questions about your rights to use or distribute this software, please contact Berkeley Lab's Innovation & Partnerships Office at  IPO@lbl.gov.

* NOTICE. This Software was developed under funding from the U.S. Department of Energy and the U.S. Government consequently retains certain rights. As such, the U.S. Government has been granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software to reproduce, distribute copies to the public, prepare derivative works, and perform publicly and display publicly, and to permit other to do so. 
 */
/**
 *
 * Email questions to dbin@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include "au.h"

using namespace std;
using namespace AU;

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    //Input data
    Array<float> *A = new Array<float>("EP_HDF5:./test-data/testf-16x16-array.h5:/testg/testd");

    /**
     * Read data point
     * 
     */
    std::cout << "Read diagonal cells, via (...) operator: \n";
    for (int i = 0; i < 16; i++)
    {
        std::cout << A->operator()(i, i) << " , ";
    }
    std::cout << " \n\n";

    std::cout << "Read diagonal cells, via GetValue() method: \n";
    //Get the data by point, same as above operation
    for (int i = 0; i < 16; i++)
    {
        std::cout << A->GetValue(i, i) << " , ";
    }
    std::cout << " \n\n";

    /**
     * Write data point
     * 
     */
    for (int i = 0; i < 16; i++)
    {
        A->SetValue(0.001, i, i);
    }
    std::cout << "Read diagonal cells, after SetValue(0.001, ...) method: \n";

    for (int i = 0; i < 16; i++)
    {
        std::cout << A->GetValue(i, i) << " , ";
    }
    std::cout << " \n\n";

    /**
     * Read  vector
     * 
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
    for (int i = 0; i < 6 * 6; i++)
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

    delete A;

    AU_Finalize();

    return 0;
}
