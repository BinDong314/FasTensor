/**
   * @brief update the seek_offset
   * 
*/
#include "au_endpoint_tdms.h"

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

    FindMlSr(MeasureLength, SpatialResolution);

    //std::cout << "MeasureLength = " << MeasureLength << " , SpatialResolution = " << SpatialResolution << "\n";

    int nTrace = MeasureLength / SpatialResolution;

    fseek(fp, 0, SEEK_END);         // seek to end of file
    long int file_size = ftell(fp); // get current file pointer
    fseek(fp, 0, SEEK_SET);

    int nPoint = ((file_size - nByte_header) / NBYTE_PER_DATUM) / nTrace;

    //std::cout << "nPoint = " << nPoint << " , nTrace = " << nTrace << "\n";

    endpoint_size[0] = nTrace;
    endpoint_size[1] = nPoint;

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