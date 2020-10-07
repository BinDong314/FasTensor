#ifndef AU_ARRAY_TRANSPOSE_H
#define AU_ARRAY_TRANSPOSE_H

/**
 * @brief transpose data in 2D from src to dst
 * 
 * @tparam T , the data element type of data
 * @param src , pointer to the source buffer
 * @param dst , pointer to the target buffer
 * @param N , the size of the first dimension of src
 * @param M , the size of the second dimension of src
 */
template <class T>
void transpose_data_2D(T *src, T *dst, const int N, const int M)
{
    for (int n = 0; n < N * M; n++)
    {
        int i = n / N;
        int j = n % N;
        dst[n] = src[M * j + i];
    }
}

#endif
