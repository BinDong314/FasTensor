//
// This ArrayUDF example code is for calculating the FFT/IFFT for DAS data
//
// Author: Bin Dong  2019 (Reviewed by Xin Xing)
//
#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include <math.h> /* ceil  and floor*/
#include <cstring>
#include "array_udf.h"

using namespace std;

#define NAME_LENGTH 1024
// help functions
void printf_help(char *cmd);

std::string i_file("./westSac_170802100007_AF.h5");
std::string o_file("./westSac_170802100007_AF_Stacked.h5");
std::string i_group("/");
std::string o_group("/");
std::string i_dataset("Xcorr");
std::string o_dataset("XcorrStacked");
int auto_chunk_dims_index = 2;
int row_major_flag = 0;
unsigned long long n0;

inline float Stack_UDF(const Stencil<float> &c)
{
    float stack_value = 0;
    for (int i = 0; i < n0; i++)
    {

        if (row_major_flag == 0)
        {
            stack_value += (double)c(i, 0);
        }
        else
        {
            stack_value += (double)c(0, i);
        }
    }
    stack_value = stack_value / n0;
    return stack_value;
}

int main(int argc, char *argv[])
{
    int config_file_set_flag = 0;

    char config_file[NAME_LENGTH] = "./das-fft-full.config";

    int copt, mpi_rank, mpi_size;
    while ((copt = getopt(argc, argv, "o:i:g:u:s:x:rhc:")) != -1)
        switch (copt)
        {
        case 'o':
            o_file.assign(optarg);
            break;
        case 'i':
            i_file.assign(optarg);
            break;
        case 'g':
            i_group.assign(optarg);
            break;
        case 'u':
            o_group.assign(optarg);
            break;
        case 'x':
            i_dataset.assign(optarg);
            break;
        case 's':
            o_dataset.assign(optarg);
            break;
        case 'r':
            row_major_flag = 1;
            auto_chunk_dims_index = 0;
            break;
        case 'h':
            printf_help(argv[0]);
            exit(0);
            break;
        case 'c':
            memset(config_file, 0, sizeof(config_file));
            strcpy(config_file, optarg);
            config_file_set_flag = 1;
            break;
        default:
            printf("Wrong option [%c] for %s \n", copt, argv[0]);
            printf_help(argv[0]);
            exit(-1);
            break;
        }

    //Do some intializatin work for parallel computing
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    //if (config_file_set_flag)
    //    read_config_file(config_file, mpi_rank);

    //Declare the input and output AU::Array
    AU::Array<float> *IFILE = new AU::Array<float>(AU_NVS, AU_HDF5, i_file, i_group, i_dataset, auto_chunk_dims_index);
    AU::Array<float> *OFILE = new AU::Array<float>(AU_COMPUTED, AU_HDF5, o_file, o_group, o_dataset, auto_chunk_dims_index);

    //Find and set chunks_size to split array for parallel processing
    std::vector<unsigned long long> i_file_dim = IFILE->GetDimSize();

    std::vector<int> strip_size(2);
    if (row_major_flag == 1)
    {
        n0 = i_file_dim[1];
        strip_size[0] = 1;
        strip_size[1] = i_file_dim[1];
        IFILE->SetApplyStripSize(strip_size);
    }
    else
    {
        n0 = i_file_dim[0];
        strip_size[0] = i_file_dim[0];
        strip_size[1] = 1;
        IFILE->SetApplyStripSize(strip_size);
    }

    //Run FFT
    IFILE->Apply(Stack_UDF, OFILE);
    IFILE->ReportTime();

    delete IFILE;
    delete OFILE;

    MPI_Finalize();
    return 0;
}

void printf_help(char *cmd)
{
    char *msg = (char *)"Usage: %s [OPTION]\n\
      	  -h help (--help)\n\
          -i input file\n\
          -o output file\n\
	      -g group name (path) for input dataset \n\
          -u group name (path) for output dataset \n\
          -t dataset name for intput time series \n\
          -x dataset name for output correlation \n\
          -w window size (only used when window size is different from chunk_size[0]) \n\
          -m index of Master channel (0 by default )\n\
          -r FFT in [Row]-direction([Column]-direction by default) \n\
          -c file for parameters (has high priority than commands if existing) \n\
          Example: mpirun -n 1 %s -i ./westSac_170802100007_AF.h5 -o ./westSac_170802100007_AF_Stacked.h5  -x /Xcorr -s /XcorrStacked\n";

    fprintf(stdout, msg, cmd, cmd);
}
