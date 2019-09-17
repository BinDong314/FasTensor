
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
#include <fstream>
#include <mpi.h>
#include <iomanip>
#include <time.h>

//time_t au_timer_global_start__inside_use_no_mpi;
clock_t timer_begin_no_mpi;

void au_time_start_no_mpi()
{
  timer_begin_no_mpi = clock();
  //time(&au_timer_global_start__inside_use_no_mpi);
}

void au_time_elap_no_mpi(std::string info_str)
{
  //time_t current_time;
  //time(&current_time);
  //double time_taken = difftime(current_time, au_timer_global_start__inside_use_no_mpi); //double(current_time - au_timer_global_start__inside_use_no_mpi);
  clock_t timer_end_no_mpi = clock();

  double elapsed_secs = double(timer_end_no_mpi - timer_begin_no_mpi) / CLOCKS_PER_SEC;
  std::cout << info_str << " : " << std::fixed << elapsed_secs << std::setprecision(10);
  std::cout << " sec " << std::endl;
  //printf("%s  : %.6lf seconds.", info_str.c_str());
  //reset timer
  //time(&au_timer_global_start__inside_use_no_mpi);
  timer_begin_no_mpi = clock();
}

void printf_help(char *cmd);

#define NAME_LENGTH 1024

int OpenReadCloseSingleFileWhole(std::string fna, std::string gna, std::string dna, std::vector<short> &dataa);

int main(int argc, char *argv[])
{
  std::string input_dir("./testH5data");
  std::string output_file("./testHDF5data-merged.h5");
  std::string group("/"); //both input and output file share the same group and dataset name
  std::string dataset("/DataTC");

  char tmp_path[NAME_LENGTH];

  std::string filter_start_str;
  std::string filter_regex_str;
  int filter_count = 1;
  int filter_flag_sc = 0, filter_flag_regex = 0;
  int row_major_flag = 1;
  int merged_file_flag = 0;
  int copt;
  char *res;
  int mpi_parallel_model = 0;
  while ((copt = getopt(argc, argv, "o:i:g:d:hls:c:e:mp")) != -1)
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
    case 'l':
      row_major_flag = 0;
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
    case 'm':
      merged_file_flag = 1;
      break;
    case 'p':
      mpi_parallel_model = 1;
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

  int mpi_size = 1, mpi_rank = 0;
  if (mpi_parallel_model)
  {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  }

  if (filter_flag_sc && filter_flag_regex)
  {
    printf("Please use filter either [start/count] or [regex] \n ");
    exit(0);
  }
  if (filter_flag_sc)
  {
    printf("Filter enabled to merge [%d] files after [%s] \n", filter_count, filter_start_str.c_str());
  }

  if (filter_flag_regex)
  {
    printf("Filter enabled to merge files with pattern of %s \n", filter_regex_str.c_str());
  }
  au_time_start_no_mpi();

  //Get the list of files under input_dir
  struct dirent **namelist;
  int namelist_length = scandir(input_dir.c_str(), &namelist, 0, alphasort);

  std::vector<std::string> file_to_merge_list;
  int temp_index = 0;
  if (namelist_length < 2)
  {
    printf("Input directory has NO files to merge !");
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

    if (find_file_to_merge == 0)
    {
      std::cout << "Cann't find start file for filter: " << filter_start_str << " \n";
      exit(-1);
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

  au_time_elap_no_mpi("Search");
  if (file_to_merge_list.size() == 0)
  {
    printf("No files after filter to merge !\n");
    exit(0);
  }

  if (merged_file_flag == 0)
  {
    std::string output_file_meata;
    output_file_meata = output_file + ".vds-meta";
    std::ofstream output_file_stream;
    output_file_stream.open(output_file_meata);

    printf("List of files to merge (# = %ld): \n", file_to_merge_list.size());
    for (int i = 0; i < file_to_merge_list.size(); i++)
    {
      if (i < 2 || i > file_to_merge_list.size() - 2)
        std::cout << file_to_merge_list[i] << std::endl;
      if (i == 2)
        std::cout << "  .... " << std::endl;
      output_file_stream << input_dir << "/" << file_to_merge_list[i] << std::endl;
    }
    output_file_stream.close();
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
  if (row_major_flag == 1)
  {
    dims_vir_dset[0] = dims_per_dset[0];
    dims_vir_dset[1] = dims_per_dset[1] * file_to_merge_list.size();
  }
  else
  {
    dims_vir_dset[0] = dims_per_dset[0] * file_to_merge_list.size();
    dims_vir_dset[1] = dims_per_dset[1];
  }

  printf("dims for the merged dataset = (%lld, %lld) \n", dims_vir_dset[0], dims_vir_dset[1]);

  H5Dclose(did);
  H5Fclose(fid);

  hid_t plist_id = H5Pcreate(H5P_FILE_ACCESS);
  if (mpi_parallel_model)
  {
    H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);
  }

  /* Create file in which virtual dataset will be stored. */
  hid_t vir_file_id = H5Fcreate(output_file.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
  H5Pclose(plist_id);

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
  H5Pset_fill_value(dcpl, H5T_STD_I16LE, &fill_value);

  if (merged_file_flag == 0)
  {
    hid_t src_space = H5Screate_simple(rank, dims_per_dset, NULL);
    int src_index = 0;
    for (int i = 0; i < file_to_merge_list.size(); i++)
    {
      temp_file = input_dir + "/" + file_to_merge_list[i];
      if (row_major_flag == 1)
      {
        start_out[1] = (hsize_t)src_index * dims_per_dset[1];
      }
      else
      {
        start_out[0] = (hsize_t)src_index * dims_per_dset[0];
      }
      H5Sselect_hyperslab(vir_space, H5S_SELECT_SET, start_out, NULL, count_out, block_out);
      H5Pset_virtual(dcpl, vir_space, temp_file.c_str(), dataset.c_str(), src_space);
      src_index++;
    }

    hid_t vir_dset, vir_group;
    if (group != "/")
    {
      vir_group = H5Gcreate(vir_file_id, group.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      vir_dset = H5Dcreate(vir_group, dataset.c_str(), H5T_STD_I16LE, vir_space, H5P_DEFAULT, dcpl, H5P_DEFAULT);
    }
    else
    {
      vir_dset = H5Dcreate(vir_file_id, dataset.c_str(), H5T_STD_I16LE, vir_space, H5P_DEFAULT, dcpl, H5P_DEFAULT);
    }
    /* Create a virtual dataset. */
    // hid_t vir_dset = H5Dcreate(vir_file_id, dataset.c_str(), H5T_STD_I16LE, vir_space, H5P_DEFAULT, dcpl, H5P_DEFAULT);
    H5Sclose(src_space);
    H5Dclose(vir_dset);
    if (group != "/")
    {
      H5Gclose(vir_group);
    }
    au_time_elap_no_mpi("Create VDS file: ");
  }
  else
  { //Create real merged file
    hid_t vir_dset, vir_group;
    if (group != "/")
    {
      vir_group = H5Gcreate(vir_file_id, group.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      vir_dset = H5Dcreate(vir_group, dataset.c_str(), H5T_STD_I16LE, vir_space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    }
    else
    {
      vir_dset = H5Dcreate(vir_file_id, dataset.c_str(), H5T_STD_I16LE, vir_space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    }

    count_out[0] = dims_per_dset[0];
    count_out[1] = dims_per_dset[1];

    start_out[0] = 0;
    start_out[1] = 0;

    hid_t memspace_id = H5Screate_simple(rank, &count_out[0], NULL);
    hid_t vir_dset_space_id = H5Dget_space(vir_dset);

    std::vector<short> data_per_file;
    data_per_file.resize(dims_per_dset[0] * dims_per_dset[1]);

    int mpi_batches;
    if (file_to_merge_list.size() % mpi_size == 0)
    {
      mpi_batches = file_to_merge_list.size() / mpi_size;
    }
    else
    {
      mpi_batches = file_to_merge_list.size() / mpi_size + 1;
    }

    hid_t plist_cio_id = H5P_DEFAULT;
    if (mpi_parallel_model)
    {
      plist_cio_id = H5Pcreate(H5P_DATASET_XFER);
      H5Pset_dxpl_mpio(plist_cio_id, H5FD_MPIO_COLLECTIVE);
    }
    int src_index = mpi_rank;
    for (int i = 0; i < mpi_batches; i++)
    {
      if (src_index < file_to_merge_list.size())
      {
        temp_file = input_dir + "/" + file_to_merge_list[src_index];
        if (row_major_flag == 1)
        {
          start_out[1] = (hsize_t)src_index * dims_per_dset[1];
        }
        else
        {
          start_out[0] = (hsize_t)src_index * dims_per_dset[0];
        }
        //Read all data on temp_file
        OpenReadCloseSingleFileWhole(temp_file, group, dataset, data_per_file);
        //Write to file
        H5Sselect_hyperslab(vir_dset_space_id, H5S_SELECT_SET, start_out, NULL, count_out, NULL);
        H5Dwrite(vir_dset, H5T_NATIVE_SHORT, memspace_id, vir_dset_space_id, plist_cio_id, &data_per_file[0]);
      }
      else
      {
        start_out[0] = 0;
        start_out[1] = 0;
        count_out[0] = 0;
        count_out[1] = 0;
        hid_t memspace_id_null = H5Screate_simple(rank, &count_out[0], NULL);
        H5Sselect_hyperslab(vir_dset_space_id, H5S_SELECT_SET, start_out, NULL, count_out, NULL);
        H5Dwrite(vir_dset, H5T_NATIVE_SHORT, memspace_id_null, vir_dset_space_id, plist_cio_id, &data_per_file[0]);
        H5Sclose(memspace_id_null);
      }
      if (i % 10 == 0)
      {
        std::cout << src_index << " : " << temp_file << std::endl;
      }
      src_index = src_index + mpi_size;
    }

    H5Sclose(memspace_id);
    H5Sclose(vir_dset_space_id);
    H5Dclose(vir_dset);
    if (group != "/")
    {
      H5Gclose(vir_group);
    }

    au_time_elap_no_mpi("Create real file:");
  }

  H5Sclose(vir_space);
  H5Fclose(vir_file_id);
  if (mpi_parallel_model)
    MPI_Finalize();

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
          -l column order (row-order by default)\n\
          -s the file name (string) to start filter (No directory path needed)  \n\
          -c counts of files after start filter \n\
          -e regex filter string (more on: http://www.cplusplus.com/reference/regex/ECMAScript/)\n\
          -m create real merged file (mostly for test) \n\
          -p in parallel model (mostly used with -m to create merged file) \n\
          Example 1: %s -i ./testH5data -o ./testHDF5data-merged.h5  -g / -d /DataCT\n\
                     PS: all files under ./testH5Data \n\
          Example 2: %s -i ./testH5data -o ./testHDF5data-merged.h5  -g / -d /DataCT -s westSac_170728224510.h5 -c 2 \n\
                     PS: Example 2 merge two files after westSac_170728224510.h5 (including) under ./testH5Data  \n\
          Example 3: %s -i ./testH5data -o ./testHDF5data-merged.h5  -g / -d /DataCT -e 170728224[567]10\n\
                     PS: Example 3 merge all files matched regex pattern [170728224[567]10]\n ";

  fprintf(stdout, msg, cmd, cmd, cmd, cmd);
}

//Used to read VDS's file list
//One file at a time and all processes read the same file
//Each process figure out is own start/end
int OpenReadCloseSingleFileWhole(std::string fna, std::string gna, std::string dna, std::vector<short> &dataa)
{

  hid_t v_fid = -1, v_gid = -1, v_did = -1;
  //v_plist_id = H5Pcreate(H5P_FILE_ACCESS);
  //H5Pset_fapl_mpio(v_plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

  v_fid = H5Fopen(fna.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
  if (v_fid < 0)
  {
    std::cout << "Error happens in open file " << fna << std::endl;
    exit(-1);
  }

  std::string root_dir = "/";
  if (gna != root_dir)
  {
    //std::cout << "Open Group : " << gn << std::endl;
    v_gid = H5Gopen(v_fid, gna.c_str(), H5P_DEFAULT);
    v_did = H5Dopen(v_gid, dna.c_str(), H5P_DEFAULT);
  }
  else
  {
    v_did = H5Dopen(v_fid, dna.c_str(), H5P_DEFAULT);
  }

  /*
    hid_t v_datatype = H5Dget_type(v_did); 
    H5T_class_t v_type_class = H5Tget_class(v_datatype);
    hid_t v_dataspace_id = H5Dget_space(v_did);
    int v_rank = H5Sget_simple_extent_ndims(v_dataspace_id);

    std::vector<hsize_t> v_dims_out;

    v_dims_out.resize(v_rank);
    H5Sget_simple_extent_dims(v_dataspace_id, &v_dims_out[0], NULL);
    */

  int ret = 1;

  ret = H5Dread(v_did, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &dataa[0]);

  H5Dclose(v_did);
  if (v_gid > 0)
    H5Gclose(v_gid);
  H5Fclose(v_fid);

  if (ret < 0)
  {
    printf("Some error happen in reading HDF5 \n");
    exit(-1);
  }
  return 1;
}