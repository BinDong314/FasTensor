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

#include "fft-header/resample.h"
#include "fft-header/das-fft-help.cc"

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
vector<double> BUTTER_A{0.031689343849711, 0.095068031549133, 0.095068031549133, 0.031689343849711};
vector<double> BUTTER_B{1.000000000000000, -1.459029062228061, 0.910369000290069, -0.197825187264319};

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

//Using n0 as intial intput
#define INIT_PARS()                                                       \
    {                                                                     \
        nPoint = ceil(n0 / (DT_NEW / DT));                                \
        FIND_M_POWER2(nPoint, nfft);                                      \
        nXCORR = 2 * nPoint - 1;                                          \
        fNyquist = 0.5 / DT;                                              \
        nPoint_hal_win = floor((2 * floor(WINLEN_SEC / DT / 2) + 1) / 2); \
        INTERP_ZF[5] = fNyquist;                                          \
        df = 2.0 * fNyquist / (double)nfft;                               \
        printf("        nPoint = %d \n", nPoint);                         \
        printf("          nfft = %d \n", nfft);                           \
        printf("      fNyquist = %f \n", fNyquist);                       \
        printf("nPoint_hal_win = %d \n", nPoint_hal_win);                 \
        printf("            df = %f \n", df);                             \
        printf("        nXCORR = %d \n", nXCORR);                         \
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

#define INIT_SHAPINGFILT()                                                        \
    {                                                                             \
        std::vector<double> F_LHS;                                                \
        F_LHS.resize(nfft / 2);                                                   \
        for (int i = 0; i < nfft; i++)                                            \
        {                                                                         \
            F_LHS[i] = df * (i + 1);                                              \
        }                                                                         \
        interp1(INTERP_ZF, INTERP_Z, F_LHS, shapingFilt);                         \
        std::reverse_copy(shapingFilt.begin(), shapingFilt.end(), F_LHS.begin()); \
        shapingFilt.insert(shapingFilt.end(), F_LHS.begin(), F_LHS.end());        \
        F_LHS.clear();                                                            \
    }

//R = dt_new/dt
#define INIT_SPACE()                                                      \
    {                                                                     \
        X.resize(n0);                                                     \
        TC.resize(nfft);                                                  \
        shapingFilt.resize(nfft / 2);                                     \
        fft_in = (fftw_complex *)malloc(nfft * sizeof(fftw_complex));     \
        fft_out = (fftw_complex *)malloc(nfft * sizeof(fftw_complex));    \
        master_fft = (fftw_complex *)malloc(nfft * sizeof(fftw_complex)); \
        gatherXcorr.resize(nXCORR);                                       \
        INIT_SHAPINGFILT();                                               \
    }

/*
 * X is the input data and Y is the output
 *  X.size() == R * XX.size()
 *  R  = dt_new / dt = (0.008/0.002)
 */
#define PRE_PROCESSING(X, Y)                                                                          \
    {                                                                                                 \
        detrend(&(X[0]), X.size());                                                                   \
        filtfilt(BUTTER_A, BUTTER_B, X, Y);                                                           \
        resample(1, DT_NEW / DT, Y, X);                                                               \
        MOVING_MEAN(X, Y, nPoint_hal_win);                                                            \
        INIT_FFTW(fft_in, Y, nPoint, nfft, fft_out);                                                  \
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
        Y.resize(nPoint);                                                                             \
        for (int i = 0; i < nPoint; i++)                                                              \
        {                                                                                             \
            Y[i] = fft_out[i][0] / ((double)nfft);                                                    \
        }                                                                                             \
    }

#endif