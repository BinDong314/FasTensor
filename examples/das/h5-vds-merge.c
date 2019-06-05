
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <math.h> /* ceil  and floor*/
#include <getopt.h>
#include "hdf5.h"
#include <limits.h> /* PATH_MAX */
#include <iostream>
#include <string>
#include <regex>

void printf_help(char *cmd);

#define NAME_LENGTH 1024

int main(int argc, char *argv[])
{
  std::string input_dir("./testH5data");
  std::string output_file("./testHDF5data-merged.h5p");
  std::string group("/"); //both input and output file share the same group and dataset name
  std::string dataset("/DataTimeChannel");

  char tmp_path[NAME_LENGTH];

  std::string filter_start_str;
  std::string filter_regex_str;
  int filter_count = 1;
  int filter_flag_sc = 0, filter_flag_regex = 0;
  int row_major_flag = 0;
  int copt;
  char *res;
  while ((copt = getopt(argc, argv, "o:i:g:d:hrs:c:e:")) != -1)
    switch (copt)
    {
    case 'o':
      output_file.assign(optarg);
      break;
    case 'i':
      res = realpath(optarg, tmp_path);
      input_dir.assign(tmp_path);
      if (res)
      {
        printf("Files to merge at:  [%s ] \n", input_dir.c_str());
      }
      else
      {
        perror("Can not resolve input directory");
        exit(EXIT_FAILURE);
      }
      break;
    case 'g':
      group.assign(optarg);
      break;
    case 'd':
      dataset.assign(optarg);
      break;
    case 'r':
      row_major_flag = 1;
      break;
    case 's':
      filter_flag_sc = 1;
      filter_start_str.assign(optarg);
      break;
    case 'c':
      filter_count = atoi(optarg);
      break;
    case 'e':
      filter_flag_regex = 1;
      filter_regex_str.assign(optarg);
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

  if (filter_flag_sc && filter_flag_regex)
  {
    printf("Please use filter either [start/count] or [regex] \n ");
    exit(0);
  }
  if (filter_flag_sc)
  {
    printf("Filer enabled to merge [%d] files after [%s] \n", filter_count, filter_start_str.c_str());
  }

  if (filter_flag_regex)
  {
    printf("Filer enabled to merge files with pattern of %s \n", filter_regex_str.c_str());
  }
  //Get the list of files under input_dir
  struct dirent **namelist;
  int namelist_length = scandir(input_dir.c_str(), &namelist, 0, alphasort);

  std::vector<std::string> file_to_merge_list;
  int temp_index = 0;
  if (namelist_length < 2)
  {
    printf("Input directory has not files to merge !");
    exit(0);
  }
  else
  {
    while (temp_index < namelist_length)
    {
      if (strcmp(namelist[temp_index]->d_name, "..") != 0 && strcmp(namelist[temp_index]->d_name, ".") != 0)
        file_to_merge_list.push_back(namelist[temp_index]->d_name);
      temp_index++;
    }
  }

  for (int i = 0; i < namelist_length; i++)
  {
    if (namelist[i] != NULL)
      free(namelist[temp_index]);
  }
  if (namelist != NULL)
    free(namelist);

  if (filter_flag_sc)
  {
    std::vector<std::string> file_to_merge_list_after_filter;
    int find_file_to_merge = 0;
    for (int i = 0; i < file_to_merge_list.size(); i++)
    {
      if (filter_start_str == file_to_merge_list[i] || find_file_to_merge)
      {
        file_to_merge_list_after_filter.push_back(file_to_merge_list[i]);
        find_file_to_merge++;
      }
      if (find_file_to_merge >= filter_count)
      {
        file_to_merge_list.resize(0);
        file_to_merge_list = file_to_merge_list_after_filter;
        break;
      }
    }
  }
  else if (filter_flag_regex)
  {
    std::vector<std::string> file_to_merge_list_after_filter;
    std::regex regex_pattern(filter_regex_str);
    std::smatch m;

    for (int i = 0; i < file_to_merge_list.size(); i++)
    {
      if (std::regex_search(file_to_merge_list[i], m, regex_pattern))
      {
        file_to_merge_list_after_filter.push_back(file_to_merge_list[i]);
      }
    }
    file_to_merge_list = file_to_merge_list_after_filter;
  }

  if (file_to_merge_list.size() == 0)
  {
    printf("No files after filter to merge !\n");
    exit(0);
  }

  printf("List of files to merge (count = [%ld): \n", file_to_merge_list.size());
  for (int i = 0; i < file_to_merge_list.size(); i++)
  {
    std::cout << file_to_merge_list[i] << std::endl;
  }

  std::string temp_file;
  temp_file = input_dir + "/" + file_to_merge_list[0];
  //printf("Use %s as template to create virtual file.\n", temp_file.c_str());
  //Open a temp file to find out its dimensions
  hid_t fid, gid, did;
  fid = H5Fopen(temp_file.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

  if (group != "/")
  {
    gid = H5Gopen(fid, group.c_str(), H5P_DEFAULT);
    did = H5Dopen(gid, dataset.c_str(), H5P_DEFAULT);
  }
  else
  {
    did = H5Dopen(fid, dataset.c_str(), H5P_DEFAULT);
  }

  hid_t datatype = H5Dget_type(did); /* datatype handle */
  H5T_class_t type_class = H5Tget_class(datatype);
  hid_t dataspace_id = H5Dget_space(did);
  int rank = H5Sget_simple_extent_ndims(dataspace_id);

  //Obtain the size of original array
  hsize_t *dims_per_dset = (hsize_t *)malloc(rank * sizeof(hsize_t));
  H5Sget_simple_extent_dims(dataspace_id, dims_per_dset, NULL);

  printf("dims per dataset = (%lld, %lld) \n", dims_per_dset[0], dims_per_dset[1]);

  hsize_t *dims_vir_dset = (hsize_t *)malloc(rank * sizeof(hsize_t));
  if (row_major_flag == 0)
  {
    dims_vir_dset[0] = dims_per_dset[0] * file_to_merge_list.size();
    dims_vir_dset[1] = dims_per_dset[1];
  }
  else
  {
    dims_vir_dset[0] = dims_per_dset[0];
    dims_vir_dset[1] = dims_per_dset[1] * file_to_merge_list.size();
  }

  printf("dims for the merged dataset = (%lld, %lld) \n", dims_vir_dset[0], dims_vir_dset[1]);

  H5Dclose(did);
  H5Fclose(fid);

  /* Create file in which virtual dataset will be stored. */
  hid_t vir_file_id = H5Fcreate(output_file.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  /* Create VDS dataspace.  */
  hid_t vir_space = H5Screate_simple(rank, dims_vir_dset, NULL);

  /* Initialize hyperslab values. */

  hsize_t start_out[2], count_out[2], block_out[2];
  start_out[0] = 0;
  start_out[1] = 0;

  count_out[0] = 1;
  count_out[1] = 1;

  block_out[0] = dims_per_dset[0];
  block_out[1] = dims_per_dset[1];

  /* Set VDS creation property. */
  hid_t dcpl = H5Pcreate(H5P_DATASET_CREATE);
  float fill_value = 0;
  H5Pset_fill_value(dcpl, H5T_STD_I16BE, &fill_value);

  hid_t src_space = H5Screate_simple(rank, dims_per_dset, NULL);
  int src_index = 0;
  for (int i = 0; i < file_to_merge_list.size(); i++)
  {
    temp_file = input_dir + "/" + file_to_merge_list[i];
    if (row_major_flag == 0)
    {
      start_out[0] = (hsize_t)src_index * dims_per_dset[0];
    }
    else
    {
      start_out[1] = (hsize_t)src_index * dims_per_dset[1];
    }
    H5Sselect_hyperslab(vir_space, H5S_SELECT_SET, start_out, NULL, count_out, block_out);
    H5Pset_virtual(dcpl, vir_space, temp_file.c_str(), dataset.c_str(), src_space);
    src_index++;
  }

  /* Create a virtual dataset. */
  hid_t vir_dset = H5Dcreate(vir_file_id, dataset.c_str(), H5T_STD_I16BE, vir_space, H5P_DEFAULT, dcpl, H5P_DEFAULT);

  H5Sclose(vir_space);
  H5Sclose(src_space);
  H5Dclose(vir_dset);
  H5Fclose(vir_file_id);

  free(dims_vir_dset);
  free(dims_per_dset);
}

void printf_help(char *cmd)
{
  char *msg = (char *)"Usage: %s [OPTION] \n\
          -h help (--help)\n\
          -i input directory for HDF5 files to merge\n\
          -o output file name for merged files\n\
          -g group name (path) (across all HDF5 files and merged file)\n\
          -d dataset name (across all HDF5 files and merged file)\n\
          -r row-direction for mergeing (column-direction by default)\n\
          -s the file name (string) to start filter   \n\
          -c counts of files after start filter \n\
          -e regex filter string (more on: http://www.cplusplus.com/reference/regex/ECMAScript/)\n\
          Example 1: %s -i ./testH5data -o ./testHDF5data-merged.h5p  -g / -d /DataByChannelTime\n\
                     PS: all files under ./testH5Data \n\
          Example 2: %s -i ./testH5data -o ./testHDF5data-merged.h5p  -g / -d /DataByChannelTime -s westSac_170728224510.h5p -c 2 \n\
                     PS: Example 2 merge two files after westSac_170728224510.h5p (including) under ./testH5Data  \n\
          Example 3: %s -i ./testH5data -o ./testHDF5data-merged.h5p  -g / -d /DataByChannelTime -e 170728224[567]10\n\
                     PS: Example 3 merge all files matched regex pattern [170728224[567]10]\n ";
          
  fprintf(stdout, msg, cmd, cmd, cmd, cmd);
}
