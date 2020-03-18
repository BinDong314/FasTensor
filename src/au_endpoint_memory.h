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
#include "au_local_mirror.h"

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

#define DASH_NONVOLATILE_CODE 0
#define DASH_VOLATILE_CODE 1
#define DASH_ENABLE_LOCAL_MIRROR_CODE 2
#define DASH_MERGE_MIRRORS_CODE 3

using dash::io::hdf5::hdf5_options;
using dash::io::hdf5::StoreHDF;

class EndpointMEMORY : public Endpoint
{
private:
    void *dash_array_p;

    //For local mirror
    void *local_mirror_buffer;
    bool local_mirror_flag = false;
    unsigned long long local_mirror_size;

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

    /**
     * @brief call a special operator on endpoint
     *        to dump file from MEMORY to HDF5, or in verse
     *                 
     * @param opt_code, specially defined code 
     */
    int SpecialOperator(int opt_code, std::string parameter) override;

    /**
     * @brief Nonvolatile the data in MEMORY to HDF5
     * 
     * @param parameter 
     * @return int 
     */
    int Nonvolatile(std::string parameter);

    /**
     * @brief Volatile the data from HDF5 to MEMORY
     * 
     * @param parameter 
     * @return int 
     */
    int Volatile(std::string parameter);

    /**
     * @brief Merger mirrors on all processes
     * 
     * @return int 
     */
    int MergeMirrors(std::string op_str);

    /**
     * @brief Create a Local Mirror object
     * 
     * @return int 
     */
    int CreateLocalMirror(std::string init_value_str);
};

#define CreateDashMatrix(dash_matrix_p_p, rank_const_p, element_type_code_p, array_size)                            \
    {                                                                                                               \
        dash::SizeSpec<rank_const_p, unsigned long> dash_size_spec = dash::SizeSpec<rank_const_p, unsigned long>(); \
        size_t num_units = dash::Team::All().size();                                                                \
        dash::TeamSpec<rank_const_p, unsigned long> teamspec;                                                       \
        teamspec.balance_extents();                                                                                 \
        dash::DistributionSpec<rank_const_p> distspec;                                                              \
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
        case AU_DOUBLE_COMPLEX:                                                                                     \
        {                                                                                                           \
            dash_matrix_p_p = new dash::Matrix<std::complex<double>, rank_const_p, unsigned long>(dash_size_spec);  \
            break;                                                                                                  \
        }                                                                                                           \
        default:                                                                                                    \
            AU_EXIT("Unsupported datatype in CreateDashMatrix !");                                                  \
        }                                                                                                           \
    }

#define AccessDashData1D(dash_array_p_p, start_p, end_p, data_p, element_type_code_p, RW_flag)                                                               \
    {                                                                                                                                                        \
        switch (element_type_code_p)                                                                                                                         \
        {                                                                                                                                                    \
        case AU_SHORT:                                                                                                                                       \
        {                                                                                                                                                    \
            dash::Matrix<short, 1, unsigned long> *dash_array_typed = (dash::Matrix<short, 1, unsigned long> *)dash_array_p_p;                               \
            short *typed_data_p = (short *)data_p;                                                                                                           \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                if (RW_flag == DASH_READ_FLAG)                                                                                                               \
                {                                                                                                                                            \
                    typed_data_p[i - start_p[0]] = dash_array_typed->at(i);                                                                                  \
                }                                                                                                                                            \
                else                                                                                                                                         \
                {                                                                                                                                            \
                    dash_array_typed->at(i) = typed_data_p[i - start_p[0]];                                                                                  \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_INT:                                                                                                                                         \
        {                                                                                                                                                    \
            dash::Matrix<int, 1, unsigned long> *dash_array_typed = (dash::Matrix<int, 1, unsigned long> *)dash_array_p_p;                                   \
            int *typed_data_p = (int *)data_p;                                                                                                               \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                if (RW_flag == DASH_READ_FLAG)                                                                                                               \
                {                                                                                                                                            \
                    typed_data_p[i - start_p[0]] = dash_array_typed->at(i);                                                                                  \
                }                                                                                                                                            \
                else                                                                                                                                         \
                {                                                                                                                                            \
                    dash_array_typed->at(i) = typed_data_p[i - start_p[0]];                                                                                  \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_LONG:                                                                                                                                        \
        {                                                                                                                                                    \
            dash::Matrix<long, 1, unsigned long> *dash_array_typed = (dash::Matrix<long, 1, unsigned long> *)dash_array_p_p;                                 \
            long *typed_data_p = (long *)data_p;                                                                                                             \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                if (RW_flag == DASH_READ_FLAG)                                                                                                               \
                {                                                                                                                                            \
                    typed_data_p[i - start_p[0]] = dash_array_typed->at(i);                                                                                  \
                }                                                                                                                                            \
                else                                                                                                                                         \
                {                                                                                                                                            \
                    dash_array_typed->at(i) = typed_data_p[i - start_p[0]];                                                                                  \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_LONG_LONG:                                                                                                                                   \
        {                                                                                                                                                    \
            dash::Matrix<long long, 1, unsigned long> *dash_array_typed = (dash::Matrix<long long, 1, unsigned long> *)dash_array_p_p;                       \
            long long *typed_data_p = (long long *)data_p;                                                                                                   \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                if (RW_flag == DASH_READ_FLAG)                                                                                                               \
                {                                                                                                                                            \
                    typed_data_p[i - start_p[0]] = dash_array_typed->at(i);                                                                                  \
                }                                                                                                                                            \
                else                                                                                                                                         \
                {                                                                                                                                            \
                    dash_array_typed->at(i) = typed_data_p[i - start_p[0]];                                                                                  \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_USHORT:                                                                                                                                      \
        {                                                                                                                                                    \
            dash::Matrix<unsigned short, 1, unsigned long> *dash_array_typed = (dash::Matrix<unsigned short, 1, unsigned long> *)dash_array_p_p;             \
            unsigned short *typed_data_p = (unsigned short *)data_p;                                                                                         \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                if (RW_flag == DASH_READ_FLAG)                                                                                                               \
                {                                                                                                                                            \
                    typed_data_p[i - start_p[0]] = dash_array_typed->at(i);                                                                                  \
                }                                                                                                                                            \
                else                                                                                                                                         \
                {                                                                                                                                            \
                    dash_array_typed->at(i) = typed_data_p[i - start_p[0]];                                                                                  \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_UINT:                                                                                                                                        \
        {                                                                                                                                                    \
            dash::Matrix<unsigned int, 1, unsigned long> *dash_array_typed = (dash::Matrix<unsigned int, 1, unsigned long> *)dash_array_p_p;                 \
            unsigned int *typed_data_p = (unsigned int *)data_p;                                                                                             \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                if (RW_flag == DASH_READ_FLAG)                                                                                                               \
                {                                                                                                                                            \
                    typed_data_p[i - start_p[0]] = dash_array_typed->at(i);                                                                                  \
                }                                                                                                                                            \
                else                                                                                                                                         \
                {                                                                                                                                            \
                    dash_array_typed->at(i) = typed_data_p[i - start_p[0]];                                                                                  \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_ULONG:                                                                                                                                       \
        {                                                                                                                                                    \
            dash::Matrix<unsigned long, 1, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long, 1, unsigned long> *)dash_array_p_p;               \
            unsigned long *typed_data_p = (unsigned long *)data_p;                                                                                           \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                if (RW_flag == DASH_READ_FLAG)                                                                                                               \
                {                                                                                                                                            \
                    typed_data_p[i - start_p[0]] = dash_array_typed->at(i);                                                                                  \
                }                                                                                                                                            \
                else                                                                                                                                         \
                {                                                                                                                                            \
                    dash_array_typed->at(i) = typed_data_p[i - start_p[0]];                                                                                  \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_ULLONG:                                                                                                                                      \
        {                                                                                                                                                    \
            dash::Matrix<unsigned long long, 1, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long long, 1, unsigned long> *)dash_array_p_p;     \
            unsigned long long *typed_data_p = (unsigned long long *)data_p;                                                                                 \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                if (RW_flag == DASH_READ_FLAG)                                                                                                               \
                {                                                                                                                                            \
                    typed_data_p[i - start_p[0]] = dash_array_typed->at(i);                                                                                  \
                }                                                                                                                                            \
                else                                                                                                                                         \
                {                                                                                                                                            \
                    dash_array_typed->at(i) = typed_data_p[i - start_p[0]];                                                                                  \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_FLOAT:                                                                                                                                       \
        {                                                                                                                                                    \
            dash::Matrix<float, 1, unsigned long> *dash_array_typed = (dash::Matrix<float, 1, unsigned long> *)dash_array_p_p;                               \
            float *typed_data_p = (float *)data_p;                                                                                                           \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                if (RW_flag == DASH_READ_FLAG)                                                                                                               \
                {                                                                                                                                            \
                    typed_data_p[i - start_p[0]] = dash_array_typed->at(i);                                                                                  \
                }                                                                                                                                            \
                else                                                                                                                                         \
                {                                                                                                                                            \
                    dash_array_typed->at(i) = typed_data_p[i - start_p[0]];                                                                                  \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_DOUBLE:                                                                                                                                      \
        {                                                                                                                                                    \
            dash::Matrix<double, 1, unsigned long> *dash_array_typed = (dash::Matrix<double, 1, unsigned long> *)dash_array_p_p;                             \
            double *typed_data_p = (double *)data_p;                                                                                                         \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                if (RW_flag == DASH_READ_FLAG)                                                                                                               \
                {                                                                                                                                            \
                    typed_data_p[i - start_p[0]] = dash_array_typed->at(i);                                                                                  \
                }                                                                                                                                            \
                else                                                                                                                                         \
                {                                                                                                                                            \
                    dash_array_typed->at(i) = typed_data_p[i - start_p[0]];                                                                                  \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_DOUBLE_COMPLEX:                                                                                                                              \
        {                                                                                                                                                    \
            dash::Matrix<std::complex<double>, 1, unsigned long> *dash_array_typed = (dash::Matrix<std::complex<double>, 1, unsigned long> *)dash_array_p_p; \
            std::complex<double> *typed_data_p = (std::complex<double> *)data_p;                                                                             \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                if (RW_flag == DASH_READ_FLAG)                                                                                                               \
                {                                                                                                                                            \
                    typed_data_p[i - start_p[0]] = dash_array_typed->at(i);                                                                                  \
                }                                                                                                                                            \
                else                                                                                                                                         \
                {                                                                                                                                            \
                    dash_array_typed->at(i) = typed_data_p[i - start_p[0]];                                                                                  \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        default:                                                                                                                                             \
            AU_EXIT("Unsupported datatype in AccessDashData !");                                                                                             \
        }                                                                                                                                                    \
    }

#define AccessDashData2D(dash_array_p_p, start_p, end_p, data_p, element_type_code_p, RW_flag)                                                               \
    {                                                                                                                                                        \
        unsigned long offset = 0;                                                                                                                            \
        switch (element_type_code_p)                                                                                                                         \
        {                                                                                                                                                    \
        case AU_SHORT:                                                                                                                                       \
        {                                                                                                                                                    \
            dash::Matrix<short, 2, unsigned long> *dash_array_typed = (dash::Matrix<short, 2, unsigned long> *)dash_array_p_p;                               \
            short *typed_data_p = (short *)data_p;                                                                                                           \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                              \
                    if (RW_flag == DASH_READ_FLAG)                                                                                                           \
                    {                                                                                                                                        \
                        typed_data_p[offset] = dash_array_typed->at(i, j);                                                                                   \
                    }                                                                                                                                        \
                    else                                                                                                                                     \
                    {                                                                                                                                        \
                        dash_array_typed->at(i, j) = typed_data_p[offset];                                                                                   \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_INT:                                                                                                                                         \
        {                                                                                                                                                    \
            dash::Matrix<int, 2, unsigned long> *dash_array_typed = (dash::Matrix<int, 2, unsigned long> *)dash_array_p_p;                                   \
            int *typed_data_p = (int *)data_p;                                                                                                               \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                              \
                    if (RW_flag == DASH_READ_FLAG)                                                                                                           \
                    {                                                                                                                                        \
                        typed_data_p[offset] = dash_array_typed->at(i, j);                                                                                   \
                    }                                                                                                                                        \
                    else                                                                                                                                     \
                    {                                                                                                                                        \
                        dash_array_typed->at(i, j) = typed_data_p[offset];                                                                                   \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_LONG:                                                                                                                                        \
        {                                                                                                                                                    \
            dash::Matrix<long, 2, unsigned long> *dash_array_typed = (dash::Matrix<long, 2, unsigned long> *)dash_array_p_p;                                 \
            long *typed_data_p = (long *)data_p;                                                                                                             \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                              \
                    if (RW_flag == DASH_READ_FLAG)                                                                                                           \
                    {                                                                                                                                        \
                        typed_data_p[offset] = dash_array_typed->at(i, j);                                                                                   \
                    }                                                                                                                                        \
                    else                                                                                                                                     \
                    {                                                                                                                                        \
                        dash_array_typed->at(i, j) = typed_data_p[offset];                                                                                   \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_LONG_LONG:                                                                                                                                   \
        {                                                                                                                                                    \
            dash::Matrix<long long, 2, unsigned long> *dash_array_typed = (dash::Matrix<long long, 2, unsigned long> *)dash_array_p_p;                       \
            long long *typed_data_p = (long long *)data_p;                                                                                                   \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                              \
                    if (RW_flag == DASH_READ_FLAG)                                                                                                           \
                    {                                                                                                                                        \
                        typed_data_p[offset] = dash_array_typed->at(i, j);                                                                                   \
                    }                                                                                                                                        \
                    else                                                                                                                                     \
                    {                                                                                                                                        \
                        dash_array_typed->at(i, j) = typed_data_p[offset];                                                                                   \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_USHORT:                                                                                                                                      \
        {                                                                                                                                                    \
            dash::Matrix<unsigned short, 2, unsigned long> *dash_array_typed = (dash::Matrix<unsigned short, 2, unsigned long> *)dash_array_p_p;             \
            unsigned short *typed_data_p = (unsigned short *)data_p;                                                                                         \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                              \
                    if (RW_flag == DASH_READ_FLAG)                                                                                                           \
                    {                                                                                                                                        \
                        typed_data_p[offset] = dash_array_typed->at(i, j);                                                                                   \
                    }                                                                                                                                        \
                    else                                                                                                                                     \
                    {                                                                                                                                        \
                        dash_array_typed->at(i, j) = typed_data_p[offset];                                                                                   \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_UINT:                                                                                                                                        \
        {                                                                                                                                                    \
            dash::Matrix<unsigned int, 2, unsigned long> *dash_array_typed = (dash::Matrix<unsigned int, 2, unsigned long> *)dash_array_p_p;                 \
            unsigned int *typed_data_p = (unsigned int *)data_p;                                                                                             \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                              \
                    if (RW_flag == DASH_READ_FLAG)                                                                                                           \
                    {                                                                                                                                        \
                        typed_data_p[offset] = dash_array_typed->at(i, j);                                                                                   \
                    }                                                                                                                                        \
                    else                                                                                                                                     \
                    {                                                                                                                                        \
                        dash_array_typed->at(i, j) = typed_data_p[offset];                                                                                   \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_ULONG:                                                                                                                                       \
        {                                                                                                                                                    \
            dash::Matrix<unsigned long, 2, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long, 2, unsigned long> *)dash_array_p_p;               \
            unsigned long *typed_data_p = (unsigned long *)data_p;                                                                                           \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                              \
                    if (RW_flag == DASH_READ_FLAG)                                                                                                           \
                    {                                                                                                                                        \
                        typed_data_p[offset] = dash_array_typed->at(i, j);                                                                                   \
                    }                                                                                                                                        \
                    else                                                                                                                                     \
                    {                                                                                                                                        \
                        dash_array_typed->at(i, j) = typed_data_p[offset];                                                                                   \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_ULLONG:                                                                                                                                      \
        {                                                                                                                                                    \
            dash::Matrix<unsigned long long, 2, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long long, 2, unsigned long> *)dash_array_p_p;     \
            unsigned long long *typed_data_p = (unsigned long long *)data_p;                                                                                 \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                              \
                    if (RW_flag == DASH_READ_FLAG)                                                                                                           \
                    {                                                                                                                                        \
                        typed_data_p[offset] = dash_array_typed->at(i, j);                                                                                   \
                    }                                                                                                                                        \
                    else                                                                                                                                     \
                    {                                                                                                                                        \
                        dash_array_typed->at(i, j) = typed_data_p[offset];                                                                                   \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_FLOAT:                                                                                                                                       \
        {                                                                                                                                                    \
            dash::Matrix<float, 2, unsigned long> *dash_array_typed = (dash::Matrix<float, 2, unsigned long> *)dash_array_p_p;                               \
            float *typed_data_p = static_cast<float *>(data_p);                                                                                              \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                              \
                    if (RW_flag == DASH_READ_FLAG)                                                                                                           \
                    {                                                                                                                                        \
                        typed_data_p[offset] = dash_array_typed->at(i, j);                                                                                   \
                    }                                                                                                                                        \
                    else                                                                                                                                     \
                    {                                                                                                                                        \
                        dash_array_typed->at(i, j) = typed_data_p[offset];                                                                                   \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_DOUBLE:                                                                                                                                      \
        {                                                                                                                                                    \
            dash::Matrix<double, 2, unsigned long> *dash_array_typed = (dash::Matrix<double, 2, unsigned long> *)dash_array_p_p;                             \
            double *typed_data_p = (double *)data_p;                                                                                                         \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                              \
                    if (RW_flag == DASH_READ_FLAG)                                                                                                           \
                    {                                                                                                                                        \
                        typed_data_p[offset] = dash_array_typed->at(i, j);                                                                                   \
                    }                                                                                                                                        \
                    else                                                                                                                                     \
                    {                                                                                                                                        \
                        dash_array_typed->at(i, j) = typed_data_p[offset];                                                                                   \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_DOUBLE_COMPLEX:                                                                                                                              \
        {                                                                                                                                                    \
            dash::Matrix<std::complex<double>, 2, unsigned long> *dash_array_typed = (dash::Matrix<std::complex<double>, 2, unsigned long> *)dash_array_p_p; \
            std::complex<double> *typed_data_p = (std::complex<double> *)data_p;                                                                             \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                              \
                    if (RW_flag == DASH_READ_FLAG)                                                                                                           \
                    {                                                                                                                                        \
                        typed_data_p[offset] = dash_array_typed->at(i, j);                                                                                   \
                    }                                                                                                                                        \
                    else                                                                                                                                     \
                    {                                                                                                                                        \
                        dash_array_typed->at(i, j) = typed_data_p[offset];                                                                                   \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        default:                                                                                                                                             \
            AU_EXIT("Unsupported datatype in AccessDashData !");                                                                                             \
        }                                                                                                                                                    \
    }

#define AccessDashData3D(dash_array_p_p, start_p, end_p, data_p, element_type_code_p, RW_flag)                                                               \
    {                                                                                                                                                        \
        unsigned long offset = 0;                                                                                                                            \
        switch (element_type_code_p)                                                                                                                         \
        {                                                                                                                                                    \
        case AU_SHORT:                                                                                                                                       \
        {                                                                                                                                                    \
            dash::Matrix<short, 3, unsigned long> *dash_array_typed = (dash::Matrix<short, 3, unsigned long> *)dash_array_p_p;                               \
            short *typed_data_p = (short *)data_p;                                                                                                           \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    for (unsigned long k = start_p[2]; k <= end_p[2]; k++)                                                                                   \
                    {                                                                                                                                        \
                        offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                          \
                        offset = offset * (end_p[2] - start_p[2] + 1) + (k - start_p[2]);                                                                    \
                        if (RW_flag == DASH_READ_FLAG)                                                                                                       \
                        {                                                                                                                                    \
                            typed_data_p[offset] = dash_array_typed->at(i, j, k);                                                                            \
                        }                                                                                                                                    \
                        else                                                                                                                                 \
                        {                                                                                                                                    \
                            dash_array_typed->at(i, j, k) = typed_data_p[offset];                                                                            \
                        }                                                                                                                                    \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_INT:                                                                                                                                         \
        {                                                                                                                                                    \
            dash::Matrix<int, 3, unsigned long> *dash_array_typed = (dash::Matrix<int, 3, unsigned long> *)dash_array_p_p;                                   \
            int *typed_data_p = (int *)data_p;                                                                                                               \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    for (unsigned long k = start_p[2]; k <= end_p[2]; k++)                                                                                   \
                    {                                                                                                                                        \
                        offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                          \
                        offset = offset * (end_p[2] - start_p[2] + 1) + (k - start_p[2]);                                                                    \
                        if (RW_flag == DASH_READ_FLAG)                                                                                                       \
                        {                                                                                                                                    \
                            typed_data_p[offset] = dash_array_typed->at(i, j, k);                                                                            \
                        }                                                                                                                                    \
                        else                                                                                                                                 \
                        {                                                                                                                                    \
                            dash_array_typed->at(i, j, k) = typed_data_p[offset];                                                                            \
                        }                                                                                                                                    \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_LONG:                                                                                                                                        \
        {                                                                                                                                                    \
            dash::Matrix<long, 3, unsigned long> *dash_array_typed = (dash::Matrix<long, 3, unsigned long> *)dash_array_p_p;                                 \
            long *typed_data_p = (long *)data_p;                                                                                                             \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    for (unsigned long k = start_p[2]; k <= end_p[2]; k++)                                                                                   \
                    {                                                                                                                                        \
                        offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                          \
                        offset = offset * (end_p[2] - start_p[2] + 1) + (k - start_p[2]);                                                                    \
                        if (RW_flag == DASH_READ_FLAG)                                                                                                       \
                        {                                                                                                                                    \
                            typed_data_p[offset] = dash_array_typed->at(i, j, k);                                                                            \
                        }                                                                                                                                    \
                        else                                                                                                                                 \
                        {                                                                                                                                    \
                            dash_array_typed->at(i, j, k) = typed_data_p[offset];                                                                            \
                        }                                                                                                                                    \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_LONG_LONG:                                                                                                                                   \
        {                                                                                                                                                    \
            dash::Matrix<long long, 3, unsigned long> *dash_array_typed = (dash::Matrix<long long, 3, unsigned long> *)dash_array_p_p;                       \
            long long *typed_data_p = (long long *)data_p;                                                                                                   \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    for (unsigned long k = start_p[2]; k <= end_p[2]; k++)                                                                                   \
                    {                                                                                                                                        \
                        offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                          \
                        offset = offset * (end_p[2] - start_p[2] + 1) + (k - start_p[2]);                                                                    \
                        if (RW_flag == DASH_READ_FLAG)                                                                                                       \
                        {                                                                                                                                    \
                            typed_data_p[offset] = dash_array_typed->at(i, j, k);                                                                            \
                        }                                                                                                                                    \
                        else                                                                                                                                 \
                        {                                                                                                                                    \
                            dash_array_typed->at(i, j, k) = typed_data_p[offset];                                                                            \
                        }                                                                                                                                    \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_USHORT:                                                                                                                                      \
        {                                                                                                                                                    \
            dash::Matrix<unsigned short, 3, unsigned long> *dash_array_typed = (dash::Matrix<unsigned short, 3, unsigned long> *)dash_array_p_p;             \
            unsigned short *typed_data_p = (unsigned short *)data_p;                                                                                         \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    for (unsigned long k = start_p[2]; k <= end_p[2]; k++)                                                                                   \
                    {                                                                                                                                        \
                        offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                          \
                        offset = offset * (end_p[2] - start_p[2] + 1) + (k - start_p[2]);                                                                    \
                        if (RW_flag == DASH_READ_FLAG)                                                                                                       \
                        {                                                                                                                                    \
                            typed_data_p[offset] = dash_array_typed->at(i, j, k);                                                                            \
                        }                                                                                                                                    \
                        else                                                                                                                                 \
                        {                                                                                                                                    \
                            dash_array_typed->at(i, j, k) = typed_data_p[offset];                                                                            \
                        }                                                                                                                                    \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_UINT:                                                                                                                                        \
        {                                                                                                                                                    \
            dash::Matrix<unsigned int, 3, unsigned long> *dash_array_typed = (dash::Matrix<unsigned int, 3, unsigned long> *)dash_array_p_p;                 \
            unsigned int *typed_data_p = (unsigned int *)data_p;                                                                                             \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    for (unsigned long k = start_p[2]; k <= end_p[2]; k++)                                                                                   \
                    {                                                                                                                                        \
                        offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                          \
                        offset = offset * (end_p[2] - start_p[2] + 1) + (k - start_p[2]);                                                                    \
                        if (RW_flag == DASH_READ_FLAG)                                                                                                       \
                        {                                                                                                                                    \
                            typed_data_p[offset] = dash_array_typed->at(i, j, k);                                                                            \
                        }                                                                                                                                    \
                        else                                                                                                                                 \
                        {                                                                                                                                    \
                            dash_array_typed->at(i, j, k) = typed_data_p[offset];                                                                            \
                        }                                                                                                                                    \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_ULONG:                                                                                                                                       \
        {                                                                                                                                                    \
            dash::Matrix<unsigned long, 3, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long, 3, unsigned long> *)dash_array_p_p;               \
            unsigned long *typed_data_p = (unsigned long *)data_p;                                                                                           \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    for (unsigned long k = start_p[2]; k <= end_p[2]; k++)                                                                                   \
                    {                                                                                                                                        \
                        offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                          \
                        offset = offset * (end_p[2] - start_p[2] + 1) + (k - start_p[2]);                                                                    \
                        if (RW_flag == DASH_READ_FLAG)                                                                                                       \
                        {                                                                                                                                    \
                            typed_data_p[offset] = dash_array_typed->at(i, j, k);                                                                            \
                        }                                                                                                                                    \
                        else                                                                                                                                 \
                        {                                                                                                                                    \
                            dash_array_typed->at(i, j, k) = typed_data_p[offset];                                                                            \
                        }                                                                                                                                    \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_ULLONG:                                                                                                                                      \
        {                                                                                                                                                    \
            dash::Matrix<unsigned long long, 3, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long long, 3, unsigned long> *)dash_array_p_p;     \
            unsigned long long *typed_data_p = (unsigned long long *)data_p;                                                                                 \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    for (unsigned long k = start_p[2]; k <= end_p[2]; k++)                                                                                   \
                    {                                                                                                                                        \
                        offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                          \
                        offset = offset * (end_p[2] - start_p[2] + 1) + (k - start_p[2]);                                                                    \
                        if (RW_flag == DASH_READ_FLAG)                                                                                                       \
                        {                                                                                                                                    \
                            typed_data_p[offset] = dash_array_typed->at(i, j, k);                                                                            \
                        }                                                                                                                                    \
                        else                                                                                                                                 \
                        {                                                                                                                                    \
                            dash_array_typed->at(i, j, k) = typed_data_p[offset];                                                                            \
                        }                                                                                                                                    \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_FLOAT:                                                                                                                                       \
        {                                                                                                                                                    \
            dash::Matrix<float, 3, unsigned long> *dash_array_typed = (dash::Matrix<float, 3, unsigned long> *)dash_array_p_p;                               \
            float *typed_data_p = (float *)data_p;                                                                                                           \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    for (unsigned long k = start_p[2]; k <= end_p[2]; k++)                                                                                   \
                    {                                                                                                                                        \
                        offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                          \
                        offset = offset * (end_p[2] - start_p[2] + 1) + (k - start_p[2]);                                                                    \
                        if (RW_flag == DASH_READ_FLAG)                                                                                                       \
                        {                                                                                                                                    \
                            typed_data_p[offset] = dash_array_typed->at(i, j, k);                                                                            \
                        }                                                                                                                                    \
                        else                                                                                                                                 \
                        {                                                                                                                                    \
                            dash_array_typed->at(i, j, k) = typed_data_p[offset];                                                                            \
                        }                                                                                                                                    \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_DOUBLE:                                                                                                                                      \
        {                                                                                                                                                    \
            dash::Matrix<double, 3, unsigned long> *dash_array_typed = (dash::Matrix<double, 3, unsigned long> *)dash_array_p_p;                             \
            double *typed_data_p = (double *)data_p;                                                                                                         \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    for (unsigned long k = start_p[2]; k <= end_p[2]; k++)                                                                                   \
                    {                                                                                                                                        \
                        offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                          \
                        offset = offset * (end_p[2] - start_p[2] + 1) + (k - start_p[2]);                                                                    \
                        if (RW_flag == DASH_READ_FLAG)                                                                                                       \
                        {                                                                                                                                    \
                            typed_data_p[offset] = dash_array_typed->at(i, j, k);                                                                            \
                        }                                                                                                                                    \
                        else                                                                                                                                 \
                        {                                                                                                                                    \
                            dash_array_typed->at(i, j, k) = typed_data_p[offset];                                                                            \
                        }                                                                                                                                    \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        case AU_DOUBLE_COMPLEX:                                                                                                                              \
        {                                                                                                                                                    \
            dash::Matrix<std::complex<double>, 3, unsigned long> *dash_array_typed = (dash::Matrix<std::complex<double>, 3, unsigned long> *)dash_array_p_p; \
            std::complex<double> *typed_data_p = (std::complex<double> *)data_p;                                                                             \
            for (unsigned long i = start_p[0]; i <= end_p[0]; i++)                                                                                           \
            {                                                                                                                                                \
                for (unsigned long j = start_p[1]; j <= end_p[1]; j++)                                                                                       \
                {                                                                                                                                            \
                    for (unsigned long k = start_p[2]; k <= end_p[2]; k++)                                                                                   \
                    {                                                                                                                                        \
                        offset = (i - start_p[0]) * (end_p[1] - start_p[1] + 1) + (j - start_p[1]);                                                          \
                        offset = offset * (end_p[2] - start_p[2] + 1) + (k - start_p[2]);                                                                    \
                        if (RW_flag == DASH_READ_FLAG)                                                                                                       \
                        {                                                                                                                                    \
                            typed_data_p[offset] = dash_array_typed->at(i, j, k);                                                                            \
                        }                                                                                                                                    \
                        else                                                                                                                                 \
                        {                                                                                                                                    \
                            dash_array_typed->at(i, j, k) = typed_data_p[offset];                                                                            \
                        }                                                                                                                                    \
                    }                                                                                                                                        \
                }                                                                                                                                            \
            }                                                                                                                                                \
            break;                                                                                                                                           \
        }                                                                                                                                                    \
        default:                                                                                                                                             \
            AU_EXIT("Unsupported datatype in AccessDashData !");                                                                                             \
        }                                                                                                                                                    \
    }

#endif
