
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

    /*
    adios2_io *write_io = adios2_declare_io(adios, "SomeName");
    adios2_engine *write_engine = adios2_open(write_io, fn_str.c_str(), adios2_mode_write);
    if (write_engine == NULL)
    {
        std::cout << "open engine failed : " << fn_str << "\n";
    }
    adios2_variable *write_variable = adios2_inquire_variable(write_io, vn_str.c_str());
    if (write_variable == NULL)
    {
        std::cout << "define variable : " << vn_str << "\n";
        write_variable = adios2_define_variable(write_io, vn_str.c_str(), adios2_data_element_type, adios_shape.size(), adios_shape.data(), adios_start.data(), adios_count.data(), adios2_constant_dims_true);
    }
    else
    {
        adios2_set_selection(write_variable, rank, adios_start.data(), adios_count.data());
    }
    adios2_put(write_engine, write_variable, data, adios2_mode_sync);
    adios2_perform_puts(write_engine);
    adios2_flush(write_engine);
    adios2_close(write_engine);
    */

    if (!GetCreateFlag())
    {
        Create();
    }
    adios2_set_selection(rw_variable, rank, adios_start.data(), adios_count.data());
    adios2_put(write_engine, rw_variable, data, adios2_mode_deferred);

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

    /*
    adios2_io *read_io = adios2_declare_io(adios, "SomeName");
    adios2_engine *read_engine = adios2_open(read_io, fn_str.c_str(), adios2_mode_read);

    //std::cout << "vn_str :" << vn_str << "\n";
    adios2_variable *read_variable = adios2_inquire_variable(read_io, vn_str.c_str());
    adios2_set_selection(read_variable, rank, adios_start.data(), adios_count.data());
    adios2_get(read_engine, variable, data, adios2_mode_sync);

    adios2_close(engine);
    //adios2_finalize(adios);
    */

    if (!GetCreateFlag())
    {
        Create();
    }
    //adios2_set_selection(rw_variable, rank, adios_start.data(), adios_count.data());
    //adios2_engine *read_engine = adios2_open(rw_io, fn_str.c_str(), adios2_mode_read);
    //adios2_get(read_engine, rw_variable, data, adios2_mode_sync);
    //adios2_close(read_engine);
    return 0;
}

int EndpointADIOS::Create()
{

    int rank = endpoint_dim_size.size();
    adios_start.resize(rank);
    adios_count.resize(rank);
    if (set_endpoint_dim_size_flag)
    {
        adios_shape.resize(rank);
    }
    for (int i = 0; i < rank; i++)
    {
        adios_start[i] = 0;
        adios_count[i] = endpoint_dim_size[i] - 1;
        if (set_endpoint_dim_size_flag)
        {
            adios_shape[i] = endpoint_dim_size[i];
        }
    }
    std::cout << "define variable : " << vn_str << ", adios_shape : " << adios_shape[0] << ", " << adios_shape[1] << " in Create\n";
    rw_variable = adios2_define_variable(rw_io, vn_str.c_str(), adios2_data_element_type, adios_shape.size(), adios_shape.data(), adios_start.data(), adios_count.data(), adios2_constant_dims_false);

    write_engine = adios2_open(rw_io, fn_str.c_str(), adios2_mode_write);
    //Do the first write to create the
    SetCreateFlag(true);
    return 0;
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