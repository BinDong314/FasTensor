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
 * Email questions to dbin@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#ifndef END_POINT_MEMORY_H
#define END_POINT_MEMORY_H

#include "au_type.h"
#include "au_endpoint.h"
#include <string>
#include <iostream>
#include <vector>

#include <unistd.h>
#include <iostream>
#include <cstddef>
#include <iomanip>
#include <libdash.h>

#define DASH_READ_FLAG 0
#define DASH_WRITE_FLAG 1

#define CreateDashMatrix(dash_matrix_p_p, rank_const_p, element_type_code_p, array_size)                            \
    {                                                                                                               \
        dash::SizeSpec<rank_const_p, unsigned long> dash_size_spec = dash::SizeSpec<rank_const_p, unsigned long>(); \
        for (int i = 0; i < rank_const_p; i++)                                                                      \
        {                                                                                                           \
            dash_size_spec.resize(i, array_size[i]);                                                                \
        }                                                                                                           \
        switch (element_type_code_p)                                                                                \
        {                                                                                                           \
        case AU_SHORT:                                                                                              \
        {                                                                                                           \
            dash_matrix_p_p = new dash::Matrix<short, rank_const_p, unsigned long>(dash_size_spec);                 \
            break;                                                                                                  \
        }                                                                                                           \
        case AU_INT:                                                                                                \
        {                                                                                                           \
            dash_matrix_p_p = new dash::Matrix<int, rank_const_p, unsigned long>(dash_size_spec);                   \
            break;                                                                                                  \
        }                                                                                                           \
        case AU_LONG:                                                                                               \
        {                                                                                                           \
            dash_matrix_p_p = new dash::Matrix<long, rank_const_p, unsigned long>(dash_size_spec);                  \
            break;                                                                                                  \
        }                                                                                                           \
        case AU_LONG_LONG:                                                                                          \
        {                                                                                                           \
            dash_matrix_p_p = new dash::Matrix<long long, rank_const_p, unsigned long>(dash_size_spec);             \
            break;                                                                                                  \
        }                                                                                                           \
        case AU_USHORT:                                                                                             \
        {                                                                                                           \
            dash_matrix_p_p = new dash::Matrix<unsigned int, rank_const_p, unsigned long>(dash_size_spec);          \
            break;                                                                                                  \
        }                                                                                                           \
        case AU_UINT:                                                                                               \
        {                                                                                                           \
            dash_matrix_p_p = new dash::Matrix<unsigned int, rank_const_p, unsigned long>(dash_size_spec);          \
            break;                                                                                                  \
        }                                                                                                           \
        case AU_ULONG:                                                                                              \
        {                                                                                                           \
            dash_matrix_p_p = new dash::Matrix<unsigned long, rank_const_p, unsigned long>(dash_size_spec);         \
            break;                                                                                                  \
        }                                                                                                           \
        case AU_ULLONG:                                                                                             \
        {                                                                                                           \
            dash_matrix_p_p = new dash::Matrix<unsigned long long, rank_const_p, unsigned long>(dash_size_spec);    \
            break;                                                                                                  \
        }                                                                                                           \
        case AU_FLOAT:                                                                                              \
        {                                                                                                           \
            dash_matrix_p_p = new dash::Matrix<float, rank_const_p, unsigned long>(dash_size_spec);                 \
            break;                                                                                                  \
        }                                                                                                           \
        case AU_DOUBLE:                                                                                             \
        {                                                                                                           \
            dash_matrix_p_p = new dash::Matrix<double, rank_const_p, unsigned long>(dash_size_spec);                \
            break;                                                                                                  \
        }                                                                                                           \
        default:                                                                                                    \
            AU_EXIT("Unsupported datatype in CreateDashMatrix !");                                                  \
        }                                                                                                           \
    }
#define AccessDashDataHelp(rank_const_p, dash_array_typed, start_p, end_p, data_p_typed, RW_flag) \
    {                                                                                             \
        unsigned long offset = 0;                                                                 \
        switch (rank_const_p)                                                                     \
        {                                                                                         \
        case 1:                                                                                   \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                \
            {                                                                                     \
                if (RW_flag == DASH_READ_FLAG)                                                    \
                {                                                                                 \
                    data_p_typed[i] = dash_array_typed->at(i);                                    \
                }                                                                                 \
                else                                                                              \
                {                                                                                 \
                    dash_array_typed->at(i) = data_p_typed[i];                                    \
                }                                                                                 \
            }                                                                                     \
            break;                                                                                \
        case 2:                                                                                   \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                \
            {                                                                                     \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                            \
                {                                                                                 \
                    offset = i * (end_p[1] - start_p[1] + 1) + j;                                 \
                    if (RW_flag == DASH_READ_FLAG)                                                \
                    {                                                                             \
                        data_p_typed[offset] = dash_array_typed->at(i, j);                        \
                    }                                                                             \
                    else                                                                          \
                    {                                                                             \
                        dash_array_typed->at(i, j) = data_p_typed[offset];                        \
                    }                                                                             \
                }                                                                                 \
            }                                                                                     \
            break;                                                                                \
        case 3:                                                                                   \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                \
            {                                                                                     \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                            \
                {                                                                                 \
                    for (unsigned long k = start_p[2]; k <= end_p[2]; k++)                        \
                    {                                                                             \
                        offset = i * (end_p[1] - start_p[1] + 1) + j;                             \
                        offset = offset * (end_p[2] - start_p[2] + 1) + k;                        \
                        if (RW_flag == DASH_READ_FLAG)                                            \
                        {                                                                         \
                            data_p_typed[offset] = dash_array_typed->at(i, j, k);                 \
                        }                                                                         \
                        else                                                                      \
                        {                                                                         \
                            dash_array_typed->at(i, j, k) = data_p_typed[offset];                 \
                        }                                                                         \
                    }                                                                             \
                }                                                                                 \
            }                                                                                     \
            break;                                                                                \
        default:                                                                                  \
            AU_EXIT("Unsupported dimensioin beyond 3D !");                                        \
            break;                                                                                \
        }                                                                                         \
    }

#define AccessDashDataHelp1D(rank_const_p, dash_array_typed, start_p, end_p, data_p_typed, RW_flag) \
    {                                                                                               \
        unsigned long offset = 0;                                                                   \
        switch (rank_const_p)                                                                       \
        {                                                                                           \
        case 1:                                                                                     \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                  \
            {                                                                                       \
                if (RW_flag == DASH_READ_FLAG)                                                      \
                {                                                                                   \
                    data_p_typed[i] = dash_array_typed->at(i);                                      \
                }                                                                                   \
                else                                                                                \
                {                                                                                   \
                    dash_array_typed->at(i) = data_p_typed[i];                                      \
                }                                                                                   \
            }                                                                                       \
            break;                                                                                  \
        case 2:                                                                                     \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                  \
            {                                                                                       \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                              \
                {                                                                                   \
                    offset = i * (end_p[1] - start_p[1] + 1) + j;                                   \
                    if (RW_flag == DASH_READ_FLAG)                                                  \
                    {                                                                               \
                        data_p_typed[offset] = dash_array_typed->at(i, j);                          \
                    }                                                                               \
                    else                                                                            \
                    {                                                                               \
                        dash_array_typed->at(i, j) = data_p_typed[offset];                          \
                    }                                                                               \
                }                                                                                   \
            }                                                                                       \
            break;                                                                                  \
        case 3:                                                                                     \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                  \
            {                                                                                       \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                              \
                {                                                                                   \
                    for (unsigned long k = start_p[2]; k <= end_p[2]; k++)                          \
                    {                                                                               \
                        offset = i * (end_p[1] - start_p[1] + 1) + j;                               \
                        offset = offset * (end_p[2] - start_p[2] + 1) + k;                          \
                        if (RW_flag == DASH_READ_FLAG)                                              \
                        {                                                                           \
                            data_p_typed[offset] = dash_array_typed->at(i, j, k);                   \
                        }                                                                           \
                        else                                                                        \
                        {                                                                           \
                            dash_array_typed->at(i, j, k) = data_p_typed[offset];                   \
                        }                                                                           \
                    }                                                                               \
                }                                                                                   \
            }                                                                                       \
            break;                                                                                  \
        default:                                                                                    \
            AU_EXIT("Unsupported dimensioin beyond 3D !");                                          \
            break;                                                                                  \
        }                                                                                           \
    }

#define AccessDashData(rank_const_p, dash_array_p_p, start_p, end_p, data_p, element_type_code_p, RW_flag)                                                             \
    {                                                                                                                                                                  \
        switch (element_type_code_p)                                                                                                                                   \
        {                                                                                                                                                              \
        case AU_SHORT:                                                                                                                                                 \
        {                                                                                                                                                              \
            dash::Matrix<short, rank_const_p, unsigned long> *matrix_p = (dash::Matrix<short, rank_const_p, unsigned long> *)dash_array_p_p;                           \
            short *short_data_p = (short *)data_p;                                                                                                                     \
            AccessDashDataHelp(rank_const_p, matrix_p, start_p, end_p, short_data_p, RW_flag);                                                                         \
            break;                                                                                                                                                     \
        }                                                                                                                                                              \
        case AU_INT:                                                                                                                                                   \
        {                                                                                                                                                              \
            dash::Matrix<int, rank_const_p, unsigned long> *matrix_p = (dash::Matrix<int, rank_const_p, unsigned long> *)dash_array_p_p;                               \
            int *short_data_p = (int *)data_p;                                                                                                                         \
            AccessDashDataHelp(rank_const_p, matrix_p, start_p, end_p, short_data_p, RW_flag);                                                                         \
                                                                                                                                                                       \
            break;                                                                                                                                                     \
        }                                                                                                                                                              \
        case AU_LONG:                                                                                                                                                  \
        {                                                                                                                                                              \
            dash::Matrix<long, rank_const_p, unsigned long> *matrix_p = (dash::Matrix<long, rank_const_p, unsigned long> *)dash_array_p_p;                             \
            long *short_data_p = (long *)data_p;                                                                                                                       \
            AccessDashDataHelp(rank_const_p, matrix_p, start_p, end_p, short_data_p, RW_flag);                                                                         \
            break;                                                                                                                                                     \
        }                                                                                                                                                              \
        case AU_LONG_LONG:                                                                                                                                             \
        {                                                                                                                                                              \
            dash::Matrix<long long, rank_const_p, unsigned long> *matrix_p = (dash::Matrix<long long, rank_const_p, unsigned long> *)dash_array_p_p;                   \
            long long *short_data_p = (long long *)data_p;                                                                                                             \
            AccessDashDataHelp(rank_const_p, matrix_p, start_p, end_p, short_data_p, RW_flag);                                                                         \
            break;                                                                                                                                                     \
        }                                                                                                                                                              \
        case AU_USHORT:                                                                                                                                                \
        {                                                                                                                                                              \
            dash::Matrix<unsigned short, rank_const_p, unsigned long> *matrix_p = (dash::Matrix<unsigned short, rank_const_p, unsigned long> *)dash_array_p_p;         \
            unsigned short *short_data_p = (unsigned short *)data_p;                                                                                                   \
            AccessDashDataHelp(rank_const_p, matrix_p, start_p, end_p, short_data_p, RW_flag);                                                                         \
            break;                                                                                                                                                     \
        }                                                                                                                                                              \
        case AU_UINT:                                                                                                                                                  \
        {                                                                                                                                                              \
            dash::Matrix<unsigned int, rank_const_p, unsigned long> *matrix_p = (dash::Matrix<unsigned int, rank_const_p, unsigned long> *)dash_array_p_p;             \
            unsigned int *short_data_p = (unsigned int *)data_p;                                                                                                       \
            AccessDashDataHelp(rank_const_p, matrix_p, start_p, end_p, short_data_p, RW_flag);                                                                         \
            break;                                                                                                                                                     \
        }                                                                                                                                                              \
        case AU_ULONG:                                                                                                                                                 \
        {                                                                                                                                                              \
            dash::Matrix<unsigned long, rank_const_p, unsigned long> *matrix_p = (dash::Matrix<unsigned long, rank_const_p, unsigned long> *)dash_array_p_p;           \
            unsigned long *short_data_p = (unsigned long *)data_p;                                                                                                     \
            AccessDashDataHelp(rank_const_p, matrix_p, start_p, end_p, short_data_p, RW_flag);                                                                         \
            break;                                                                                                                                                     \
        }                                                                                                                                                              \
        case AU_ULLONG:                                                                                                                                                \
        {                                                                                                                                                              \
            dash::Matrix<unsigned long long, rank_const_p, unsigned long> *matrix_p = (dash::Matrix<unsigned long long, rank_const_p, unsigned long> *)dash_array_p_p; \
            unsigned long long *short_data_p = (unsigned long long *)data_p;                                                                                           \
            AccessDashDataHelp(rank_const_p, matrix_p, start_p, end_p, short_data_p, RW_flag);                                                                         \
            break;                                                                                                                                                     \
        }                                                                                                                                                              \
        case AU_FLOAT:                                                                                                                                                 \
        {                                                                                                                                                              \
            dash::Matrix<float, rank_const_p, unsigned long> *matrix_p = (dash::Matrix<float, rank_const_p, unsigned long> *)dash_array_p_p;                           \
            float *short_data_p = (float *)data_p;                                                                                                                     \
            AccessDashDataHelp(rank_const_p, matrix_p, start_p, end_p, short_data_p, RW_flag);                                                                         \
            break;                                                                                                                                                     \
        }                                                                                                                                                              \
        case AU_DOUBLE:                                                                                                                                                \
        {                                                                                                                                                              \
            dash::Matrix<double, rank_const_p, unsigned long> *matrix_p = (dash::Matrix<double, rank_const_p, unsigned long> *)dash_array_p_p;                         \
            double *short_data_p = (double *)data_p;                                                                                                                   \
            AccessDashDataHelp(rank_const_p, matrix_p, start_p, end_p, short_data_p, RW_flag);                                                                         \
            break;                                                                                                                                                     \
        }                                                                                                                                                              \
        default:                                                                                                                                                       \
            AU_EXIT("Unsupported datatype in AccessDashData !");                                                                                                       \
        }                                                                                                                                                              \
    }

#define AccessDashData1D(dash_array_p_p, start_p, end_p, data_p, element_type_code_p, RW_flag)                                 \
    {                                                                                                                          \
        switch (element_type_code_p)                                                                                           \
        {                                                                                                                      \
        case AU_SHORT:                                                                                                         \
        {                                                                                                                      \
            dash::Matrix<short, 1, unsigned long> *dash_array_typed = (dash::Matrix<short, 1, unsigned long> *)dash_array_p_p; \
            short *typed_data_p = (short *)data_p;                                                                             \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                             \
            {                                                                                                                  \
                if (RW_flag == DASH_READ_FLAG)                                                                                 \
                {                                                                                                              \
                    typed_data_p[i] = dash_array_typed->at(i);                                                                 \
                }                                                                                                              \
                else                                                                                                           \
                {                                                                                                              \
                    dash_array_typed->at(i) = typed_data_p[i];                                                                 \
                }                                                                                                              \
            }                                                                                                                  \
            break;                                                                                                             \
        }                                                                                                                      \
        case AU_INT:                                                                                                           \
        {                                                                                                                      \
            dash::Matrix<int, 1, unsigned long> *dash_array_typed = (dash::Matrix<int, 1, unsigned long> *)dash_array_p_p;     \
            int *typed_data_p = (int *)data_p;                                                                                 \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                             \
            {                                                                                                                  \
                if (RW_flag == DASH_READ_FLAG)                                                                                 \
                {                                                                                                              \
                    typed_data_p[i] = dash_array_typed->at(i);                                                                 \
                }                                                                                                              \
                else                                                                                                           \
                {                                                                                                              \
                    dash_array_typed->at(i) = typed_data_p[i];                                                                 \
                }                                                                                                              \
            }                                                                                                                  \
            break;                                                                                                             \
            break;                                                                                                             \
        }                                                                                                                      \
        default:                                                                                                               \
            AU_EXIT("Unsupported datatype in AccessDashData !");                                                               \
        }                                                                                                                      \
    }

#define AccessDashData2D(dash_array_p_p, start_p, end_p, data_p, element_type_code_p, RW_flag)                                 \
    {                                                                                                                          \
        unsigned long offset = 0;                                                                                              \
        switch (element_type_code_p)                                                                                           \
        {                                                                                                                      \
        case AU_SHORT:                                                                                                         \
        {                                                                                                                      \
            dash::Matrix<short, 2, unsigned long> *dash_array_typed = (dash::Matrix<short, 2, unsigned long> *)dash_array_p_p; \
            short *typed_data_p = (short *)data_p;                                                                             \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                             \
            {                                                                                                                  \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                         \
                {                                                                                                              \
                    offset = i * (end_p[1] - start_p[1] + 1) + j;                                                              \
                    if (RW_flag == DASH_READ_FLAG)                                                                             \
                    {                                                                                                          \
                        typed_data_p[offset] = dash_array_typed->at(i, j);                                                     \
                    }                                                                                                          \
                    else                                                                                                       \
                    {                                                                                                          \
                        dash_array_typed->at(i, j) = typed_data_p[offset];                                                     \
                    }                                                                                                          \
                }                                                                                                              \
            }                                                                                                                  \
            break;                                                                                                             \
        }                                                                                                                      \
        case AU_INT:                                                                                                           \
        {                                                                                                                      \
            dash::Matrix<int, 2, unsigned long> *dash_array_typed = (dash::Matrix<int, 2, unsigned long> *)dash_array_p_p;     \
            int *typed_data_p = (int *)data_p;                                                                                 \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                             \
            {                                                                                                                  \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                         \
                {                                                                                                              \
                    offset = i * (end_p[1] - start_p[1] + 1) + j;                                                              \
                    if (RW_flag == DASH_READ_FLAG)                                                                             \
                    {                                                                                                          \
                        typed_data_p[offset] = dash_array_typed->at(i, j);                                                     \
                    }                                                                                                          \
                    else                                                                                                       \
                    {                                                                                                          \
                        dash_array_typed->at(i, j) = typed_data_p[offset];                                                     \
                    }                                                                                                          \
                }                                                                                                              \
            }                                                                                                                  \
            break;                                                                                                             \
        }                                                                                                                      \
        default:                                                                                                               \
            AU_EXIT("Unsupported datatype in AccessDashData !");                                                               \
        }                                                                                                                      \
    }

#define AccessDashData3D(dash_array_p_p, start_p, end_p, data_p, element_type_code_p, RW_flag)                                 \
    {                                                                                                                          \
        unsigned long offset = 0;                                                                                              \
        switch (element_type_code_p)                                                                                           \
        {                                                                                                                      \
        case AU_SHORT:                                                                                                         \
        {                                                                                                                      \
            dash::Matrix<short, 3, unsigned long> *dash_array_typed = (dash::Matrix<short, 3, unsigned long> *)dash_array_p_p; \
            short *typed_data_p = (short *)data_p;                                                                             \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                             \
            {                                                                                                                  \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                         \
                {                                                                                                              \
                    for (unsigned long k = start_p[2]; k <= end_p[2]; k++)                                                     \
                    {                                                                                                          \
                        offset = i * (end_p[1] - start_p[1] + 1) + j;                                                          \
                        offset = offset * (end_p[2] - start_p[2] + 1) + k;                                                     \
                        if (RW_flag == DASH_READ_FLAG)                                                                         \
                        {                                                                                                      \
                            typed_data_p[offset] = dash_array_typed->at(i, j, k);                                              \
                        }                                                                                                      \
                        else                                                                                                   \
                        {                                                                                                      \
                            dash_array_typed->at(i, j, k) = typed_data_p[offset];                                              \
                        }                                                                                                      \
                    }                                                                                                          \
                }                                                                                                              \
            }                                                                                                                  \
            break;                                                                                                             \
        }                                                                                                                      \
        case AU_INT:                                                                                                           \
        {                                                                                                                      \
            dash::Matrix<int, 3, unsigned long> *dash_array_typed = (dash::Matrix<int, 3, unsigned long> *)dash_array_p_p;     \
            int *typed_data_p = (int *)data_p;                                                                                 \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                             \
            {                                                                                                                  \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                         \
                {                                                                                                              \
                    for (unsigned long k = start_p[2]; k <= end_p[2]; k++)                                                     \
                    {                                                                                                          \
                        offset = i * (end_p[1] - start_p[1] + 1) + j;                                                          \
                        offset = offset * (end_p[2] - start_p[2] + 1) + k;                                                     \
                        if (RW_flag == DASH_READ_FLAG)                                                                         \
                        {                                                                                                      \
                            typed_data_p[offset] = dash_array_typed->at(i, j, k);                                              \
                        }                                                                                                      \
                        else                                                                                                   \
                        {                                                                                                      \
                            dash_array_typed->at(i, j, k) = typed_data_p[offset];                                              \
                        }                                                                                                      \
                    }                                                                                                          \
                }                                                                                                              \
            }                                                                                                                  \
            break;                                                                                                             \
        }                                                                                                                      \
        default:                                                                                                               \
            AU_EXIT("Unsupported datatype in AccessDashData !");                                                               \
        }                                                                                                                      \
    }
class EndpointMEMORY : public Endpoint
{
private:
    void *dash_array_p;

public:
    /**
     * @brief Construct a new EndpointMEMORY object
     * 
     * @param data_endpoint contains the info of the endpoint, e.g., file type + file info
     */
    EndpointMEMORY(std::string endpoint_info_p)
    {
        endpoint_info = endpoint_info_p;
        ParseEndpointInfo();
        SetOpenFlag(false);
        SetEndpointType(EP_MEMORY);
    }
    /**
     * @brief Construct a new Endpoint in MEMORY 
     *         Nothing to do there, can be used as sub-endpoint of directory
     */
    EndpointMEMORY()
    {
        SetOpenFlag(false);
        SetEndpointType(EP_MEMORY);
    }

    ~EndpointMEMORY()
    {
        Close();
    }
    /**
     * @brief extracts metadata, possbile endpoint_ranks/endpoint_dim_size/data_element_type
     * 
     * @return int < 0 error, >= 0 works 
     */
    int ExtractMeta() override;
    /**
     * @brief print information about the endpoint
     * 
     * @return < 0 error, >= 0 works 
     */
    int PrintInfo() override;

    /**
     * @brief create the endpoint
     * 
     * @return  < 0 error, >= 0 works 
     */
    int Create() override;

    /**
     * @brief open the endpoint
     * 
     * @return < 0 error, >= 0 works 
     */
    int Open() override;

    /**
     * @brief read the data from end-point
     * 
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data 
     * @return int < 0 error, >= 0 works
     */
    int Read(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data) override;

    /**
     * @brief write the data to the end-point
     * 
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data 
     * @return int < 0 error, >= 0 works
     */
    int Write(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data) override;

    /**
     * @brief close the end-point
     * 
     * @return int int < 0 error, >= 0 works
     */
    int Close() override;

    void Map2MyType() override;

    void EnableCollectiveIO() override;

    void DisableCollectiveIO() override;

    /**
     * @brief parse endpoint_info to my own info
     *        In MEMORY, it map endpoint_info to filename, group name and datasetname
     * @return int: 0 works,  < 0 error,
     */
    int ParseEndpointInfo() override;
};
#endif
