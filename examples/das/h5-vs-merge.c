
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <math.h>       /* ceil  and floor*/
#include <getopt.h>
#include "hdf5.h" 

void  printf_help(char *cmd);



#define NAME_LENGTH 1024

int main(int argc, char *argv[])
{
  char  input_dir[NAME_LENGTH]="./testH5data";
  char  output_file[NAME_LENGTH]="./testHDF5data-merged.h5p";
  char  filter_str[NAME_LENGTH]="";
  char  group[NAME_LENGTH]="/";  //both input and output file share the same group and dataset name
  char  dataset[NAME_LENGTH]="/DataByChannelTime";

  int filter_flag = 0;
  int copt; 
  while ((copt = getopt (argc, argv, "o:i:g:d:hf")) != -1)
    switch (copt)
    {
      case 'o':
        memset(output_file, 0, sizeof(output_file));
        strcpy(output_file, optarg);
        break;
      case 'i':
        memset(input_dir, 0, sizeof(input_dir));
	      strcpy(input_dir, optarg);
	      break;
      case 'g':
        memset(group, 0, sizeof(group));
        strcpy(group, optarg);
	       break;
      case 'd':
	       memset(dataset, 0, sizeof(dataset));
         strcpy(dataset, optarg);
	       break;
      case 'h':
         printf_help(argv[0]);
         exit(0);	
      case 'f':
         filter_flag = 1;
         memset(filter_str, 0, sizeof(filter_str));
         strcpy(filter_str, optarg);
      default:
         printf("Wrong option [%c] for %s \n",copt, argv[0]);
         printf_help(argv[0]);
         exit(-1);
         break;
    }


 
  struct dirent **namelist;
  int namelist_length = scandir(input_dir, &namelist, 0, alphasort);

  int temp_index = 0;
  int max_file_name_legnth=0, file_to_merge_count= 0;
  if (namelist_length < 2)
       perror("Input directory has not files to merge !");
   else {
       while (temp_index < namelist_length) {
        if(strcmp(namelist[temp_index]->d_name, "..") == 0 || strcmp(namelist[temp_index]->d_name, ".") == 0){          
          free(namelist[temp_index]);
          namelist[temp_index] = NULL;
        }else{
          file_to_merge_count++;
          max_file_name_legnth = (strlen(namelist[temp_index]->d_name) > max_file_name_legnth ? strlen(namelist[temp_index]->d_name) : max_file_name_legnth);
        }        
        temp_index++;
       }
   }
  char *temp_file = malloc(strlen(input_dir) + max_file_name_legnth + 2);


  printf("List of files to merge (count = %d ): \n", file_to_merge_count);
  int i, picke_the_first_one_already=0;
  for(i = 0; i < namelist_length; i++){
    if(namelist[i] != NULL){
      printf(" %s\n", namelist[i]->d_name); 
      if(picke_the_first_one_already != 1 )
        sprintf(temp_file, "%s/%s", input_dir, namelist[i]->d_name);
        picke_the_first_one_already = 1;
     }
  }

  printf("Use %s as template to create virtual file.\n", temp_file);


  hid_t fid, gid, did;
  fid  = H5Fopen(temp_file, H5F_ACC_RDONLY, H5P_DEFAULT);

    
  if (strcmp(group, "/") != 0){
      //std::cout << "Open Group : " << gn << std::endl;
      gid  = H5Gopen(fid, group, H5P_DEFAULT);
      did  = H5Dopen(gid,  dataset,  H5P_DEFAULT);
  }else{
      did  = H5Dopen(fid,  dataset,  H5P_DEFAULT);
  }
    
  hid_t datatype       = H5Dget_type(did);     /* datatype handle */ 
  H5T_class_t type_class     = H5Tget_class(datatype);  
  hid_t dataspace_id   = H5Dget_space(did);
  int rank = H5Sget_simple_extent_ndims(dataspace_id);

  //Obtain the size of original array
  hsize_t *dims_per_dset = malloc(rank * sizeof(hsize_t));
  H5Sget_simple_extent_dims(dataspace_id, dims_per_dset, NULL);

  printf("dims_per_dset = (%lld, %lld) \n",dims_per_dset[0], dims_per_dset[1]);

  hsize_t *dims_vir_dset = malloc(rank * sizeof(hsize_t));
  dims_vir_dset[0] = dims_per_dset[0] * file_to_merge_count;
  dims_vir_dset[1] = dims_per_dset[1] ;

  printf("dims for finale merged file = (%lld, %lld) \n",dims_vir_dset[0], dims_vir_dset[1]);

  H5Dclose(did);
  H5Fclose(fid);


  /* Create file in which virtual dataset will be stored. */
  hid_t vir_file_id = H5Fcreate (output_file, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  /* Create VDS dataspace.  */
  hid_t vir_space = H5Screate_simple (rank, dims_vir_dset, NULL);

  /* Initialize hyperslab values. */

  hsize_t start_out[2], count_out[2];
  start_out[0] = 0;
  start_out[1] = 0;
  count_out[0] = dims_per_dset[0];
  count_out[1] = dims_per_dset[1];

  /* Set VDS creation property. */
  hid_t dcpl = H5Pcreate(H5P_DATASET_CREATE);

  hid_t  src_space = H5Screate_simple (rank, dims_per_dset, NULL);
  int src_index =0;
  for (i = 0; i < namelist_length; i++) {
    if(namelist[i] != NULL){
        start_out[0] = (hsize_t)src_index * dims_per_dset[0];
        /* Select i-th row in the virtual dataset; selection in the source datasets is the same. */
        H5Sselect_hyperslab (vir_space, H5S_SELECT_SET, start_out, NULL, count_out, NULL);
        H5Pset_virtual (dcpl, vir_space, namelist[i]->d_name, dataset, src_space);
        src_index++;
    }
  }

  /* Create a virtual dataset. */
  hid_t  vir_dset = H5Dcreate2 (vir_file_id, dataset, H5T_IEEE_F32LE, vir_space, H5P_DEFAULT, dcpl, H5P_DEFAULT);
  H5Sclose (vir_space);
  H5Sclose (src_space);
  H5Dclose (vir_dset);
  H5Fclose (vir_file_id);




  free(dims_vir_dset);
  free(dims_per_dset);
  free(temp_file);
  for(i = 0; i < namelist_length; i++){
    if(namelist[i] != NULL)
            free(namelist[temp_index]);
  }
}


void  printf_help(char *cmd){
  char *msg= (char *) "Usage: %s [OPTION] \n\
          -h help (--help)\n\
          -i input directory for HDF5 files to merge\n\
          -o output file name for merged files\n\
          -g group name (path) (across all HDF5 files and merged file)\n\
          -d dataset name (across all HDF5 files and merged file)\n\
          Example: %s -i ./testHDF5data -o ./testHDF5data-merged.h5p  -g / -d /DataByChannelTime\n";
  fprintf(stdout, msg, cmd, cmd);
}
