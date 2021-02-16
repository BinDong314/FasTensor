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

#ifndef END_POINT_ADIOS_H
#define END_POINT_ADIOS_H

#include "ft_type.h"
#include "ft_endpoint.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>

//#define HAS_ADIOS_END_POINT 1

#ifdef HAS_ADIOS_END_POINT

#include <adios2_c.h>
class EndpointADIOS : public Endpoint
{
private:
    std::string fn_str, vn_str; //file name and variable name
    adios2_type adios2_data_element_type;
    //adios2_variable *v;
    std::vector<size_t> adios_shape, adios_start, adios_count;

    adios2_adios *adios;
    adios2_io *rw_io;
    adios2_variable *rw_variable;
    adios2_engine *rw_engine;

public:
    /**
     * @brief Construct a new EndpointHDF5 object
     * 
     * @param data_endpoint contains the info of the endpoint, e.g., file type + file info
     */
    EndpointADIOS(std::string endpoint_info_p)
    {
        endpoint_info = endpoint_info_p;
        ParseEndpointInfo();

        adios = adios2_init(MPI_COMM_WORLD, adios2_debug_mode_on);
        if (adios == NULL)
        {
            printf("ERROR: invalid %s handler at line %d  \n", "adios2_init", __LINE__);
            exit(EXIT_FAILURE);
        }

        rw_io = adios2_declare_io(adios, "SomeName");
        if (rw_io == NULL)
        {
            printf("ERROR: invalid %s handler at line %d  \n", "adios2_declare_io", __LINE__);
            exit(EXIT_FAILURE);
        }

        //std::cout << "Init done \n";
        SetOpenFlag(false);
        SetCreateFlag(false);
    }
    ~EndpointADIOS()
    {
        Close();
        adios2_finalize(adios);
        //std::cout << "Close done \n";
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

    int ParseEndpointInfo() override;
};
#else
class EndpointADIOS : public Endpoint
{
private:
public:
    /**
     * @brief Construct a new EndpointHDF5 object
     * 
     * @param data_endpoint contains the info of the endpoint, e.g., file type + file info
     */
    EndpointADIOS(std::string endpoint_info_p)
    {
        std::cout << "EndpointADIOS is not configured and compiled ! \n";
        AU_EXIT(-1);
    }
    ~EndpointADIOS()
    {
        Close();
    }

    int ParseEndpointInfo() override
    {
        return -1;
    }

    int ExtractMeta() override
    {
        return -1;
    }

    int PrintInfo() override
    {
        return -1;
    }

    int Create() override
    {
        return -1;
    }

    int Open() override
    {
        return -1;
    }

    void Map2MyType() override
    {
    }

    int Read(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data) override
    {
        return -1;
    }

    int Write(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data) override
    {
        return -1;
    }

    int Close() override
    {
        return -1;
    }
};
#endif
#endif
