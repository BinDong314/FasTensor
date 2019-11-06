/**
 *ArrayUDF Copyright (c) 2017, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy).  All rights reserved.
 *
 *If you have questions about your rights to use or distribute this software, please contact Berkeley Lab's Innovation & Partnerships Office at  IPO@lbl.gov.
 *
 * NOTICE. This Software was developed under funding from the U.S. Department of Energy and the U.S. Government consequently retains certain rights. As such, the U.S. Government has been granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software to reproduce, distribute copies to the public, prepare derivative works, and perform publicly and display publicly, and to permit other to do so. 
 *
 */

/**
 *
 * Email questions to {dbin, kwu, sbyna}@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#ifndef ARRAY_UDF_UTILITY_H
#define ARRAY_UDF_UTILITY_H

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
#include "au_type.h"

int ExtractFileTypeInfo(std::string data_endpoint_info, AuEndpointType &endpoint_type, std::vector<std::string> &endpoint_info);
int file_exist(const char *filename);

template <typename T>
inline void PrintVector(std::string name, std::vector<T> v)
{
    int n = v.size();
    std::cout << name << ": ";
    if (n > 60)
    {
        for (int i = 0; i < 30; i++)
        {
            std::cout << v[i] << ",";
        }
        std::cout << " ... ";
        for (int i = n - 30; i < n; i++)
        {
            std::cout << v[i] << ",";
        }
    }
    else
    {
        for (int i = 0; i < n - 1; i++)
        {
            std::cout << v[i] << ",";
        }
        std::cout << v[n - 1];
    }
    std::cout << std::endl;
}

template <typename T>
inline void PrintScalar(std::string name, T v)
{
    std::cout << name << ": " << v << std::endl;
}

/**
 * @brief convert coordinate to linearized one
 * 
 * @param dsize : size of all dimensions of the data
 * @param coordinate : multidimensional coordinate
 * @return unsigned long long : linearized one
 */
inline unsigned long long RowMajorOrder(std::vector<unsigned long long> dsize, std::vector<unsigned long long> coordinate)
{
    unsigned long long offset = coordinate[0];
    int n = dsize.size();
    for (int i = 1; i < n; i++)
    {
        offset = offset * dsize[i] + coordinate[i];
    }
    return offset;
}

/**
 * @brief convert linearized coordinate to multidimensional one
 * 
 * @param offset : linearized coordinate
 * @param dsize : data size 
 * @return std::vector<unsigned long long> :multidimensional coordinate
 */
inline std::vector<unsigned long long> RowMajorOrderReverse(unsigned long long offset, std::vector<unsigned long long> dsize)
{
    int n = dsize.size();
    std::vector<unsigned long long> original_coordinate;
    original_coordinate.resize(n);
    //unsigned long long reminder;
    for (unsigned long long i = n - 1; i >= 1; i--)
    {
        original_coordinate[i] = offset % dsize[i];
        offset = offset / dsize[i];
    }
    //Last dimenstion
    original_coordinate[0] = offset;

    return original_coordinate;
}

/**
 * @brief macro version of above two functions for speed
 * 
 */
#ifndef ROW_MAJOR_ORDER_MACRO
#define ROW_MAJOR_ORDER_MACRO(dsize, dsize_len, coordinate, offset) \
    {                                                               \
        offset = coordinate[0];                                     \
        for (int iii = 1; iii < dsize_len; iii++)                   \
        {                                                           \
            offset = offset * dsize[iii] + coordinate[iii];         \
        }                                                           \
    }
#endif

#ifndef ROW_MAJOR_ORDER_REVERSE_MACRO
#define ROW_MAJOR_ORDER_REVERSE_MACRO(offset, dsize, dsize_len, result_coord_v) \
    {                                                                           \
        unsigned long long temp_offset = offset;                                \
        for (int iii = dsize_len - 1; iii >= 1; iii--)                          \
        {                                                                       \
            result_coord_v[iii] = temp_offset % dsize[iii];                     \
            temp_offset = temp_offset / dsize[iii];                             \
        }                                                                       \
        result_coord_v[0] = temp_offset;                                        \
    }
#endif

#define AU_EXIT(info)                                                                                     \
    {                                                                                                     \
        std::cout << "Exit happens at " << __FILE__ << ", " << __func__ << ", " << __LINE__ << std::endl; \
        std::cout << "Log : " << info << std::endl;                                                       \
        std::exit(EXIT_FAILURE);                                                                          \
    }

/**
 * @brief 
 *flat vector of vector to 1D vector
 * direction specify row-major or colum major
 *  AU_FLAT_OUTPUT_ROW (0): row major, e.g., for a 2 by 2 vector
 *     v[0][0] v[0][1] v[1][0] v[1][1]
 * AU_FLAT_OUTPUT_COL (1): column major, e.g., for a 2 by 2 vector
 *    v[0][0] v[1][0] v[0][1] v[1][1]
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
            AU_EXIT("Size of each element of vector must be queal.");
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

std::string ExtractFileName(const std::string &fullPath);

std::string ExtractPath(const std::string &fullPath);

#endif
