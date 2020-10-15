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
#include <chrono>
#include <ctime>

#include "ft.h"

using namespace std;
using namespace FT;

//UDF One: duplicate the original data
inline Stencil<float> udf_hello_world(const Stencil<float> &iStencil)
{
    Stencil<float> oStencil;
    oStencil = iStencil(0, 0) * 2.0;
    return oStencil;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {4, 16};
    std::vector<int> overlap_size = {1, 1};

    //Input data
    Array<float> *A = new Array<float>("EP_HDF5:./test-data/testf-16x16-tag.h5:/testg/testd", chunk_size, overlap_size);

    //Result data
    Array<float> *B = new Array<float>("EP_HDF5:./test-data/testf-16x16-tag-output.h5:/testg/testd");

    auto start = std::chrono::system_clock::now();
    //Run
    A->Apply(udf_hello_world, B);
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::string end_time_str(std::ctime(&end_time));
    B->SetTag("FinishedAt", end_time_str);

    double elapsed_seconds_count = elapsed_seconds.count();
    B->SetTag("ElapsedTime", elapsed_seconds_count);

    std::vector<float> data_size = {16, 16};
    B->SetTag("DataSize", data_size);

    std::string end_time_str_get;
    B->GetTag("FinishedAt", end_time_str_get);
    std::cout << "Get FinishedAt: " << end_time_str_get;

    double elapsed_seconds_count_get;
    B->GetTag("ElapsedTime", elapsed_seconds_count_get);
    std::cout << "Get ElapsedTime: " << elapsed_seconds_count_get << "\n";

    std::vector<float> data_size_get(2);
    B->GetTag("DataSize", data_size_get);
    std::cout << "Get DataSize: " << data_size_get[0] << ", " << data_size_get[1] << "\n";

    //Clear
    delete A;
    delete B;

    AU_Finalize();

    return 0;
}
