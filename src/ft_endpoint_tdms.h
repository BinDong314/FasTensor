

/*
****************************

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy). 
All rights reserved.

If you have questions about your rights to use or distribute this software,
please contact Berkeley Lab's Intellectual Property Office at
IPO@lbl.gov.

NOTICE.  This Software was developed under funding from the U.S. Department
of Energy and the U.S. Government consequently retains certain rights.  As
such, the U.S. Government has been granted for itself and others acting on
its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the
Software to reproduce, distribute copies to the public, prepare derivative 
works, and perform publicly and display publicly, and to permit others to do so.


****************************


*** License Agreement ***

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy). 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

(1) Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

(2) Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

(3) Neither the name of the University of California, Lawrence Berkeley
National Laboratory, U.S. Dept. of Energy nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

You are under no obligation whatsoever to provide any bug fixes, patches,
or upgrades to the features, functionality or performance of the source
code ("Enhancements") to anyone; however, if you choose to make your
Enhancements available either publicly, or directly to Lawrence Berkeley
National Laboratory, without imposing a separate written license agreement
for such Enhancements, then you hereby grant the following license: a
non-exclusive, royalty-free perpetual license to install, use, modify,
prepare derivative works, incorporate into other computer software,
distribute, and sublicense such enhancements or derivative works thereof,
in binary and source code form.
*/

/**
 *
 * Author: Bin Dong dbin@lbl.gov
 * Web: https://crd.lbl.gov/bin-dong
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#ifndef END_POINT_TDMS_H
#define END_POINT_TDMS_H

#include "ft_type.h"
#include "ft_endpoint.h"
#include "ft_endpoint_binary.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <map>

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

template <typename T>
std::string tostr(const T &t)
{
    std::ostringstream os;
    os << t;
    return os.str();
}

//
//I/O layer
class EndpointTDMS : public EndpointBinary
{
private:
    std::map<std::string, std::string> meta_map; //Get the list of metadata from TDMS file
    bool tdms_ExtractMeta_once_flag = false;

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

    /**
     * @brief Get the Populate data into MataMap
     * 
     * @return int 
     */
    int PopulateMetaMap();

    /**
     * @brief Set the Attribute object
     *   Do not need to be pure virtual method
     * @param name 
     * @param data 
     * @return int 
     */
    int WriteAttribute(const std::string &name, const void *data, FTDataType data_type_p, const size_t &data_length_p = 0) override;

    /**
     * @brief Get the Attribute object
     *  Do not need to be pure virtual method
     * @param name 
     * @param data 
     * @return int 
     */
    int ReadAttribute(const std::string &name, void *data, FTDataType data_type_p, const size_t &data_length_p = 0) override;

    /**
     * @brief call a special operator on endpoint
     *        such as, enable collective I/O for HDF5
     *                 dump file from MEMORY to HDF5
     * @param opt_code, specially defined code 
     */
    int Control(int opt_code, std::vector<std::string> &parameter_v) override;

    /**
     * @brief Read all attribute name
     * 
     * @param attri_name 
     * @return int 
     */
    int ReadAllAttributeName(std::vector<std::string> &attr_name);

    int GetAttributeSize(const std::string &name, FTDataType data_type_p) override;
};
#endif
