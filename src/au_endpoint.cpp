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

#define VOID2UNION_HELPER(TARGET_TYPE, EMPLACE_INDEX)                                     \
    {                                                                                     \
        TARGET_TYPE *temp_pointer_of_type = (TARGET_TYPE *)data_vector_in_void;           \
        for (int i = 0; i < n_elements; i++)                                              \
        {                                                                                 \
            data_vector_in_union_type[i].emplace<EMPLACE_INDEX>(temp_pointer_of_type[i]); \
        }                                                                                 \
    }

std::vector<AuEndpointDataTypeUnion> Endpoint::Void2Union(void *data_vector_in_void, size_t n_elements)
{
    std::vector<AuEndpointDataTypeUnion> data_vector_in_union_type;
    data_vector_in_union_type.resize(n_elements);

    switch (data_element_type)
    {
    case AU_SHORT:
    {
        VOID2UNION_HELPER(short, AU_SHORT);
        break;
    }
    case AU_INT:
    {
        VOID2UNION_HELPER(int, AU_INT);
        break;
    }
    case AU_LONG:
    {
        VOID2UNION_HELPER(int, AU_INT);
        break;
    }
    case AU_LONG_LONG:
    {
        VOID2UNION_HELPER(int, AU_INT);
        break;
    }
    case AU_USHORT:
    {
        VOID2UNION_HELPER(int, AU_INT);
        break;
    }
    case AU_UINT:
    {
        VOID2UNION_HELPER(int, AU_INT);
        break;
    }
    case AU_ULONG:
    {
        VOID2UNION_HELPER(int, AU_INT);
        break;
    }
    case AU_ULLONG:
    {
        VOID2UNION_HELPER(int, AU_INT);
        break;
    }
    case AU_FLOAT:
    {
        VOID2UNION_HELPER(int, AU_INT);
        break;
    }
    case AU_DOUBLE:
    {
        VOID2UNION_HELPER(int, AU_INT);
        break;
    }
    default:
        std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
        std::flush(std::cout);
        std::exit(EXIT_FAILURE);
    }

    return data_vector_in_union_type;
}

#define UNION2VOID_HELPER(TARGET_TYPE, EMPLACE_INDEX)                                        \
    {                                                                                        \
        std::vector<TARGET_TYPE> temp_pointer_of_type;                                       \
        temp_pointer_of_type.resize(n_elements);                                             \
        for (int i = 0; i < n_elements; i++)                                                 \
        {                                                                                    \
            temp_pointer_of_type[i] = std::get<EMPLACE_INDEX>(data_vector_in_union_type[i]); \
        }                                                                                    \
        return static_cast<void *>(temp_pointer_of_type.data());                             \
    }

void *Endpoint::Union2Void(std::vector<AuEndpointDataTypeUnion> &data_vector_in_union_type)
{
    size_t n_elements = data_vector_in_union_type.size();
    switch (data_element_type)
    {
    case AU_SHORT:
    {
        UNION2VOID_HELPER(short, AU_SHORT);
        break;
    }
    case AU_INT:
    {
        UNION2VOID_HELPER(int, AU_INT);
        break;
    }
    case AU_LONG:
    {
        UNION2VOID_HELPER(int, AU_INT);
        break;
    }
    case AU_LONG_LONG:
    {
        UNION2VOID_HELPER(int, AU_INT);
        break;
    }
    case AU_USHORT:
    {
        UNION2VOID_HELPER(int, AU_INT);
        break;
    }
    case AU_UINT:
    {
        UNION2VOID_HELPER(int, AU_INT);
        break;
    }
    case AU_ULONG:
    {
        UNION2VOID_HELPER(int, AU_INT);
        break;
    }
    case AU_ULLONG:
    {
        UNION2VOID_HELPER(int, AU_INT);
        break;
    }
    case AU_FLOAT:
    {
        UNION2VOID_HELPER(int, AU_INT);
        break;
    }
    case AU_DOUBLE:
    {
        UNION2VOID_HELPER(int, AU_INT);
        break;
    }
    default:
        std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
        std::flush(std::cout);
        std::exit(EXIT_FAILURE);
    }
}

/**
     * @brief set the endpoint_info string 
     * 
     * @param endpoint_info 
     */
void Endpoint::SetEndpointInfo(std::string endpoint_info_p)
{
    endpoint_info = endpoint_info_p;
    ParseEndpointInfo(); //After set, we need to refresh related other parts
}

/**
    * @brief Get the endpoint_info string 
     * 
     * @return std::string 
     */
std::string Endpoint::GetEndpointInfo()
{
    return endpoint_info;
}

/**
     * @brief Set the Endpoint Type object
     * 
     * @param endpoint_type_p 
     */
void Endpoint::SetEndpointType(AuEndpointType endpoint_type_p)
{
    endpoint_type = endpoint_type_p;
}

/**
     * @brief Get the Endpoint Type object
     * 
     * @return AuEndpointType 
     */
AuEndpointType Endpoint::GetEndpointType()
{
    return endpoint_type;
}

std::vector<std::string> Endpoint::GetDirFileVector()
{
    AU_EXIT("Error: should not be called");
    std::vector<std::string> sv;
    return sv;
}

void Endpoint::SetDirFileVector(std::vector<std::string> &file_list)
{
    AU_EXIT("Error: should not be called");
}

/**
     * @brief Get the Dir Chunk Size object
     * 
     * @return std::vector<int> 
     */
std::vector<int> Endpoint::GetDirChunkSize()
{
    AU_EXIT("Error: should not be called");
}

/**
     * @brief Set the Dir Chunk Size object
     * 
     * @param dir_chunk_size_p 
     */
void Endpoint::SetDirChunkSize(std::vector<int> &dir_chunk_size_p)
{
    AU_EXIT("Error: should not be called");
}

/**
     * @brief call a special operator on endpoint
     *        such as, enable collective I/O for HDF5
     *                 dump file from MEMORY to HDF5
     * @param opt_code, specially defined code 
     */
int Endpoint::SpecialOperator(int opt_code, std::string parameter)
{
    AU_EXIT("Error: should not be called");
    return 0;
}