/*
 *ArrayUDF Copyright (c) 2017, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy).  All rights reserved.

 *If you have questions about your rights to use or distribute this software, please contact Berkeley Lab's Innovation & Partnerships Office at  IPO@lbl.gov.

* NOTICE. This Software was developed under funding from the U.S. Department of Energy and the U.S. Government consequently retains certain rights. As such, the U.S. Government has been granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software to reproduce, distribute copies to the public, prepare derivative works, and perform publicly and display publicly, and to permit other to do so. 
 */
/**
 *
 * Email questions to dbin@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include "au.h"
#include "DasLib.h"

using namespace std;
using namespace AU;

#define LTS 14999 //length of time series
#define CHS 201   //channels

double t_start = -59.9920000000000, t_end = 59.9920000000000, sample_rate = 0.00800000000000000;

std::vector<unsigned long long> sc_size(2);

Array<double> *semblance_denom_sum;
Array<std::complex<double>> *coherency_sum;
Array<double> *data_in_sum;

inline Stencil<double>
stack_udf(const Stencil<double> &iStencil)
{
    std::vector<int> start_offset{0, 0}, end_offset{CHS - 1, LTS - 1};
    std::vector<double> ts = iStencil.Read(start_offset, end_offset);
    std::vector<std::vector<double>> ts2d = DasLib::Vector1D2D(LTS, ts);

    std::cout << "Read data: ts2d.size = " << ts2d.size() << " x " << ts2d[0].size() << std::endl;
    //Remove the media
    for (int i = 0; i < CHS; i++)
    {
        double median = DasLib::Median(ts2d[i]);
        for (int j = 0; j < LTS; j++)
        {
            ts2d[i][j] = ts2d[i][j] - median;
        }
        //Subset
        //ts2d[i] = DasLib::TimeSubset(ts2d[i], t_start, t_end, -59, 59, sample_rate);
    }
    /*
    bool flag = CausalityFlagging(ts_sub, 0.05, 3.0, 10, t_start, t_end, sample_rate);
    if (flag == flase)
    {
        //Flipud(ts_sub);
    }*/
    size_t LTS_new = ts2d[0].size();
    //std::cout << " LTS_new = " << LTS_new << "\n";
    std::vector<std::vector<double>> semblance_denom;
    std::vector<std::vector<std::complex<double>>> coherency;
    semblance_denom.resize(CHS);
    coherency.resize(CHS);
    for (int i = 0; i < CHS; i++)
    {
        semblance_denom[i].resize(LTS_new);
        coherency[i].resize(LTS_new);
        for (int j = 0; j < LTS_new; j++)
        {
            semblance_denom[i][j] = ts2d[i][j] * ts2d[i][j];
        }

        coherency[i] = DasLib::instanPhaseEstimator(ts2d[i]);
    }

    std::vector<unsigned long long> H_start{0, 0}, H_end{CHS - 1, LTS_new - 1};
    std::vector<double> semblance_denom_sum_v = semblance_denom_sum->ReadArray(H_start, H_end);
    std::vector<std::complex<double>> coherency_sum_v = coherency_sum->ReadArray(H_start, H_end);
    std::vector<double> data_in_sum_v = data_in_sum->ReadArray(H_start, H_end);

    int offset;
    for (int i = 0; i < CHS; i++)
    {
        for (int j = 0; j < LTS_new; j++)
        {
            offset = i * LTS_new + j;
            coherency_sum_v[offset] = coherency_sum_v[offset] + coherency[i][j];
            semblance_denom_sum_v[offset] = semblance_denom_sum_v[offset] + semblance_denom[i][j];
            data_in_sum_v[offset] = data_in_sum_v[offset] + ts2d[i][j];
        }
    }

    semblance_denom_sum->WriteArray(H_start, H_end, semblance_denom_sum_v);
    coherency_sum->WriteArray(H_start, H_end, coherency_sum_v);
    data_in_sum->WriteArray(H_start, H_end, data_in_sum_v);

    //std::cout << "finish one file, temp_index " << std::endl;
    return 0;
}

int main(int argc, char *argv[])
{
    //Init the MPICH, etc.
    AU_Init(argc, argv);

    // set up the chunk size and the overlap size
    std::vector<int> chunk_size = {CHS, LTS};
    std::vector<int> overlap_size = {0, 0};

    size_t size_after_subset = DasLib::InferTimeSubsetSize(t_start, t_end, -59, 59, sample_rate);
    sc_size[0] = CHS;
    sc_size[1] = size_after_subset;

    std::cout << "size_after_subset = " << size_after_subset << "\n";

    semblance_denom_sum = new Array<double>("EP_MEMORY", sc_size);
    coherency_sum = new Array<std::complex<double>>("EP_MEMORY", sc_size);
    data_in_sum = new Array<double>("EP_MEMORY", sc_size);

    //Input data,
    Array<double> *A = new Array<double>("EP_DIR:EP_HDF5:/Users/dbin/work/arrayudf-git-svn-test-on-bitbucket/examples/das/stacking_files/xcorr_examples_h5:/xcoor", chunk_size, overlap_size);
    std::vector<int> skip_size = {1, 14999};
    A->EnableApplyStride(skip_size);

    //Clone to create local copy
    std::complex<double> complex_zero(0, 0);
    coherency_sum->Fill(complex_zero);

    semblance_denom_sum->Clone(0);
    coherency_sum->Clone();
    data_in_sum->Clone(0);

    //Run
    A->Apply(stack_udf);

    semblance_denom_sum->Merge(AU_SUM);
    coherency_sum->Merge(AU_SUM);
    data_in_sum->Merge(AU_SUM);

    semblance_denom_sum->Nonvolatile("EP_HDF5:/Users/dbin/work/arrayudf-git-svn-test-on-bitbucket/examples/das/stacking_files/xcorr_examples_h5_stack_semblance_denom_sum.h5:/semblance_denom_sum");
    coherency_sum->Nonvolatile("EP_HDF5:/Users/dbin/work/arrayudf-git-svn-test-on-bitbucket/examples/das/stacking_files/xcorr_examples_h5_stack_coherency_sum.h5:/coherency_sum");

    //Clear
    delete A;

    delete semblance_denom_sum;
    delete coherency_sum;
    delete data_in_sum;

    AU_Finalize();

    return 0;
}
