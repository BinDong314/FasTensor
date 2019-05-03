//
// This ArrayUDF example code is for calculating the FFT/IFFT for DAS data
// with a few pre-processing steps and post-porcessing steps.
// The pre-proccsing steps include
//     detread
//     filtfilt
//     esample
//     MOVING_MEAN
//     interp1
//     Spectral whitening
// The post-processing steps are:
//     frequency domain cross-correlation
//
// All these steps are documented in "doc/preprocess.pdf" by Xin Xing
// The das-fft.h contains the code for the these steps.
//
// The code below has following structure:
//      TTF_UDF(){....}           : define function for each channel,
//                                : include all pre-processing steps, FFT/IFFT, post-porcessing steps
//      Array A( ...HDF5 file..)  : define pointer to DAS data in HDF5 file
//      A->Apply(FFT_UDF)         : run TTF_UDF over all channels
//
// Please use the "-h" to get the usage information
//
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

/*
 * Note that: das-fft.h contains all parameters & used variables & functions. 
 *            Go to "das-fft.h" to find more details
 */
#include "das-fft.h"

using namespace std;

// help functions
void printf_help(char *cmd);

/*
 * This function describes the operation per channel
 *  The return value is a coorelation vector
 * See more details at das-fft.h
 */
inline std::vector<float> FFT_UDF(const Stencil<int> &c)
{
    for (int bi = 0; bi < window_batch; bi++)
    {
        X.resize(n0);
        TC.resize(nfft);
        for (int i = 0; i < n0; i++)
        {
            if (row_major_flag == 0)
            {
                X[i] = (double)c(i + bi * n0, 0);
            }
            else
            {
                X[i] = (double)c(0, i + bi * n0);
            }
        }

        //X is the input
        //TC is a extra cache space for calculation
        //gatherXcorr_per_batch is output
        //master_fft is the FFT for master
        //bi is the index of the current window
        //nfft is the size of current window
        FFT_PROCESSING(X, TC, gatherXcorr_per_batch, master_fft, bi, nfft);
        std::copy_n(gatherXcorr_per_batch.begin(), nXCORR, gatherXcorr.begin() + bi * nXCORR);
    }
    return gatherXcorr;
}

int main(int argc, char *argv[])
{
    char i_file[NAME_LENGTH] = "./westSac_170802100007.h5";
    char o_file[NAME_LENGTH] = "./westSac_170802100007_AF.h5";
    char group[NAME_LENGTH] = "/"; //both input and output file share the same group and dataset name
    char i_dataset[NAME_LENGTH] = "/DataTimeChannel";
    char o_dataset[NAME_LENGTH] = "/Xcorr";

    int copt, mpi_rank, mpi_size;
    while ((copt = getopt(argc, argv, "o:i:g:t:x:m:w:rh")) != -1)
        switch (copt)
        {
        case 'o':
            memset(o_file, 0, sizeof(o_file));
            strcpy(o_file, optarg);
            break;
        case 'i':
            memset(i_file, 0, sizeof(i_file));
            strcpy(i_file, optarg);
            break;
        case 'g':
            memset(group, 0, sizeof(group));
            strcpy(group, optarg);
            break;
        case 't':
            memset(i_dataset, 0, sizeof(i_dataset));
            strcpy(i_dataset, optarg);
            break;
        case 'x':
            memset(o_dataset, 0, sizeof(o_dataset));
            strcpy(o_dataset, optarg);
            break;
        case 'w':
            set_window_size_flag = 1;
            user_window_size = atoi(optarg);
            break;
        case 'm':
            MASTER_INDEX = atoi(optarg);
            break;
        case 'r':
            row_major_flag = 1;
            break;
        case 'h':
            printf_help(argv[0]);
            exit(0);
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

    //Declare the input and output array
    Array<int, std::vector<float>> *IFILE = new Array<int, std::vector<float>>(AU_NVS, AU_HDF5, i_file, group, i_dataset, chunk_size, ghost_size);
    Array<std::vector<float>> *OFILE = new Array<std::vector<float>>(AU_COMPUTED, AU_HDF5, o_file, group, o_dataset, chunk_size, ghost_size);

    //Find and set chunks_size to split array for parallel processing
    std::vector<unsigned long long> i_file_dim = IFILE->GetDimSize();

    //Assign the n0 with the size of the first dimension
    INIT_PARS(mpi_rank, mpi_size, i_file_dim);
    INIT_SPACE();
    IFILE->SetChunkSize(chunk_size);
    IFILE->SetApplyStripSize(strip_size);
    if (row_major_flag == 0)
    {
        IFILE->SetOutputVector(nXCORR * window_batch, 0); //output vector size
    }
    else
    {
        IFILE->SetOutputVector(nXCORR * window_batch, 1); //output vector size
    }

    //Get FFT of master vector
    std::vector<int> masterv;
    masterv.resize(n0);
    std::vector<double> mastervf, masterv_ppf;
    mastervf.resize(n0);
    masterv_ppf.resize(nfft);
    std::vector<unsigned long long> master_start, master_end;
    master_start.resize(2);
    master_end.resize(2);
    for (int bi = 0; bi < window_batch; bi++)
    {
        if (row_major_flag == 0)
        {
            master_start[0] = 0 + bi * n0;
            master_start[1] = MASTER_INDEX;
            master_end[0] = master_start[0] + n0 - 1;
            master_end[1] = MASTER_INDEX;
        }
        else
        {
            master_start[0] = MASTER_INDEX;
            master_start[1] = 0 + bi * n0;
            master_end[0] = MASTER_INDEX;
            master_end[1] = master_start[0] + n0 - 1;
        }
        //Get master chunk's data and store in double type vector
        IFILE->ReadData(master_start, master_end, masterv);
        for (int i = 0; i < n0; i++)
        {
            mastervf[i] = (double)(masterv[i]);
        }
        FFT_PREPROCESSING(mastervf, masterv_ppf); //masterv_ppf is result
        //master_processing(mastervf, masterv_ppf);
        INIT_FFTW(fft_in, masterv_ppf, nPoint, nfft, fft_out);
        FFT_HELP_W(nfft, fft_in, fft_out, FFTW_FORWARD);
        for (int j = 0; j < nfft; j++)
        {
            master_fft[bi * nfft + j][0] = fft_out[j][0];
            master_fft[bi * nfft + j][1] = fft_out[j][1];
        }
    }
    masterv_ppf.clear();
    mastervf.clear();
    masterv.clear();

    //Run FFT
    IFILE->Apply(FFT_UDF, OFILE);
    IFILE->ReportTime();

    delete IFILE;
    delete OFILE;

    CLEAR_SPACE();

    MPI_Finalize();
    return 0;
}

void printf_help(char *cmd)
{
    char *msg = (char *)"Usage: %s [OPTION]\n\
      	  -h help (--help)\n\
          -i input file\n\
          -o output file\n\
	      -g group name (path) for both input and output \n\
          -t dataset name for intput time series \n\
          -x dataset name for output correlation \n\
          -w window size (only used when window size is different from chunk_size[0]) \n\
          -m index of Master channel (0 by default )\n\
          -r FFT in [Row]-direction([Column]-direction by default) \n\
          Example: mpirun -n 1 %s -i ./test-data/fft-test.h5 -o ./test-data/fft-test.arrayudf.h5  -g / -t /white -x /Xcorr\n";

    fprintf(stdout, msg, cmd, cmd);
}