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

#include "ft_endpoint_tdms.h"

void EndpointTDMS::UpdateSeekOffset()
{

    uint64_t nByte_metadata;
    FILE *fp = this->GetFP();
    fseek(fp, 20, SEEK_SET);
    fread(&nByte_metadata, sizeof(uint64_t), 1, fp);
    long int seek_offset = nByte_metadata + NBYTE_LEADIN;

    this->SetSeekOffset(seek_offset);

    //printf("TDMS header: %ld (%d fixed + %lld user's)\n", seek_offset, NBYTE_LEADIN, nByte_metadata);
}

/**
     * @brief extracts metadata, possbile endpoint_ranks/endpoint_dim_size/data_element_type
     * 
     * @return int < 0 error, >= 0 works 
     */
int EndpointTDMS::ExtractMeta()
{
    if (tdms_ExtractMeta_once_flag)
    {
        return 0;
    }
    //PrintInfo();
    //Open();
    //endpoint_ranks = H5Sget_simple_extent_ndims(dataspace_id);
    //endpoint_dim_size.resize(endpoint_ranks);
    //H5Sget_simple_extent_dims(dataspace_id, &endpoint_dim_size[0], NULL);
    endpoint_ranks = 2;
    endpoint_size.resize(endpoint_ranks);

    if (GetOpenFlag() == false)
    {
        Open();
    }

    uint64_t nByte_metadata;
    FILE *fp = this->GetFP();
    fseek(fp, 20, SEEK_SET);
    fread(&nByte_metadata, sizeof(uint64_t), 1, fp);
    int nByte_header = nByte_metadata + NBYTE_LEADIN;

    unsigned int MeasureLength;
    double SpatialResolution;

    //FindMlSr(MeasureLength, SpatialResolution);

    PopulateMetaMap();
    std::map<std::string, std::string>::iterator it;
    it = meta_map.find("MeasureLength[m]");
    if (it == meta_map.end())
    {
        AU_EXIT("Can not find MeasureLength[m] in TDMS file's header. ");
    }
    std::istringstream reader(it->second);
    reader >> MeasureLength;

    it = meta_map.find("SpatialResolution[m]");
    if (it == meta_map.end())
    {
        AU_EXIT("Can not find SpatialResolution[m] in TDMS file's header. ");
    }
    std::istringstream reader2(it->second);
    reader2 >> SpatialResolution;

    //std::cout << "MeasureLength = " << MeasureLength << " , SpatialResolution = " << SpatialResolution << "\n";

    int nTrace = MeasureLength / SpatialResolution;

    fseek(fp, 0, SEEK_END);         // seek to end of file
    long int file_size = ftell(fp); // get current file pointer
    fseek(fp, 0, SEEK_SET);

    int nPoint = ((file_size - nByte_header) / NBYTE_PER_DATUM) / nTrace;

    //std::cout << "nPoint = " << nPoint << " , nTrace = " << nTrace << "\n";

    endpoint_size[0] = nTrace;
    endpoint_size[1] = nPoint;

    tdms_ExtractMeta_once_flag = true;

    return 0;
}

/**
     * @brief find MeasureLength and SpatialResolution
     * 
     * @param MeasureLength 
     * @param SpatialResolution 
     * @return int 
     */
int EndpointTDMS::FindMlSr(unsigned int &MeasureLength, double &SpatialResolution)
{
    FILE *fp = this->GetFP();
    //#define TDMS_MLS "MeasureLength[m]"
    //#define TDMS_SRS "SpatialResolution[m]"
    bool find_mls_flag = false;
    bool find_srs_flag = false;
    //fp = fopen(filename_input, "r");

    //Go to the standard hearder
    uint64_t nByte_metadata;
    fseek(fp, 20, SEEK_SET);
    fread(&nByte_metadata, sizeof(uint64_t), 1, fp);

#ifdef OUTPUT_META_TO_SCREEN
    printf("Size of customized metadata right after the fixed %d byte header : %lld\n", NBYTE_LEADIN, nByte_metadata);
#endif

    //Find the number of objects
    fseek(fp, NBYTE_LEADIN, SEEK_SET);
    int number_of_objects;
    fread(&number_of_objects, sizeof(uint32_t), 1, fp);

#ifdef OUTPUT_META_TO_SCREEN
    printf("Metadata: \n");
    printf("Number of objects : %d \n", number_of_objects);
#endif

    //Write number_of_objects to HDF5
    int length_of_first_object_path;
    fread(&length_of_first_object_path, sizeof(uint32_t), 1, fp);

#ifdef DEBUG_OUTPUT
    printf("length_of_first_object_path : %d \n", length_of_first_object_path);
#endif

    char object_path[1];
    fread(object_path, sizeof(char), 1, fp);

#ifdef OUTPUT_META_TO_SCREEN
    printf("object_path : %s \n", object_path);
#endif

    //Write the first object name "/"
    //Since the "/" exits in HDF5 by default
    //There is no need to create/or record it
    int raw_data_index;
    fread(&raw_data_index, sizeof(uint32_t), 1, fp);

#ifdef DEBUG_OUTPUT
    printf("raw_data_index : 0x%.8X\n", raw_data_index);
#endif

    int number_of_properties;
    fread(&number_of_properties, sizeof(uint32_t), 1, fp);

#ifdef OUTPUT_META_TO_SCREEN
    printf("number_of_properties of the first object : %d \n", number_of_properties);
#endif

    //write "number_of_properties" to the "/" as property
    int properties_name_length, properties_value_length, properties_value_type, properties_value_type_code;
    char *properties_name;
    void *properties_value;
    int64_t seconds_v;
    uint64_t seconds_v_frac;
    time_t tt;
    char buff[40];

    //Iteratively read the properties of the "/"
    // and attach them to the "/"
    for (int ii = 0; ii < number_of_properties; ii++)
    {
        //Name Length and name
        fread(&properties_name_length, sizeof(uint32_t), 1, fp);
        properties_name = (char *)malloc(properties_name_length * sizeof(char) + 1);
        memset(properties_name, '\0', properties_name_length * sizeof(char) + 1);

        fread(properties_name, sizeof(char), properties_name_length, fp);
        //printf("properties_name : %s \n",  properties_name);

        //Value Type
        fread(&properties_value_type_code, sizeof(uint32_t), 1, fp);
        properties_value_type = properties_value_type_code;

        //Read "Value Length" only for string type
        if (properties_value_type == tdsTypeString)
        {
            fread(&properties_value_length, sizeof(uint32_t), 1, fp);
        }
        else
        {
            properties_value_length = 1;
        }
        // HDF5 Types
        //printf("properties_value_type : type index %d, type code: %d , properties_value_length : %d\n", properties_value_type,  properties_value_type_code, properties_value_length);
        switch (properties_value_type)
        {
        case tdsTypeI32:
            properties_value = malloc(properties_value_length * sizeof(int32_t));
            fread(properties_value, sizeof(int32_t), properties_value_length, fp);
#ifdef OUTPUT_META_TO_SCREEN
            printf("I32: %s : %d  \n", properties_name, *((int32_t *)properties_value));
#endif
            //attach_attribute_non_string(file_id, properties_name, H5T_STD_I32BE, H5T_NATIVE_INT, properties_value);
            free(properties_value);
            break;
        case tdsTypeU16:
            properties_value = malloc(properties_value_length * sizeof(uint16_t));
            fread(properties_value, sizeof(uint16_t), properties_value_length, fp);
            //attach_attribute_non_string(file_id, properties_name, H5T_STD_U16BE, H5T_NATIVE_USHORT, properties_value);
#ifdef OUTPUT_META_TO_SCREEN
            printf("U16: %s : %d  \n", properties_name, *((uint16_t *)properties_value));
#endif
            free(properties_value);
            break;
        case tdsTypeU32:
            properties_value = malloc(properties_value_length * sizeof(uint32_t));
            fread(properties_value, sizeof(uint32_t), properties_value_length, fp);
            //attach_attribute_non_string(file_id, properties_name, H5T_STD_U32BE, H5T_NATIVE_UINT, properties_value);
#ifdef OUTPUT_META_TO_SCREEN
            printf("U32: %s : %d  \n", properties_name, *((uint32_t *)properties_value));
#endif
            if (!strcmp(properties_name, TDMS_MLS))
            {
                MeasureLength = *((uint32_t *)properties_value);
                find_mls_flag = true;
            }
            free(properties_value);
            break;
        case tdsTypeSingleFloat:
            properties_value = malloc(properties_value_length * sizeof(float));
            fread(properties_value, sizeof(float), properties_value_length, fp);
            //attach_attribute_non_string(file_id, properties_name, H5T_IEEE_F32BE, H5T_NATIVE_FLOAT, properties_value);
#ifdef OUTPUT_META_TO_SCREEN
            printf("SingleFloat : %s : %f  \n", properties_name, *((float *)properties_value));
#endif
            free(properties_value);
            break;
        case tdsTypeDoubleFloat:
            properties_value = malloc(properties_value_length * sizeof(double));
            fread(properties_value, sizeof(double), properties_value_length, fp);
            //attach_attribute_non_string(file_id, properties_name, H5T_IEEE_F64BE, H5T_NATIVE_DOUBLE, properties_value);
#ifdef OUTPUT_META_TO_SCREEN
            printf("DoubleFloat: %s : %f  \n", properties_name, *((double *)properties_value));
#endif
            if (!strcmp(properties_name, TDMS_SRS))
            {
                SpatialResolution = *((double *)properties_value);
                find_srs_flag = true;
            }
            free(properties_value);
            break;
        case tdsTypeString:
            if (properties_value_length == 0)
                break;
            properties_value = malloc(properties_value_length * sizeof(char) + 1);
            memset(properties_value, '\0', properties_value_length * sizeof(char) + 1);
            fread(properties_value, sizeof(char), properties_value_length, fp);
            //attach_attribute_string(file_id, properties_name, properties_value, properties_value_length);
#ifdef OUTPUT_META_TO_SCREEN
            printf("String: %s : %s  \n", properties_name, properties_value);
#endif
            free(properties_value);
            break;
        case tdsTypeBoolean:
            properties_value = malloc(properties_value_length * sizeof(uint8_t));
            fread(properties_value, sizeof(uint8_t), properties_value_length, fp);
            uint32_t temp_bool_value;
            temp_bool_value = *((uint8_t *)properties_value);
            //attach_attribute_non_string(file_id, properties_name, H5T_STD_U32BE, H5T_NATIVE_UINT, &temp_bool_value);
#ifdef OUTPUT_META_TO_SCREEN
            printf("Boolean: %s : %d  \n", properties_name, *((uint8_t *)properties_value));
#endif
            free(properties_value);
            break;
        case tdsTypeTimeStamp:
            seconds_v_frac = 0;
            seconds_v = 0;
            fread(&seconds_v_frac, sizeof(uint64_t), properties_value_length, fp);
            fread(&seconds_v, sizeof(int64_t), properties_value_length, fp);
            tt = seconds_v - EPOCH_DIFF;
            //westSac_170728224610
            strftime(buff, 20, "%Y-%m-%d %H:%M:%S", gmtime(&tt));
            uint64_t time_sec_and_sec_fract[2];
            time_sec_and_sec_fract[0] = seconds_v;
            time_sec_and_sec_fract[1] = seconds_v_frac;
            char *temp_name;
            temp_name = (char *)malloc(sizeof(char) * strlen(properties_name) + 10);
            sprintf(temp_name, "%s(approx.)", properties_name);
            //attach_attribute_timestamp(file_id, properties_name, H5T_STD_U64BE, H5T_NATIVE_ULLONG, time_sec_and_sec_fract, temp_name, buff, 40);
#ifdef OUTPUT_META_TO_SCREEN
            printf("TimeStamp: %s : (%lld, %llu)  = (s, s_frac), %s  \n", properties_name, seconds_v, seconds_v_frac, buff);
#endif
            break;
        default:
            fseek(fp, properties_value_length, SEEK_CUR);
            printf("Found some un-common data type %d !\n", properties_value_type);
            return -1;
        }
        free(properties_name);

        if (find_srs_flag && find_mls_flag)
        {
            break;
        }
    }
    //fclose(fp);

    return (0);
}

/**
     * @brief Get the Populate data into MataMap
     * 
     * @return int 
     */
int EndpointTDMS::PopulateMetaMap()
{
    FILE *fp = this->GetFP();
    //Go to the standard hearder
    uint64_t nByte_metadata;
    fseek(fp, 20, SEEK_SET);
    fread(&nByte_metadata, sizeof(uint64_t), 1, fp);

#ifdef OUTPUT_META_TO_SCREEN
    printf("Size of customized metadata right after the fixed %d byte header : %lld\n", NBYTE_LEADIN, nByte_metadata);
#endif

    //Find the number of objects
    fseek(fp, NBYTE_LEADIN, SEEK_SET);
    int number_of_objects;
    fread(&number_of_objects, sizeof(uint32_t), 1, fp);

#ifdef OUTPUT_META_TO_SCREEN
    printf("Metadata: \n");
    printf("Number of objects : %d \n", number_of_objects);
#endif

    //Write number_of_objects to HDF5
    int length_of_first_object_path;
    fread(&length_of_first_object_path, sizeof(uint32_t), 1, fp);

#ifdef DEBUG_OUTPUT
    printf("length_of_first_object_path : %d \n", length_of_first_object_path);
#endif

    char object_path[1];
    fread(object_path, sizeof(char), 1, fp);

#ifdef OUTPUT_META_TO_SCREEN
    printf("object_path : %s \n", object_path);
#endif

    //Write the first object name "/"
    //Since the "/" exits in HDF5 by default
    //There is no need to create/or record it
    int raw_data_index;
    fread(&raw_data_index, sizeof(uint32_t), 1, fp);

#ifdef DEBUG_OUTPUT
    printf("raw_data_index : 0x%.8X\n", raw_data_index);
#endif

    int number_of_properties;
    fread(&number_of_properties, sizeof(uint32_t), 1, fp);

#ifdef OUTPUT_META_TO_SCREEN
    printf("number_of_properties of the first object : %d \n", number_of_properties);
#endif

    //write "number_of_properties" to the "/" as property
    int properties_name_length, properties_value_length, properties_value_type, properties_value_type_code;
    char *properties_name;
    void *properties_value;
    int64_t seconds_v;
    uint64_t seconds_v_frac;
    time_t tt;
    char buff[40];

    /**
     *  start to attach metadata, all as string
     */
    std::string ss; //properties_value_str
    for (int ii = 0; ii < number_of_properties; ii++)
    {
        //Name Length and name
        fread(&properties_name_length, sizeof(uint32_t), 1, fp);
        properties_name = (char *)malloc(properties_name_length * sizeof(char) + 1);
        memset(properties_name, '\0', properties_name_length * sizeof(char) + 1);

        fread(properties_name, sizeof(char), properties_name_length, fp);
        //printf("properties_name : %s \n",  properties_name);

        //Value Type
        fread(&properties_value_type_code, sizeof(uint32_t), 1, fp);
        properties_value_type = properties_value_type_code;

        //Read "Value Length" only for string type
        if (properties_value_type == tdsTypeString)
        {
            fread(&properties_value_length, sizeof(uint32_t), 1, fp);
        }
        else
        {
            properties_value_length = 1;
        }
        // HDF5 Types
        //printf("properties_value_type : type index %d, type code: %d , properties_value_length : %d\n", properties_value_type,  properties_value_type_code, properties_value_length);
        switch (properties_value_type)
        {
        case tdsTypeI32:
            properties_value = malloc(properties_value_length * sizeof(int32_t));
            fread(properties_value, sizeof(int32_t), properties_value_length, fp);
#ifdef OUTPUT_META_TO_SCREEN
            printf("I32: %s : %d \n", properties_name, *((int32_t *)properties_value));
#endif
            ss = tostr<int32_t>(*((int32_t *)properties_value));
            free(properties_value);
            break;
        case tdsTypeU16:
            properties_value = malloc(properties_value_length * sizeof(uint16_t));
            fread(properties_value, sizeof(uint16_t), properties_value_length, fp);
            ss = tostr<uint16_t>(*((uint16_t *)properties_value));
            //attach_attribute_string(dataset_id, properties_name, properties_value, properties_value_length);
#ifdef OUTPUT_META_TO_SCREEN
            printf("U16: %s : %d  \n", properties_name, *((uint16_t *)properties_value));
#endif
            free(properties_value);
            break;
        case tdsTypeU32:
            properties_value = malloc(properties_value_length * sizeof(uint32_t));
            fread(properties_value, sizeof(uint32_t), properties_value_length, fp);
            ss = tostr<uint32_t>(*((uint32_t *)properties_value));
            //attach_attribute_string(dataset_id, properties_name, properties_value, properties_value_length);
#ifdef OUTPUT_META_TO_SCREEN
            printf("U32: %s : %d  \n", properties_name, *((uint32_t *)properties_value));
#endif
            free(properties_value);
            break;
        case tdsTypeSingleFloat:
            properties_value = malloc(properties_value_length * sizeof(float));
            fread(properties_value, sizeof(float), properties_value_length, fp);
            ss = tostr<float>(*((float *)properties_value));
            //attach_attribute_string(dataset_id, properties_name, properties_value, properties_value_length);
#ifdef OUTPUT_META_TO_SCREEN
            printf("SingleFloat : %s : %f  \n", properties_name, *((float *)properties_value));
#endif
            free(properties_value);
            break;
        case tdsTypeDoubleFloat:
            properties_value = malloc(properties_value_length * sizeof(double));
            fread(properties_value, sizeof(double), properties_value_length, fp);
            ss = tostr<double>(*((double *)properties_value));
            //attach_attribute_string(dataset_id, properties_name, properties_value, properties_value_length);
#ifdef OUTPUT_META_TO_SCREEN
            printf("DoubleFloat: %s : %f  \n", properties_name, *((double *)properties_value));
#endif
            free(properties_value);
            break;
        case tdsTypeString:
            if (properties_value_length == 0)
                break;
            properties_value = malloc(properties_value_length * sizeof(char) + 1);
            memset(properties_value, '\0', properties_value_length * sizeof(char) + 1);
            fread(properties_value, sizeof(char), properties_value_length, fp);
            ss.assign((char *)properties_value);
            //attach_attribute_string(dataset_id, properties_name, properties_value, properties_value_length);
#ifdef OUTPUT_META_TO_SCREEN
            printf("String: %s : %s  \n", properties_name, properties_value);
#endif
            free(properties_value);
            break;
        case tdsTypeBoolean:
            properties_value = malloc(properties_value_length * sizeof(uint8_t));
            fread(properties_value, sizeof(uint8_t), properties_value_length, fp);
            uint32_t temp_bool_value;
            temp_bool_value = *((uint8_t *)properties_value);
            ss = tostr<uint32_t>(temp_bool_value);
            //attach_attribute_string(dataset_id, properties_name, properties_value, properties_value_length);
#ifdef OUTPUT_META_TO_SCREEN
            printf("Boolean: %s : %d  \n", properties_name, *((uint8_t *)properties_value));
#endif
            free(properties_value);
            break;
        case tdsTypeU8:
            properties_value = malloc(properties_value_length * sizeof(uint8_t));
            fread(properties_value, sizeof(uint8_t), properties_value_length, fp);
            uint32_t temp_u8_value;
            temp_u8_value = *((uint8_t *)properties_value);
            ss = tostr<uint32_t>(temp_u8_value);
            // attach_attribute_string(dataset_id, properties_name, properties_value, properties_value_length);
#ifdef OUTPUT_META_TO_SCREEN
            printf("Boolean: %s : %d  \n", properties_name, *((uint8_t *)properties_value));
#endif
            free(properties_value);
            break;
        case tdsTypeTimeStamp:
            seconds_v_frac = 0;
            seconds_v = 0;
            fread(&seconds_v_frac, sizeof(uint64_t), properties_value_length, fp);
            fread(&seconds_v, sizeof(int64_t), properties_value_length, fp);
            tt = seconds_v - EPOCH_DIFF;
            strftime(buff, 20, "%Y/%m/%d %H:%M:%S", gmtime(&tt));
            //uint64_t time_sec_and_sec_fract[2];
            //time_sec_and_sec_fract[0] = seconds_v;
            //time_sec_and_sec_fract[1] = seconds_v_frac;
            ss.assign(buff);
            ss = ss + "." + tostr<uint64_t>(seconds_v_frac) + " (UTC)";
            //2017/08/02 10:00:07.16594163686976509952
            //12/11/2020 18:41:32.602 (UTC)
            //"2017/08/02 10:00:03.346478104144773120 (UTC)"
            //char *temp_name;
            //temp_name = (char *)malloc(sizeof(char) * strlen(properties_name) + 10);
            //sprintf(temp_name, "%s(approx.)", properties_name);
            //attach_attribute_timestamp(dataset_id, properties_name, H5T_STD_U64BE, H5T_NATIVE_ULLONG, time_sec_and_sec_fract, temp_name, buff, 40);
#ifdef OUTPUT_META_TO_SCREEN
            printf("TimeStamp: %s : (%lld, %llu)  = (s, s_frac), %s  \n", properties_name, seconds_v, seconds_v_frac, buff);
#endif
            break;
        default:
            fseek(fp, properties_value_length, SEEK_CUR);
            printf("Found some un-common data type %d !\n", properties_value_type);
            return -1;
        }

        //attach_attribute_string(dataset_id, properties_name, (void *)(ss.c_str()), ss.length());
        std::string properties_name_str(properties_name);
        meta_map[properties_name_str] = ss;
        free(properties_name);
    }
}

/**
     * @brief Set the Attribute object
     *   Do not need to be pure virtual method
     * @param name 
     * @param data 
     * @return int 
     */
int EndpointTDMS::WriteAttribute(const std::string &name, const void *data, FTDataType data_type_p, const size_t &data_length_p)
{
    AU_EXIT("Not support WriteAttribute to TDMS file !");
}

/**
     * @brief Get the Attribute object
     *  Do not need to be pure virtual method
     * @param name 
     * @param data 
     * @return int 
     */
int EndpointTDMS::ReadAttribute(const std::string &name, void *data, FTDataType data_type_p, const size_t &data_length_p)
{
    ExtractMeta();

    std::map<std::string, std::string>::iterator it;
    it = meta_map.find(name);
    if (it == meta_map.end())
        return -1;
    std::string v = it->second;
    //data = v.data();
    memcpy(data, v.data(), v.size());

    return 0;
}

/**
     * @brief call a special operator on endpoint
     *        such as, enable collective I/O for HDF5
     *                 dump file from MEMORY to HDF5
     * @param opt_code, specially defined code 
     */
int EndpointTDMS::Control(int opt_code, std::vector<std::string> &parameter_v)
{
    switch (opt_code)
    {
    case OP_LIST_TAG:
        ReadAllAttributeName(parameter_v);
        break;
    default:
        break;
    }
    return 0;
}

/**
     * @brief Read all attribute name
     * 
     * @param attri_name 
     * @return int 
     */
int EndpointTDMS::ReadAllAttributeName(std::vector<std::string> &attr_name)
{
    ExtractMeta();

    attr_name.clear();
    for (std::map<std::string, std::string>::iterator it = meta_map.begin(); it != meta_map.end(); ++it)
    {
        attr_name.push_back(it->first);
    }

    return 0;
}

int EndpointTDMS::GetAttributeSize(const std::string &name, FTDataType data_type_p)
{
    ExtractMeta();
    std::map<std::string, std::string>::iterator it;
    it = meta_map.find(name);
    if (it == meta_map.end())
        return -1;

    std::string v = it->second;

    return v.size();
}
