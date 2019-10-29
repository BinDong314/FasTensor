

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

using namespace std;

//N0: the size of orignal vector
//nPoint: size after resample = N0 * 1/R (R = 4)
//nfft: power of 2 and > 2*nPoint - 1
int N0 = 50;

//Paramters fom butter(3, 2*0.25, 'low'), for filtfilt
//vector<double> A{0.0317, 0.0951, 0.0951, 0.0317};
//vector<double> B{1.0000, -1.4590, 0.9104, -0.1978};

vector<double> A{0.031689343849711, 0.095068031549133, 0.095068031549133, 0.031689343849711};
vector<double> B{1.000000000000000, -1.459029062228061, 0.910369000290069, -0.197825187264319};

//For resample, R = dt_new/dt
double dt = 0.002;
double dt_new = 0.008;

//For moveing mean
double winLen_sec = 0.5;
//int nPoint_hal_win = floor((2 * floor(winLen_sec / dt / 2) + 1) / 2);
int nPoint_hal_win = 1; //for test only
//int nPoint_mid_win = floor(nPoint_per_win / 2) + 1;  //not used, for reference
//int nPoint_per_win = 2 * floor(winLen_sec / dt / 2) + 1;//not used, for reference

//For interp1
double fNyquist = 62.5; // for test 0.5 / dt;
double fSampling = 2 * fNyquist;
int nPoint;
int nfft;
double df;
std::vector<double> z{0, 0.5, 1, 1, 0.5, 0};
std::vector<double> zf{0, 0.002, 0.006, 14.5, 15, 62.5};

std::vector<double> X;
std::vector<double> XX;
/*
 * X is the input data and XX is the output
 *  X.size() == R * XX.size()
 *  R  = dt_new / dt = (0.008/0.002)
 */
void fft_pre_processing(std::vector<double> X, std::vector<double> &XX)
{

    detrend(&(X[0]), X.size());
    cout << "After detrend(S =  " << X.size() << " ) : ";
    for (auto i : X)
        cout << i << ", ";
    cout << endl;

    filtfilt(A, B, X, XX);

    cout << "After filtfilt(S =  " << XX.size() << " ) : ";
    for (auto i : XX)
        cout << i << ", ";
    cout << endl;

    resample(1, dt_new / dt, XX, X);

    cout << "After resample(S =  " << X.size() << " ) : ";
    for (auto i : X)
        cout << i << ", ";
    cout << endl;

    /*
    double XResample[] = {-0.2346,
                          -0.9902,
                          0.8752,
                          0.3960,
                          0.0782,
                          0.4312,
                          0.3309,
                          -0.0480,
                          -1.1379,
                          -0.3032,
                          -0.1718,
                          -0.1262,
                          0.6404};
    for (int i = 0; i < X.size(); i++)
    {
        X[i] = XResample[i];
        cout << X[i] << ", ";
    }
    cout << endl;*/

    MOVING_MEAN(X, XX, nPoint_hal_win);
    //XX[10] = -0.8575;
    //XX[11] = -0.3293;

    cout << "After mv(S =  " << XX.size() << " ) : ";
    for (auto i : XX)
        cout << i << ", ";
    cout << endl;

    //We can only call this once;
    nPoint = XX.size();
    //nPoint = X.size();
    FIND_M_POWER2(nPoint, nfft);
    df = fSampling / nfft;
    std::vector<double> f_LHS, shapingFilt_LHS;
    f_LHS.resize(nfft / 2);
    shapingFilt_LHS.resize(nfft / 2);

    FILL_LHS(nfft / 2, f_LHS, df);

    cout << "After f_LHS(S =  " << f_LHS.size() << " ) : ";
    for (auto i : f_LHS)
        cout << i << ", ";
    cout << endl;

    //interp1(x (xf), y (z), new_x (f_LHS)), return new_y (shapingFilt_LHS)
    interp1(zf, z, f_LHS, shapingFilt_LHS);

    cout << "After interp1(S =  " << shapingFilt_LHS.size() << " ) : ";
    for (auto i : shapingFilt_LHS)
        cout << i << ", ";
    cout << endl;

    //Let's resue the f_LHS to store the shapingFilt_RHS, and  Merge f_LHS and f_LHS
    std::reverse_copy(shapingFilt_LHS.begin(), shapingFilt_LHS.end(), f_LHS.begin());
    shapingFilt_LHS.insert(shapingFilt_LHS.end(), f_LHS.begin(), f_LHS.end());

    //FFT on XX
    fftw_complex *fft_in_temp;
    fftw_complex *fft_out_temp;
    int fft_in_legnth = sizeof(fftw_complex) * nfft;
    fft_in_temp = (fftw_complex *)malloc(fft_in_legnth);
    fft_out_temp = (fftw_complex *)malloc(fft_in_legnth);

    printf("nfft = %d, nPoint = %d \n", nfft, nPoint);
    for (int iii = 0; iii < nfft; iii++)
    {
        if (iii < nPoint)
        {
            fft_in_temp[iii][0] = XX[iii];
            fft_in_temp[iii][1] = 0;
        }
        else
        {
            fft_in_temp[iii][0] = 0;
            fft_in_temp[iii][1] = 0;
        }
        fft_out_temp[iii][0] = 0;
        fft_out_temp[iii][1] = 0;
        printf("%f, ", fft_in_temp[iii][0]);
    }

    printf("\n");

    FFT_HELP_W(nfft, fft_in_temp, fft_out_temp, FFTW_FORWARD);

    double temp_f;
    for (int ii = 0; ii < nfft; ii++)
    {
        printf(" %f + %f i\n", fft_out_temp[ii][0], fft_out_temp[ii][1]);
        temp_f = sqrt(fft_out_temp[ii][0] * fft_out_temp[ii][0] + fft_out_temp[ii][1] * fft_out_temp[ii][1]) + 0.001;
        fft_in_temp[ii][0] = (fft_out_temp[ii][0] + 0.001) / temp_f * shapingFilt_LHS[ii];
        fft_in_temp[ii][1] = (fft_out_temp[ii][1]) / temp_f * shapingFilt_LHS[ii];
        fft_out_temp[ii][0] = 0;
        fft_out_temp[ii][1] = 0;
    }

    //iFFT
    FFT_HELP_W(nfft, fft_in_temp, fft_out_temp, FFTW_BACKWARD);
    XX.resize(nPoint);
    for (int i = 0; i < nPoint; i++)
    {
        XX[i] = fft_out_temp[i][0] / ((double)nfft);
    }
    free(fft_in_temp);
    free(fft_out_temp);

    cout << "After fft/ifft(S =  " << XX.size() << " ) : ";
    for (auto i : XX)
        cout << i << ", ";
    cout << endl;
}

int main()
{
    double testdata[] = {
        0.537667139546100,
        1.833885014595087,
        -2.258846861003648,
        0.862173320368121,
        0.318765239858981,
        -1.307688296305273,
        -0.433592022305684,
        0.342624466538650,
        3.578396939725761,
        2.769437029884877,
        -1.349886940156521,
        3.034923466331855,
        0.725404224946106,
        -0.063054873189656,
        0.714742903826096,
        -0.204966058299775,
        -0.124144348216312,
        1.489697607785465,
        1.409034489800479,
        1.417192413429614,
        0.671497133608081,
        -1.207486922685038,
        0.717238651328838,
        1.630235289164729,
        0.488893770311789,
        1.034693009917860,
        0.726885133383238,
        -0.303440924786016,
        0.293871467096658,
        -0.787282803758638,
        0.888395631757642,
        -1.147070106969151,
        -1.068870458168032,
        -0.809498694424876,
        -2.944284161994896,
        1.438380292815098,
        0.325190539456198,
        -0.754928319169703,
        1.370298540095228,
        -1.711516418853698,
        -0.102242446085491,
        -0.241447041607358,
        0.319206739165502,
        0.312858596637428,
        -0.864879917324456,
        -0.030051296196269,
        -0.164879019209038,
        0.627707287528727,
        1.093265669039484,
        1.109273297614398,
    };
    X.resize(N0);
    for (int i = 0; i < N0; i++)
    {
        /*if (i % 2 != 0)
        {
            X[i] = i - 0.01;
        }
        else
        {
            X[i] = i + 0.01;
        }*/
        X[i] = testdata[i];
    }

    for (auto i : X)
        cout << i << ", ";
    cout << endl;

    fft_pre_processing(X, XX);
}