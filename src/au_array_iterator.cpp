
#include "au_array_iterator.h"

/**
 * @brief get the next ordinates_p from current ordinates_p 
 *        call 
 * @param ordinates_p, input and output for  current and next coordinate
 * @param start_p  is the start range, n-dimensional, including 
 * @param end_p is the end range, n-dimensional. including
 */
inline void iterate(std::vector<int> &ordinates_p, const std::vector<int> start_p, const std::vector<int> end_p)
{
    int dimensions = start_p.size();
    // iterate over dimensions in reverse...
    for (int dimension = dimensions - 1; dimension >= 0; dimension--)
    {

        if (ordinates_p[dimension] < end_p[dimension])
        {
            // If this dimension can handle another increment... then done.
            ordinates_p[dimension]++;
            break;
        }

        // Otherwise, reset this dimension and bubble up to the next dimension to take a look
        ordinates_p[dimension] = start_p[dimension];
    }
}
