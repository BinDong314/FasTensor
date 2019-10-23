
/*
 *ArrayUDF Copyright (c) 2017, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy).  All rights reserved.

 *If you have questions about your rights to use or distribute this software, please contact Berkeley Lab's Innovation & Partnerships Office at  IPO@lbl.gov.

* NOTICE. This Software was developed under funding from the U.S. Department of Energy and the U.S. Government consequently retains certain rights. As such, the U.S. Government has been granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software to reproduce, distribute copies to the public, prepare derivative works, and perform publicly and display publicly, and to permit other to do so. 
 */
/**
 *
 * Email questions to {dbin, kwu, sbyna}@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#include "stdlib.h"
#include "hdf5.h"
#include "getopt.h"
#include <string.h>

#define SDS_CLIENT_MPI 1
#define FN_NAME_MAX 255

void get_dimension(int dims, char *dims_str, hsize_t *dims_size)
{
  int i;
  char *pch;
  char temp[FN_NAME_MAX];

  if (dims == 1)
  {
    dims_size[0] = atoi(dims_str);
  }
  else
  {
    strcpy(temp, dims_str);
    pch = strtok(temp, ",");

    i = 0;
    while (pch != NULL)
    {
      //printf("%s \n", pch);
      dims_size[i] = atoi(pch);
      pch = strtok(NULL, ",");
      i++;
    }
  }
  return;
}

void figure_out_o_c(int dims, hsize_t *dims_size, int mpi_rank, int mpi_size, hsize_t *offset, hsize_t *count)
{
  hsize_t data_size;
  int i;
  //Partition along last dimension
  data_size = dims_size[dims - 1];
  if (mpi_rank != (mpi_size - 1))
  {
    count[dims - 1] = data_size / mpi_size;
  }
  else
  {
    count[dims - 1] = data_size / mpi_size + data_size % mpi_size;
  }
  offset[dims - 1] = (data_size / mpi_size) * mpi_rank;

  for (i = 0; i < (dims - 1); i++)
  {
    count[i] = dims_size[i];
    offset[i] = 0;
  }
}

void print_help()
{
  const char *msg = "Usage: %s [OPTION] \n\
      	  -h help (--help)\n\
          -f name of the file (only HDF5 file in current version) \n\
          -g group path within HDF5 file to data set \n\
          -d name of the dataset \n\
          -n dimension of the dataset \n\
          -s size of each dimension (format 1D: n 2D: x,y 3D x,y,z, etc)  \n\
          -t type of the element(0: unsigned int, 1 : float (by default), 2: short int), no other type supported \n\
          -c number of cores. \n\
          -m mode(1: array, 2: rational), no suport right now \n\
          -r fill random number \n\
          -i intial valur to fill (not random number) \n\
          -p sparse array     \n\
          Example: mpirun -n 1 ./fake-hdf5-data -f testf.h5p -g /testg -d /testg/testd -n 2 -s 100,100 -t 0 \n";

  fprintf(stdout, msg, "fake-hdf5");
}

//#define SDS_MPI

int main(int argc, char **argv)
{
  int mpi_size, mpi_rank, dims, cores, multi_dataset, type = 0, c, i, random_number_flag = 0, random_max = 100, sparse_flag = 0;
  float sparse_rate = 30.0;
  hid_t file_id, dset_id, group_id, plist_id, dataspace_id, write_space, memspace_id;
  hsize_t *offset, *count, *dims_size;
  char dims_str[FN_NAME_MAX], filename[FN_NAME_MAX], groupname[FN_NAME_MAX], dsetname[FN_NAME_MAX];
  int start_value = 0;
#ifdef SDS_CLIENT_MPI
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Info info = MPI_INFO_NULL;
#endif

  while ((c = getopt(argc, argv, "f:g:d:n:c:s:ht:rpa:i:")) != -1)
    switch (c)
    {
    case 'f':
      strncpy(filename, optarg, FN_NAME_MAX);
      break;
    case 'g':
      strncpy(groupname, optarg, FN_NAME_MAX);
      break;
    case 'd':
      strncpy(dsetname, optarg, FN_NAME_MAX);
      break;
    case 'n':
      dims = atoi(optarg);
      break;
    case 'c':
      cores = atoi(optarg);
      break;
    case 's':
      //string formart is like X,Y,Z, which is seperated by commas
      strcpy(dims_str, optarg);
      break;
    case 'm':
      multi_dataset = atoi(optarg);
      break;
    case 't':
      type = atoi(optarg);
      break;
    case 'r':
      random_number_flag = 1;
      break;
    case 'p':
      sparse_flag = 1;
      break;
    case 'a':
      sparse_rate = atof(optarg);
      break;
    case 'i':
      start_value = atoi(optarg);
      break;
    case 'h':
      print_help();
      return 1;
      break;
    default:
      break;
    }

#ifdef SDS_CLIENT_MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_size(comm, &mpi_size);
  MPI_Comm_rank(comm, &mpi_rank);
  if (mpi_rank == 0)
    printf("Use MPI \n");
#else
  printf("NO MPI \n");
  mpi_size = 1;
  mpi_rank = 0;
#endif

  if (mpi_rank == 0)
  {
    printf("Using %d cores to generate dataset [%s] of dimensions [%d: %s ] within file [%s]. \n", mpi_size, dsetname, dims, dims_str, filename);
    fflush(stdout);
  }
  dims_size = (hsize_t *)malloc(dims * sizeof(hsize_t));
  get_dimension(dims, dims_str, dims_size);

  if (mpi_rank == 0)
  {
    printf("Size : ");
    for (i = 0; i < dims; i++)
    {
      printf(" %lld ", dims_size[i]);
    }
    printf(" \n");
    fflush(stdout);
  }

  //Create new file and write result to this file
#ifdef SDS_CLIENT_MPI
  plist_id = H5Pcreate(H5P_FILE_ACCESS);
  H5Pset_fapl_mpio(plist_id, comm, info);
#else
  plist_id = H5P_DEFAULT;
#endif

  file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
  H5Pclose(plist_id);

  group_id = H5Gcreate(file_id, groupname, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  dataspace_id = H5Screate_simple(dims, dims_size, NULL);
  if (type == 0)
  { // type = 0 , data is int
    dset_id = H5Dcreate(group_id, dsetname, H5T_STD_I32LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  }
  else if (type == 1)
  { //By default, data is float
    dset_id = H5Dcreate(group_id, dsetname, H5T_IEEE_F32LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  }
  else
  {
    dset_id = H5Dcreate(group_id, dsetname, H5T_STD_I16LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  }

  H5Sclose(dataspace_id);

  offset = (hsize_t *)malloc(dims * sizeof(hsize_t));
  count = (hsize_t *)malloc(dims * sizeof(hsize_t));

  //printf("mpirank %d : offset ");
  figure_out_o_c(dims, dims_size, mpi_rank, mpi_size, offset, count);

  if (mpi_rank == 0)
  {
    printf("Size : ");
    for (i = 0; i < dims; i++)
    {
      printf("%lld,  ", dims_size[i]);
    }
    printf(" \n");
    printf("type = %d, random_number_flag = %d \n", type, random_number_flag);
    fflush(stdout);
  }

  write_space = H5Dget_space(dset_id);
  H5Sselect_hyperslab(write_space, H5S_SELECT_SET, offset, NULL, count, NULL);

  memspace_id = H5Screate_simple(dims, count, NULL);

  hsize_t data_size = 1;
  for (i = 0; i < dims; i++)
  {
    //data_size = data_size * dims_size[0];
    data_size = data_size * count[i];
  }

  float size_in_gb = data_size / 1024.0 / 1024.0 / 1024.0;

  int *pi;
  float *pf;
  int16_t *pi16;
  if (type == 0)
  {
    //pus = (int *)buf;
    pi = (int *)malloc(data_size * sizeof(int));
    if (pi == NULL)
    {
      printf("Memory allocation fails (size=%f) \n", size_in_gb);
      exit(1);
    }
  }
  else if (type == 1)
  {
    //pf  = (float *)buf;
    pf = (float *)malloc(data_size * sizeof(float));
    if (pf == NULL)
    {
      printf("Memory allocation fails (size=%f) \n", size_in_gb);
      exit(1);
    }
  }
  else
  {
    pi16 = (int16_t *)malloc(data_size * sizeof(int16_t));
    if (pi16 == NULL)
    {
      printf("Memory allocation fails (size=%f) \n", size_in_gb);
      exit(1);
    }
  }

  for (i = 0; i < data_size; i++)
  {
    if (type == 0)
    {
      if (random_number_flag)
      {
        pi[i] = rand() % random_max;
        if (sparse_flag == 1 && pi[i] < sparse_rate)
          pi[i] = 0;
      }
      else
      {
        pi[i] = i * 10 + start_value;
      }
    }
    else if (type == 1)
    {
      //Comment out for create a temp file
      if (random_number_flag)
      {
        pf[i] = ((float)(rand() % random_max) / (float)random_max) * (float)random_max + 0.5;
        if (sparse_flag == 1 && pf[i] < sparse_rate)
          pf[i] = 0;
      }
      else
      {
        pf[i] = i + 1.81 + start_value; //(float) i / (float) data_size + (float)i;
        //printf("defulat value = %f \n", pf[i]);
      }
    }
    else
    {
      if (random_number_flag)
      {
        pi16[i] = rand() % random_max;
        if (sparse_flag == 1 && pi16[i] < sparse_rate)
          pi16[i] = 0;
      }
      else
      {
        pi16[i] = i * 10 + start_value;
      }
    }
  }

  if (mpi_rank == 0)
  {
    printf("Write data ... \n");
  }

  hid_t plist_cio_id = H5Pcreate(H5P_DATASET_XFER);
  H5Pset_dxpl_mpio(plist_cio_id, H5FD_MPIO_COLLECTIVE);

  if (type == 0)
  {
    H5Dwrite(dset_id, H5T_NATIVE_INT, memspace_id, write_space, plist_cio_id, pi);
    free(pi);
  }
  else if (type == 1)
  {
    H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace_id, write_space, plist_cio_id, pf);
    free(pf);
  }
  else
  {
    H5Dwrite(dset_id, H5T_NATIVE_SHORT, memspace_id, write_space, plist_cio_id, pi16);
    free(pi16);
  }

  H5Sclose(write_space);
  H5Sclose(memspace_id);
  H5Dclose(dset_id);
  H5Gclose(group_id);
  H5Fclose(file_id);

#ifdef SDS_CLIENT_MPI
  MPI_Finalize();
#endif
}
