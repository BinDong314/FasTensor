#include <complex.h>
#include <fftw3.h>
#define NAME_LENGTH 1024

#define INIT_FFTW(FFT_IN_V, XXX_P, XN_P, NFFT_P, FFT_OUT_V) \
    {                                                       \
        for (int iii = 0; iii < NFFT_P; iii++)              \
        {                                                   \
            if (iii < XN_P)                                 \
            {                                               \
                FFT_IN_V[iii][0] = XXX_P[iii];              \
                FFT_IN_V[iii][1] = 0;                       \
            }                                               \
            else                                            \
            {                                               \
                FFT_IN_V[iii][0] = 0;                       \
                FFT_IN_V[iii][1] = 0;                       \
            }                                               \
            FFT_OUT_V[iii][0] = 0;                          \
            FFT_OUT_V[iii][1] = 0;                          \
        }                                                   \
    }

//Add function to init FFT_IN
#define INIT_FFTW_FILL(FFT_IN_V, XXX_P, XN_P, NFFT_P) \
    {                                                 \
        for (int iii = 0; iii < NFFT_P; iii++)        \
        {                                             \
            if (iii < XN_P)                           \
            {                                         \
                FFT_IN_V[iii][0] = XXX_P[iii];        \
                FFT_IN_V[iii][1] = 0;                 \
            }                                         \
            else                                      \
            {                                         \
                FFT_IN_V[iii][0] = 0;                 \
                FFT_IN_V[iii][1] = 0;                 \
            }                                         \
        }                                             \
    }

#define INIT_FFTW_V_ZERO(FFT_IN_V, NFFT_P)     \
    {                                          \
        for (int iii = 0; iii < NFFT_P; iii++) \
        {                                      \
            FFT_IN_V[iii][0] = 0;              \
            FFT_IN_V[iii][1] = 0;              \
        }                                      \
    }

//direction: FFTW_FORWARD,  FFTW_BACKWARD
#define FFT_HELP_W(NNN, fft_in_p, fft_out_p, direction_p)                               \
    {                                                                                   \
        fftw_plan fft_p;                                                                \
        fft_p = fftw_plan_dft_1d(NNN, fft_in_p, fft_out_p, direction_p, FFTW_ESTIMATE); \
        fftw_execute(fft_p);                                                            \
        fftw_destroy_plan(fft_p);                                                       \
    }

//#include "fft/kiss_fft.h"
//kiss_fft_cpx *fft_in_temp;
//kiss_fft_cpx *fft_out_temp;
//kiss_fft_cpx *master_vector_fft;
//unsigned int fft_in_legnth;
//direction: 0,  1
#define FFT_HELP_K(N, fft_in, fft_out, direction)       \
    {                                                   \
        kiss_fft_cfg cfg;                               \
        cfg = kiss_fft_alloc(N, direction, NULL, NULL); \
        kiss_fft(cfg, fft_in, fft_out);                 \
        free(cfg);                                      \
    }

// Use "xi >= XN - HAL_WIN - 1" to match matlab code
#define MOVING_MEAN(XV, YV, HAL_WIN)                                \
    {                                                               \
        unsigned long long XN = XV.size(), index1, index2;          \
        YV.resize(XN);                                              \
        for (unsigned long long xi = 0; xi < XN; xi++)              \
        {                                                           \
            if (xi < HAL_WIN)                                       \
            {                                                       \
                index1 = 0;                                         \
                index2 = xi;                                        \
            }                                                       \
            else if (xi >= XN - HAL_WIN - 1)                        \
            {                                                       \
                index1 = xi;                                        \
                index2 = XN - 1;                                    \
            }                                                       \
            else                                                    \
            {                                                       \
                index1 = xi - HAL_WIN;                              \
                index2 = xi + HAL_WIN;                              \
            }                                                       \
                                                                    \
            double norm_denom = 0;                                  \
            for (int j = index1; j <= index2; j++)                  \
            {                                                       \
                norm_denom = norm_denom + std::abs(XV[j]);          \
            }                                                       \
            YV[xi] = XV[xi] / (norm_denom / (index2 - index1 + 1)); \
        }                                                           \
    }
//It is power of 2 and greater than minimum_m
#define FIND_M_POWER2(MIN_M, M)                \
    {                                          \
        unsigned long long mt = 2 * MIN_M - 1; \
        while ((mt & (mt - 1)) != 0)           \
        {                                      \
            mt = mt + 1;                       \
        }                                      \
        M = mt;                                \
    }

#define FILL_LHS(NFFT, F_LHS, DF)      \
    {                                  \
        for (int i = 0; i < NFFT; i++) \
        {                              \
            F_LHS[i] = DF * (i + 1);   \
        }                              \
    }

template <typename T>
void detrend(T *y, int m)
{
    T xmean, ymean;
    int i;
    T Sxy;
    T Sxx;

    T grad;
    T yint;

    std::unique_ptr<T[]> x(new T[m]);

    /********************************
    Set the X axis Liner Values
    *********************************/
    for (i = 0; i < m; i++)
        x[i] = i;

    /********************************
    Calculate the mean of x and y
    *********************************/
    xmean = 0;
    ymean = 0;
    for (i = 0; i < m; i++)
    {
        xmean += x[i];
        ymean += y[i];
    }
    xmean /= m;
    ymean /= m;

    /********************************
    Calculate Covariance
    *********************************/

    Sxy = 0;
    for (i = 0; i < m; i++)
        Sxy += (x[i] - xmean) * (y[i] - ymean);

    Sxx = 0;
    for (i = 0; i < m; i++)
        Sxx += (x[i] - xmean) * (x[i] - xmean);

    /********************************
    Calculate Gradient and Y intercept
    *********************************/
    grad = Sxy / Sxx;
    yint = -grad * xmean + ymean;

    /********************************
    Removing Linear Trend
    *********************************/
    for (i = 0; i < m; i++)
    {
        y[i] = y[i] - (grad * i + yint);
    }
}

/*
 *filtfilt
 */

#include <vector>
#include <exception>
#include <algorithm>
#include "Eigen/Dense"

typedef std::vector<int> vectori;
typedef std::vector<double> vectord;

using namespace Eigen;

void add_index_range(vectori &indices, int beg, int end, int inc = 1)
{
    for (int i = beg; i <= end; i += inc)
        indices.push_back(i);
}

void add_index_const(vectori &indices, int value, size_t numel)
{
    while (numel--)
        indices.push_back(value);
}

void append_vector(vectord &vec, const vectord &tail)
{
    vec.insert(vec.end(), tail.begin(), tail.end());
}

vectord subvector_reverse(const vectord &vec, int idx_end, int idx_start)
{
    vectord result(&vec[idx_start], &vec[idx_end + 1]);
    std::reverse(result.begin(), result.end());
    return result;
}

inline int max_val(const vectori &vec)
{
    return std::max_element(vec.begin(), vec.end())[0];
}

void filtfilt(vectord B, vectord A, const vectord &X, vectord &Y);

void filter(vectord B, vectord A, const vectord &X, vectord &Y, vectord &Zi)
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

void filtfilt(vectord B, vectord A, const vectord &X, vectord &Y)
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

    vectori rows, cols;
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
    vectord data;
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

    vectord leftpad = subvector_reverse(X, nfact, 1);
    double _2x0 = 2 * X[0];
    std::transform(leftpad.begin(), leftpad.end(), leftpad.begin(), [_2x0](double val) { return _2x0 - val; });

    vectord rightpad = subvector_reverse(X, len - 2, len - nfact - 1);
    double _2xl = 2 * X[len - 1];
    std::transform(rightpad.begin(), rightpad.end(), rightpad.begin(), [_2xl](double val) { return _2xl - val; });

    double y0;
    vectord signal1, signal2, zi;

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

/*
 *  interp1
 */
template <typename Real>
int nearestNeighbourIndex(std::vector<Real> &x, Real &value)
{
    Real dist = std::numeric_limits<Real>::max();
    Real newDist = dist;
    size_t idx = 0;

    for (size_t i = 0; i < x.size(); ++i)
    {
        newDist = std::abs(value - x[i]);
        if (newDist <= dist)
        {
            dist = newDist;
            idx = i;
        }
    }

    return idx;
}

template <typename Real>
void interp1(std::vector<Real> &x, std::vector<Real> &y, std::vector<Real> &x_new, std::vector<Real> &y_new)
{
    Real dx, dy, m, b;
    size_t x_max_idx = x.size() - 1;
    size_t x_new_size = x_new.size();

    for (size_t i = 0; i < x_new_size; ++i)
    {
        size_t idx = nearestNeighbourIndex(x, x_new[i]);

        if (x[idx] > x_new[i])
        {
            dx = idx > 0 ? (x[idx] - x[idx - 1]) : (x[idx + 1] - x[idx]);
            dy = idx > 0 ? (y[idx] - y[idx - 1]) : (y[idx + 1] - y[idx]);
        }
        else
        {
            dx = idx < x_max_idx ? (x[idx + 1] - x[idx]) : (x[idx] - x[idx - 1]);
            dy = idx < x_max_idx ? (y[idx + 1] - y[idx]) : (y[idx] - y[idx - 1]);
        }

        m = dy / dx;
        b = y[idx] - x[idx] * m;

        y_new[i] = (x_new[i] * m + b);
    }
}