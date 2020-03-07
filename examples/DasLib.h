#ifndef DASLIB_H
#define DASLIB_H

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
inline bool causalityflagging(std::vector<T> &v, double tmin, double tmax, double fmax, double start_t, double end_t, double smaple_rate)
{
    std::vector<T> v_sub;
    v_sub = TimeSubset(v, start_t, end_t, -tmax, tmax, smaple_rate);
    //dsi_filter_iir
}

template <classname T>
inline void Hilbert(std::vector<T> &in, fftw_complex *out)
{
    size_t INN = v.size();
    for (size_t i = 0; i < INN; i++)
    {
        out[i][REAL] = in[i];
        out[i][IMAG] = 0;
    }

    fftw_plan pf = fftw_plan_dft_1d(INN, out, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(pf);
    fftw_destroy_plan(pf);

    size_t HN = INN >> 1;
    size_t numRem = HN;
    for (size_t i = 0; i < HN; i++)
    {
        out[i][REAL] *= 2;
        out[i][IMAG] *= 2;
    }

    if (INN % 2 == 0)
    {
        numRem--;
    }
    else if (INN > 1)
    {
        out[HN][REAL] *= 2;
        out[HN][IMAG] *= 2;
    }

    memset(&out[HW + 1][REAL], 0, numRem * sizeof(fftw_complex));

    pf = fftw_plan_dft_1d(INN, out, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(pf);
    fftw_destroy_plan(pf);

    fftw_cleanup();

    for (size_t i = 0; i < INN; i++)
    {
        out[i][REAL] /= INN;
        out[i][IMAG] /= INN;
    }
}

template <classname T = double>
inline void Angle(fftw_complex *v, size_t N, std::vector<T> &out)
{
    for (size_t i = 0; i < N; i++)
    {
        out[i] = atan(v[i][REAL] / v[i][IMAG]);
    }
}

} // namespace DasLib

#endif