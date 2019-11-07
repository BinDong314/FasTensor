#include "au_endpoint.h"

/**
     * @brief Get the Dimensions of the data
     * 
     * @return vector for the size of data  endpoint_dim_size.size() is the rank
     */
std::vector<unsigned long long> Endpoint::GetDimensions()
{
    return endpoint_dim_size;
}

/**
     * @brief Set the Dimensions 
     * 
     * @return < 0 error, works otherwise
     */
void Endpoint::SetDimensions(std::vector<unsigned long long> endpoint_dim_size_p)
{
    endpoint_ranks = endpoint_dim_size_p.size();
    endpoint_dim_size = endpoint_dim_size_p;
}

void Endpoint::SetDataElementType(AuEndpointDataType data_element_type_p)
{
    data_element_type = data_element_type_p;
}

AuEndpointDataType Endpoint::GetDataElementType()
{
    return data_element_type;
}

bool Endpoint::GetOpenFlag()
{
    return open_flag;
}

void Endpoint::SetOpenFlag(bool open_flag_p)
{
    open_flag = open_flag_p;
}

void Endpoint::SetRwFlag(unsigned read_write_flag_p)
{
    read_write_flag = read_write_flag_p;
}

unsigned Endpoint::GetRwFlag()
{
    return read_write_flag;
}