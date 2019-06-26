// overload_array.cpp
// overloading the c++ array subscript operator []
//http://neondataskills.org/HDF5/TimeSeries-Data-In-HDF5-Using-R/

#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include <math.h> /* ceil  and floor*/
#include <cstring>
#include <optional> //Need c++17
#include <limits>
#include "array_udf.h"

using namespace std;

struct Particle
{
    AU_UDT_INIT(Particle)
    float Ux;
    float Uy;
    float Uz;
};

int n_bins = 10;
float local_max = std::numeric_limits<float>::min(), local_min = std::numeric_limits<float>::max();
float global_max, global_min, hist_interval;
std::vector<int> local_hist(n_bins, 0), global_hist(n_bins, 0);

//Find the global Z
inline int MinMax_UDF(const Stencil<Particle> &p)
{
    if (p(0).Ux > local_max)
    {
        local_max = p(0).Ux;
    }

    if (p(0).Ux < local_min)
    {
        local_min = p(0).Ux;
    }

    return 0;
}

//Find the global Z
inline int Binning_UDF(const Stencil<Particle> &p)
{
    int bin_index = floor((p(0).Ux - global_min) / hist_interval);
    local_hist[bin_index] = local_hist[bin_index] + 1;
    return 0;
}

int main(int argc, char *argv[])
{
    int copt, has_set_output_flag = 0;
    int c_size = 3277, o_size = 0;

    char p_file[1024] = "./test-file/electron_50.h5";
    char group[1024] = "/Timestep_50";

    char o_file[1024] = "./test-file/electron_50_filtered.h5";

    int mpi_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    Array<Particle, float> *P = new Array<Particle, float>(AU_VIRTUAL);

    P->PushBackAttribute(AU_NVS, AU_HDF5, p_file, group, "Ux", 0);
    P->PushBackAttribute(AU_NVS, AU_HDF5, p_file, group, "Uy", 0);
    P->PushBackAttribute(AU_NVS, AU_HDF5, p_file, group, "Uz", 0);

    P->Apply(MinMax_UDF);
    MPI_Allreduce(&local_max, &global_max, 1, MPI_FLOAT, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(&local_min, &global_min, 1, MPI_FLOAT, MPI_MAX, MPI_COMM_WORLD);

    hist_interval = (global_max - global_min) / n_bins;

    P->Apply(Binning_UDF);

    MPI_Gather(&local_hist[0], n_bins, MPI_INT, &global_hist[0], n_bins, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < n_bins; i++)
        std::cout << i << " : " << global_hist[i] << "\n";

    P->ReportTime();

    delete P;

    MPI_Finalize();
    return 0;
}
