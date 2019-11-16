#include "au_endpoint.h"
#include "au_endpoint_hdf5.h"
#include "au_endpoint_dir.h"
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
        /* if (description == "EP_IARRAY")
            return new EndpointIArray(endpoint);
        if (description == "EP_IVECTOR")
            return new EndpointIVECTOR(endpoint);*/
        return nullptr;
    }
};