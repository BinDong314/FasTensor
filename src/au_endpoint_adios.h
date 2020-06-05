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

#ifndef END_POINT_ADIOS_H
#define END_POINT_ADIOS_H

#include "au_type.h"
#include "au_endpoint.h"
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
    adios2_engine *write_engine;

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
        rw_io = adios2_declare_io(adios, "SomeName");
        SetOpenFlag(false);
        SetCreateFlag(false);
    }
    ~EndpointADIOS()
    {
        adios2_finalize(adios);
        //adios2_perform_puts(write_engine);
        //adios2_flush(write_engine);
        adios2_close(write_engine);
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
