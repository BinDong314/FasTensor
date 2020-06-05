#include "au_endpoint.h"
#include "au_endpoint_hdf5.h"
#include "au_endpoint_dir.h"
#include "au_endpoint_memory.h"
#include "au_endpoint_adios.h"
#include "au_endpoint_pnetcdf.h"

#include <string.h>

class EndpointFactory
{
public:
    /**
     * @brief create a normal endpoint
     * 
     * @param endpoint 
     * @return Endpoint* 
     */
    static Endpoint *NewEndpoint(std::string endpoint)
    {
        AuEndpointType endpoint_type;
        std::string endpoint_info;
        ExtractEndpointTypeInfo(endpoint, endpoint_type, endpoint_info);
        if (endpoint_type == EP_HDF5)
            return new EndpointHDF5(endpoint_info);
        if (endpoint_type == EP_DIR)
            return new EndpointDIR(endpoint_info);
        if (endpoint_type == EP_MEMORY)
            return new EndpointMEMORY(endpoint_info);
        if (endpoint_type == EP_ADIOS)
            return new EndpointADIOS(endpoint_info);
        if (endpoint_type == EP_PNETCDF)
            return new EndpointPnetCDF(endpoint_info);
        AU_EXIT("Not supported endpoint");
        return nullptr;
    }
};