#ifndef AU_CHUNK_H
#define AU_CHUNK_H

#include <vector>

extern int default_total_chunk_cells; //It is 1GB in float type

/**
 * @brief Get the Default chunk size
 *        Convert default_total_chunk_cells into chunk_size
 * @param chunk_size_p 
 * @param overlap_size_p 
 */
void GetDefaultChunkSize(std::vector<int> &chunk_size_p, std::vector<int> &overlap_size_p);

#endif