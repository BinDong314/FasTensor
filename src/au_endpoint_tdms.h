

/**
 *
 * Email questions to dbin@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#ifndef END_POINT_TDMS_H
#define END_POINT_TDMS_H

#include "au_type.h"
#include "au_endpoint.h"
#include "au_endpoint_binary.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>

#define NBYTE_LEADIN 28

//
//I/O layer
class EndpointTDMS : public EndpointBinary
{
public:
    EndpointTDMS(std::string endpoint_info_p) : EndpointBinary(endpoint_info_p)
    {
    }
    /**
     * @brief Construct a new Endpoint in Binary 
     *         Nothing to do there, can be used as sub-endpoint of directory
     */
    EndpointTDMS() : EndpointBinary()
    {
    }

    virtual ~EndpointTDMS()
    {
    }

    /**
     * @brief update the seek_offset
     * 
     */
    void UpdateSeekOffset() override;
};
#endif
