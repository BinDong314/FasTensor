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

#ifndef END_POINT_H
#define END_POINT_H

#include "array_udf_attribute.h"
#include "endpoint_hdf5.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

/**
 * @brief Define the class for the Endpoint used by ArrayUDF 
 * to store the data. It contains basic infomation for the endpoint
 * as well as the operations supported by the endpoint
 * 
 */
class Endpoint
{
private:
    AuDataEndpointType endpoint_type;
    std::vector<std::string> endpoint_info;
    std::vector<unsigned long long> endpoint_dim_size;
    int endpoint_ranks;
    AuEndpointDataType data_element_type;

public:
    Endpoint(){};
    ~Endpoint(){};

    Endpoint(std::string data_endpoint)
    {
        ExtractFileTypeInfo(data_endpoint, endpoint_type, endpoint_info);
    }

    /**
     * @brief Get the Dimensions of the data
     * 
     * @return vector for the size of data  endpoint_dim_size.size() is the rank
     */
    std::vector<unsigned long long> GetDimensions()
    {
        return endpoint_dim_size;
    }

    /**
     * @brief Set the Dimensions 
     * 
     * @return < 0 error, works otherwise
     */
    void SetDimensions(std::vector<unsigned long long> endpoint_dim_size_p)
    {
        endpoint_ranks = endpoint_dim_size_p.size();
        endpoint_dim_size = endpoint_dim_size_p;
    }

    void SetDataElementType(AuEndpointDataType data_element_type_p)
    {
        data_element_type = data_element_type_p;
    }

    AuEndpointDataType GetDataElementType(AuEndpointDataType data_element_type_p)
    {
        return data_element_type;
    }

    /**
     * @brief extracts metadata, possbile endpoint_ranks/endpoint_dim_size/data_element_type
     * 
     * @return int < 0 error, >= 0 works 
     */
    virtual int ExtractMeta() = 0;
    /**
     * @brief print information about the endpoint
     * 
     * @return < 0 error, >= 0 works 
     */
    virtual int PrintInfo() = 0;

    /**
     * @brief create the endpoint
     * 
     * @return  < 0 error, >= 0 works 
     */
    virtual int Create() = 0;

    /**
     * @brief open the endpoint
     * 
     * @return < 0 error, >= 0 works 
     */
    virtual int Open() = 0;

    /**
     * @brief read the data from end-point
     * 
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data 
     * @return int < 0 error, >= 0 works
     */
    virtual int Read(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data) = 0;

    /**
     * @brief write the data to the end-point
     * 
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data 
     * @return int < 0 error, >= 0 works
     */
    virtual int Write(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data) = 0;

    /**
     * @brief close the end-point
     * 
     * @return int int < 0 error, >= 0 works
     */
    virtual int Close() = 0;

    virtual void Map2MyType() = 0;

    virtual void EnableCollectiveIO() = 0;

    virtual void DisableCollectiveIO() = 0;
};

class EndpointFactory
{
public:
    static Endpoint *NewEndpoint(const std::string &description, std::string endpoint)
    {
        if (description == "EP_HDF5")
            return new EndpointHDF5(endpoint);
        /* if (description == "EP_IARRAY")
            return new EndpointIArray(endpoint);
        if (description == "EP_IVECTOR")
            return new EndpointIVECTOR(endpoint);*/
        return nullptr;
    }
};

#endif
