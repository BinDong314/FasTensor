
#include "au_chunk.h"

int default_total_chunk_cells = 268435456; //It is 1GB in float type

/**
 * @brief Get the Default chunk size
 *        Convert default_total_chunk_cells into chunk_size
 * @param chunk_size_p 
 * @param overlap_size_p 
 */
void GetDefaultChunkSize(std::vector<int> &chunk_size_p, std::vector<int> &overlap_size_p, std::vector<unsigned long long> data_size)
{
    int ranks = chunk_size_p.size();
    if (ranks < 0)
        EU_EXIT("chunk_size_p has zero dimensions");

    for ()
}
