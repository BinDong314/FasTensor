
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

#define VEC_SIZE 8

/**
 * @brief UDF to support output of vector
 * 
 * @param iStencil : input stencil
 * @return Stencil<std::vector<float>> : output stencil
 */
inline Stencil<std::vector<float>> udf_vector(const Stencil<float> &iStencil)
{

    Stencil<std::vector<float>> oStencil;
    std::vector<float> temp_vec(VEC_SIZE);

    for (int i = 0; i < VEC_SIZE; i++)
    {
        temp_vec[i] = iStencil(0, i) * 2.0;
    }
    oStencil = temp_vec;

    std::vector<size_t> vector_shape(2);
    vector_shape[0] = 1;
    vector_shape[1] = VEC_SIZE;
    oStencil.SetOutputVectorShape(vector_shape);
    return oStencil;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {4, 16};
    std::vector<int> overlap_size = {0, 0};

    //Orginal data set
    Array<float> *A = new Array<float>("EP_HDF5:./test-data/testf-16x16-vector.h5:/testg/testd", chunk_size, overlap_size);

    //Results data to be stored in a file
    Array<float> *B = new Array<float>("EP_HDF5:./test-data/testf-16x16-vector-output.h5:/testg/testd");

    //Set the direction to flat vector of output
    //A->SetVectorDirection(AU_FLAT_OUTPUT_ROW);

    //Skip paramter, run the udf_vector on the first cell of each row
    std::vector<int> skip_size = {1, 16};
    A->EnableApplyStride(skip_size);

    //Run udf_vector
    A->Apply(udf_vector, B);

    //Clear
    delete A;
    delete B;

    AU_Finalize();

    return 0;
}
