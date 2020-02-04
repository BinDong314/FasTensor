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

#ifndef END_POINT_DIR_H
#define END_POINT_DIR_H

#include "au_utility.h"
#include "au_type.h"
#include "au_endpoint.h"
#include "au_endpoint_hdf5.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>

//
//I/O layer
class EndpointDIR : public Endpoint
{
private:
    std::string endpoint_info;
    AuEndpointType sub_endpoint_type;
    std::string sub_endpoint_info; //Directory of files
    Endpoint *sub_endpoint;

    std::string dir_str;
    std::vector<std::string> dir_file_list;
    std::string append_sub_endpoint_info;              //dir_file_list[i] + append_sub_endpoint_info is the finale sub_endpoint_info
    std::vector<int> dir_chunk_size, dir_overlap_size; //set chunk size to be each sub_endpoint

public:
    /**
     * @brief Construct a new EndpointDIR object
     * 
     * @param data_endpoint contains the info of the endpoint, e.g., file type + file info
     */
    EndpointDIR(std::string endpoint_info_p)
    {
        endpoint_info = endpoint_info_p;
        ParseEndpointInfo();
        if (sub_endpoint_type == EP_HDF5)
            sub_endpoint = new EndpointHDF5();
        SetEndpointType(EP_DIR);
        sub_endpoint->SetDataElementType(data_element_type);
    }

    EndpointDIR()
    {
    }

    ~EndpointDIR()
    {
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

    int ParseEndpointInfo() override;

    /**
     * @brief Get the Chunk Size object
     * 
     * @return std::vector<int> 
     */
    std::vector<int> GetDirChunkSize() override;

    void SetDirChunkSize(std::vector<int> &dir_chunk_size_p) override;

    std::vector<std::string> GetDirFileVector() override;

    void SetDirFileVector(std::vector<std::string> &file_list) override;
};
#endif