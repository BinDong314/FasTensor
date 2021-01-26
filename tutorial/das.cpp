#include "ft.h"
#include <numeric>

int c_ofst = 20, c_n = 2 * c_ofst + 1;
int ch_ofst = 2, w_ofst = 10;
inline Stencil<float> similarity_udf(const Stencil<short> &c)
{
    float max_corr1 = 0, max_corr2 = 0, temp_corr1, temp_corr2;
    float c_sqSum = 0, n_sqSum1, n_sqSum2, cn_Sum1, cn_Sum2;
    std::vector<float> cw(c_n);               //current window
    std::vector<float> nw1(c_n + 2 * w_ofst); //neighbor window 1
    std::vector<float> nw2(c_n + 2 * w_ofst); //neighbor window 2
    for (int i = -c_ofst; i <= c_ofst; i++)
    {
        cw[i + c_ofst] = c(0, i);
    }
    c_sqSum = sqrt(std::inner_product(cw.begin(), cw.end(), cw.begin(), 0));
    for (int j = -c_ofst - w_ofst; j <= c_ofst + w_ofst; j++)
    {
        nw1[j + c_ofst + w_ofst] = c(ch_ofst, j);
        nw2[j + c_ofst + w_ofst] = c(-ch_ofst, j);
    }
    for (int j = -w_ofst; j <= w_ofst; j++)
    {
        int nws = j + w_ofst;
        cn_Sum1 = inner_product(cw.begin(), cw.end(), nw1.begin() + nws, 0);
        cn_Sum2 = inner_product(cw.begin(), cw.end(), nw2.begin() + nws, 0);
        n_sqSum1 = sqrt(inner_product(nw1.begin() + nws, nw1.begin() + nws + c_n, nw1.begin() + nws, 0));
        n_sqSum2 = sqrt(inner_product(nw2.begin() + nws, nw2.begin() + nws + c_n, nw2.begin() + nws, 0));
        temp_corr1 = abs(cn_Sum1) / (c_sqSum * n_sqSum1);
        temp_corr2 = abs(cn_Sum2) / (c_sqSum * n_sqSum2);
        if (temp_corr1 > max_corr1)
            max_corr1 = temp_corr1;
        if (temp_corr2 > max_corr2)
            max_corr2 = temp_corr2;
    }
    return Stencil<float>((max_corr1 + max_corr2) / 2);
}
int main(int argc, char *argv[])
{
    std::vector<int> os = {50, 50};
    std::vector<int> cs = {11648, 30000};
    std::vector<int> ss = {1456, 10000};
    FT_Init(argc, argv);
    FT::Array<short> *IFILE = new FT::Array<short>("EP_HDF5:./das.h5:/DataCT", cs, os);
    FT::Array<float> *OFILE = new FT::Array<float>("EP_HDF5:./das_similarity.h5:/dat");
    IFILE->SetStride(ss);
    IFILE->Transform(similarity_udf, OFILE);
    IFILE->ReportTime();
    delete IFILE;
    delete OFILE;
    FT_Finalize();
    return 0;
}
