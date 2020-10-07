#ifndef AU_ARRAY_ITERATOR_H
#define AU_ARRAY_ITERATOR_H

#include <array>
#include <iostream>
#include <vector>

/**
 * @brief get the next ordinates_p from current ordinates_p 
 *        call 
 * @param ordinates_p, input and output for  current and next coordinate
 * @param start_p  is the start range, n-dimensional, including 
 * @param end_p is the end range, n-dimensional. including
 */
inline void
iterate(std::vector<int> &ordinates_p, const std::vector<int> start_p, const std::vector<int> end_p);

#define ITERATOR_MACRO(ordinates_p, start_p, end_p)                                         \
    {                                                                                       \
        int dimensions = start_p.size();                                                    \
        for (int dimension_index = dimensions - 1; dimension_index >= 0; dimension_index--) \
        {                                                                                   \
            if (ordinates_p[dimension_index] < end_p[dimension_index])                      \
            {                                                                               \
                ordinates_p[dimension_index]++;                                             \
                break;                                                                      \
            }                                                                               \
            ordinates_p[dimension_index] = start_p[dimension_index];                        \
        }                                                                                   \
    }

#endif

/*
//Example file of usage
#include <vector>
#include <iostream>

inline void
iterate(std::vector<int> &ordinates, const std::vector<int> minimums, const std::vector<int> maximums)
{
    int dimensions = minimums.size();
    // iterate over dimensions in reverse...
    for (int dimension = dimensions - 1; dimension >= 0; dimension--)
    {

        if (ordinates[dimension] < maximums[dimension])
        {
            // If this dimension can handle another increment... then done.
            ordinates[dimension]++;
            break;
        }

        // Otherwise, reset this dimension and bubble up to the next dimension to take a look
        ordinates[dimension] = minimums[dimension];
    }
}

int main()
{
    int N = 3;
    std::vector<int> min_c, max_c, ord_c;
    min_c.resize(N);
    max_c.resize(N);
    ord_c.resize(N);

    min_c[0] = 1;
    min_c[1] = 2;
    min_c[2] = 1;

    max_c[0] = 4;
    max_c[1] = 5;
    max_c[2] = 6;

    ord_c[0] = min_c[0];
    ord_c[1] = min_c[1];
    ord_c[2] = min_c[2];

    int maxIterations = 1;
    for (int i = 0; i < N; i++)
    {
        maxIterations = maxIterations * (max_c[i] - min_c[i] + 1);
    }

    for (int iteration = 0; iteration < maxIterations; iteration++)
    {
        for (int i = 0; i < N; i++)
        {
            std::cout << ord_c[i] << " , ";
        }
        std::cout << "\n";
        //iterate(n, ord, min, max)

        iterate(ord_c, min_c, max_c);
    }
}

*/