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
#include <fftw3.h>
#include "array_udf.h"

/*
 * Note that: das-fft.h contains all parameters & used variables & functions. 
 *            Go to "das-fft.h" to find more details
 */
#include "das-fft.h"

using namespace std;

// help functions
void printf_help(char *cmd);

inline std::vector<float> DEC_UDF(const Stencil<short> &c)
{
    std::vector<float> dec_result(decimation_size * window_batch);
    std::vector<double> X_l(n0);
    std::vector<double> C_l(nfft); //temp cache

    for (int bi = 0; bi < window_batch; bi++)
    {
        //au_time_start();

        X_l.resize(n0);
        C_l.resize(nfft);
        for (int i = 0; i < n0; i++)
        {
            if (row_major_flag == 0)
            {
                X_l[i] = (double)c(i + bi * n0, 0);
            }
            else
            {
                X_l[i] = (double)c(0, i + bi * n0);
            }
        }
        //au_time_elap(" ++ Read Stencil ");

        detrend(&(X_l[0]), X_l.size());
        //au_time_elap(" ++ detrend ");

        filtfilt(BUTTER_A, BUTTER_B, X_l, C_l);
        //au_time_elap(" ++ filtfilt ");

        resample(1, DT_NEW / DT, C_l, X_l);
        //au_time_elap(" ++ resample ");
        std::copy(X_l.begin(), X_l.end(), dec_result.begin() + decimation_size * bi);
    }

    return dec_result;
}
/*
 * This function describes the operation per channel
 *  The return value is a coorelation vector
 * See more details at das-fft.h
 */
int debug_index = 0;
inline std::vector<float> FFT_UDF(const Stencil<short> &c)
{
    std::vector<double> X_l(n0);
    std::vector<double> C_l(nfft); //temp cache

    std::vector<float> gatherXcorr_l(nXCORR);
    std::vector<float>::iterator gatherXcorr_per_batch_l;
    FFT_DATA_TYPEP fft_in_l;
    FFT_DATA_TYPEP fft_out_l;

    for (int bi = 0; bi < window_batch; bi++)
    {
        //au_time_start();

        X_l.resize(n0);
        C_l.resize(nfft);
        for (int i = 0; i < n0; i++)
        {
            if (row_major_flag == 0)
            {
                X_l[i] = (double)c(i + bi * n0, 0);
            }
            else
            {
                X_l[i] = (double)c(0, i + bi * n0);
            }
        }
        //au_time_elap(" ++ Read Stencil ");

        detrend(&(X_l[0]), X_l.size());
        //au_time_elap(" ++ detrend ");

        filtfilt(BUTTER_A, BUTTER_B, X_l, C_l);
        //au_time_elap(" ++ filtfilt ");

        resample(1, DT_NEW / DT, C_l, X_l);
        //au_time_elap(" ++ resample ");

        MOVING_MEAN(X_l, C_l, nPoint_hal_win);
        //au_time_elap(" ++ MOVING_MEAN ");

        clear_vector(X_l);
        ALLOCATE_FFT(fft_in_l, nfft);
        ALLOCATE_FFT(fft_out_l, nfft);

        PREPARE_FFT(fft_in_l, C_l, nPoint, nfft, fft_out_l);
        //au_time_elap(" ++ Init fft ");

        RUN_FFT(nfft, fft_in_l, fft_out_l, FORWARD_FLAG);
        //au_time_elap(" ++ First fft ");

        for (int ii = 0; ii < nfft; ii++)
        {
            double temp_f;
#ifdef FFTW_AVAI

            temp_f = pow(sqrt(fft_out_l[ii][0] * fft_out_l[ii][0] + fft_out_l[ii][1] * fft_out_l[ii][1]), eCoeff) + 0.001;
            fft_in_l[ii][0] = (fft_out_l[ii][0] + 0.001) / temp_f * shapingFilt[ii];
            fft_in_l[ii][1] = (fft_out_l[ii][1]) / temp_f * shapingFilt[ii];
            fft_out_l[ii][0] = 0;
            fft_out_l[ii][1] = 0;
#else
            temp_f = pow(sqrt(fft_out_l[ii].r * fft_out_l[ii].r + fft_out_l[ii].i * fft_out_l[ii].i), eCoeff) + 0.001;
            fft_in_l[ii].r = (fft_out_l[ii].r + 0.001) / temp_f * shapingFilt[ii];
            fft_in_l[ii].i = (fft_out_l[ii].i) / temp_f * shapingFilt[ii];
            fft_out_l[ii].r = 0;
            fft_out_l[ii].i = 0;
#endif
        }
        //au_time_elap(" ++ Shapping ");

        RUN_FFT(nfft, fft_in_l, fft_out_l, BACKWARD_FLAG);
        //au_time_elap(" ++ First Rev fft ");

        C_l.resize(nPoint);
        for (int i = 0; i < nPoint; i++)
        {
#ifdef FFTW_AVAI
            C_l[i] = fft_out_l[i][0] / ((double)nfft);
#else
            C_l[i] = fft_out_l[i].r / ((double)nfft);
#endif
        }
        PREPARE_FFT(fft_in_l, C_l, nPoint, nfft, fft_out_l);
        RUN_FFT(nfft, fft_in_l, fft_out_l, FORWARD_FLAG);

        //au_time_elap(" ++ Second fft ");

        for (int j = 0; j < nfft; j++)
        {
#ifdef FFTW_AVAI
            fft_in_l[j][0] = master_fft[j + bi * nfft][0] * fft_out_l[j][0] + master_fft[j + bi * nfft][1] * fft_out_l[j][1];
            fft_in_l[j][1] = master_fft[j + bi * nfft][1] * fft_out_l[j][0] - master_fft[j + bi * nfft][0] * fft_out_l[j][1];
            fft_out_l[j][0] = 0;
            fft_out_l[j][1] = 0;
#else
            fft_in_l[j].r = master_fft[j + bi * nfft][0] * fft_out_l[j].r + master_fft[j + bi * nfft][1] * fft_out_l[j].i;
            fft_in_l[j].i = master_fft[j + bi * nfft][1] * fft_out_l[j].r - master_fft[j + bi * nfft][0] * fft_out_l[j].i;
            fft_out_l[j].r = 0;
            fft_out_l[j].i = 0;
#endif
        }
        //au_time_elap(" ++ XCorr ");

        RUN_FFT(nfft, fft_in_l, fft_out_l, BACKWARD_FLAG);

        //au_time_elap(" ++ Second rev fft ");

        gatherXcorr_per_batch_l = gatherXcorr_l.begin() + bi * nXCORR;
        //int gatherXcorr_index = 0;
        for (int k = nfft - nPoint + 1; k < nfft; k++)
        {
#ifdef FFTW_AVAI
            //gatherXcorr_per_batch_l[gatherXcorr_index] = (float)(fft_out_l[k][0] / (double)nfft);
            *gatherXcorr_per_batch_l = (float)(fft_out_l[k][0] / (double)nfft);
#else
            //gatherXcorr_per_batch_l[gatherXcorr_index] = (float)(fft_out_l[k].r / (double)nfft);
            *gatherXcorr_per_batch_l = (float)(fft_out_l[k].r / (double)nfft);
#endif
            //gatherXcorr_index++;
            gatherXcorr_per_batch_l++;
        }
        for (int l = 0; l < nPoint; l++)
        {
#ifdef FFTW_AVAI
            //gatherXcorr_per_batch_l[gatherXcorr_index] = (float)(fft_out_l[l][0] / (double)nfft);
            *gatherXcorr_per_batch_l = (float)(fft_out_l[l][0] / (double)nfft);
#else
            //gatherXcorr_per_batch_l[gatherXcorr_index] = (float)(fft_out_l[l].r / (double)nfft);
            *gatherXcorr_per_batch_l = (float)(fft_out_l[l].r / (double)nfft);

#endif
            //gatherXcorr_index++;
            gatherXcorr_per_batch_l++;
        }
        //std::copy_n(gatherXcorr_per_batch_l.begin(), nXCORR, gatherXcorr_l.begin() + bi * nXCORR);
        //au_time_elap(" ++ Gather result ");
        CLEAR_FFT(fft_in_l);
        CLEAR_FFT(fft_out_l);
    }
    clear_vector(C_l);

    return gatherXcorr_l;
}

std::string i_file("./westSac_170802100007.h5");
std::string o_file("./westSac_170802100007_AF.h5");
std::string i_group("/");
std::string o_group("/");
std::string i_dataset("DataTimeChannel");
std::string o_dataset("Xcorr");
int chunk_size_on_split_dim = 1;
int user_chunk_flag = 0;
int main(int argc, char *argv[])
{
#if defined(_OPENMP)
    fftw_make_planner_thread_safe();
#endif
    int config_file_set_flag = 0;

    char config_file[NAME_LENGTH] = "./das-fft-full.config";

    int copt, mpi_rank, mpi_size;
    while ((copt = getopt(argc, argv, "o:i:g:u:t:x:m:w:lhc:k:d")) != -1)
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
            user_chunk_flag = 1;
            break;
        case 'l':
            row_major_flag = 0;
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
        case 'd':
            decimation_flag = 1;
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

    if (!mpi_rank)
        printf("decimation_flag =  %d \n", decimation_flag);

    if (config_file_set_flag)
        read_config_file(config_file, mpi_rank);

    if (row_major_flag == 0)
    {
        auto_chunk_dims_index = 1;
    }
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
    INIT_SPACE_OMP();

    if (decimation_flag)
    {
        decimation_size = get_resampled_size(1, DT_NEW / DT, n0);
        if (!mpi_rank)
            printf("decimation_size is %d \n", decimation_size);
        //return 0;
    }

    if (row_major_flag)
    {
        if (user_chunk_flag)
        {
            strip_size[0] = chunk_size_on_split_dim;
            IFILE->SetChunkSize(strip_size);
        }
        strip_size[0] = 1;
        IFILE->SetApplyStripSize(strip_size);
    }
    else
    {
        if (user_chunk_flag)
        {
            strip_size[1] = chunk_size_on_split_dim;
            IFILE->SetChunkSize(strip_size);
        }
        strip_size[1] = 1;
        IFILE->SetApplyStripSize(strip_size);
    }
    int output_vector_size = nXCORR * window_batch;
    if (!decimation_flag)
    {
        output_vector_size = nXCORR * window_batch;
    }
    else
    {
        output_vector_size = decimation_size * window_batch;
    }
    if (row_major_flag == 0)
    {
        IFILE->SetOutputVector(output_vector_size, 0); //output vector size
    }
    else
    {
        IFILE->SetOutputVector(output_vector_size, 1); //output vector size
    }

    //Get FFT of master vector
    if (!decimation_flag)
    {
        std::vector<short> masterv;
        masterv.resize(n0);
        std::vector<double> mastervf, masterv_ppf;
        mastervf.resize(n0);
        masterv_ppf.resize(nfft);
        std::vector<unsigned long long> master_start, master_end;
        master_start.resize(2);
        master_end.resize(2);
        ALLOCATE_FFT(fft_in, nfft);
        ALLOCATE_FFT(fft_out, nfft);
        ALLOCATE_FFT(master_fft, nfft * window_batch);

        //Dis enable collective IO  to only allow rank 0 to read data
        IFILE->DisableCollO();
        for (int bi = 0; bi < window_batch; bi++)
        {
            if (mpi_rank == 0)
            {
                au_time_start_no_mpi();
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
                au_time_elap_no_mpi("  Read ch time : ");
            }

            //Only mpi_rank 0 to get the master channel
            //Broadcast to all other mpi ranks.
            MPI_Barrier(MPI_COMM_WORLD);
            au_time_start();
            MPI_Bcast(&masterv[0], n0, MPI_SHORT, 0, MPI_COMM_WORLD);
            au_time_elap(" MPI_Bcast master ch time :  ");

            for (int i = 0; i < n0; i++)
            {
                mastervf[i] = (double)(masterv[i]);
            }

            FFT_PREPROCESSING(mastervf, masterv_ppf); //masterv_ppf is result
            //master_processing(mastervf, masterv_ppf); //for debug
            INIT_FFTW(fft_in, masterv_ppf, nPoint, nfft, fft_out);
            FFT_HELP_W(nfft, fft_in, fft_out, FFTW_FORWARD);
            for (int j = 0; j < nfft; j++)
            {
                master_fft[bi * nfft + j][0] = fft_out[j][0];
                master_fft[bi * nfft + j][1] = fft_out[j][1];
            }
        }
        //Re enable collective IO  to make following read faster

        IFILE->EnableCollIO(); //Comment out for VDS test on single node

        //masterv_ppf.clear();
        //mastervf.clear();
        //masterv.clear();
        clear_vector(masterv_ppf);
        clear_vector(mastervf);
        clear_vector(masterv);
        CLEAR_FFT(fft_in);
        CLEAR_FFT(fft_out);
        if (!mpi_rank)
            std::cout << "Finish the processing on Master block \n";
    }
    //Run FFT
    if (!decimation_flag)
    {
        IFILE->Apply(FFT_UDF, OFILE);
    }
    else
    {
        IFILE->Apply(DEC_UDF, OFILE);
    }
    IFILE->ReportTime();

    if (!decimation_flag)
    {
        CLEAR_FFT(master_fft);
    }
    delete IFILE;
    delete OFILE;

    CLEAR_SPACE_OMP();

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
          -l FFT in [Column]-direction([Row]-direction by default) \n\
          -c file for parameters (has high priority than commands if existing) \n\
          -d run untial the Decimation steps \n\
          Example: mpirun -n 1 %s -i fft-test.h5 -o fft-test.arrayudf.h5  -g / -t /DataCT -x /Xcorr\n";

    fprintf(stdout, msg, cmd, cmd);
}
