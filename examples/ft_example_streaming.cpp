#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include "ft.h"
#include <filesystem>
namespace fs = std::filesystem;
using namespace FT;

inline Stencil<float> udf_1f1p(const Stencil<float> &iStencil)
{
    Stencil<float> oStencil;
    oStencil = iStencil(0, 0) * 2.0;
    return oStencil;
}

int main(int argc, char** argv) {
    FT_Init(argc,argv);

    if (ft_size < 2) {
        std::cerr << "This program requires at least 2 MPI processes." << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
        
    }

    std::vector<int> cs  = {128,128};
    auto input_stream = new Array<float>("STREAM:EP_HDF5:"+ fs::current_path().string() 
                                        + ":/testg/testd:(^|.*/)stream_\\d{3}\\.h5$",
                                        cs,
                                        {},
                                        MPI_COMM_WORLD);
    std::cout << fs::current_path().string() <<std::endl;                                    
    Array<float>* output_stream = new Array<float>("STREAM:EP_HDF5:"+  fs::current_path().string() + "/:/testg/testd",
                                                {},{},MPI_COMM_NULL);
    input_stream->Transform(udf_1f1p,output_stream);
    

    FT_Finalize();
    return 0;
}