#include "au_endpoint_memory.h"

int EndpointMEMORY::ExtractMeta()
{

    return 0;
}

int EndpointMEMORY::Create()
{
    AU_INFO("Memory Create");

    std::vector<unsigned long> endpoint_dim_size_ul;
    endpoint_dim_size_ul.resize(endpoint_dim_size.size());
    for (int i = 0; i < endpoint_dim_size.size(); i++)
    {
        endpoint_dim_size_ul[i] = endpoint_dim_size[i];
    }

    switch (endpoint_dim_size.size())
    {
    case 1:
    {
        CreateDashMatrix(dash_array_p, 1, data_element_type, endpoint_dim_size_ul);
        break;
    }
    case 2:
    {
        CreateDashMatrix(dash_array_p, 2, data_element_type, endpoint_dim_size_ul);
        break;
    }
    case 3:
    {
        CreateDashMatrix(dash_array_p, 3, data_element_type, endpoint_dim_size_ul);
        break;
    }
    default:
        AU_EXIT("Only support until 3D in memory data!");
        break;
    }

    return 0;
}

int EndpointMEMORY::Open()
{
    AU_INFO("Memory Open");
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
    AU_INFO("Memory read");

    std::vector<unsigned long> start_ul, end_ul;
    start_ul.resize(start.size());
    end_ul.resize(end.size());

    for (int i = 0; i < start.size(); i++)
    {
        start_ul[i] = start[i];
        end_ul[i] = end[i];
    }
    switch (endpoint_dim_size.size())
    {
    case 1:
    {
        AccessDashData1D(dash_array_p, start_ul, end_ul, data, data_element_type, DASH_READ_FLAG);
        break;
    }
    case 2:
    {
        AccessDashData2D(dash_array_p, start_ul, end_ul, data, data_element_type, DASH_READ_FLAG);
        break;
    }
    case 3:
    {
        AccessDashData3D(dash_array_p, start_ul, end_ul, data, data_element_type, DASH_READ_FLAG);
        break;
    }
    default:
        AU_EXIT("Only support until 3D in memory data!");
        break;
    }

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
    AU_INFO("Memory Write");

    switch (endpoint_dim_size.size())
    {
    case 1:
        /* code */
        AccessDashData1D(dash_array_p, start, end, data, data_element_type, DASH_WRITE_FLAG);
        break;
    case 2:
        AccessDashData2D(dash_array_p, start, end, data, data_element_type, DASH_WRITE_FLAG);
        /* code */
        break;
    case 3:
        AccessDashData3D(dash_array_p, start, end, data, data_element_type, DASH_WRITE_FLAG);
        /* code */
        break;
    default:
        AU_EXIT("Only support until 3D in memory data!");
        break;
    }
    return 0;
}

/**
     * @brief close the end-point
     * 
     * @return int int < 0 error, >= 0 works
     */
int EndpointMEMORY::Close()
{
    AU_INFO("Memory Close");

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
