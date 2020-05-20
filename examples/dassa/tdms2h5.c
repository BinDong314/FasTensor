#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "hdf5.h"
#include <sys/stat.h>
#include <getopt.h>
#include <dirent.h>
#include <mpi.h>
#include <assert.h>
#include <time.h>

//#define OUTPUT_META_TO_SCREEN 1
//#define DEBUG_OUTPUT 1

//Don't change below values
#define NBYTE_LEADIN 28
#define EPOCH_DIFF 2082844800 //seconds from epoch(1904, Mac HFS+ ) to epoch (1970, Unix)
#define NBYTE_PER_DATUM 2

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

void attach_attribute_non_string(hid_t obj_id, char *name, hid_t type_create, hid_t type_write, void *value);
void attach_attribute_string(hid_t obj_id, char *name, void *value_str, int value_str_length);
void attach_attribute_timestamp(hid_t obj_id, char *name, hid_t type_create, hid_t type_write, void *value_sec_and_secfrac, char *name2, void *time_as_str, int time_as_str_length);
int convert_file(char *filename_output, char *filename_input, int compression_flag);
void printf_help(char *cmd);
void transpose_data(int16_t *src, int16_t *dst, const int N, const int M);
int transpose_flag = 1;
int channel_togo = 1;
int start_channel = 0;
int subset_flag = 0;

int main(int argc, char *argv[])
{
   char filename_input[1024] = "./test.tdms";
   char filename_output[1024] = "./test.tdms.h5";
   int batch_flag = 0;
   int compression_flag = 0;
   int copt;
   char *res;
   while ((copt = getopt(argc, argv, "o:i:lhbcs:g:")) != -1)
      switch (copt)
      {
      case 'o':
         memset(filename_output, 0, sizeof(filename_output));
         strcpy(filename_output, optarg);
         break;
      case 'i':
         memset(filename_input, 0, sizeof(filename_output));
         strcpy(filename_input, optarg);
         break;
      case 'b':
         batch_flag = 1;
         break;
      case 'c':
         compression_flag = 1;
         break;
      case 'l':
         transpose_flag = 0;
         break;
      case 's':
         start_channel = atoi(optarg);
         break;
      case 'g':
         channel_togo = atoi(optarg);
         subset_flag = 1;
         break;
      case 'h':
         printf_help(argv[0]);
         exit(0);
      default:
         printf("Wrong option [%c] for %s \n", copt, argv[0]);
         printf_help(argv[0]);
         exit(-1);
         break;
      }
   if (batch_flag == 0)
   {
      convert_file(filename_output, filename_input, compression_flag);
   }
   else
   {
      //In batch mode, get file list
      int mpi_size, mpi_rank;
      MPI_Init(&argc, &argv);
      MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
      MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

      //Let all ranks to get file list in "filename_input"
      struct dirent **namelist;
      int namelist_length = scandir(filename_input, &namelist, 0, alphasort);

      int temp_index = 0, max_file_name_legnth = 0;
      if (namelist_length < 2)
      {
         printf("Input directory has not files to merge !");
         return 0;
      }
      else
      {
         while (temp_index < namelist_length)
         {
            if (strcmp(namelist[temp_index]->d_name, "..") == 0 || strcmp(namelist[temp_index]->d_name, ".") == 0)
            {
               free(namelist[temp_index]);
               namelist[temp_index] = NULL;
            }
            else
            {
               max_file_name_legnth = (strlen(namelist[temp_index]->d_name) > max_file_name_legnth ? strlen(namelist[temp_index]->d_name) : max_file_name_legnth);
            }
            temp_index++;
         }
      }
      MPI_Barrier(MPI_COMM_WORLD);
      //#ifdef DEBUG_OUTPUT
      if (mpi_rank == 0)
         printf("Count of files under %s :  %d \n", filename_input, namelist_length);
      //#endif

      int my_current_file_index = mpi_rank;
      int total_batch;
      if (namelist_length % mpi_size == 0)
      {
         total_batch = namelist_length / mpi_size;
      }
      else
      {
         total_batch = namelist_length / mpi_size + 1;
      }

      char *temp_output_filename = (char *)malloc(strlen(filename_output) + max_file_name_legnth + 5);
      memset(temp_output_filename, '\0', strlen(filename_output) + max_file_name_legnth + 5);

      char *temp_input_filename = (char *)malloc(strlen(filename_input) + max_file_name_legnth + 5);
      memset(temp_input_filename, '\0', strlen(filename_input) + max_file_name_legnth + 5);

      for (int batch_index = 0; batch_index < total_batch; batch_index++)
      {
         if (my_current_file_index < namelist_length && (namelist[my_current_file_index] != NULL))
         {
            memset(temp_output_filename, '\0', strlen(filename_output) + max_file_name_legnth + 5);
            sprintf(temp_output_filename, "%s/%s.h5", filename_output, namelist[my_current_file_index]->d_name);

            memset(temp_input_filename, '\0', strlen(filename_input) + max_file_name_legnth + 5);
            sprintf(temp_input_filename, "%s/%s", filename_input, namelist[my_current_file_index]->d_name);
            //#ifdef DEBUG_OUTPUT
            printf("mpi rank = %d, input file: %s, output file: %s \n", mpi_rank, temp_input_filename, temp_output_filename);
            //#endif
            convert_file(temp_output_filename, temp_input_filename, compression_flag);
         }
         my_current_file_index = my_current_file_index + mpi_size;
      }

      free(temp_output_filename);
      MPI_Finalize();
   }
   return 0;
}

int convert_file(char *filename_output, char *filename_input, int compression_flag)
{
   printf("filename_output= %s , filename_input = %s \n", filename_output, filename_input);

   //Create output file

   hid_t file_id, dataset_id, dataspace_id; // attribute_dataspace_id, attribute_id;
   file_id = H5Fcreate(filename_output, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

   //Opne input file
   FILE *fp;
   fp = fopen(filename_input, "r");

   //printf("");
   /*
    *Go to parse metdata, structure of the metadaa
    *   1,  28 bytes of leadin (fixed)
    *   2,  user-customized metadata (in below main structure)
    *       "/"  (root object)
    *       |    |
    *       |    | --  property #1
    *       |    | --  property #2
    *       |    | -- ....  
    *       | 
    *       | -- object #1
    *       |    |
    *       |    | --  property #1
    *       |     ....
    *       |
    *       | -- object #2   
    *       |    |
    *       |    |--  object #2.1
    *       |          |
    *       |          | --  property #1  
    *       ...     ...  
    * 
    *   3,  actual data
    *       .....   
    *   More information about metadata:
    *     http://www.ni.com/white-paper/5696/en/ 
    * 
    * In the output HDF5, it has same structure 
    *      -- Each object is a HDF5 group
    *      -- Each property is a HDF5 attributes (key-value) attached to the group
    */

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
   attach_attribute_non_string(file_id, (char *)"NumberOfObjects", H5T_STD_I32BE, H5T_NATIVE_INT, &number_of_objects);

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
   //attach_attribute_string(file_id, "ObjectPath", object_path, length_of_first_object_path);

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
   attach_attribute_non_string(file_id, (char *)"NumberOfProperties", H5T_STD_I32BE, H5T_NATIVE_INT, &number_of_properties);
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
         attach_attribute_non_string(file_id, properties_name, H5T_STD_I32BE, H5T_NATIVE_INT, properties_value);
         free(properties_value);
         break;
      case tdsTypeU16:
         properties_value = malloc(properties_value_length * sizeof(uint16_t));
         fread(properties_value, sizeof(uint16_t), properties_value_length, fp);
         attach_attribute_non_string(file_id, properties_name, H5T_STD_U16BE, H5T_NATIVE_USHORT, properties_value);
#ifdef OUTPUT_META_TO_SCREEN
         printf("U16: %s : %d  \n", properties_name, *((uint16_t *)properties_value));
#endif
         free(properties_value);
         break;
      case tdsTypeU32:
         properties_value = malloc(properties_value_length * sizeof(uint32_t));
         fread(properties_value, sizeof(uint32_t), properties_value_length, fp);
         attach_attribute_non_string(file_id, properties_name, H5T_STD_U32BE, H5T_NATIVE_UINT, properties_value);
#ifdef OUTPUT_META_TO_SCREEN
         printf("U32: %s : %d  \n", properties_name, *((uint32_t *)properties_value));
#endif
         free(properties_value);
         break;
      case tdsTypeSingleFloat:
         properties_value = malloc(properties_value_length * sizeof(float));
         fread(properties_value, sizeof(float), properties_value_length, fp);
         attach_attribute_non_string(file_id, properties_name, H5T_IEEE_F32BE, H5T_NATIVE_FLOAT, properties_value);
#ifdef OUTPUT_META_TO_SCREEN
         printf("SingleFloat : %s : %f  \n", properties_name, *((float *)properties_value));
#endif
         free(properties_value);
         break;
      case tdsTypeDoubleFloat:
         properties_value = malloc(properties_value_length * sizeof(double));
         fread(properties_value, sizeof(double), properties_value_length, fp);
         attach_attribute_non_string(file_id, properties_name, H5T_IEEE_F64BE, H5T_NATIVE_DOUBLE, properties_value);
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
         attach_attribute_string(file_id, properties_name, properties_value, properties_value_length);
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
         attach_attribute_non_string(file_id, properties_name, H5T_STD_U32BE, H5T_NATIVE_UINT, &temp_bool_value);
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
         attach_attribute_timestamp(file_id, properties_name, H5T_STD_U64BE, H5T_NATIVE_ULLONG, time_sec_and_sec_fract, temp_name, buff, 40);
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
   }

   int length_of_object_path;
   fread(&length_of_object_path, sizeof(uint32_t), 1, fp);
#ifdef DEBUG_OUTPUT
   printf("length_of_object_path : %d \n", length_of_object_path);
#endif
   char *object_path_str = (char *)malloc(sizeof(char) * length_of_object_path + 1);
   memset(object_path_str, '\0', sizeof(char) * length_of_object_path + 1);
   fread(object_path_str, sizeof(char), length_of_object_path, fp);

#ifdef OUTPUT_META_TO_SCREEN
   printf("object_path : %s \n", object_path_str);
#endif

   int raw_data_index_int;
   fread(&raw_data_index_int, sizeof(uint32_t), 1, fp);
#ifdef DEBUG_OUTPUT
   printf("raw_data_index_int : 0x%.8X\n", raw_data_index_int);
#endif

   //int number_of_properties;
   fread(&number_of_properties, sizeof(uint32_t), 1, fp);
#ifdef OUTPUT_META_TO_SCREEN
   printf("number_of_properties : %d \n", number_of_properties);
#endif

   //Create the group "/Measurement"
   hid_t group_id = H5Gcreate(file_id, object_path_str, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
   assert(group_id >= 0);
   free(object_path_str);
   //H5Fflush(file_id, H5F_SCOPE_GLOBAL);
   //H5Grefresh(group_id);

   //Create all sub-groups for left objects

   int index_data_type, array_dimensions;
   uint64_t Number_of_values;
   //int debug_print_index = 0;
   for (int jj = 0; jj < number_of_objects - 2; jj++)
   {
      fread(&length_of_object_path, sizeof(uint32_t), 1, fp);
#ifdef DEBUG_OUTPUT
      printf("length_of_object_path : %d \n", length_of_object_path);
#endif
      object_path_str = (char *)malloc(sizeof(char) * length_of_object_path + 1);
      memset(object_path_str, '\0', sizeof(char) * length_of_object_path + 1);
      fread(object_path_str, sizeof(char), length_of_object_path, fp);
#ifdef OUTPUT_META_TO_SCREEN
      printf("object_path : %s \n", object_path_str);
#endif

      hid_t group_id_temp = H5Gcreate(group_id, object_path_str, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      assert(group_id_temp >= 0);
      //H5Grefresh(group_id_temp);
      free(object_path_str);
      fread(&raw_data_index_int, sizeof(uint32_t), 1, fp);
#ifdef DEBUG_OUTPUT
      printf("raw_data_index_int : 0x%.8X, lenght of indiex %d \n", raw_data_index_int, raw_data_index_int);
#endif
      fread(&index_data_type, sizeof(uint32_t), 1, fp);
#ifdef OUTPUT_META_TO_SCREEN
      printf("index_data_type : %d  tdsTypeI16 = %d \n", index_data_type, tdsTypeI16);
#endif
      fread(&array_dimensions, sizeof(uint32_t), 1, fp);
#ifdef OUTPUT_META_TO_SCREEN
      printf("Array dimension  : %d   \n", array_dimensions);
#endif
      fread(&Number_of_values, sizeof(uint64_t), 1, fp);
#ifdef OUTPUT_META_TO_SCREEN
      printf("Number of raw data values  : %llu   \n", Number_of_values);
#endif
      //int number_of_properties;
      fread(&number_of_properties, sizeof(uint32_t), 1, fp);
#ifdef OUTPUT_META_TO_SCREEN
      printf("number_of_properties : %d \n", number_of_properties);
#endif
      attach_attribute_non_string(group_id_temp, (char *)"DataTypeCode", H5T_STD_U32BE, H5T_NATIVE_UINT, &index_data_type);
      attach_attribute_non_string(group_id_temp, (char *)"ArrayDimension ", H5T_STD_U32BE, H5T_NATIVE_UINT, &array_dimensions);
      attach_attribute_non_string(group_id_temp, (char *)"NumberOfRawDataValues ", H5T_STD_U64BE, H5T_NATIVE_ULONG, &Number_of_values);
      attach_attribute_non_string(group_id_temp, (char *)"NumberOfProperties", H5T_STD_U32BE, H5T_NATIVE_INT, &number_of_properties);

      H5Gclose(group_id_temp);
   }

   H5Gclose(group_id);

   /*
    *    Start to read and store the data  
    */

   //Find the size of the data from previously created attributes
   unsigned int MeasureLength;
   hid_t read_attr_MeasureLength_id = H5Aopen(file_id, "MeasureLength[m]", H5P_DEFAULT);
   H5Aread(read_attr_MeasureLength_id, H5T_NATIVE_UINT, &MeasureLength);

   double SpatialResolution;
   hid_t read_attr_SpatialResolution_id = H5Aopen(file_id, "SpatialResolution[m]", H5P_DEFAULT);
   H5Aread(read_attr_SpatialResolution_id, H5T_NATIVE_DOUBLE, &SpatialResolution);

#ifdef DEBUG_OUTPUT
   printf("MeasureLength : %d,  SpatialResolution :  %f \n", MeasureLength, SpatialResolution);
#endif

   int nTrace = MeasureLength / SpatialResolution;

   H5Aclose(read_attr_MeasureLength_id);
   H5Aclose(read_attr_SpatialResolution_id);

   fseek(fp, 0, SEEK_END);         // seek to end of file
   long int file_size = ftell(fp); // get current file pointer
   fseek(fp, 0, SEEK_SET);

   int nByte_header = nByte_metadata + NBYTE_LEADIN;
#ifdef DEBUG_OUTPUT
   printf("nByte_header before actual data =  %d, file size = %ld \n", nByte_header, file_size);
#endif

   int nPoint = ((file_size - nByte_header) / NBYTE_PER_DATUM) / nTrace;

#ifdef OUTPUT_META_TO_SCREEN
   printf("Dataset size = (%d, %d) (Time, Channel) \n", nPoint, nTrace);
#endif

   //Start to read the data and store it into HDF5
   int16_t *data, *data_transposed, *data_subset;
   data = (int16_t *)malloc(sizeof(int16_t) * nTrace * nPoint);

   time_t start, end;
   double dif;
   time(&start);

   fseek(fp, nByte_header, SEEK_SET);
   fread(data, sizeof(int16_t), nTrace * nPoint, fp);

#ifdef DEBUG_OUTPUT
   int i, j;
   for (i = 0; i < 5; i++)
   {
      for (j = 0; j < 5; j++)
      {
         printf(" %d ", data[i * nTrace + j]);
      }
      printf("\n");
   }
#endif

   hsize_t dims[2];

   int nTrace_before_subset = nTrace;
   if (subset_flag)
   {
      printf("Subset: channel %d -> channel %d (non included)\n", start_channel, start_channel + channel_togo);
      nTrace = channel_togo;
      data_subset = (int16_t *)malloc(sizeof(int16_t) * nTrace * nPoint);
      for (int i = 0; i < nPoint; i++)
      {
         for (int j = start_channel; j < channel_togo + start_channel; j++)
         {
            data_subset[i * nTrace + j - start_channel] = data[i * nTrace_before_subset + j];
         }
      }
      free(data);
      data = data_subset;
   }

   if (transpose_flag)
   {
      dims[0] = nTrace;
      dims[1] = nPoint;
   }
   else
   {
      dims[0] = nPoint;
      dims[1] = nTrace;
   }
   dataspace_id = H5Screate_simple(2, dims, NULL);
   hid_t dataset_plist_id = H5Pcreate(H5P_DATASET_CREATE);

   hsize_t cdims[2];
   if (compression_flag == 1)
   {
      if (transpose_flag)
      {
         cdims[0] = nTrace / 16;
         cdims[1] = nPoint / 100;
      }
      else
      {
         cdims[0] = nPoint / 100;
         cdims[1] = nTrace / 16;
      }

      H5Pset_chunk(dataset_plist_id, 2, cdims);
      H5Pset_deflate(dataset_plist_id, 6);
   }

   if (transpose_flag)
   {
      data_transposed = (int16_t *)malloc(sizeof(int16_t) * nTrace * nPoint);
      transpose_data(data, data_transposed, nPoint, nTrace);
   }

   if (transpose_flag)
   {
      ///DataTC = /DataChannelTime
      dataset_id = H5Dcreate(file_id, "/DataCT", H5T_STD_I16LE, dataspace_id, H5P_DEFAULT, dataset_plist_id, H5P_DEFAULT);
      H5Dwrite(dataset_id, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data_transposed);
   }
   else
   {
      ///DataTC = /DataTimeChannel
      dataset_id = H5Dcreate(file_id, "/DataTC", H5T_STD_I16LE, dataspace_id, H5P_DEFAULT, dataset_plist_id, H5P_DEFAULT);
      H5Dwrite(dataset_id, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
   }
   H5Pclose(dataset_plist_id);
   H5Sclose(dataspace_id);
   H5Dclose(dataset_id);
   free(data);
   if (transpose_flag)
      free(data_transposed);

   time(&end);
   dif = difftime(end, start);
   printf("IO took %.5lf seconds to run.\n", dif);

   /*
   * Store all header of TDMS as a H5T_OPAQUE type
   * In case, in future, we want to rebuid TDMS file  
   */

   hid_t metadata_dtype = H5Tcreate(H5T_OPAQUE, 1);
   H5Tset_tag(metadata_dtype, "RAW Metadata from TDMS file as OPAQUE type");

   hsize_t metadata_dims[1];
   metadata_dims[0] = nByte_header;
   hid_t metadata_space = H5Screate_simple(1, metadata_dims, NULL);

   void *metadata_buf = malloc(nByte_header);
   fseek(fp, 0, SEEK_SET);
   fread(metadata_buf, 1, nByte_header, fp);

   hid_t metadata_dset = H5Dcreate(file_id, "/RawMetadataTDMS", metadata_dtype, metadata_space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
   H5Dwrite(metadata_dset, metadata_dtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, metadata_buf);

   H5Sclose(metadata_space);
   H5Dclose(metadata_dset);
   free(metadata_buf);
   H5Fflush(file_id, H5F_SCOPE_GLOBAL);
   H5Fclose(file_id);
   fclose(fp);

   return (0);
}
void attach_attribute_string(hid_t obj_id, char *name, void *value_str, int value_str_length)
{
   hid_t attribute_space_scalar = H5Screate(H5S_SCALAR);
   hid_t attribute_str_type = H5Tcopy(H5T_C_S1);
   H5Tset_size(attribute_str_type, value_str_length);
   H5Tset_strpad(attribute_str_type, H5T_STR_NULLTERM);

   hid_t attribute_id = H5Acreate(obj_id, name, attribute_str_type, attribute_space_scalar, H5P_DEFAULT, H5P_DEFAULT);
   H5Awrite(attribute_id, attribute_str_type, value_str);

   H5Aclose(attribute_id);
   H5Sclose(attribute_space_scalar);
   H5Tclose(attribute_str_type);
}

void attach_attribute_non_string(hid_t obj_id, char *name, hid_t type_create, hid_t type_write, void *value)
{
   hsize_t attribute_dims = 1;
   hid_t attribute_dataspace_id = H5Screate_simple(1, &attribute_dims, NULL);
   hid_t attribute_id = H5Acreate(obj_id, name, type_create, attribute_dataspace_id, H5P_DEFAULT, H5P_DEFAULT);
   H5Awrite(attribute_id, type_write, value);
   H5Sclose(attribute_dataspace_id);
   H5Aclose(attribute_id);
}

void attach_attribute_timestamp(hid_t obj_id, char *name, hid_t type_create, hid_t type_write, void *value_sec_and_secfrac, char *name2, void *time_as_str, int time_as_str_length)
{
   hsize_t attribute_dims = 2;
   hid_t attribute_dataspace_id = H5Screate_simple(1, &attribute_dims, NULL);
   hid_t attribute_id = H5Acreate(obj_id, name, type_create, attribute_dataspace_id, H5P_DEFAULT, H5P_DEFAULT);
   H5Awrite(attribute_id, type_write, value_sec_and_secfrac);
   H5Sclose(attribute_dataspace_id);
   H5Aclose(attribute_id);

   //Create String format
   hid_t attribute_space_scalar = H5Screate(H5S_SCALAR);
   hid_t attribute_str_type = H5Tcopy(H5T_C_S1);
   H5Tset_size(attribute_str_type, time_as_str_length);
   H5Tset_strpad(attribute_str_type, H5T_STR_NULLTERM);

   hid_t attribute_id_str = H5Acreate(obj_id, name2, attribute_str_type, attribute_space_scalar, H5P_DEFAULT, H5P_DEFAULT);
   H5Awrite(attribute_id_str, attribute_str_type, time_as_str);

   H5Aclose(attribute_id_str);
   H5Sclose(attribute_space_scalar);
   H5Tclose(attribute_str_type);
}

void transpose_data(int16_t *src, int16_t *dst, const int N, const int M)
{
   for (int n = 0; n < N * M; n++)
   {
      int i = n / N;
      int j = n % N;
      dst[n] = src[M * j + i];
   }
}
void printf_help(char *cmd)
{
   char *msg = (char *)"Usage: %s [OPTION] \n\
          -h help (--help)\n\
          -i input  file/directory for TDMS file(s) to convert\n\
          -o output file/directory for HDF5 file(s)\n\
          -b batch mode (i.e., both input and output are directory)\n\
          -p parallel conversion on MPI\n\
          -l use column order (by default tranposed to row order as [Channel]x[[Time])\n\
          -s start channel (zero based) \n\
          -g counts of channels to go \n\
          Example: %s -i test.tdms -o test.tdms.h5\n";
   fprintf(stdout, msg, cmd, cmd);
}
