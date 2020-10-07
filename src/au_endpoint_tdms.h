

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
#define NBYTE_PER_DATUM 2
#define EPOCH_DIFF 2082844800 //seconds from epoch(1904, Mac HFS+ ) to epoch (1970, Unix)

#define TDMS_MLS "MeasureLength[m]"
#define TDMS_SRS "SpatialResolution[m]"
//#define OUTPUT_META_TO_SCREEN 1

typedef enum
{
    tdsTypeVoid,
    tdsTypeI8,
    tdsTypeI16,
    tdsTypeI32,
    tdsTypeI64,
    tdsTypeU8,
    tdsTypeU16,
    tdsTypeU32,
    tdsTypeU64,
    tdsTypeSingleFloat,
    tdsTypeDoubleFloat,
    tdsTypeExtendedFloat,
    tdsTypeSingleFloatWithUnit = 0x19,
    tdsTypeDoubleFloatWithUnit,
    tdsTypeExtendedFloatWithUnit,
    tdsTypeString = 0x20,
    tdsTypeBoolean = 0x21,
    tdsTypeTimeStamp = 0x44,
    tdsTypeFixedPoint = 0x4F,
    tdsTypeComplexSingleFloat = 0x08000c,
    tdsTypeComplexDoubleFloat = 0x10000d,
    tdsTypeDAQmxRawData = 0xFFFFFFFF
} tdsDataType;

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

    /**
     * @brief extracts metadata, possbile endpoint_ranks/endpoint_dim_size/data_element_type
     * 
     * @return int < 0 error, >= 0 works 
     */
    int ExtractMeta() override;

    /**
     * @brief find MeasureLength and SpatialResolution
     * 
     * @param MeasureLength 
     * @param SpatialResolution 
     * @return int 
     */
    int FindMlSr(unsigned int &MeasureLength, double &SpatialResolution);
};
#endif
