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

    //Run
    A->Apply(udf_hello_world, B);

    std::cout << setprecision(17);

    std::string test_str = "This is long test string for set tag";
    B->SetTag("TestString", test_str);

    double test_double = 0.00001234;
    B->SetTag("TestDouble", test_double);

    float test_float = 0.12345678;
    B->SetTag("TestFloat", test_float);

    int test_int = 1234;
    B->SetTag("TestInt", test_int);

    std::vector<int> data_size = {16, 16};
    B->SetTag("TestVector", data_size);

    std::string test_str_get;
    B->GetTag("TestString", test_str_get);
    std::cout << "Get TestString: " << test_str_get << "\n";

    double test_double_get;
    B->GetTag("TestDouble", test_double_get);
    std::cout << "Get TestDouble: " << test_double_get << "\n";

    float test_float_get;
    B->GetTag("TestFloat", test_float_get);
    std::cout << "Get TestFloat: " << test_float_get << "\n";

    int test_int_get;
    B->GetTag("TestInt", test_int_get);
    std::cout << "Get TestInt: " << test_int_get << "\n";

    std::vector<int> data_size_get(2);
    B->GetTag("TestVector", data_size_get);
    std::cout << "Get TestVector: " << data_size_get[0] << ", " << data_size_get[1] << "\n";

    std::vector<std::string> B_tags;
    B->GetAllTagName(B_tags);
    for (auto t : B_tags)
        std::cout << t << "\n";

    //Clear
    delete A;
    delete B;

    AU_Finalize();

    return 0;
}
