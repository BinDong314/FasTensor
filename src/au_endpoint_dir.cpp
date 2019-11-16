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

#include "au_endpoint_dir.h"

int EndpointDIR::ExtractMeta()
{
    return 0;
}

int EndpointDIR::Create()
{
    return 0;
}

int EndpointDIR::Open()
{
    return 0;
}

/**
     * @brief read the data from end-point
     * 
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data 
     * @return int < 0 error, >= 0 works
     */
int EndpointDIR::Read(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{
    sub_endpoint->Open();
    sub_endpoint->Read(start, end, data);
    sub_endpoint->Close();
    return 0;
}

/**
     * @brief write the data to the end-point
     * 
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data 
     * @return int < 0 error, >= 0 works
     */
int EndpointDIR::Write(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{
    sub_endpoint->Create();
    sub_endpoint->Write(start, end, data);
    sub_endpoint->Close();
    return 0;
}

/**
     * @brief close the end-point
     * 
     * @return int int < 0 error, >= 0 works
     */
int EndpointDIR::Close()
{
    return 0;
};

void EndpointDIR::EnableCollectiveIO()
{
}

void EndpointDIR::DisableCollectiveIO()
{
}

int EndpointDIR::PrintInfo()
{
    std::cout << data_endpoint_orig << std::endl;
    return 0;
}

void EndpointDIR::Map2MyType()
{
}

int EndpointDIR::ParseEndpointInfo()
{
    ExtractEndpointTypeInfo(endpoint_info, sub_endpoint_type, sub_endpoint_info);
}