
#ifndef AU_ARRAY_VIEW_ACCESS_H
#define AU_ARRAY_VIEW_ACCESS_H

#include <vector>
#include <cstdint>
#include <cstring>
#include "au_array_iterator.h"
#include "au_utility.h"

#define ARRAY_VIEW_READ 0
#define ARRAY_VIEW_WRITE 1

#define MEMCPY_ACCESS_HELP(dst_p, dst_offset_p, src_p, src_offset_p, count_p, element_size_p, rw_code)                           \
    {                                                                                                                            \
        if (rw_code == ARRAY_VIEW_READ)                                                                                          \
        {                                                                                                                        \
            std::memcpy(dst_p + dst_offset_p * element_size_p, src_p + src_offset_p * element_size_p, count_p * element_size_p); \
        }                                                                                                                        \
        else                                                                                                                     \
        {                                                                                                                        \
            std::memcpy(src_p + src_offset_p * element_size_p, dst_p + dst_offset_p * element_size_p, count_p * element_size_p); \
        }                                                                                                                        \
    }

/**
 * @brief Access an view (subset) of an array 
 * 
 * @param view_buffer : pointer to the buffer for the view
 * @param array_buffer : pointer to the buffer for the array
 * @param array_size : the size for the array
 * @param start : the start address of the view
 * @param end : the end address of the view
 * @param read_write_code : mode to read or write, e.g., ARRAY_VIEW_READ , ARRAY_VIEW_WRITE 1
 * @param element_size : the size of each element data
 * @return int : non zero if works, otherwise failed
 */
inline int ArrayViewAccess(void *view_buffer, void *array_buffer, std::vector<unsigned long long> &array_size, std::vector<unsigned long long> &start, std::vector<unsigned long long> &end, int read_write_code, int element_size);

#endif