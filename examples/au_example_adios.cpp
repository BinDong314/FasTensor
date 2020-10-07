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

    std::vector<unsigned long long> A_size = {8, 8};
    //Input data
    Array<float> *A = new Array<float>("EP_ADIOS:./test-data/adios_test.bp:testv", A_size);

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
    std::cout << "  ... done \n\n";

    std::cout << "Read all cells, via GetValue() method: \n";
    //Get the data by point, same as above operation
    for (int i = 0; i < A_size[0]; i++)
    {
        for (int j = 0; j < A_size[1]; j++)
        {
            std::cout << A->GetValue(i, j) << " , ";
        }
        std::cout << "\n";
    }
    std::cout << "  ... done \n\n";
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

    /*
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
*/
    delete A;

    AU_Finalize();

    return 0;
}
