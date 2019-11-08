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

/**
     * @brief Get the size of the type for the element
     * 
     * @return int 
     */
int Endpoint::GetDataElementTypeSize()
{
    switch (data_element_type)
    {
    case AU_SHORT:
        return sizeof(short);
    case AU_INT:
        return sizeof(int);
    case AU_LONG:
        return sizeof(long);
    case AU_LONG_LONG:
        return sizeof(long long);
    case AU_USHORT:
        return sizeof(unsigned short);
    case AU_UINT:
        return sizeof(unsigned int);
    case AU_ULONG:
        return sizeof(unsigned long);
    case AU_ULLONG:
        return sizeof(unsigned long long);
    case AU_FLOAT:
        return sizeof(float);
    case AU_DOUBLE:
        return sizeof(double);
    default:
        std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
        std::flush(std::cout);
        std::exit(EXIT_FAILURE);
    }
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