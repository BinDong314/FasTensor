#ifndef DASLIB_H
#define DASLIB_H

#include "DasLib3rd.h"
#include "DasLib_filtfilt.h"

#include <complex>
#include <cmath>
#include <fftw3.h>

namespace DasLib
{

//double rms(double x[], int n)
template <class T>
inline T Rms(std::vector<T> &x)
{
    T sum = 0;
    int n = x.size();
    for (int i = 0; i < n; i++)
        sum += pow(x[i], 2);

    return sqrt(sum / n);
}

template <class T>
inline T Mean(std::vector<T> &v)
{
    T sum = 0;
    for (int i = 0; i < v.size(); i++)
    {
        sum = sum + v[i];
    }
    return sum / v.size();
}
// Function for calculating median
template <class T>
inline T Median(std::vector<T> &v)
{
    sort(v.begin(), v.end());
    size_t n = v.size();
    // check for even case
    if (n % 2 != 0)
        return v[n / 2];

    return (v[(n - 1) / 2] + v[n / 2]) / 2.0;
}

// n = order of the filter
// fc = filter cutoff frequency as a fraction of Pi [0,1]
int ButterLow(int n, double fcf, std::vector<double> &A, std::vector<double> &B)
{
    int sff = 1;  // scale flag: 1 to scale, 0 to not scale ccof
    int i;        // loop variables
    double sf;    // scaling factor
    double *dcof; // d coefficients
    int *ccof;    // c coefficients

    /* calculate the d coefficients */
    dcof = dcof_bwlp(n, fcf);
    if (dcof == NULL)
    {
        perror("Unable to calculate d coefficients");
        return (-1);
    }

    /* calculate the c coefficients */
    ccof = ccof_bwlp(n);
    if (ccof == NULL)
    {
        perror("Unable to calculate c coefficients");
        return (-1);
    }

    sf = sf_bwlp(n, fcf); /* scaling factor for the c coefficients */

    A.resize(n + 1);
    B.resize(n + 1);
    /* Output the c (A) coefficients */
    if (sff == 0)
    {
        for (i = 0; i <= n; ++i)
        {
            A[i] = ccof[i];
        }
    }
    else
    {
        for (i = 0; i <= n; ++i)
        {
            A[i] = (double)ccof[i] * sf;
        }
    }

    /* Output the d(B) coefficients */
    for (i = 0; i <= n; ++i)
    {
        B[i] = dcof[i];
    }

    free(dcof);
    free(ccof);

    return 0;
}

template <class T>
inline void DeleteMedian(std::vector<std::vector<T>> &v)
{
    int rows = v.size();
    int cols = v[0].size();

    //std::cout << "rows = " << rows << ", cols = " << cols << "\n";
    std::vector<T> temp_cols(rows);
    T temp_median;
    for (int j = 0; j < cols; j++)
    {
        for (int i = 0; i < rows; i++)
        {
            temp_cols[i] = v[i][j];
        }
        temp_median = Median(temp_cols);
        for (int i = 0; i < rows; i++)
        {
            v[i][j] = v[i][j] - temp_median;
        }
    }
}

/**
 * @brief 
 * 
 * @tparam T 
 * @param v input time series data
 * @param start_t, start time of correlation fh(9) in Matlab code, e.g., -60
 * @param end_t,  end time of correlation fh(9) in Matlab code, e.g., 60
 * @param sub_start_t, start time of subset 
 * @param sub_end_t, end time of subset 
 * @param smaple_rate, interval of each point, fh(8) in Matlab code , 0.008
 * @return std::vector<T>  return sub array
 */
template <class T>
inline std::vector<T> TimeSubset(std::vector<T> &v, double start_t, double end_t, double sub_start_t, double sub_end_t, double smaple_rate)
{
    size_t sub_start_index = round((sub_start_t - start_t) / smaple_rate);
    size_t sub_end_index = round((sub_end_t - start_t) / smaple_rate);

    //std::cout << "sub_start_index = " << sub_start_index << ", sub_end_index = " << sub_end_index << "\n";
    assert(sub_start_index >= 0);
    assert(sub_end_index >= 0);
    assert(sub_end_index >= sub_start_index);
    std::vector<T> v_subset;
    v_subset.resize(sub_end_index - sub_start_index + 1);

    size_t v_subset_index = 0;
    for (int i = sub_start_index; i <= sub_end_index; i++)
    {
        v_subset[v_subset_index] = v[i];
        v_subset_index++;
    }

    return v_subset;
}
/**
 * @brief infer the size of vector after time subset
 * 
 * @param start_t 
 * @param end_t 
 * @param sub_start_t 
 * @param sub_end_t 
 * @param smaple_rate 
 * @return size_t 
 */
inline size_t InferTimeSubsetSize(double start_t, double end_t, double sub_start_t, double sub_end_t, double smaple_rate)
{
    size_t sub_start_index = round((sub_start_t - start_t) / smaple_rate + 1);
    size_t sub_end_index = round((sub_end_t - start_t) / smaple_rate + 1);

    return sub_end_index - sub_start_index + 1;
}

typedef std::vector<std::vector<double>> DoubleVector2D;

template <class T>
inline std::vector<std::vector<T>> Vector1D2D(size_t cols, std::vector<T> &data1d)
{
    std::vector<std::vector<T>> result;
    size_t rows = data1d.size() / cols;
    result.resize(rows);
    for (std::size_t i = 0; i < rows; ++i)
    {
        result[i].resize(cols);
        for (std::size_t j = 0; j < cols; ++j)
        {
            result[i][j] = data1d[i * cols + j];
        }
    }
    return result;
}

template <class T>
inline bool CausalityFlagging(std::vector<std::vector<T>> &v, double tmin, double tmax, double fmax, double start_t, double end_t, double smaple_rate)
{
    int N = v.size();
    std::vector<std::vector<T>> tv1, tv2, tv3, tv4;
    tv1.resize(N);
    tv2.resize(N);
    tv3.resize(N);
    tv4.resize(N);

    std::vector<double> A, B;
    ButterLow(3, 0.16, A, B);

    /*PrintScalar("N", N);
    PrintVector("A", A);
    PrintVector("B", B);
    PrintScalar("start_t", start_t);
    PrintScalar("end_t", end_t);
    PrintScalar("smaple_rate", smaple_rate);
    PrintScalar("tmax", tmax);*/

    for (int i = 0; i < N; i++)
    {
        tv1[i] = TimeSubset(v[i], start_t, end_t, -tmax, tmax, smaple_rate);
        filtfilt(A, B, tv1[i], tv2[i]);
        tv3[i] = TimeSubset(tv2[i], -tmax, tmax, tmin, tmax * 0.9, smaple_rate);   //causal
        tv4[i] = TimeSubset(tv2[i], -tmax, tmax, -tmax * 0.9, -tmin, smaple_rate); //acausal
    }
    std::vector<double> rms_acausal(N), rms_causal(N);

    for (int i = 0; i < N; i++)
    {
        rms_causal[i] = Rms(tv3[i]);
        rms_acausal[i] = Rms(tv4[i]);
    }

    //PrintVector("rms_causal: ", rms_causal);
    //PrintVector("rms_acausal: ", rms_acausal);

    double rms_acausal_mean = Mean(rms_acausal);
    double rms_causal_mean = Mean(rms_causal);
    //std::cout << "rms_causal_mean = " << rms_causal_mean << ", rms_causal_mean =" << rms_causal_mean << "\n";
    if (rms_acausal_mean < rms_causal_mean)
    {
        return true;
    }
    else
    {
        return false;
    }
} // namespace DasLib

template <class T>
inline std::vector<std::complex<T>> Hilbert(std::vector<T> &in)
{
    int INN = in.size();
    //std::cout << "INN =" << INN << "\n";
    fftw_complex *in_temp = (fftw_complex *)fftw_alloc_complex(sizeof(fftw_complex) * INN);
    //std::memset(in_temp, 0, sizeof(fftw_complex) * INN);

    for (size_t i = 0; i < INN; i++)
    {
        in_temp[i][0] = in[i];
        in_temp[i][1] = 0;
    }

    fftw_complex *out_temp = (fftw_complex *)fftw_alloc_complex(sizeof(fftw_complex) * INN);
    //std::memset(out_temp, 0, sizeof(fftw_complex) * INN);

    fftw_plan pf = fftw_plan_dft_1d(INN, in_temp, out_temp, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(pf);
    fftw_destroy_plan(pf);
    //fftw_cleanup();

    size_t HN = INN >> 1;
    size_t numRem = HN;
    for (size_t i = 1; i < HN; ++i)
    {
        out_temp[i][0] *= 2;
        out_temp[i][1] *= 2;
    }

    if (INN % 2 == 0)
    {
        numRem--;
    }
    else if (INN > 1)
    {
        out_temp[HN][0] *= 2;
        out_temp[HN][1] *= 2;
    }

    memset(&out_temp[HN + 1][0], 0, numRem * sizeof(fftw_complex));

    //std::memset(in_temp, 0, sizeof(fftw_complex) * INN);

    fftw_plan pf2 = fftw_plan_dft_1d(INN, out_temp, in_temp, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(pf2);
    fftw_destroy_plan(pf2);
    //std::cout << "INN 2 =" << INN << "\n";

    std::vector<std::complex<T>> out(INN);
    for (size_t i = 0; i < INN; ++i)
    {
        out[i].real(in_temp[i][0] / INN);
        out[i].imag(in_temp[i][1] / INN);
    }
    fftw_free(out_temp);
    fftw_free(in_temp);
    fftw_cleanup();

    return out;
}

//
//phiMat = angle(hilbert(gather));
//phaseshiftMat = exp(sqrt(-1)*phiMat);
template <class T>
inline std::vector<std::complex<T>> instanPhaseEstimator(std::vector<T> &v)
{
    std::vector<std::complex<T>> ov;
    ov = Hilbert(v);
    size_t N = ov.size();
    T ov_angle;
    std::complex<T> minus_one(0, -1);
    for (int i = 0; i < N; i++)
    {
        ov_angle = std::arg(ov[i]);
        ov[i] = std::exp(minus_one * ov_angle);
    }
    return ov;
}

} // namespace DasLib

#endif