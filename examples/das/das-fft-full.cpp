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
inline std::vector<float> FFT_UDF(const Stencil<short> &c)
{
    for (int bi = 0; bi < window_batch; bi++)
    {
        au_time_start();
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
            //std::cout << X[i] << ", ";
        }

        //std::cout << "\n";

        au_time_elap("Read Stencil ");

        //X is the input
        //TC is a extra cache space for calculation
        //gatherXcorr_per_batch is output
        //master_fft is the FFT for master
        //bi is the index of the current window
        //nfft is the size of current window
        FFT_PROCESSING(X, TC, gatherXcorr_per_batch, master_fft, bi, nfft);

        au_time_elap("Run FFT ");

        std::copy_n(gatherXcorr_per_batch.begin(), nXCORR, gatherXcorr.begin() + bi * nXCORR);
    }
    return gatherXcorr;
}

std::string i_file("./westSac_170802100007.h5");
std::string o_file("./westSac_170802100007_AF.h5");
std::string i_group("/");
std::string o_group("/");
std::string i_dataset("DataTimeChannel");
std::string o_dataset("Xcorr");
int chunk_size_on_split_dim = 1;
int main(int argc, char *argv[])
{
    int config_file_set_flag = 0;
    char config_file[NAME_LENGTH] = "./das-fft-full.config";

    int copt, mpi_rank, mpi_size;
    while ((copt = getopt(argc, argv, "o:i:g:u:t:x:m:w:rhc:k:")) != -1)
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
        case 't':
            i_dataset.assign(optarg);
            break;
        case 'x':
            o_dataset.assign(optarg);
            break;
        case 'w':
            set_window_size_flag = 1;
            user_window_size = atoi(optarg);
            break;
        case 'm':
            MASTER_INDEX = atoi(optarg);
            break;
        case 'k':
            chunk_size_on_split_dim = atoi(optarg);
            break;
        case 'r':
            row_major_flag = 1;
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

    if (config_file_set_flag)
        read_config_file(config_file, mpi_rank);

    //Declare the input and output AU::Array
    AU::Array<short> *IFILE = new AU::Array<short>(AU_NVS, AU_HDF5, i_file, i_group, i_dataset, auto_chunk_dims_index);
    AU::Array<float> *OFILE = new AU::Array<float>(AU_COMPUTED, AU_HDF5, o_file, o_group, o_dataset, auto_chunk_dims_index);

    //Find and set chunks_size to split array for parallel processing
    std::vector<unsigned long long> i_file_dim;

    //SelectView must be called before ApplyStripSize
    if (enable_view_flag)
    {
        IFILE->SelectView(view_start, view_count, view_os_size, auto_chunk_dims_index);
        i_file_dim = view_count;
    }
    else
    {
        i_file_dim = IFILE->GetDimSize();
    }

    //After this step, n0 is the size of input time series
    INIT_PARS(mpi_rank, mpi_size, i_file_dim);
    INIT_SPACE();

    if (row_major_flag)
    {
        strip_size[0] = chunk_size_on_split_dim;
        IFILE->SetChunkSize(strip_size);
        strip_size[0] = 1;
        IFILE->SetApplyStripSize(strip_size);
    }
    else
    {
        strip_size[1] = chunk_size_on_split_dim;
        IFILE->SetChunkSize(strip_size);
        strip_size[1] = 1;
        IFILE->SetApplyStripSize(strip_size);
    }
    if (row_major_flag == 0)
    {
        IFILE->SetOutputVector(nXCORR * window_batch, 0); //output vector size
    }
    else
    {
        IFILE->SetOutputVector(nXCORR * window_batch, 1); //output vector size
    }

    //Get FFT of master vector
    std::vector<short> masterv;
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
            if (enable_view_flag)
                MASTER_INDEX = view_start[1] + MASTER_INDEX;
            master_start[0] = 0 + bi * n0;
            master_start[1] = MASTER_INDEX;
            master_end[0] = master_start[0] + n0 - 1;
            master_end[1] = MASTER_INDEX;
        }
        else
        {
            if (enable_view_flag)
                MASTER_INDEX = view_start[0] + MASTER_INDEX;
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
    if (!mpi_rank)
        std::cout << "Finish the processing on Master block \n";

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
	      -g group name (path) for input dataset \n\
          -u group name (path) for output dataset \n\
          -t dataset name for intput time series \n\
          -x dataset name for output correlation \n\
          -w window size (only used when window size is different from chunk_size[0]) \n\
          -m index of Master channel (0 by default )\n\
          -r FFT in [Row]-direction([Column]-direction by default) \n\
          -c file for parameters (has high priority than commands if existing) \n\
          Example: mpirun -n 1 %s -i fft-test.h5 -o fft-test.arrayudf.h5  -g / -t /DataCT -x /Xcorr\n";

    fprintf(stdout, msg, cmd, cmd);
}
