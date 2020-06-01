
#include "au_endpoint_adios.h"

#ifdef HAS_ADIOS_END_POINT

int EndpointADIOS::ParseEndpointInfo()
{
    //std::cout << endpoint_info << "\n";
    std::stringstream ss(endpoint_info);
    if (!std::getline(ss, fn_str, ':'))
    {
        AU_EXIT("Invalued endpoint_info");
    }

    if (!std::getline(ss, vn_str, ':'))
    {
        AU_EXIT("Invalued endpoint_info");
    }

    std::cout << "fn_str =" << fn_str << ", vn_str = " << vn_str << std::endl;
    return 0;
}

//bindings/C/adios2/c/adios2_c_types.h
void EndpointADIOS::Map2MyType()
{
    switch (data_element_type)
    {
    case AU_SHORT:
        adios2_data_element_type = adios2_type_int16_t;
        return;
    case AU_INT:
        adios2_data_element_type = adios2_type_int32_t;
        return;
    case AU_LONG:
        adios2_data_element_type = adios2_type_int64_t;
        return;
    case AU_LONG_LONG:
        adios2_data_element_type = adios2_type_int64_t;
        return;
    case AU_USHORT:
        adios2_data_element_type = adios2_type_uint16_t;
        return;
    case AU_UINT:
        adios2_data_element_type = adios2_type_uint32_t;
        return;
    case AU_ULONG:
        adios2_data_element_type = adios2_type_uint64_t;
        return;
    case AU_ULLONG:
        adios2_data_element_type = adios2_type_uint64_t;
        return;
    case AU_FLOAT:
        adios2_data_element_type = adios2_type_float;
        cout << "float type! \n";
        return;
    case AU_DOUBLE:
        adios2_data_element_type = adios2_type_double;
        return;
    default:
        std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
        std::flush(std::cout);
        std::exit(EXIT_FAILURE);
    }
};

/**
     * @brief print information about the endpoint
     * 
     * @return < 0 error, >= 0 works 
     */
int EndpointADIOS::PrintInfo()
{
    std::cout << "fn_str : " << fn_str << ", vn_str :" << vn_str << "\n";
    return 0;
}

/**
    * @brief extracts metadata, possbile endpoint_ranks/endpoint_dim_size/data_element_type
    * 
    * @return int < 0 error, >= 0 works 
    */
int EndpointADIOS::ExtractMeta()
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
int EndpointADIOS::Write(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{
    Map2MyType();
    int rank = start.size();
    adios_start.resize(rank);
    adios_count.resize(rank);
    if (set_endpoint_dim_size_flag)
    {
        adios_shape.resize(rank);
    }
    for (int i = 0; i < rank; i++)
    {
        adios_start[i] = start[i];
        adios_count[i] = end[i] - start[i] + 1;
        if (set_endpoint_dim_size_flag)
        {
            adios_shape[i] = endpoint_dim_size[i];
        }
    }
    float *pf = (float *)data;
    cout << "adios_shape " << adios_shape[0] << ", " << adios_shape[1] << ", adios_start: " << adios_start[0] << ", " << adios_start[1] << ", adios_count: " << adios_count[0] << ", " << adios_count[1] << ", value =" << pf[0] << "\n";

    adios2_adios *adios;
    adios2_engine *engine;
    adios = adios2_init(MPI_COMM_WORLD, adios2_debug_mode_on);
    adios2_io *io = adios2_declare_io(adios, "BPFile_Write");

    adios2_variable *variable = adios2_define_variable(io, vn_str.c_str(), adios2_data_element_type, adios_shape.size(), adios_shape.data(), adios_start.data(), adios_count.data(), adios2_constant_dims_true);

    engine = adios2_open(io, fn_str.c_str(), adios2_mode_write);

    adios2_put(engine, variable, data, adios2_mode_deferred);

    adios2_close(engine);

    adios2_finalize(adios);
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
int EndpointADIOS::Read(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{
    Map2MyType();
    int rank = start.size();
    adios_start.resize(rank);
    adios_count.resize(rank);
    for (int i = 0; i < rank; i++)
    {
        adios_start[i] = start[i];
        adios_count[i] = end[i] - start[i] + 1;

        //cout << "Read, adios_start: " << adios_start[0] << ", " << adios_start[1] << ", adios_count: " << adios_count[0] << ", " << adios_count[1] << "\n";
    }

    //std::cout << "vn_str :" << vn_str << "\n";
    adios2_adios *adios;
    adios2_engine *engine;
    adios = adios2_init(MPI_COMM_WORLD, adios2_debug_mode_on);
    adios2_io *io = adios2_declare_io(adios, "BPFile_READ_C");
    engine = adios2_open(io, fn_str.c_str(), adios2_mode_read);
    adios2_variable *variable = adios2_inquire_variable(io, vn_str.c_str());
    adios2_set_selection(variable, rank, adios_start.data(), adios_count.data());

    adios2_get(engine, variable, data, adios2_mode_sync);

    adios2_close(engine);
    adios2_finalize(adios);

    return 0;
}

int EndpointADIOS::Create()
{
    return -1;
}

int EndpointADIOS::Open()
{
    return -1;
}

int EndpointADIOS::Close()
{
    return -1;
}

#endif