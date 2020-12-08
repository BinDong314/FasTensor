
#include "au_endpoint_pnetcdf.h"

#ifdef HAS_PNETCDF_END_POINT

static void handle_pnetcdf_error(int status, int lineno)
{
    fprintf(stderr, "Error at line %d: %s\n", lineno, ncmpi_strerror(status));
    MPI_Abort(MPI_COMM_WORLD, 1);
}

int EndpointPnetCDF::ParseEndpointInfo()
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
void EndpointPnetCDF::Map2MyType()
{
    switch (data_element_type)
    {
    case AU_SHORT:
        element_type_mpi = MPI_SHORT;
        element_type_nc = NC_SHORT;
        return;
    case AU_INT:
        element_type_mpi = MPI_INT;
        element_type_nc = NC_INT;
        return;
    case AU_LONG:
        element_type_mpi = MPI_LONG;
        element_type_nc = NC_LONG;
        return;
    case AU_LONG_LONG:
        element_type_mpi = MPI_LONG_LONG;
        element_type_nc = NC_LONG;
        return;
    case AU_USHORT:
        element_type_mpi = MPI_UNSIGNED_SHORT;
        element_type_nc = NC_USHORT;
        return;
    case AU_UINT:
        element_type_mpi = MPI_UNSIGNED;
        element_type_nc = NC_UINT;
        return;
    case AU_ULONG:
        element_type_mpi = MPI_UNSIGNED_LONG;
        element_type_nc = NC_UINT64;
        return;
    case AU_ULLONG:
        element_type_mpi = MPI_UINT64_T;
        element_type_nc = NC_UINT64;
        return;
    case AU_FLOAT:
        element_type_mpi = MPI_FLOAT;
        element_type_nc = NC_FLOAT;
        return;
    case AU_DOUBLE:
        element_type_mpi = MPI_DOUBLE;
        element_type_nc = NC_DOUBLE;
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
int EndpointPnetCDF::PrintInfo()
{
    std::cout << "fn_str : " << fn_str << ", vn_str :" << vn_str << "\n";
    return 0;
}

/**
    * @brief extracts metadata, possbile endpoint_ranks/endpoint_dim_size/data_element_type
    * 
    * @return int < 0 error, >= 0 works 
    */
int EndpointPnetCDF::ExtractMeta()
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
int EndpointPnetCDF::Write(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{
    Map2MyType();
    if (!GetCreateFlag())
    {
        Create();
    }

    std::vector<MPI_Offset> pnetcdf_start, pnetcdf_count;
    int rank = start.size();
    pnetcdf_start.resize(rank);
    pnetcdf_count.resize(rank);

    MPI_Offset pnetcdf_element_count = 1;
    for (int i = 0; i < rank; i++)
    {
        pnetcdf_start[i] = start[i];
        pnetcdf_count[i] = end[i] - start[i] + 1;
        pnetcdf_element_count = pnetcdf_count[i] * pnetcdf_element_count;
    }

    int ret = ncmpi_put_vara_all(ncfile, ncvar, &pnetcdf_start[0], &pnetcdf_count[0], data, pnetcdf_element_count, element_type_mpi);
    if (ret != NC_NOERR)
        handle_pnetcdf_error(ret, __LINE__);
}

/**
     * @brief read the data from end-point
     * 
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data 
     * @return int < 0 error, >= 0 works
     */
int EndpointPnetCDF::Read(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{
    Map2MyType();

    if (!GetOpenFlag())
    {
        Open();
    }
    std::vector<MPI_Offset> pnetcdf_start, pnetcdf_count;
    int rank = start.size();
    pnetcdf_start.resize(rank);
    pnetcdf_count.resize(rank);

    MPI_Offset pnetcdf_element_count = 1;
    for (int i = 0; i < rank; i++)
    {
        pnetcdf_start[i] = start[i];
        pnetcdf_count[i] = end[i] - start[i] + 1;
        pnetcdf_element_count = pnetcdf_count[i] * pnetcdf_element_count;
    }

    int ret = ncmpi_get_vara_all(ncfile, ncvar, &pnetcdf_start[0], &pnetcdf_count[0], data, pnetcdf_element_count, element_type_mpi);
    if (ret != NC_NOERR)
        handle_pnetcdf_error(ret, __LINE__);
    return 0;
}

int EndpointPnetCDF::Create()
{
    int ret;
    ret = ncmpi_create(MPI_COMM_WORLD_DEFAULT, fn_str.c_str(), NC_64BIT_OFFSET, MPI_INFO_NULL_DEFAULT, &ncfile);
    if (ret != NC_NOERR)
        handle_pnetcdf_error(ret, __LINE__);

    int rank = endpoint_size.size();

    std::vector<int> dimid;
    dimid.resize(rank);

    std::string dim_name;
    for (int i = 0; i < rank; i++)
    {
        dim_name = "dim" + std::to_string(i);
        ret = ncmpi_def_dim(ncfile, dim_name.c_str(), endpoint_size[i], &dimid[i]);
        if (ret != NC_NOERR)
            handle_pnetcdf_error(ret, __LINE__);
    }
    ret = ncmpi_def_var(ncfile, vn_str.c_str(), element_type_nc, rank, &dimid[0], &ncvar);
    if (ret != NC_NOERR)
        handle_pnetcdf_error(ret, __LINE__);

    ret = ncmpi_enddef(ncfile);
    if (ret != NC_NOERR)
        handle_pnetcdf_error(ret, __LINE__);

    SetCreateFlag(true);
    SetOpenFlag(true);
    return 0;
}

int EndpointPnetCDF::Open()
{
    int ret = ncmpi_open(MPI_COMM_WORLD_DEFAULT, fn_str.c_str(), NC_WRITE, MPI_INFO_NULL_DEFAULT,
                         &ncfile);
    if (ret != NC_NOERR)
        handle_pnetcdf_error(ret, __LINE__);

    ret = ncmpi_inq_varid(ncfile, vn_str.c_str(), &ncvar);
    if (ret != NC_NOERR)
        handle_pnetcdf_error(ret, __LINE__);
    return ret;
}

int EndpointPnetCDF::Close()
{
    SetCreateFlag(false);
    SetOpenFlag(false);
    return ncmpi_close(ncfile);
}

#endif