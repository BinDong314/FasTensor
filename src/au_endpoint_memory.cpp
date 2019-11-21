#include "au_endpoint_memory.h"

int EndpointMEMORY::ExtractMeta()
{

    return 0;
}

int EndpointMEMORY::Create()
{

    return 0;
}

int EndpointMEMORY::Open()
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
int EndpointMEMORY::Read(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{

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
int EndpointMEMORY::Write(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{
    return 0;
}

/**
     * @brief close the end-point
     * 
     * @return int int < 0 error, >= 0 works
     */
int EndpointMEMORY::Close()
{
    return 0;
};

void EndpointMEMORY::EnableCollectiveIO()
{
}

void EndpointMEMORY::DisableCollectiveIO()
{
}

int EndpointMEMORY::PrintInfo()
{
    std::cout << data_endpoint_orig << std::endl;
    return 0;
}

void EndpointMEMORY::Map2MyType()
{
}

int EndpointMEMORY::ParseEndpointInfo()
{
    return 0;
}
