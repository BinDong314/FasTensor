

/*
****************************

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy).
All rights reserved.

If you have questions about your rights to use or distribute this software,
please contact Berkeley Lab's Intellectual Property Office at
IPO@lbl.gov.

NOTICE.  This Software was developed under funding from the U.S. Department
of Energy and the U.S. Government consequently retains certain rights.  As
such, the U.S. Government has been granted for itself and others acting on
its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the
Software to reproduce, distribute copies to the public, prepare derivative
works, and perform publicly and display publicly, and to permit others to do so.


****************************


*** License Agreement ***

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy).
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

(1) Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

(2) Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

(3) Neither the name of the University of California, Lawrence Berkeley
National Laboratory, U.S. Dept. of Energy nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

You are under no obligation whatsoever to provide any bug fixes, patches,
or upgrades to the features, functionality or performance of the source
code ("Enhancements") to anyone; however, if you choose to make your
Enhancements available either publicly, or directly to Lawrence Berkeley
National Laboratory, without imposing a separate written license agreement
for such Enhancements, then you hereby grant the following license: a
non-exclusive, royalty-free perpetual license to install, use, modify,
prepare derivative works, incorporate into other computer software,
distribute, and sublicense such enhancements or derivative works thereof,
in binary and source code form.
*/

/**
 *
 * Author: Bin Dong dbin@lbl.gov
 * Web: https://crd.lbl.gov/bin-dong
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#ifndef END_POINT_MEMORY_H
#define END_POINT_MEMORY_H
#include <string>
#include <iostream>
#include <vector>
#include "ft_type.h"
#include "ft_endpoint.h"
#include "ft_local_mirror.h"
#include "ft_endpoint_hdf5.h"

#define DASH_READ_FLAG 0
#define DASH_WRITE_FLAG 1

// Use the (OP_USER_DEFINED_START )
#define DASH_NONVOLATILE_CODE 0
#define DASH_VOLATILE_CODE 1
#define DASH_ENABLE_LOCAL_MIRROR_CODE 2
#define DASH_MERGE_MIRRORS_CODE 3

#define MEMORY_BACKUP 0
#define MEMORY_RESTORE 1
#define MEMORY_CLONE 2
#define MEMORY_MERGE 3
#define MEMORY_SYNC_OVERLAP 4

#ifdef HAS_DASH_ENDPOINT
#include <unistd.h>
#include <iostream>
#include <cstddef>
#include <iomanip>
#include <libdash.h>

using dash::io::hdf5::hdf5_options;
using dash::io::hdf5::StoreHDF;
#endif

class EndpointMEMORY : public Endpoint
{
private:
    void *dash_array_p;

    // For local mirror
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
        SetCreateFlag(false);
    }
    /**
     * @brief Construct a new Endpoint in MEMORY
     *         Nothing to do there, can be used as sub-endpoint of directory
     */
    EndpointMEMORY()
    {
        SetOpenFlag(false);
        SetEndpointType(EP_MEMORY);
        SetCreateFlag(false);
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
    int Control(int opt_code, std::vector<std::string> &parameter_v) override;

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

#ifdef HAS_DASH_ENDPOINT
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
            dash_matrix_p_p = new dash::Matrix<unsigned short, rank_const_p, unsigned long>(dash_size_spec);        \
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

// #else
// class EndpointMEMORY : public Endpoint
// {
// private:
// public:
//     /**
//      * @brief Construct a new EndpointMEMORY object
//      *
//      * @param data_endpoint contains the info of the endpoint, e.g., file type + file info
//      */
//     EndpointMEMORY(std::string endpoint_info_p)
//     {
//         std::cout << "EP_MEMORY is not configured and compiled ! \n";
//         AU_EXIT(-1);
//     }
//     /**
//      * @brief Construct a new Endpoint in MEMORY
//      *         Nothing to do there, can be used as sub-endpoint of directory
//      */
//     EndpointMEMORY()
//     {
//         std::cout << "EP_MEMORY is not configured and compiled ! \n";
//         AU_EXIT(-1);
//     }

//     ~EndpointMEMORY()
//     {
//         std::cout << "EP_MEMORY is not configured and compiled ! \n";
//         AU_EXIT(-1);
//     }
//  };

// #endif
#endif
