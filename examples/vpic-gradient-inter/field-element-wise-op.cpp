// overload_array.cpp
// overloading the c++ array subscript operator []
//http://neondataskills.org/HDF5/TimeSeries-Data-In-HDF5-Using-R/

#include <iostream>
#include <stdarg.h>
#include <vector>
#include <stdlib.h>
#include <math.h> /* ceil  and floor*/
#include <cstring>

#include "array_udf.h"

//Some help funcitons
using namespace std;
void find_value_3D(std::vector<int> &value, char *value_str, int dims);
void print_help();
void create_xdmf_file(char *xml_output_file, char *dimensions, char *origin_dxdydz_dimensions, char *origin, char *dxdydz, char *attribute_name1, char *attribute_name2, char *attribute_name3, char *attribute_name4, char *hdf5_file_dataset1, char *hdf5_file_dataset2, char *hdf5_file_dataset3, char *hdf5_file_dataset4);

//Define a compund data strcuture
struct CompundPoint
{
  //Field dump
  float ex;
  float ey;
  float ez;
  float bx;
  float by;
  float bz;
  //electron current
  float ejx;
  float ejy;
  float ejz;
  //ion current vector
  float ijx;
  float ijy;
  float ijz;

  //Some help functions.
  friend std::ostream &operator<<(std::ostream &os, CompundPoint &p) //test
  {
    return os;
  }
  friend bool operator==(CompundPoint &lp, CompundPoint &rp)
  {
    return (lp.ex == rp.ex && lp.ey == rp.ey && lp.ez == rp.ez && lp.bx == rp.bx && lp.by == rp.by && lp.bz == rp.bz && lp.ejx == rp.ejx && lp.ejy == rp.ejy && lp.ejz == rp.ejz && lp.ijx == rp.ijx && lp.ijy == rp.ijy && lp.ijz == rp.ijz);
  }

  void set_value(int index, float value)
  {
    switch (index)
    {
    case 0:
      ex = value;
      break;
    case 1:
      ey = value;
      break;
    case 2:
      ez = value;
      break;
    case 3:
      bx = value;
      break;
    case 4:
      by = value;
      break;
    case 5:
      bz = value;
      break;
    case 6:
      ejx = value;
      break;
    case 7:
      ejy = value;
      break;
    case 8:
      ejz = value;
      break;
    case 9:
      ijx = value;
      break;
    case 10:
      ijy = value;
      break;
    case 11:
      ijz = value;
      break;
    default:
      printf("The object only has 12 variables !\n");
      break;
    }
  }
};

// Section 7.1 Total Current
inline float totalCurrent_UDF(const Stencil<CompundPoint> &hp)
{
  CompundPoint hpt = hp(0, 0, 0);
  return (hpt.ejx + hpt.ijx) + (hpt.ejy + hpt.ijy) + (hpt.ejz + hpt.ijz);
}

// Section 7.2  Absolute Vlues of Current
inline float absJ_UDF(const Stencil<CompundPoint> &hp)
{
  CompundPoint hpt = hp(0, 0, 0);
  return sqrt((hpt.ejx + hpt.ijx) * (hpt.ejx + hpt.ijx) + (hpt.ejy + hpt.ijy) * (hpt.ejy + hpt.ijy) + (hpt.ejz + hpt.ijz) * (hpt.ejz + hpt.ijz));
}

//Section 7.3 RMS Fluctions of absJ
//It follows by a reduce in main()
float local_sum = 0;
unsigned long local_n = 0;
inline float squared_absJ_UDF(const Stencil<float> &hp)
{
  //printf(" jx,jy,jz = (%f, %f, %f) \n", hpt.jx,  hpt.jy,  hpt.jz);
  local_sum = local_sum + hp(0) * hp(0);
  local_n++;
  return 0; //Actually, we return nothing here
}

//Section 7.4 Work done be Paticles
inline float DotEJ_UDF(const Stencil<CompundPoint> &hp)
{
  CompundPoint hpt = hp(0, 0, 0);
  return hpt.ex * (hpt.ejx + hpt.ijx) + hpt.ey * (hpt.ejy + hpt.ijy) + hpt.ez * (hpt.ejz + hpt.ijz);
}

//Section 7.5 Parallel Electric Field
// Simple version Eq: 27
inline float Eparallel_UDF(const Stencil<CompundPoint> &hp)
{
  CompundPoint hpt = hp(0, 0, 0);
  return (hpt.ex * hpt.bx + hpt.ey * hpt.by + hpt.ez * hpt.bz) / sqrt(hpt.bx * hpt.bx + hpt.by * hpt.by + hpt.bz * hpt.bz);
}

//Section 7.5 Parallel Electric Field
// Simple version Eq: 28 - 34
inline float Eparallel_UDF_V2(const Stencil<CompundPoint> &hp)
{
  //Eq 28
  float ex_p = ((hp(0, 0, 0).ex + hp(0, 1, 0).ex + hp(0, 0, 1).ex + hp(0, 1, 1).ex)) / 4.0;
  //Eq 29
  float ey_p = ((hp(0, 0, 0).ey + hp(1, 0, 0).ey + hp(0, 0, 1).ey + hp(1, 0, 1).ey)) / 4.0;
  //Eq 30
  float ez_p = ((hp(0, 0, 0).ez + hp(1, 0, 0).ez + hp(0, 1, 0).ez + hp(1, 1, 0).ez)) / 4.0;

  //Eq 31
  float bx_p = (hp(0, 0, 0).bx + hp(1, 0, 0).bx) / 2.0;
  //Eq 32
  float by_p = (hp(0, 0, 0).by + hp(0, 1, 0).by) / 2.0;
  //Eq 33
  float bz_p = (hp(0, 0, 0).bz + hp(0, 0, 1).bz) / 2.0;

  return (ex_p * bx_p + ey_p * by_p + ez_p * bz_p) / sqrt(bx_p * bx_p + by_p * by_p + bz_p * bz_p);
}

int main(int argc, char *argv[])
{

  char i_file_field[1024] = "./test-file/fields_50.h5";
  char i_file_electron[1024] = "./test-file/hydro_electron_50.h5";
  char i_file_ion[1024] = "./test-file/hydro_ion_50.h5";

  char o_file[1024] = "./test-file/fields_50.h5"; //  "./fields_50_field_element.h5";
  char group[64] = "/Timestep_50";

  //Split array into chunks for parallel processing
  std::vector<int> chunk_size(3);
  std::vector<int> overlap_size(3);
  chunk_size[0] = 32;
  chunk_size[1] = 1;
  chunk_size[2] = 32;
  overlap_size[0] = 0;
  overlap_size[1] = 0;
  overlap_size[2] = 0;

  int copt;
  while ((copt = getopt(argc, argv, "f:e:i:r:g:c:o:h")) != -1)
    switch (copt)
    {
    case 'f':
      memset(i_file_field, 0, sizeof(i_file_field));
      strcpy(i_file_field, optarg);
      break;
    case 'e':
      memset(i_file_electron, 0, sizeof(i_file_electron));
      strcpy(i_file_electron, optarg);
      break;
    case 'i':
      memset(i_file_ion, 0, sizeof(i_file_ion));
      strcpy(i_file_ion, optarg);
      break;
    case 'r':
      memset(o_file, 0, sizeof(o_file));
      strcpy(o_file, optarg);
      break;
    case 'g':
      memset(group, 0, sizeof(group));
      strcpy(group, optarg);
      break;
    case 'c':
      find_value_3D(chunk_size, optarg, 3);
      break;
    case 'o':
      find_value_3D(overlap_size, optarg, 3);
      break;
    case 'h':
      print_help();
      exit(0);
    default:
      break;
    }

  MPI_Init(&argc, &argv);

  //Orginal data sets
  Array<float> *EX = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, "ex", chunk_size, overlap_size);
  Array<float> *EY = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, "ey", chunk_size, overlap_size);
  Array<float> *EZ = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, "ez", chunk_size, overlap_size);

  Array<float> *BX = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, "cbx", chunk_size, overlap_size);
  Array<float> *BY = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, "cby", chunk_size, overlap_size);
  Array<float> *BZ = new Array<float>(AU_NVS, AU_HDF5, i_file_field, group, "cbz", chunk_size, overlap_size);

  Array<float> *EJX = new Array<float>(AU_NVS, AU_HDF5, i_file_electron, group, "jx", chunk_size, overlap_size);
  Array<float> *EJY = new Array<float>(AU_NVS, AU_HDF5, i_file_electron, group, "jy", chunk_size, overlap_size);
  Array<float> *EJZ = new Array<float>(AU_NVS, AU_HDF5, i_file_electron, group, "jz", chunk_size, overlap_size);

  Array<float> *IJX = new Array<float>(AU_NVS, AU_HDF5, i_file_ion, group, "jx", chunk_size, overlap_size);
  Array<float> *IJY = new Array<float>(AU_NVS, AU_HDF5, i_file_ion, group, "jy", chunk_size, overlap_size);
  Array<float> *IJZ = new Array<float>(AU_NVS, AU_HDF5, i_file_ion, group, "jz", chunk_size, overlap_size);

  //Create a compound data strucutre to include JX/JY/JZ/....
  Array<CompundPoint, float> *XYZ = new Array<CompundPoint, float>(AU_VIRTUAL);
  XYZ->SetAttributes(EX, EY, EZ, BX, BY, BZ, EJX, EJY, EJZ, IJX, IJY, IJZ);
  char dsetR[1024];

  sprintf(dsetR, "%s/%s", group, "totalCurrent");
  Array<float> *R1 = new Array<float>(AU_COMPUTED, AU_HDF5, o_file, group, dsetR, chunk_size, overlap_size);
  XYZ->Apply(totalCurrent_UDF, R1);
  delete R1;

  MPI_Barrier(MPI_COMM_WORLD);

  sprintf(dsetR, "%s/%s", group, "absJ");
  Array<float> *R2 = new Array<float>(AU_COMPUTED, AU_HDF5, o_file, group, dsetR, chunk_size, overlap_size);
  XYZ->Apply(absJ_UDF, R2);
  delete R2;

  MPI_Barrier(MPI_COMM_WORLD);

  sprintf(dsetR, "%s/%s", group, "DotEJ");
  Array<float> *R3 = new Array<float>(AU_COMPUTED, AU_HDF5, o_file, group, dsetR, chunk_size, overlap_size);
  XYZ->Apply(DotEJ_UDF, R3);
  delete R3;

  MPI_Barrier(MPI_COMM_WORLD);

  sprintf(dsetR, "%s/%s", group, "Eparallel");
  Array<float> *R4 = new Array<float>(AU_COMPUTED, AU_HDF5, o_file, group, dsetR, chunk_size, overlap_size);
  XYZ->Apply(Eparallel_UDF, R4);
  delete R4;

  MPI_Barrier(MPI_COMM_WORLD);

  sprintf(dsetR, "%s/%s", group, "EparallelV2");
  Array<float> *R5 = new Array<float>(AU_COMPUTED, AU_HDF5, o_file, group, dsetR, chunk_size, overlap_size);
  XYZ->Apply(Eparallel_UDF_V2, R5);
  delete R5;

  //Compute   squared_absJ
  int mpi_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  Array<float> *absJ = new Array<float>(AU_NVS, AU_HDF5, o_file, group, "/Timestep_50/absJ", chunk_size, overlap_size);
  absJ->Apply(squared_absJ_UDF, NULL);
  delete absJ;
  float global_sum, final_squared_absJ;
  MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
  unsigned long global_n;
  MPI_Reduce(&local_n, &global_n, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
  if (mpi_rank == 0)
  {
    global_sum = global_sum / (float)global_n;
    final_squared_absJ = sqrt(global_sum);

    printf("final_squared_absJ = %f \n", final_squared_absJ);

    //Manually add  final_squared_absJ to o_file
    hid_t squared_absJ_file_id = H5Fopen(o_file, H5F_ACC_RDWR, H5P_DEFAULT);
    hsize_t dims[1];
    dims[0] = 1;
    hid_t dataspace_id = H5Screate_simple(1, dims, NULL);
    hid_t group_id = H5Gopen(squared_absJ_file_id, group, H5P_DEFAULT);
    hid_t dataset_id;
    if (H5Lexists(group_id, "squared_absJ", H5P_DEFAULT) == 0)
    {
      dataset_id = H5Dcreate(group_id, "squared_absJ", H5T_IEEE_F32BE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    }
    else
    {
      dataset_id = H5Dopen(group_id, "squared_absJ", H5P_DEFAULT);
    }
    H5Dwrite(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &final_squared_absJ);
    H5Dclose(dataset_id);
    H5Gclose(group_id);
    H5Sclose(dataspace_id);
    H5Fclose(squared_absJ_file_id);

    //Manually find metadata from input file
    float attr_data[2][3];
    hid_t attr_file_id = H5Fopen(i_file_field, H5F_ACC_RDONLY, H5P_DEFAULT);
    hid_t attr_id = H5Aopen(attr_file_id, "VPIC-ArrayUDF-GEO", H5P_DEFAULT);
    H5Aread(attr_id, H5T_NATIVE_FLOAT, &attr_data);
    H5Aclose(attr_id);

    std::vector<unsigned long long> output_array_size;
    output_array_size.resize(3);
    output_array_size = EX->GetDimSize();
    char xml_output_file[1024], hdf5_file_dataset1[1024], hdf5_file_dataset2[1024], hdf5_file_dataset3[1024], hdf5_file_dataset4[1024], hdf5_file_dataset5[1024];
    char dimensions[128], origin_dxdydz_dimensions[128], origin[128], dxdydz[128], attribute_name1[128], attribute_name2[128], attribute_name3[128], attribute_name4[128];

    sprintf(xml_output_file, "%s.xmf", o_file);
    sprintf(hdf5_file_dataset1, "%s:%s/%s", o_file, group, "totalCurrent");
    sprintf(hdf5_file_dataset2, "%s:%s/%s", o_file, group, "absJ");
    sprintf(hdf5_file_dataset3, "%s:%s/%s", o_file, group, "DotEJ");
    sprintf(hdf5_file_dataset4, "%s:%s/%s", o_file, group, "Eparallel");

    sprintf(dimensions, "%lld %lld %lld", output_array_size[0], output_array_size[1], output_array_size[2]);
    sprintf(origin_dxdydz_dimensions, "%d", 3);
    sprintf(origin, "%f %f %f", attr_data[0][0], attr_data[0][1], attr_data[0][2]);
    sprintf(dxdydz, "%f %f %f", attr_data[1][0], attr_data[1][1], attr_data[1][2]);
    sprintf(attribute_name1, "%s", "totalCurrent");
    sprintf(attribute_name2, "%s", "absJ");
    sprintf(attribute_name3, "%s", "DotEJ");
    sprintf(attribute_name4, "%s", "Eparallel");

    create_xdmf_file(xml_output_file, dimensions, origin_dxdydz_dimensions, origin, dxdydz, attribute_name1, attribute_name2, attribute_name3, attribute_name4, hdf5_file_dataset1, hdf5_file_dataset2, hdf5_file_dataset3, hdf5_file_dataset4);
  }

  delete EX;
  delete EY;
  delete EZ;
  delete BX;
  delete BY;
  delete BZ;
  delete EJX;
  delete EJY;
  delete EJZ;
  delete IJX;
  delete IJY;
  delete IJZ;
  delete XYZ;

  MPI_Finalize();

  return 0;
}

const char *xml_body = "<?xml version=\"1.0\"?> \n \
<!DOCTYPE Xdmf SYSTEM \"Xdmf.dtd\" []> \n \
<Xdmf xmlns:xi=\"http://www.w3.org/2001/XInclude\" Version=\"2.0\"> \n \
\t<Domain> \n \
\t\t<Grid GridType=\"Uniform\" Name=\"MESH\"> \n \
\t\t\t<Topology Dimensions=\" %s \" TopologyType=\"3DCoRectMesh\" name=\"topo\"/> \n\
\t\t\t<Geometry Type=\"ORIGIN_DXDYDZ\" name=\"geo\"> \n\
\t\t\t\t<!-- Origin -->  \n\
\t\t\t\t<DataItem Dimensions=\" %s \" Format=\"XML\"> %s </DataItem> \n\
\t\t\t\t<!-- DxDyDz -->  \n\
\t\t\t\t<DataItem Dimensions=\" %s \" Format=\"XML\"> %s </DataItem> \n\
\t\t\t</Geometry> \n\
\t\t\t<Attribute AttributeType=\"Scalar\" Center=\"Node\" Name=\" %s \"> \n\
\t\t\t\t<DataItem  Dimensions=\" %s \" DataType=\"Float\" Precision=\"4\" Format=\"HDF\"> \n\
\t\t\t\t %s \n\
\t\t\t\t</DataItem>  \n\
\t\t\t</Attribute>  \n\
\t\t\t<Attribute AttributeType=\"Scalar\" Center=\"Node\" Name=\" %s \"> \n\
\t\t\t\t<DataItem  Dimensions=\" %s \" DataType=\"Float\" Precision=\"4\" Format=\"HDF\"> \n\
\t\t\t\t %s \n\
\t\t\t\t</DataItem>  \n\
\t\t\t</Attribute>  \n\
\t\t\t<Attribute AttributeType=\"Scalar\" Center=\"Node\" Name=\" %s \"> \n\
\t\t\t\t<DataItem  Dimensions=\" %s \" DataType=\"Float\" Precision=\"4\" Format=\"HDF\"> \n\
\t\t\t\t %s \n\
\t\t\t\t</DataItem>  \n\
\t\t\t</Attribute>  \n\
\t\t\t<Attribute AttributeType=\"Scalar\" Center=\"Node\" Name=\" %s \"> \n\
\t\t\t\t<DataItem  Dimensions=\" %s \" DataType=\"Float\" Precision=\"4\" Format=\"HDF\"> \n\
\t\t\t\t %s \n\
\t\t\t\t</DataItem>  \n\
\t\t\t</Attribute>  \n\
\t\t</Grid>  \n\
\t</Domain> \n\
</Xdmf>";

void create_xdmf_file(char *xml_output_file, char *dimensions, char *origin_dxdydz_dimensions, char *origin, char *dxdydz,
                      char *attribute_name1, char *attribute_name2, char *attribute_name3, char *attribute_name4,
                      char *hdf5_file_dataset1, char *hdf5_file_dataset2, char *hdf5_file_dataset3, char *hdf5_file_dataset4)
{
  FILE *fp;
  fp = fopen(xml_output_file, "w");
  fprintf(fp, xml_body, dimensions, origin_dxdydz_dimensions,
          origin, origin_dxdydz_dimensions, dxdydz,
          attribute_name1, dimensions, hdf5_file_dataset1,
          attribute_name2, dimensions, hdf5_file_dataset2,
          attribute_name3, dimensions, hdf5_file_dataset3,
          attribute_name4, dimensions, hdf5_file_dataset4);
  fclose(fp);
}

void find_value_3D(std::vector<int> &value, char *value_str, int dims)
{
  int i;
  char *pch;
  char temp[1024];

  if (dims == 1)
  {
    value[0] = atoi(value_str);
  }
  else
  {
    strcpy(temp, value_str);
    pch = strtok(temp, ",");
    i = 0;
    while (pch != NULL)
    {
      //printf("%s \n", pch);
      value[i] = atoi(pch);
      pch = strtok(NULL, ",");
      i++;
    }
  }
}

void print_help()
{
  const char *msg = "Usage: %s [OPTION] \n\
      	  -h help \n\
          -f field file \n\
          -e hydro electron file \n\
          -i hydro ion file \n\
          -r result file \n\
          -g group(or step) name \n\
          -c chunk size string (3D, e.g. \"32,1,32\") \n\
          -o overlap (ghost zone) size (3D, e.g. \"0,0,0\"). \n\
          Example: mpirun -n 2 ./element-wise  -f ./test-file/fields_50.h5 -e ./test-file/hydro_electron_50.h5  -i ./test-file/hydro_ion_50.h5 -r ./test-file/fields_50.h5 -g /Timestep_50 -c  \"32,1,32\"\n";

  fprintf(stdout, msg, "element-wise");
}