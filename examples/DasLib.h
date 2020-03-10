#ifndef DASLIB_H
#define DASLIB_H

#include "DasLib3rd.h"

namespace DasLib
{
// Function for calculating median
template <classname T>
inline T Median(std::vector<T> &v)
{
    sort(v.begin(), v.end());
    size_t n = v.size;
    // check for even case
    if (n % 2 != 0)
        return v[n / 2];

    return (v[(n - 1) / 2] + v[n / 2]) / 2.0;
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
 * @param smaple_rate, interval of each point, fh(8) in Matlab code 
 * @return std::vector<T>  return sub array
 */
template <classname T>
inline std::vector<T> TimeSubset(std::vector<T> &v, double start_t, double end_t, double sub_start_t, double sub_end_t, double smaple_rate)
{
    size_t sub_start_index, sub_end_index;
    sub_start_index = round((sub_start_t - start_t) / smaple_rate + 1);
    sub_end_index = round((sub_end_t - start_t) / smaple_rate + 1);

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

template <classname T>
inline bool CausalityFlagging(std::vector<T> &v, double tmin, double tmax, double fmax, double start_t, double end_t, double smaple_rate)
{
    std::vector<T> v_sub;
    v_sub = TimeSubset(v, start_t, end_t, -tmax, tmax, smaple_rate);

    /*
      dsi_filter_iir
    */
    std::vector<double> A, B;
    butter_low(3, 0.16, A, B);
    std::vector<T> v_sub_new;
    filtfilt(B, A, v_sub, v_sub_new);

    std::vector<double> Dsi_causal = TimeSubset(v_sub_new, tmin, tmax * 0.9);
    std::vector<double> rms_causal = TimeSubset(v_sub_new, -tmax * 0.9, -tmin);
}

//double rms(double x[], int n)
template <classname T>
T Rms(std::vector<T> &x, int n)
{
    T sum = 0;

    for (int i = 0; i < n; i++)
        sum += pow(x[i], 2);

    return sqrt(sum / n);
}

template <classname T>
inline void Hilbert(std::vector<T> &in, std::vector<std::complex<T>> &out)
{
    size_t INN = v.size();
    fftw_complex *in_temp = malloc(sizeof(fftw_complex() * INN);
    std::memset(in_temp, 0, sizeof(fftw_complex() * INN );

    for (size_t i = 0; i < INN; i++)
    {
        in_temp[i][REAL] = in[i];
        in_temp[i][IMAG] = 0;
    }

    fftw_complex *out_temp = malloc(sizeof(fftw_complex() * INN);
    std::memset(out_temp, 0, sizeof(fftw_complex() * INN );
    fftw_plan pf = fftw_plan_dft_1d(INN, in_temp, out_temp, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(pf);
    fftw_destroy_plan(pf);

    size_t HN = INN >> 1;
    size_t numRem = HN;
    for (size_t i = 0; i < HN; i++)
    {
        out_temp[i][REAL] *= 2;
        out_temp[i][IMAG] *= 2;
    }

    if (INN % 2 == 0)
    {
        numRem--;
    }
    else if (INN > 1)
    {
        out_temp[HN][REAL] *= 2;
        out_temp[HN][IMAG] *= 2;
    }

    memset(&out_temp[HW + 1][REAL], 0, numRem * sizeof(fftw_complex));

    std::memset(in_temp, 0, sizeof(fftw_complex() * INN );

    pf = fftw_plan_dft_1d(INN, out_temp, in_temp, FFTW_BACKWORD, FFTW_ESTIMATE);
    fftw_execute(pf);
    fftw_destroy_plan(pf);

    fftw_cleanup();

    free(out_temp);

    out.resize(INN);
    for (size_t i = 0; i < INN; i++)
    {
        out[i].real(in_temp[i][REAL] / INN);
        out[i].imag(in_temp[i][IMAG] / INN);
    }
    free(in_temp);
}

/*
 *filtfilt
 */

#include <vector>
#include <exception>
#include <algorithm>
#include "Eigen/Dense"

using namespace Eigen;

inline void add_index_range(std::vector<int> &indices, int beg, int end, int inc = 1)
{
    for (int i = beg; i <= end; i += inc)
        indices.push_back(i);
}

inline void add_index_const(std::vector<int> &indices, int value, size_t numel)
{
    while (numel--)
        indices.push_back(value);
}

template <classname T = double>
inline void append_vector(std::vector<T> &vec, const std::vector<T> &tail)
{
    vec.insert(vec.end(), tail.begin(), tail.end());
}

template <classname T = double>
std::vector<T> subvector_reverse(const std::vector<T> &vec, int idx_end, int idx_start)
{
    std::vector<T> result(&vec[idx_start], &vec[idx_end + 1]);
    std::reverse(result.begin(), result.end());
    return result;
}

inline int max_val(const std::vector<int> &vec)
{
    return std::max_element(vec.begin(), vec.end())[0];
}

template <classname T = double>
inline void filter(std::vector<T> B, std::vector<T> A, const std::vector<T> &X, std::vector<T> &Y, std::vector<T> &Zi)
{
    if (A.empty())
        throw std::domain_error("The feedback filter coefficients are empty.");
    if (std::all_of(A.begin(), A.end(), [](double coef) { return coef == 0; }))
        throw std::domain_error("At least one of the feedback filter coefficients has to be non-zero.");
    if (A[0] == 0)
        throw std::domain_error("First feedback coefficient has to be non-zero.");

    // Normalize feedback coefficients if a[0] != 1;
    auto a0 = A[0];
    if (a0 != 1.0)
    {
        std::transform(A.begin(), A.end(), A.begin(), [a0](double v) { return v / a0; });
        std::transform(B.begin(), B.end(), B.begin(), [a0](double v) { return v / a0; });
    }

    size_t input_size = X.size();
    size_t filter_order = std::max(A.size(), B.size());
    B.resize(filter_order, 0);
    A.resize(filter_order, 0);
    Zi.resize(filter_order, 0);
    Y.resize(input_size);

    const double *x = &X[0];
    const double *b = &B[0];
    const double *a = &A[0];
    double *z = &Zi[0];
    double *y = &Y[0];

    for (size_t i = 0; i < input_size; ++i)
    {
        size_t order = filter_order - 1;
        while (order)
        {
            if (i >= order)
                z[order - 1] = b[order] * x[i - order] - a[order] * y[i - order] + z[order];
            --order;
        }
        y[i] = b[0] * x[i] + z[0];
    }
    Zi.resize(filter_order - 1);
}

template <classname T = double>
inline void Filtfilt(std::vector<T> B, std::vector<T> A, const std::vector<T> &X, std::vector<T> &Y)
{
    int len = X.size(); // length of input
    int na = A.size();
    int nb = B.size();
    int nfilt = (nb > na) ? nb : na;
    int nfact = 3 * (nfilt - 1); // length of edge transients

    if (len <= nfact)
    {
        printf("len = %d, nfact = %d \n", len, nfact);
        throw std::domain_error("Input data too short! Data must have length more than 3 times filter order.");
    }

    // set up filter's initial conditions to remove DC offset problems at the
    // beginning and end of the sequence
    B.resize(nfilt, 0);
    A.resize(nfilt, 0);

    std::vector<int> rows, cols;
    //rows = [1:nfilt-1           2:nfilt-1             1:nfilt-2];
    add_index_range(rows, 0, nfilt - 2);
    if (nfilt > 2)
    {
        add_index_range(rows, 1, nfilt - 2);
        add_index_range(rows, 0, nfilt - 3);
    }
    //cols = [ones(1,nfilt-1)         2:nfilt-1          2:nfilt-1];
    add_index_const(cols, 0, nfilt - 1);
    if (nfilt > 2)
    {
        add_index_range(cols, 1, nfilt - 2);
        add_index_range(cols, 1, nfilt - 2);
    }
    // data = [1+a(2)         a(3:nfilt)        ones(1,nfilt-2)    -ones(1,nfilt-2)];

    auto klen = rows.size();
    std::vector<T> data;
    data.resize(klen);
    data[0] = 1 + A[1];
    int j = 1;
    if (nfilt > 2)
    {
        for (int i = 2; i < nfilt; i++)
            data[j++] = A[i];
        for (int i = 0; i < nfilt - 2; i++)
            data[j++] = 1.0;
        for (int i = 0; i < nfilt - 2; i++)
            data[j++] = -1.0;
    }

    std::vector<T> leftpad = subvector_reverse(X, nfact, 1);
    double _2x0 = 2 * X[0];
    std::transform(leftpad.begin(), leftpad.end(), leftpad.begin(), [_2x0](double val) { return _2x0 - val; });

    std::vector<T> rightpad = subvector_reverse(X, len - 2, len - nfact - 1);
    double _2xl = 2 * X[len - 1];
    std::transform(rightpad.begin(), rightpad.end(), rightpad.begin(), [_2xl](double val) { return _2xl - val; });

    double y0;
    std::vector<T> signal1, signal2, zi;

    signal1.reserve(leftpad.size() + X.size() + rightpad.size());
    append_vector(signal1, leftpad);
    append_vector(signal1, X);
    append_vector(signal1, rightpad);

    // Calculate initial conditions
    MatrixXd sp = MatrixXd::Zero(max_val(rows) + 1, max_val(cols) + 1);
    for (size_t k = 0; k < klen; ++k)
        sp(rows[k], cols[k]) = data[k];
    auto bb = VectorXd::Map(B.data(), B.size());
    auto aa = VectorXd::Map(A.data(), A.size());
    MatrixXd zzi = (sp.inverse() * (bb.segment(1, nfilt - 1) - (bb(0) * aa.segment(1, nfilt - 1))));
    zi.resize(zzi.size());

    // Do the forward and backward filtering
    y0 = signal1[0];
    std::transform(zzi.data(), zzi.data() + zzi.size(), zi.begin(), [y0](double val) { return val * y0; });
    filter(B, A, signal1, signal2, zi);
    std::reverse(signal2.begin(), signal2.end());
    y0 = signal2[0];
    std::transform(zzi.data(), zzi.data() + zzi.size(), zi.begin(), [y0](double val) { return val * y0; });
    filter(B, A, signal2, signal1, zi);
    Y = subvector_reverse(signal1, signal1.size() - nfact - 1, nfact);
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

template <classname T = double>
Flipud(std::vector<T> &v, size_t rows, size_t cols)
{
    std::vector<T> vo;
    vo.resize(rows * cols);
    for ()
    {
    }
    return vo;
}

//
//phiMat = angle(hilbert(gather));
//phaseshiftMat = exp(sqrt(-1)*phiMat);
template <classname T = double>
inline std::vector<std::complex<T>> instanPhaseEstimator(std::vector<T> &v)
{
    std::vector<std::complex<T>> ov;
    T ov_angle;
    Hilbert(v, ov);
    size_t N = ov.size();
    std::complex<T> minus_one(-1, 0);
    for (int i = 0; i < N; i++)
    {
        ov_angle = std::arg(ov[i]);
        ov[i] = std::exp(minus_one * ov_angle);
    }
    return ov;
}

} // namespace DasLib

#endif