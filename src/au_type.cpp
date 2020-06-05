#include "au_type.h"
#include "au_utility.h"

AuEndpointType MapString2EndpointType(std::string endpoint_type_str)
{
    AuEndpointType endpoint_type;
    if (endpoint_type_str == "EP_HDF5")
    {
        endpoint_type = EP_HDF5;
    }
    else if (endpoint_type_str == "EP_PNETCDF")
    {
        endpoint_type = EP_PNETCDF;
    }
    else if (endpoint_type_str == "EP_AUDIOS")
    {
        endpoint_type = EP_ADIOS;
    }
    else if (endpoint_type_str == "EP_BINARY")
    {
        endpoint_type = EP_BINARY;
    }
    else if (endpoint_type_str == "EP_VIRTUAL")
    {
        endpoint_type = EP_VIRTUAL;
    }
    else if (endpoint_type_str == "EP_MEMORY")
    {
        endpoint_type = EP_MEMORY;
    }
    else if (endpoint_type_str == "EP_H5VDS")
    {
        endpoint_type = EP_H5VDS;
    }
    else if (endpoint_type_str == "EP_DIR")
    {
        endpoint_type = EP_DIR;
    }
    else if (endpoint_type_str == "EP_DASSA")
    {
        endpoint_type = EP_DASSA;
    }
    else if (endpoint_type_str == "EP_TDMS")
    {
        endpoint_type = EP_TDMS;
    }
    else
    {
        AU_EXIT("Not supported AuDataEndpointType");
    }
    return endpoint_type;
}