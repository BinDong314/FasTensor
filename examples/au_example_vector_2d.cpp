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

//Chunk size
#define INPUT_CHK_ROWS 4
#define INPUT_CHK_COLS 4

//Size of the output vector per chunk
#define OUTPUT_VEC_ROWS 2
#define OUTPUT_VEC_COLS 2

/**
 * @brief UDF to support output of 2D vector
 * 
 * @param iStencil : input stencil
 * @return Stencil<std::vector<float>> : output stencil
 */
inline Stencil<std::vector<float>> udf_vector_2d(const Stencil<float> &iStencil)
{

    Stencil<std::vector<float>> oStencil;
    std::vector<float> temp_vec(OUTPUT_VEC_ROWS * OUTPUT_VEC_COLS);

    //each output element is sum of a OUTPUT_VEC_ROWS by OUTPUT_VEC_COLS
    //e.g. input 4 by 4 chunks is
    //      A[0][0] A[0][1] A[0][2] A[0][3]
    //      A[1][0] A[1][1] A[1][2] A[1][3]
    //      A[2][0] A[2][1] A[2][2] A[2][3]
    //      A[3][0] A[3][1] A[3][2] A[3][3]
    //
    // The output B is
    //      B[0][0] =  A[0][0] +  A[0][1] + A[1][0] + A[1][1]
    //      B[0][1] =  A[0][2] +  A[0][3] + A[1][2] + A[1][3]
    //      .....
    for (int i = 0; i < OUTPUT_VEC_ROWS; i++)
    {
        for (int j = 0; j < OUTPUT_VEC_COLS; j++)
        {
            temp_vec[i * OUTPUT_VEC_COLS + j] = iStencil(i * OUTPUT_VEC_ROWS, j * OUTPUT_VEC_COLS) + iStencil(i * OUTPUT_VEC_ROWS, j * OUTPUT_VEC_COLS + 1) + iStencil(i * OUTPUT_VEC_ROWS + 1, j * OUTPUT_VEC_COLS) + iStencil(i * OUTPUT_VEC_ROWS + 1, j * OUTPUT_VEC_COLS + 1);
        }
    }
    oStencil = temp_vec;

    PrintVector("temp_vec = ", temp_vec);
    std::vector<size_t> vector_shape(2);
    vector_shape[0] = OUTPUT_VEC_ROWS;
    vector_shape[1] = OUTPUT_VEC_COLS;
    oStencil.SetOutputVectorShape(vector_shape);
    return oStencil;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {4, 4};
    std::vector<int> overlap_size = {0, 0};

    //Orginal data set
    Array<float> *A = new Array<float>("EP_HDF5:./test-data/testf-16x16-vector-2d.h5:/testg/testd", chunk_size, overlap_size);

    //Results data to be stored in a file
    Array<float> *B = new Array<float>("EP_HDF5:./test-data/testf-16x16-vector-2d-output.h5:/testg/testd");

    //Set the direction to flat vector of output
    //A->SetVectorDirection(AU_FLAT_OUTPUT_ROW);

    //Skip paramter, run the udf_vector on the first cell of each 4 by 4 chunk
    std::vector<int> skip_size = {4, 4};
    A->EnableApplyStride(skip_size);

    //Run udf_vector
    A->Apply(udf_vector_2d, B);

    //std::vector<float> AV = A->ReadArray(std::vector<unsigned long long> (0,0), std::vector<unsigned long long> (16,16));
    //std::vector<float> BV = B->ReadArray();

    //Clear
    delete A;
    delete B;

    AU_Finalize();

    return 0;
}
