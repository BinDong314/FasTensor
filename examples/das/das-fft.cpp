// overload_array.cpp
// overloading the c++ array subscript operator []
//http://neondataskills.org/HDF5/TimeSeries-Data-In-HDF5-Using-R/

#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include <math.h> /* ceil  and floor*/
#include <cstring>
#include <complex> // std::complex

//#define FFTW_LIB_AVAILABLE 1

#ifndef FFTW_LIB_AVAILABLE
#include "fft/kiss_fft.h"
#else
#include <fftw3.h>
#endif
#include "array_udf.h"

#define NAME_LENGTH 1024
void convert_str_vector(int n, char *str, int *vector);
void printf_help(char *cmd);

using namespace std;

//Some global variables
unsigned long long m_TIME_SERIESE_LENGTH = 30000;                              //window size
unsigned long long M_TIME_SERIESE_LENGTH_EXTENDED = m_TIME_SERIESE_LENGTH * 2; //size of windoe for FFT
unsigned long long x_GATHER_X_CORR_LENGTH = m_TIME_SERIESE_LENGTH * 2 - 1;     //Result size

//When time series is spelit into windows
// following variables are used
int window_batch = 1;
unsigned long long x_GATHER_X_CORR_LENGTH_total = x_GATHER_X_CORR_LENGTH * window_batch;
unsigned long long M_TIME_SERIESE_LENGTH_EXTENDED_total = M_TIME_SERIESE_LENGTH_EXTENDED * window_batch;

//FFT for channel 0 or n
//master_vector_fft is filled before we run FFT_UDF
unsigned long long MASTER_INDEX = 0;
std::vector<std::complex<float>> master_vector_fft;

//Some help functions
inline void fft_help(const std::vector<float> &fft_in, std::vector<std::complex<float>> &fft_out, unsigned long long extend_size);
inline void ifft_help(std::vector<std::complex<float>> &fft_in_out);
unsigned long long find_m(unsigned long long minimum_m);

//Variable used by FFT_UDF
//Put here for performnace resason
std::vector<float> ts;
std::vector<std::complex<float>> temp_fft_v;
std::vector<float> gatherXcorr_per_batch;
std::vector<float> gatherXcorr_final;

inline std::vector<float> FFT_UDF(const Stencil<float> &c)
{
    for (int bi = 0; bi < window_batch; bi++)
    {
        double start = au_current_time();
        //Get the input time series on a single channel
        for (unsigned long long i = 0; i < m_TIME_SERIESE_LENGTH; i++)
        {
            ts[i] = c(i, 0);
        }
        double end = au_current_time();
        au_reduce_time(end - start, "read data ");

        //FFT on the channel
        fft_help(ts, temp_fft_v, M_TIME_SERIESE_LENGTH_EXTENDED);

        double fft_end = au_current_time();
        au_reduce_time(fft_end - end, "fft_help time ");

        return gatherXcorr_final;

        //specXcorr
        for (unsigned long long j = 0; j < M_TIME_SERIESE_LENGTH_EXTENDED; j++)
        {
            temp_fft_v[j] = master_vector_fft[j] * std::conj(temp_fft_v[j]);
        }

        //IFFT, result_v also holds the result
        ifft_help(temp_fft_v);

        /* Get copy of our VOL info from FAPL */
        //Subset specXcorr
        unsigned long long gatherXcorr_index = 0;
        for (unsigned long long k = M_TIME_SERIESE_LENGTH_EXTENDED - m_TIME_SERIESE_LENGTH + 1; k < M_TIME_SERIESE_LENGTH_EXTENDED; k++)
        {
            gatherXcorr_per_batch[gatherXcorr_index] = temp_fft_v[k].real();
            gatherXcorr_index++;
        }
        for (unsigned long long l = 0; l < m_TIME_SERIESE_LENGTH; l++)
        {
            gatherXcorr_per_batch[gatherXcorr_index] = temp_fft_v[l].real();
            gatherXcorr_index++;
        }

        assert(gatherXcorr_index == x_GATHER_X_CORR_LENGTH);

        std::copy_n(gatherXcorr_per_batch.begin(), x_GATHER_X_CORR_LENGTH, gatherXcorr_final.begin() + bi * x_GATHER_X_CORR_LENGTH);
    }

    return gatherXcorr_final;
}

int main(int argc, char *argv[])
{
    char i_file[NAME_LENGTH] = "test-data/fft-test.h5";
    char o_file[NAME_LENGTH] = "fft-test.arrayudf.h5";
    char group[NAME_LENGTH] = "/"; //both input and output file share the same group and dataset name
    char i_dataset[NAME_LENGTH] = "/dat";
    char o_dataset[NAME_LENGTH] = "/Xcorr";

    char chunk_size_str[NAME_LENGTH];
    std::vector<int> ghost_size;
    std::vector<int> chunk_size;
    std::vector<int> strip_size;
    int array_ranks = 2;

    ghost_size.resize(array_ranks);
    ghost_size[0] = 0;
    ghost_size[1] = 0;
    chunk_size.resize(array_ranks);
    chunk_size[0] = 30000;
    chunk_size[1] = 2912;
    strip_size.resize(array_ranks);
    strip_size[0] = 30000;
    strip_size[1] = 1;
    m_TIME_SERIESE_LENGTH = chunk_size[0];

    unsigned long long user_window_size;
    int set_chunk_size_flag = 0;
    int set_window_size_flag = 0;
    int copt, mpi_rank;
    while ((copt = getopt(argc, argv, "o:i:g:t:x:c:m:w:h")) != -1)
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
            user_window_size = atoll(optarg);
            break;
        case 'c':
            set_chunk_size_flag = 1;
            memset(chunk_size_str, 0, NAME_LENGTH);
            strcpy(chunk_size_str, optarg);
            break;
        case 'm':
            MASTER_INDEX = atoi(optarg);
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

    if (set_chunk_size_flag)
    {
        convert_str_vector(array_ranks, chunk_size_str, &(chunk_size[0]));
    }

    //Be defuat values
    strip_size[0] = chunk_size[0];         //skip per chunk
    strip_size[1] = 1;                     //per channel
    m_TIME_SERIESE_LENGTH = chunk_size[0]; //chunk size = window size

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    Array<float, std::vector<float>> *IFILE = new Array<float, std::vector<float>>(AU_NVS, AU_HDF5, i_file, group, i_dataset, chunk_size, ghost_size);
    Array<std::vector<float>> *OFILE = new Array<std::vector<float>>(AU_COMPUTED, AU_HDF5, o_file, group, o_dataset, chunk_size, ghost_size);

    std::vector<unsigned long long> i_file_dim = IFILE->GetDimSize();
    if (i_file_dim[0] != chunk_size[0])
    {
        printf("Chunk size must be equal to the size of first dimension: %lld \n", i_file_dim[0]);
        exit(-1);
    }

    window_batch = 1;
    if (set_window_size_flag && (user_window_size != chunk_size[0]))
    {
        if (user_window_size < chunk_size[0])
        {
            m_TIME_SERIESE_LENGTH = user_window_size;
            if (chunk_size[0] % user_window_size == 0)
            {
                window_batch = chunk_size[0] / user_window_size;
                if (!mpi_rank)
                {
                    printf("user_window_size = %lld, window_batch = %d \n", user_window_size, window_batch);
                }
            }
            else
            {
                //Todo: consider impact of this
                window_batch = chunk_size[0] / user_window_size + 1;
            }
        }
        else
        {
            printf("Window size (-w) must be smaller than chunk_size[0] (-c) \n");
            exit(-1);
        }
    }

    //For each window
    x_GATHER_X_CORR_LENGTH = 2 * m_TIME_SERIESE_LENGTH - 1;
    M_TIME_SERIESE_LENGTH_EXTENDED = find_m(x_GATHER_X_CORR_LENGTH);
    if (!mpi_rank)
        printf("M value is : %lld\n", M_TIME_SERIESE_LENGTH_EXTENDED);

    x_GATHER_X_CORR_LENGTH_total = x_GATHER_X_CORR_LENGTH * window_batch;
    M_TIME_SERIESE_LENGTH_EXTENDED_total = M_TIME_SERIESE_LENGTH_EXTENDED * window_batch;

    //Get the mater vector and its fft
    std::vector<float> master_vector_per_batch;
    master_vector_per_batch.resize(m_TIME_SERIESE_LENGTH);

    std::vector<std::complex<float>> master_vector_fft_per_batch;
    master_vector_fft_per_batch.resize(M_TIME_SERIESE_LENGTH_EXTENDED);

    master_vector_fft.resize(M_TIME_SERIESE_LENGTH_EXTENDED_total);

    for (int bi = 0; bi < window_batch; bi++)
    {
        for (int i = 0; i < m_TIME_SERIESE_LENGTH; i++)
        {
            master_vector_per_batch[i] = IFILE->operator()(i, MASTER_INDEX);
        }
        fft_help(master_vector_per_batch, master_vector_fft_per_batch, M_TIME_SERIESE_LENGTH_EXTENDED);
        std::copy_n(master_vector_fft_per_batch.begin(), M_TIME_SERIESE_LENGTH_EXTENDED, master_vector_fft.begin() + bi * m_TIME_SERIESE_LENGTH);
    }

    //release the memory
    master_vector_fft_per_batch.clear();
    master_vector_fft_per_batch.shrink_to_fit();
    master_vector_per_batch.clear();
    master_vector_per_batch.shrink_to_fit();

    //Set the strip size and output vector size before the run
    IFILE->SetApplyStripSize(strip_size);
    IFILE->SetOutputVector(x_GATHER_X_CORR_LENGTH_total, 0);

    //Allocate some space for FFT_UDF
    ts.resize(m_TIME_SERIESE_LENGTH);
    temp_fft_v.resize(M_TIME_SERIESE_LENGTH_EXTENDED);
    gatherXcorr_per_batch.resize(x_GATHER_X_CORR_LENGTH);
    gatherXcorr_final.resize(x_GATHER_X_CORR_LENGTH_total);

    //Run FFT
    IFILE->Apply(FFT_UDF, OFILE);
    IFILE->ReportTime();

    delete IFILE;
    delete OFILE;
    MPI_Finalize();
    return 0;
}

//It is power of 2 and greater than minimum_m
unsigned long long find_m(unsigned long long minimum_m)
{
    unsigned long long m = minimum_m;
    while ((minimum_m & (minimum_m - 1)) != 0)
    {
        minimum_m = minimum_m + 1;
    }
    return minimum_m;
}

void fft_help(const std::vector<float> &fft_in, std::vector<std::complex<float>> &fft_out, unsigned long long extended_size)
{
    unsigned long long fft_in_size = fft_in.size();
    assert(fft_out.size() == extended_size);

#ifndef FFTW_LIB_AVAILABLE
    kiss_fft_cpx *fft_in_temp = (kiss_fft_cpx *)malloc(extended_size * sizeof(kiss_fft_cpx));
    kiss_fft_cpx *fft_out_temp = (kiss_fft_cpx *)malloc(extended_size * sizeof(kiss_fft_cpx));

    if (fft_in_temp == NULL || fft_out_temp == NULL)
    {
        printf("not enough memory, in %s:%d\n", __FILE__, __LINE__);
        exit(-1);
    }

    for (int i = 0; i < extended_size; i++)
    {
        if (i < fft_in_size)
        {
            fft_in_temp[i].r = fft_in[i];
            fft_in_temp[i].i = 0;
        }
        else
        {
            fft_in_temp[i].r = 0;
            fft_in_temp[i].i = 0;
        }
    }

    double start = au_current_time();
    kiss_fft_cfg cfg;
    if ((cfg = kiss_fft_alloc(extended_size, 0, NULL, NULL)) != NULL)
    {
        kiss_fft(cfg, fft_in_temp, fft_out_temp);
        free(cfg);
    }
    else
    {
        printf("not enough memory, in %s:%d\n", __FILE__, __LINE__);
        exit(-1);
    }
    double end = au_current_time();
    au_reduce_time(end - start, "fft time per ");
    for (int i = 0; i < extended_size; i++)
    {
        fft_out[i].real(fft_out_temp[i].r);
        fft_out[i].imag(fft_out_temp[i].i);
    }
    free(fft_in_temp);
    free(fft_out_temp);
#else
    fftw_complex *fft_in_temp, *fft_out_temp;
    fftw_plan fft_p;
    fft_in_temp = (fftw_complex *)malloc(sizeof(fftw_complex) * extended_size);
    fft_out_temp = (fftw_complex *)malloc(sizeof(fftw_complex) * extended_size);

    if (fft_in_temp == NULL || fft_out_temp == NULL)
    {
        printf("not enough memory, in %s:%d\n", __FILE__, __LINE__);
        exit(-1);
    }

    for (int i = 0; i < fft_in_size; i++)
    {
        fft_in_temp[i][0] = fft_in[i];
        fft_in_temp[i][1] = 0;
    }

    for (int i = fft_in_size; i < extended_size; i++)
    {
        fft_in_temp[i][0] = 0;
        fft_in_temp[i][1] = 0;
    }

    double start = au_current_time();

    fft_p = fftw_plan_dft_1d(extended_size, fft_in_temp, fft_out_temp, FFTW_FORWARD, FFTW_ESTIMATE);

    fftw_execute(fft_p);

    double end = au_current_time();
    au_reduce_time(end - start, "fftw time/step: ");
    for (int i = 0; i < extended_size; i++)
    {
        fft_out[i].real(fft_out_temp[i][0]);
        fft_out[i].imag(fft_out_temp[i][1]);
    }
    fftw_destroy_plan(fft_p);
#endif
}

inline void ifft_help(std::vector<std::complex<float>> &fft_in_out)
{
    unsigned long long fft_in_size = fft_in_out.size();

#ifndef FFTW_LIB_AVAILABLE

    kiss_fft_cpx *fft_in_temp = (kiss_fft_cpx *)malloc(fft_in_size * sizeof(kiss_fft_cpx));
    kiss_fft_cpx *fft_out_temp = (kiss_fft_cpx *)malloc(fft_in_size * sizeof(kiss_fft_cpx));

    if (fft_in_temp == NULL || fft_out_temp == NULL)
    {
        printf("not enough memory, in %s:%d\n", __FILE__, __LINE__);
        exit(-1);
    }

    for (int i = 0; i < fft_in_size; i++)
    {
        fft_in_temp[i].r = fft_in_out[i].real();
        fft_in_temp[i].i = fft_in_out[i].imag();
    }
    kiss_fft_cfg cfg;
    if ((cfg = kiss_fft_alloc(fft_in_size, 1, NULL, NULL)) != NULL)
    {
        kiss_fft(cfg, fft_in_temp, fft_out_temp);
        free(cfg);
    }
    else
    {
        printf("not enough memory, in %s:%d\n", __FILE__, __LINE__);
        exit(-1);
    }

    //https://stackoverflow.com/questions/39109615/fftw-c-computes-fft-wrong-compared-to-matlab
    for (int i = 0; i < fft_in_size; i++)
    {
        fft_in_out[i].real(fft_out_temp[i].r / fft_in_size);
        fft_in_out[i].imag(fft_out_temp[i].i);
    }
    free(fft_in_temp);
    free(fft_out_temp);
#else
    fftw_complex *fft_in_temp, *fft_out_temp;
    fftw_plan fft_p;
    fft_in_temp = (fftw_complex *)malloc(sizeof(fftw_complex) * fft_in_size);
    fft_out_temp = (fftw_complex *)malloc(sizeof(fftw_complex) * fft_in_size);

    if (fft_in_temp == NULL || fft_out_temp == NULL)
    {
        printf("not enough memory, in %s:%d\n", __FILE__, __LINE__);
        exit(-1);
    }

    for (int i = 0; i < fft_in_size; i++)
    {
        fft_in_temp[i][0] = fft_in_out[i].real();
        fft_in_temp[i][1] = fft_in_out[i].imag();
    }

    fft_p = fftw_plan_dft_1d(fft_in_size, fft_in_temp, fft_out_temp, FFTW_BACKWARD, FFTW_ESTIMATE);

    fftw_execute(fft_p);

    //https://stackoverflow.com/questions/39109615/fftw-c-computes-fft-wrong-compared-to-matlab
    for (int i = 0; i < fft_in_size; i++)
    {
        fft_in_out[i].real(fft_out_temp[i][0] / fft_in_size);
        fft_in_out[i].imag(fft_out_temp[i][1]);
    }
    fflush(stdout);
    fftw_destroy_plan(fft_p);
#endif
}

void convert_str_vector(int n, char *str, int *vector)
{
    int i;
    char *pch;
    char temp[NAME_LENGTH];

    if (n == 1)
    {
        vector[0] = atoi(str);
    }
    else
    {
        strcpy(temp, str);
        pch = strtok(temp, ",");

        i = 0;
        while (pch != NULL)
        {
            //printf("%s \n", pch);
            vector[i] = atoi(pch);
            pch = strtok(NULL, ",");
            i++;
        }
    }
    return;
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
          -c chunk size (seperate by comma ',')\n\
             chunk_size[0] is used as window size by default \n\
          -w window size (only used when window size is different from chunk_size[0]) \n\
          -m index of master channel (0 by default )\n\
          Example: mpirun -n 1 %s -i ./test-data/fft-test.h5 -o ./test-data/fft-test.arrayudf.h5  -g / -t /dat -x /Xcorr -c 7500,101\n";

    fprintf(stdout, msg, cmd, cmd);
}
