/**
 *
 * Author:  Bin Dong, dbin@lbl.gov
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
