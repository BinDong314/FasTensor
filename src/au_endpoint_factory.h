#include "au_endpoint.h"
#include "au_endpoint_hdf5.h"
#include <string.h>

class EndpointFactory
{
public:
    static Endpoint *NewEndpoint(std::string endpoint)
    {
        AuEndpointType endpoint_type;
        std::vector<std::string> endpoint_info;
        ExtractFileTypeInfo(endpoint, endpoint_type, endpoint_info);
        if (endpoint_type == EP_HDF5)
            return new EndpointHDF5(endpoint);
        /* if (description == "EP_IARRAY")
            return new EndpointIArray(endpoint);
        if (description == "EP_IVECTOR")
            return new EndpointIVECTOR(endpoint);*/
        return nullptr;
    }
};