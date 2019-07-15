#include <omp.h>
#include <time.h>

#include "das-fft-help.cc"

#define N 43199999
int main()
{
    std::vector<float> testdata;
    testdata.resize(N);

    for (int i = 0; i < N; i++)
    {
        testdata[i] = 100.0 / (i + 1);
    }

    time_t start, end;
    time(&start);
    detrend_mp(&testdata[0], N);
    time(&end);

    double dif = difftime(end, start);
    printf("detrend_mp:  %.2lf seconds.", dif);

    std::vector<float> veridata;
    veridata.resize(N);

    for (int i = 0; i < N; i++)
    {
        veridata[i] = 100.0 / (i + 1);
    }

    time(&start);
    detrend(&veridata[0], N);
    time(&end);
    dif = difftime(end, start);
    printf("detrend:  %.2lf seconds.", dif);

    int find_missing_value_flag = 0;
    for (int i = 0; i < N; i++)
    {
        if (veridata[i] != testdata[i])
        {
            find_missing_value_flag = 1;
            break;
        }
    }

    if (find_missing_value_flag)
    {
        std::cout << "Wrong results \n";
    }
    else
    {
        std::cout << "Corret results \n";
    }
}