#ifndef __DAS_FFT_H__
#define __DAS_FFT_H__

#include <vector>
#include <exception>
#include <algorithm>
#include <Eigen/Dense>
#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include <math.h> /* ceil  and floor*/
#include <cstring>
#include <numeric>
#include <iomanip> // std::setprecision

#include "fft-header/resample.h"
#include "fft-header/das-fft-help.cc"
#include "fft-header/liir.c"
#include "fft-header/bwlp.cc"
#include "fft-header/config-reader.cc"

//#define DEBUG_VERBOSE 0
using namespace std;

//N0: the size of orignal vector
//nPoint: size after resample = N0 * 1/R (R = 4)
//nfft: size of vector for FFT, power of 2 and > 2*nPoint - 1
//nXCORR: size of result gatherXcorr vector,  = 2*nPoint - 1
int n0;
int nPoint;
int nfft;
int nXCORR;

//Paramters fom butter(3, 2*0.25, 'low'), for filtfilt
int butter_order = 3;
double cut_frequency_low = 0.25;
vector<double> BUTTER_A;
vector<double> BUTTER_B;

//vector<double> BUTTER_A{0.031689343849711, 0.095068031549133, 0.095068031549133, 0.031689343849711};
//vector<double> BUTTER_B{1.000000000000000, -1.459029062228061, 0.910369000290069, -0.197825187264319};

//For resample, R = dt_new/dt
double DT = 0.002;
double DT_NEW = 0.008;

//For moveing mean
double WINLEN_SEC = 0.5;
int nPoint_hal_win;

//For interp1
double fNyquist; //250
std::vector<double> INTERP_Z{0, 0.5, 1, 1, 0.5, 0};
std::vector<double> INTERP_ZF{0, 0.002, 0.006, 14.5, 15, fNyquist};
double df;
double eCoeff = 1.0;

//Maste channel
unsigned long long MASTER_INDEX = 0;

//Parameters for ArrayUDF
int auto_chunk_dims_index = 1;
std::vector<int> strip_size(2);
std::vector<int> chunk_size(2);
int chunked_batch_factor = 32; //To enable a large chunk

//Parameters to enable "window" operation
int user_window_size = 1;
int set_window_size_flag = 0;
int window_batch = 1;

//Flag to have FFT in row-direction (first dimension)
//By default, it work in column-direction (second dimension)
int row_major_flag = 0;

//View's parameter
bool enable_view_flag = 0;
std::vector<unsigned long long> view_start{0, 0};
std::vector<unsigned long long> view_count{30000, 11648};
std::vector<int> view_os_size{0, 0};

//Using n0 as intial intput
//MR: mpi rank
//MS: mpi size
//DATADIMS: size of 2D data
#define INIT_PARS(MR, MS, DATADIMS)                                                     \
    {                                                                                   \
        if (row_major_flag == 0)                                                        \
        {                                                                               \
            auto_chunk_dims_index = 0;                                                  \
            strip_size[0] = DATADIMS[0];                                                \
            strip_size[1] = 1;                                                          \
            if (set_window_size_flag == 0)                                              \
            {                                                                           \
                n0 = DATADIMS[0];                                                       \
            }                                                                           \
            else                                                                        \
            {                                                                           \
                assert(user_window_size > 0 && user_window_size < DATADIMS[0]);         \
                n0 = user_window_size;                                                  \
                if (DATADIMS[0] % user_window_size == 0)                                \
                {                                                                       \
                    window_batch = DATADIMS[0] / user_window_size;                      \
                }                                                                       \
                else                                                                    \
                {                                                                       \
                    window_batch = DATADIMS[0] / user_window_size + 1;                  \
                }                                                                       \
            }                                                                           \
        }                                                                               \
        else                                                                            \
        {                                                                               \
            auto_chunk_dims_index = 1;                                                  \
            strip_size[0] = 1;                                                          \
            strip_size[1] = DATADIMS[1];                                                \
            if (set_window_size_flag == 0)                                              \
            {                                                                           \
                n0 = DATADIMS[1];                                                       \
            }                                                                           \
            else                                                                        \
            {                                                                           \
                assert(user_window_size > 0 && user_window_size < DATADIMS[1]);         \
                n0 = user_window_size;                                                  \
                if (DATADIMS[1] % user_window_size == 0)                                \
                {                                                                       \
                    window_batch = DATADIMS[1] / user_window_size;                      \
                }                                                                       \
                else                                                                    \
                {                                                                       \
                    window_batch = DATADIMS[1] / user_window_size + 1;                  \
                }                                                                       \
            }                                                                           \
        }                                                                               \
        nPoint = ceil(n0 / (DT_NEW / DT));                                              \
        FIND_M_POWER2(nPoint, nfft);                                                    \
        nXCORR = 2 * nPoint - 1;                                                        \
        fNyquist = 0.5 / DT_NEW;                                                        \
        nPoint_hal_win = floor((2 * floor(WINLEN_SEC / DT_NEW / 2) + 1) / 2);           \
        INTERP_ZF[5] = fNyquist;                                                        \
        df = 2.0 * fNyquist / (double)nfft;                                             \
        cut_frequency_low = (0.5 / DT_NEW) / (0.5 / DT);                                \
        butter_low(butter_order, cut_frequency_low, BUTTER_A, BUTTER_B);                \
        if (MR == 0)                                                                    \
        {                                                                               \
            printf("\n Some parameters for DAS:\n");                                    \
            printf("    n0(intput size) = %d \n", n0);                                  \
            printf("             nPoint = %d \n", nPoint);                              \
            printf("               nfft = %d \n", nfft);                                \
            printf("           fNyquist = %f \n", fNyquist);                            \
            printf("     nPoint_hal_win = %d \n", nPoint_hal_win);                      \
            printf("                 df = %f \n", df);                                  \
            printf("nXCORR(output size) = %d \n", nXCORR);                              \
            printf("    butter low freq = %f \n", cut_frequency_low);                   \
            printf("                 \n");                                              \
            printf("ArrayUDF strip  size  = (%d, %d)\n", strip_size[0], strip_size[1]); \
            printf("ArrayUDF window size  = %d\n", user_window_size);                   \
            printf("ArrayUDF window batch = %d\n", window_batch);                       \
            printf("                 \n");                                              \
        }                                                                               \
        fflush(stdout);                                                                 \
    }

//Pre-allocated data space, to save time
std::vector<double> X;
std::vector<double> TC; //temp cache
std::vector<double> shapingFilt;
fftw_complex *fft_in;
fftw_complex *fft_out;
fftw_complex *master_fft;

//Final results
std::vector<float> gatherXcorr;
std::vector<float> gatherXcorr_per_batch;

//WS: window size
#define INIT_SPACE()                                                             \
    {                                                                            \
        X.resize(n0);                                                            \
        TC.resize(nfft);                                                         \
        shapingFilt.resize(nfft);                                                \
        fft_in = fftw_alloc_complex(nfft);                                       \
        fft_out = fftw_alloc_complex(nfft);                                      \
        master_fft = fftw_alloc_complex(nfft * window_batch);                    \
        if (fft_in == NULL || fft_out == NULL || master_fft == NULL)             \
        {                                                                        \
            printf("not enough memory for fft, in %s:%d\n", __FILE__, __LINE__); \
            exit(-1);                                                            \
        }                                                                        \
        gatherXcorr_per_batch.resize(nXCORR);                                    \
        gatherXcorr.resize(nXCORR *window_batch);                                \
        std::vector<double> FF_LHS, LHS;                                         \
        FF_LHS.resize(nfft / 2);                                                 \
        LHS.resize(nfft / 2);                                                    \
        for (int i = 0; i < nfft / 2; i++)                                       \
        {                                                                        \
            FF_LHS[i] = df * (i + 1);                                            \
        }                                                                        \
        interp1(INTERP_ZF, INTERP_Z, FF_LHS, LHS);                               \
        int nfft_half = nfft / 2;                                                \
        for (int i = 0; i < nfft_half; i++)                                      \
        {                                                                        \
            shapingFilt[i] = LHS[i];                                             \
            shapingFilt[i + nfft_half] = LHS[nfft_half - i - 1];                 \
        }                                                                        \
        FF_LHS.clear();                                                          \
        LHS.clear();                                                             \
    }

//R = dt_new/dt
#define CLEAR_SPACE()                  \
    {                                  \
        X.clear();                     \
        TC.clear();                    \
        shapingFilt.clear();           \
        fftw_free(fft_in);             \
        fftw_free(fft_out);            \
        fftw_free(master_fft);         \
        gatherXcorr.clear();           \
        gatherXcorr_per_batch.clear(); \
    }

/*
 * XX is the input data 
 * YY is used as cache
 * GX is the result
 */
#define FFT_PROCESSING(XX, YY, GX, MFFT, BI, BS)                                                                   \
    {                                                                                                              \
        detrend(&(XX[0]), XX.size());                                                                              \
        filtfilt(BUTTER_A, BUTTER_B, XX, YY);                                                                      \
        resample(1, DT_NEW / DT, YY, XX);                                                                          \
        MOVING_MEAN(XX, YY, nPoint_hal_win);                                                                       \
        INIT_FFTW(fft_in, YY, nPoint, nfft, fft_out);                                                              \
        FFT_HELP_W(nfft, fft_in, fft_out, FFTW_FORWARD);                                                           \
        double temp_f;                                                                                             \
        for (int ii = 0; ii < nfft; ii++)                                                                          \
        {                                                                                                          \
            temp_f = pow(sqrt(fft_out[ii][0] * fft_out[ii][0] + fft_out[ii][1] * fft_out[ii][1]), eCoeff) + 0.001; \
            fft_in[ii][0] = (fft_out[ii][0] + 0.001) / temp_f * shapingFilt[ii];                                   \
            fft_in[ii][1] = (fft_out[ii][1]) / temp_f * shapingFilt[ii];                                           \
            fft_out[ii][0] = 0;                                                                                    \
            fft_out[ii][1] = 0;                                                                                    \
        }                                                                                                          \
        FFT_HELP_W(nfft, fft_in, fft_out, FFTW_BACKWARD);                                                          \
        YY.resize(nPoint);                                                                                         \
        for (int i = 0; i < nPoint; i++)                                                                           \
        {                                                                                                          \
            YY[i] = fft_out[i][0] / ((double)nfft);                                                                \
        }                                                                                                          \
        INIT_FFTW(fft_in, YY, nPoint, nfft, fft_out);                                                              \
        FFT_HELP_W(nfft, fft_in, fft_out, FFTW_FORWARD);                                                           \
        for (int j = 0; j < nfft; j++)                                                                             \
        {                                                                                                          \
            fft_in[j][0] = MFFT[j + BI * BS][0] * fft_out[j][0] + MFFT[j + BI * BS][1] * fft_out[j][1];            \
            fft_in[j][1] = MFFT[j + BI * BS][1] * fft_out[j][0] - MFFT[j + BI * BS][0] * fft_out[j][1];            \
            fft_out[j][0] = 0;                                                                                     \
            fft_out[j][1] = 0;                                                                                     \
        }                                                                                                          \
        FFT_HELP_W(nfft, fft_in, fft_out, FFTW_BACKWARD);                                                          \
        int gatherXcorr_index = 0;                                                                                 \
        for (int k = nfft - nPoint + 1; k < nfft; k++)                                                             \
        {                                                                                                          \
            GX[gatherXcorr_index] = (float)(fft_out[k][0] / (double)nfft);                                         \
            gatherXcorr_index++;                                                                                   \
        }                                                                                                          \
        for (int l = 0; l < nPoint; l++)                                                                           \
        {                                                                                                          \
            GX[gatherXcorr_index] = (float)(fft_out[l][0] / (double)nfft);                                         \
            gatherXcorr_index++;                                                                                   \
        }                                                                                                          \
    }

#define FFT_PREPROCESSING(XPP, YPP)                                                                   \
    {                                                                                                 \
        detrend(&(XPP[0]), XPP.size());                                                               \
        filtfilt(BUTTER_A, BUTTER_B, XPP, YPP);                                                       \
        resample(1, DT_NEW / DT, YPP, XPP);                                                           \
        MOVING_MEAN(XPP, YPP, nPoint_hal_win);                                                        \
        INIT_FFTW(fft_in, YPP, nPoint, nfft, fft_out);                                                \
        FFT_HELP_W(nfft, fft_in, fft_out, FFTW_FORWARD);                                              \
        double temp_f;                                                                                \
        for (int ii = 0; ii < nfft; ii++)                                                             \
        {                                                                                             \
            temp_f = sqrt(fft_out[ii][0] * fft_out[ii][0] + fft_out[ii][1] * fft_out[ii][1]) + 0.001; \
            fft_in[ii][0] = (fft_out[ii][0] + 0.001) / temp_f * shapingFilt[ii];                      \
            fft_in[ii][1] = (fft_out[ii][1]) / temp_f * shapingFilt[ii];                              \
            fft_out[ii][0] = 0;                                                                       \
            fft_out[ii][1] = 0;                                                                       \
        }                                                                                             \
        FFT_HELP_W(nfft, fft_in, fft_out, FFTW_BACKWARD);                                             \
        YPP.resize(nPoint);                                                                           \
        for (int i = 0; i < nPoint; i++)                                                              \
        {                                                                                             \
            YPP[i] = fft_out[i][0] / ((double)nfft);                                                  \
        }                                                                                             \
    }

//For debug only
void master_processing(std::vector<double> XPP, std::vector<double> &YPP)
{
    for (int i = 0; i < 100; i++)
    {
        printf("master_processing shapingFilt [%d] = %f\n", i, shapingFilt[i]);
    }
    detrend(&(XPP[0]), XPP.size());
    for (int i = 0; i < 10; i++)
    {
        printf("detrend (%d): %f \n", i, XPP[i]);
    }
    for (int i = 2999; i < 3010; i++)
    {
        printf("detrend (%d): %f, nfft =%d, nPoint =%d \n", i, XPP[i], nfft, nPoint);
    }

    for (int i = nPoint - 10 - 1; i < nPoint; i++)
    {
        printf("detrend (%d): %f, nfft =%d, nPoint =%d \n", i, XPP[i], nfft, nPoint);
    }

    filtfilt(BUTTER_A, BUTTER_B, XPP, YPP);
    for (int i = 0; i < 10; i++)
    {
        printf("filtfilt (%d): %f \n", i, YPP[i]);
    }
    for (int i = 2999; i < 3010; i++)
    {
        printf("filtfilt (%d): %f, nfft =%d, nPoint =%d \n", i, YPP[i], nfft, nPoint);
    }

    for (int i = nPoint - 10 - 1; i < nPoint; i++)
    {
        printf("filtfilt (%d): %f, nfft =%d, nPoint =%d \n", i, YPP[i], nfft, nPoint);
    }
    resample(1, DT_NEW / DT, YPP, XPP);
    MOVING_MEAN(XPP, YPP, nPoint_hal_win);
    for (int i = 0; i < 10; i++)
    {
        printf("MOVING_MEAN (%d): %f \n", i, YPP[i]);
    }
    for (int i = 2999; i < 3010; i++)
    {
        printf("MOVING_MEAN (%d): %f, nfft =%d, nPoint =%d \n", i, YPP[i], nfft, nPoint);
    }

    for (int i = nPoint - 10 - 1; i < nPoint; i++)
    {
        printf("MOVING_MEAN (%d): %f, nfft =%d, nPoint =%d \n", i, YPP[i], nfft, nPoint);
    }

    for (int i = 0; i < 100; i++)
    {
        printf("After MOVING_MEAN shapingFilt [%d] = %f\n", i, shapingFilt[i]);
    }
    fftw_complex *fft_in_temp;
    fftw_complex *fft_out_temp;
    fft_in_temp = fftw_alloc_complex(nfft);
    fft_out_temp = fftw_alloc_complex(nfft);
    for (int i = 0; i < nfft; i++)
    {
        if (i < nPoint)
        {
            fft_in_temp[i][0] = YPP[i];
            fft_in_temp[i][1] = 0;
        }
        else
        {
            fft_in_temp[i][0] = 0;
            fft_in_temp[i][1] = 0;
        }
        fft_out_temp[i][0] = 0;
        fft_out_temp[i][1] = 0;
    }
    //INIT_FFTW(fft_in, YPP, nPoint, nfft, fft_out);
    FFT_HELP_W(nfft, fft_in_temp, fft_out_temp, FFTW_FORWARD);
    double temp_f;
    for (int ii = 0; ii < nfft; ii++)
    {
        if (ii < 10)
            printf("fft: %f + %f i , shapingFilt = %f \n", fft_out_temp[ii][0], fft_out_temp[ii][1], shapingFilt[ii]);
        temp_f = sqrt(fft_out_temp[ii][0] * fft_out_temp[ii][0] + fft_out_temp[ii][1] * fft_out_temp[ii][1]) + 0.001;
        fft_in_temp[ii][0] = (fft_out_temp[ii][0] + 0.001) / temp_f * shapingFilt[ii];
        fft_in_temp[ii][1] = (fft_out_temp[ii][1]) / temp_f * shapingFilt[ii];
        fft_out_temp[ii][0] = 0;
        fft_out_temp[ii][1] = 0;
    }
    FFT_HELP_W(nfft, fft_in_temp, fft_out_temp, FFTW_BACKWARD);
    YPP.resize(nPoint);
    for (int i = 0; i < nPoint; i++)
    {
        YPP[i] = fft_out_temp[i][0] / ((double)nfft);
        if (i < 10)
            printf("White: %f \n", YPP[i]);
    }
    fftw_free(fft_in_temp);
    fftw_free(fft_out_temp);
}

#endif