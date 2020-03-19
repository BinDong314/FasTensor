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

/**
 * @brief Example to sum each row of A (8 x 8)
 *        Store result in H (8 x 2)
 *        H[0,0] = sum (A[0,0], A[0,1], A[0,2], A[0,3])  
 *        H[0,1] = sum (A[0,4], A[0,5], A[0,6], A[0,7])  
 *        H[1,0] = sum (A[1,0], A[1,1], A[1,2], A[1,3]) 
 *        ...  ...
 *       
 *        A points to  a list of files and each have size of 8 x 8.
 */

std::vector<unsigned long long> H_size = {8, 2};
Array<float> *H;

inline Stencil<float> udf_hist_2d(const Stencil<float> &iStencil)
{

    float temp_value = 0;
    int temp_index;
    std::vector<unsigned long long> temp_coord = iStencil.GetCoordinate();
    temp_index = temp_coord[0];

    for (int i = 0; i < 4; i++)
        temp_value += iStencil(0, i);
    H->SetValue(H->GetValue(temp_index, 0) + temp_value, temp_index, 0);
    //std::cout << "temp_index = " << temp_index << ", temp_value 1 =  " << temp_value << " H->GetValue(temp_index, 0) =" << H->GetValue(temp_index, 0) << std::endl;

    temp_value = 0;
    for (int i = 4; i < 8; i++)
        temp_value += iStencil(0, i);
    H->SetValue(H->GetValue(temp_index, 1) + temp_value, temp_index, 1);

    //std::cout << "temp_index = " << temp_index << ", temp_value 2 =  " << temp_value << std::endl;

    return 0;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {8, 8};
    std::vector<int> overlap_size = {0, 0};

    H = new Array<float>("EP_MEMORY", H_size);

    //Input data, 8 by 8
    Array<float> *A = new Array<float>("EP_DIR:EP_HDF5:./test-data/test_1f1p_dir_2d:/testg/testd", chunk_size, overlap_size);

    //Initialize H
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            H->SetValue(0, i, j);
        }
    }

    //Result data
    H->Clone();

    std::vector<int> skip_size = {1, 8};
    A->EnableApplyStride(skip_size);

    //Run
    A->Apply(udf_hist_2d);

    H->Merge(AU_SUM);

    H->Nonvolatile("EP_HDF5:./test-data/test_1f1p_dir_2d_result.h5:/testg/testd");

    //Clear
    delete A;
    delete H;

    AU_Finalize();

    return 0;
}