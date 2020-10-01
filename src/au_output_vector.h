#ifndef _AU_OUTPUT_VECTOR_H_
#define _AU_OUTPUT_VECTOR_H_

#include <vector>
#include <type_traits>
#include <cstring>
#include <cmath>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <tuple>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <utility>
#include <variant>
#include <dirent.h>

#include "au_type.h"
#include "au_utility_macro.h"
#include "au_array_view_access.h"
/**
 * @brief Get the Chunk Address object
 * 
 * @param chunk_id 
 * @param array_size 
 * @param chunk_size 
 * @param chunk_start_address 
 * @param chunk_end_address 
 * @return int 
 */
inline void GetChunkAddress(const unsigned long long chunk_id, const std::vector<unsigned long long> array_size, const std::vector<size_t> chunk_size, std::vector<unsigned long long> &chunk_start_address, std::vector<unsigned long long> &chunk_end_address)
{
    int rank = array_size.size();
    //PrintScalar("rank = ", rank);
    //PrintVector("array_size = ", array_size);
    //PrintVector("chunk_size = ", chunk_size);

    std::vector<unsigned long long> chunks;
    unsigned long long total_chunks = 1;
    chunks.resize(rank);

    for (int i = 0; i < rank; i++)
    {
        if (array_size[i] % chunk_size[i] == 0)
        {
            chunks[i] = array_size[i] / chunk_size[i];
        }
        else
        {
            chunks[i] = array_size[i] / chunk_size[i] + 1;
        }
        total_chunks = chunks[i] * total_chunks;
    }

    if (chunk_id >= total_chunks)
    {
        AU_EXIT("chunk_id >= total_chunks: chunk_id =" + std::to_string(chunk_id) + ", total_chunks = " + std::to_string(total_chunks));
    }
    std::vector<unsigned long long> chunk_coordinate(rank);
    //PrintVector("chunks = ", chunks);
    //PrintScalar("chunk_id = ", chunk_id);
    //PrintVector("chunk_coordinate = ", chunk_coordinate);

    ROW_MAJOR_ORDER_REVERSE_MACRO(chunk_id, chunks, rank, chunk_coordinate);

    chunk_start_address.resize(rank);
    chunk_end_address.resize(rank);
    for (int i = 0; i < rank; i++)
    {

        if (chunk_coordinate[i] * chunk_size[i] < array_size[i])
        {
            chunk_start_address[i] = chunk_coordinate[i] * chunk_size[i];
        }
        else
        {
            chunk_start_address[i] = array_size[i];
        }

        if (chunk_start_address[i] + chunk_size[i] - 1 < array_size[i])
        {
            chunk_end_address[i] = chunk_start_address[i] + chunk_size[i] - 1;
        }
        else
        {
            chunk_end_address[i] = array_size[i] - 1;
        }
    }

    PrintVector("chunk_start_address = ", chunk_start_address);
    PrintVector("chunk_end_address = ", chunk_end_address);
}

/**
 * @brief Insert output (vector of vector) into a buffer (vector) to write
 * 
 * @tparam T, element type 
 * @param v, the output vector of vector 
 * @param v_shape, the shape of the output in array space 
 * @param write_start_address, the start address to write (get updated by the function too) 
 * @param write_end_address,  the end address to write (get updated by the function too) 
 * @return void* the buffer containing the vector to write
 */
template <typename T>
void *InsertOutputVV2WriteV(std::vector<std::vector<T>> &v, std::vector<size_t> &v_shape, std::vector<unsigned long long> &write_start_address, std::vector<unsigned long long> &write_end_address, bool last_chunk_flag, std::vector<size_t> &prev_v_shape)
{
    //First to infer the size of the WriteVector from (write_end_address  -  write_start_address) and v_shape

    int rank = write_start_address.size();

    //We can only deal with one extra dimension
    if (v_shape.size() > (rank + 1))
    {
        AU_INFO("Warning: the shape of output Stencil has more dimensions than current support !\n");
    }

    std::vector<unsigned long long> write_vector_size;
    unsigned long long write_vector_length = 1, temp_address;
    for (int i = 0; i < v_shape.size(); i++)
    {
        if (i >= rank) //have extra dimension to deal with
        {
            write_start_address.push_back(0);
            write_end_address.push_back(v_shape[i] - 1);
        }
        else
        {
            // write_start_address = 2  write_end_address = 3  v_shape = 2
            //   ==> write_start_address = 4 write_end_address = 7
            temp_address = write_start_address[i];
            if (last_chunk_flag)
            {
                write_start_address[i] = write_start_address[i] * prev_v_shape[i];
            }
            else
            {
                write_start_address[i] = write_start_address[i] * v_shape[i];
            }
            write_end_address[i] = write_start_address[i] + (write_end_address[i] - temp_address + 1) * v_shape[i] - 1;
        }
        //std::cout << write_end_address[i] << "  ,,  " << write_start_address[i] << "\n";
        write_vector_size.push_back(write_end_address[i] - write_start_address[i] + 1);
        write_vector_length = write_vector_length * write_vector_size[i];
    }

    //update the rank to add new dimensions
    rank = v_shape.size();

    T *rv = (T *)malloc(write_vector_length * sizeof(T));
    //Now we have output vector with the shape write_vector_size and each element in v has the shape v_shape.
    //we need to write the v into
    //template <class T>
    //inline int ArrayViewAccessP(T * view_v, T * array_v, std::vector<unsigned long long> ///array_size, std::vector<unsigned long long> start, std::vector<unsigned long long> end, int read_write_code)

    PrintVector("v_shape = ", v_shape);
    PrintVector("write_vector_size = ", write_vector_size);
    PrintVector("write_start_address = ", write_start_address);
    PrintVector("write_end_address = ", write_end_address);

    std::vector<unsigned long long> view_start(rank), view_end(rank);
    for (size_t i = 0; i < v.size(); i++)
    {
        GetChunkAddress(i, write_vector_size, v_shape, view_start, view_end);
        ArrayViewAccessP(v[i].data(), rv, write_vector_size, view_start, view_end, ARRAY_VIEW_WRITE);
    }

    return rv;
}

template <typename T>
void *InsertOutputVV2WriteV(std::vector<T> &v, std::vector<size_t> &v_shape, std::vector<unsigned long long> &write_start_address, std::vector<unsigned long long> &write_end_address, bool last_chunk_flag, std::vector<size_t> &prev_v_shape)
{
    AU_EXIT("This function should not be called !");
}

template <typename T>
void *InsertOutputVV2WriteV(T &v, std::vector<size_t> &v_shape, std::vector<unsigned long long> &write_start_address, std::vector<unsigned long long> &write_end_address, bool last_chunk_flag, std::vector<size_t> &prev_v_shape)
{
    AU_EXIT("This function should not be called !");
}
//
//Below are old code to delete
//

/**
 * @brief 
 *flat vector of vector to 1D vector
 * direction specify row-major or colum major
 *  AU_FLAT_OUTPUT_ROW (0): row major, e.g., for a 2 by 2 vector of vector
 *     v[0][0] v[0][1] v[1][0] v[1][1]
 * AU_FLAT_OUTPUT_COL (1): column major, e.g., for a 2 by 2 vector of vector
 *    v[0][0] v[1][0] v[0][1] v[1][1]
 * 
 * AU_FLAT_OUTPUT_RC: both row and column major, e.g. for a 2 by 2 vector of vector
 *    v[0][0] v[0][1]
 *    v[1][0] v[1][1]
 *    Then, linearized as  v[0][0] v[0][1] v[1][0] v[1][1]
 *    It looks like AU_FLAT_OUTPUT_ROW, but the start_address and end_address are handled differently
 *    start_address is adjusted by v.size()
 *    end_address is adjusted by v[0].size()  
 * AU_FLAT_OUTPUT_CR: both row and column major, e.g. for a 2 by 2 vector of vector
 * 
 * Since this is the support function to WriteEndpoint,
 * It also convert the address associated with it. 
 * 
 * @tparam T type of data element
 * @param v the vector of vector
 * @param direction how to flat the data
 * @param start_address the start address before/after flat, it may chage the # of dimensions
 * @param end_address  the end address before/after flat, it may chage the # of dimensions
 * @param v_size, the size of each element vector
 * @return void* the flat data
 */
template <typename T>
void *FlatVector(std::vector<std::vector<T>> &v, OutputVectorFlatDirection direction, std::vector<unsigned long long> &start_address, std::vector<unsigned long long> &end_address, size_t &v_size)
{
    if (v.size() < 1)
    {
        return NULL;
    }

    if (v[0].size() < 1)
    {
        return NULL;
    }

    //Check v's elemtnt has same size
    for (unsigned j = 0; j < v.size() - 1; j++)
    {
        if (v[j].size() != v[j + 1].size())
        {
            AU_EXIT("Size of each element of vector must be equal.");
        }
    }
    v_size = v[0].size();

    if (direction == AU_FLAT_OUTPUT_NEW)
    {
        start_address.push_back(0);
        end_address.push_back(v_size - 1);
        return &v[0];
    }

    T *rv = (T *)malloc(v.size() * v_size * sizeof(T)); //Assuming all rows have the same size
    if (rv == NULL)
    {
        AU_EXIT("Not enough memory");
    }

    if (direction == AU_FLAT_OUTPUT_COL)
    {
        for (unsigned i = 0; i < v_size; i++)
        {
            for (unsigned j = 0; j < v.size(); j++)
            {
                memcpy(rv + v.size() * i + j, &(v[j][i]), sizeof(T));
            }
        }
    }
    else if (direction == AU_FLAT_OUTPUT_ROW)
    {

        for (unsigned i = 0; i < v.size(); i++)
        {
            memcpy(rv + v_size * i, &(v[i][0]), v_size * sizeof(T));
        }
    }
    else
    {
        AU_EXIT("Not supported option yet to flat vector");
    }

    end_address[direction] = start_address[direction] + (end_address[direction] - start_address[direction] + 1) * v_size - 1;

    return (void *)rv;
}

template <typename T>
void *FlatVector(std::vector<T> &v, OutputVectorFlatDirection direction, std::vector<unsigned long long> &start_address, std::vector<unsigned long long> &end_address, size_t &v_size)
{
    AU_EXIT("Should not be here");
}

template <typename T>
void *FlatVector(T &v, OutputVectorFlatDirection direction, std::vector<unsigned long long> &start_address, std::vector<unsigned long long> &end_address, size_t &v_size)
{
    AU_EXIT("Should not be here");
}

#endif