

/**
 *
 * Email questions to dbin@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#ifndef AU_IARRAY_H
#define AU_IARRAY_H

#include <vector>
#include "au_type.h"

/**
 * @brief array of local data
 * 
 */
struct iArrayLocal
{
    int rank;
    std::vector<size_t> count;      //Size for each dim
    std::vector<size_t> offset;     //Starting point for each dim
    std::vector<size_t> row_stride; // rowStride = count
    void *local_array_data;

    iArrayLocal(std::vector<int> &count_p, std::vector<int> &row_stride_p)
    {
        count = count_p;
        row_stride = row_stride_p;
        rank = count_p.size();
        offset.resize(rank);
        for (int i = 0; i < rank; i++)
            offset[i] = 0;
    }

    iArrayLocal(std::vector<int> &count_p, std::vector<int> row_stride_p, std::vector<int> offset_p)
    {
        count = count_p;
        row_stride = row_stride_p;
        offset = offset_p;
        rank = count_p.size();
    }

    iArrayLocal()
    {
    }
};

/**
 * @brief global array
 * 
 */
struct iArray
{
    //for portability, they are au_mpi_size_global, au_mpi_rank_global
    int mpi_size;
    int mpi_rank;

    //Global array infomation
    //Same across all process
    int array_rank;
    AuEndpointDataType array_data_element_type;
    std::vector<unsigned long long> g_dimension;
    std::vector<int> g_ghost_size;
    std::vector<int> g_chunk_size; //g_chunk_size is different from chunK_size of Array in au_array.h
                                   //g_chunk_size is the size of local cached data
                                   //we only supported regular chunk, i.e., each rank has continious data
                                   //  and sum(g_dimension/g_chunk_size) = mpi_size

    //Local array location within the global array
    //l_start and l_count is the for the data without ghost_size
    //l_offset is the place to start the
    std::vector<unsigned long long> l_start;
    std::vector<unsigned long long> l_count;
    std::vector<unsigned long long> l_start_ghost; //l_start - g_ghost_size
    std::vector<unsigned long long> l_count_ghost; //l_start + lount + g_ghost_size

    //local_array contains data from l_start_ghost to l_count_ghost
    iArrayLocal *local_array;

    iArray(std::vector<unsigned long long> &g_dimension_p, std::vector<int> &g_ghost_size_p, std::vector<int> &g_chunk_size_p, int mpi_size_p, int mpi_rank_p)
    {
        g_dimension = g_dimension_p;
        g_ghost_size = g_ghost_size_p;
        g_chunk_size = g_chunk_size_p;
        mpi_size = mpi_size_p;
        mpi_rank = mpi_rank_p;
        array_rank = g_dimension.size();
    }
};

#endif
