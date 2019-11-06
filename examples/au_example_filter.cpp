inline Stencil<float> udf_filter(const Stencil<float> &iStencil)
{

    Stencil<float> oStencil;
    if (iStencil(0) > 100.0)
    {
        oStencil = iStencil(0);
    }
    return oStencil;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {4};
    std::vector<int> overlap_size = {0};

    //Input data
    Array<float> *A = new Array<float>("EP_HDF5:./testf-16x16.h5p:/testg/testd", chunk_size, overlap_size);

    //Result data
    Array<float> *B = new Array<float>("EP_HDF5:./testf-16x16-f1.h5p:/testg/testd");

    //Run
    A->Apply(udf_filter, B);

    //Clear
    delete A;
    delete B;

    AU_Finalize();

    return 0;
}
