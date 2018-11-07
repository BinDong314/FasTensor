#define HSIZE_T "%llu"

#define DUMP_INFO_DEBUG 1
int mpi_size, mpi_rank;
MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

species_t * sp = find_species_name(speciesname, species_list);
if( !sp ) ERROR(( "Invalid species name: %s", speciesname ));
clear_hydro_array( hydro_array );
accumulate_hydro_p( hydro_array, sp, interpolator_array );
synchronize_hydro_array( hydro_array );

#ifdef DUMP_INFO_DEBUG
printf("MPI rank = %d, size = %d \n", mpi_rank, mpi_size);
printf("base dir for field: %s \n", global->fdParams.baseDir);
printf("stride x y z  = (%ld, %ld, %ld)\n", global->fdParams.stride_x,global->fdParams.stride_y, global->fdParams.stride_z );
printf("grid x, y z  = (%d, %d, %d) \n", grid->nx, grid->ny, grid->nz);
printf("domain loc (x0, y0, z0) -> (x1, y1, z1) = (%f, %f, %f) -> (%f, %f, %f) \n", grid->x0, grid->y0, grid->z0,grid->x1, grid->y1, grid->z1);
printf("global->topology_x, y, z =  %f, %f, %f \n ", global->topology_x, global->topology_y, global->topology_z);
printf("grid -> sx, sy, sz =  (%d, %d, %d), nv=%d \n", grid->sx, grid->sy, grid->sz, grid->nv);
#endif

char hname[256];
char hydro_scratch[128];
char subhydro_scratch[128];

sprintf(hydro_scratch, "./%s", "hydro_hdf5");
dump_mkdir(hydro_scratch);
sprintf(subhydro_scratch, "%s/T.%ld/", hydro_scratch, step());
dump_mkdir(subhydro_scratch);

sprintf (hname, "%s/hydro_%s_%ld.h5", subhydro_scratch, speciesname, step());
double el1 = uptime();
hid_t plist_id = H5Pcreate(H5P_FILE_ACCESS);
H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);
hid_t file_id = H5Fcreate(hname , H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
H5Pclose(plist_id);

sprintf (hname, "Timestep_%ld", step());
hid_t group_id = H5Gcreate(file_id, hname, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

el1 = uptime() - el1;
sim_log("TimeHDF5Open: "<< el1 << " s");  //Easy to handle results for scripts
double el2 = uptime();


// Create a variable list of field values to output.
size_t numvars = std::min(global->fdParams.output_vars.bitsum(), total_field_variables);
size_t * varlist = new size_t[numvars];

for(size_t i(0), c(0); i<total_field_variables; i++)
  if(global->fdParams.output_vars.bitset(i)) varlist[c++] = i;

printf("\nBEGIN_OUTPUT: numvars = %zd \n", numvars);

# define hydro(x,y,z) hydro_array->h[VOXEL(x,y,z, grid->nx,grid->ny,grid->nz)]
//typedef struct hydro {
//  float jx, jy, jz, rho; // Current and charge density => <q v_i f>, <q f>
//  float px, py, pz, ke;  // Momentum and K.E. density  => <p_i f>, <m c^2 (gamma-1) f>
//  float txx, tyy, tzz;   // Stress diagonal            => <p_i v_j f>, i==j
//  float tyz, tzx, txy;   // Stress off-diagonal        => <p_i v_j f>, i!=j
//  float _pad[2];         // 16-byte align
//} hydro_t;

//typedef struct hydro_array {
//  hydro_t * ALIGNED(128) h;
//  grid_t * g;
//} hydro_array_t;


float *temp_buf = (float *)malloc(sizeof(float) * (grid->nx) * (grid->ny) * (grid->nz));
hsize_t temp_buf_index;
hid_t dset_id;
//char  *field_var_name[] = {"ex","ey","ez","div_e_err","cbx","cby","cbz","div_b_err","tcax","tcay","tcaz","rhob","jfx","jfy","jfz","rhof"};
plist_id = H5Pcreate(H5P_DATASET_XFER);
//Comment out for test only
H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
//H5Sselect_hyperslab(filespace, H5S_SELECT_SET, (hsize_t *) &offset, NULL, (hsize_t *) &numparticles, NULL);

//global->topology_x

hsize_t hydro_global_size[3], hydro_local_size[3], global_offset[3], global_count[3];
hydro_global_size[0] = (grid->nx * global->topology_x); 
hydro_global_size[1] = (grid->ny * global->topology_y);
hydro_global_size[2] = (grid->nz * global->topology_z);

hydro_local_size[0]  = grid->nx; 
hydro_local_size[1]  = grid->ny;
hydro_local_size[2]  = grid->nz;

# define RANK_TO_INDEX2(rank,ix,iy,iz) BEGIN_PRIMITIVE {                 \
    int _ix, _iy, _iz;                                                  \
    _ix  = (rank);                /* ix = ix+gpx*( iy+gpy*iz ) */       \
    _iy  = _ix/int(global->topology_x);   /* iy = iy+gpy*iz */                  \
    _ix -= _iy*int(global->topology_x);   /* ix = ix */                         \
    _iz  = _iy/int(global->topology_y);   /* iz = iz */                         \
    _iy -= _iz*int(global->topology_y);   /* iy = iy */                         \
    (ix) = _ix;                                                         \
    (iy) = _iy;                                                         \
    (iz) = _iz;                                                         \
  } END_PRIMITIVE

int mpi_rank_x, mpi_rank_y,mpi_rank_z;
RANK_TO_INDEX2(mpi_rank, mpi_rank_x, mpi_rank_y, mpi_rank_z);

global_offset[0]     = (grid->nx) * mpi_rank_x;
global_offset[1]     = (grid->ny) * mpi_rank_y;
global_offset[2]     = (grid->nz) * mpi_rank_z;

global_count[0]      = (grid->nx);
global_count[1]      = (grid->ny);
global_count[2]      = (grid->nz);


#ifdef DUMP_INFO_DEBUG
printf("global size   = " HSIZE_T ", " HSIZE_T ", " HSIZE_T "\n", hydro_global_size[0],hydro_global_size[1], hydro_global_size[2]);
printf("global_offset = " HSIZE_T ", " HSIZE_T ", " HSIZE_T "\n", global_offset[0], global_offset[1], global_offset[2]);
printf("global_count  = " HSIZE_T ", " HSIZE_T ", " HSIZE_T "\n",  global_count[0],  global_count[1], global_count[2]);
printf("mpi-rank = %d, rank index = (%d, %d, %d) \n",mpi_rank,  mpi_rank_x, mpi_rank_y ,  mpi_rank_z);
fflush(stdout);
#endif


hid_t filespace = H5Screate_simple(3, hydro_global_size, NULL);
hid_t memspace =  H5Screate_simple(3, hydro_local_size, NULL);
hid_t dataspace_id;
dset_id = H5Dcreate(group_id, "jx", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
temp_buf_index = 0;
for(size_t i(1); i<grid->nx+1; i++) {
    for(size_t j(1); j<grid->ny+1; j++) {
        for(size_t k(1); k<grid->nz+1; k++) {
            temp_buf[temp_buf_index] = hydro(i,j,k).jx;
            temp_buf_index = temp_buf_index + 1;
}}} 
dataspace_id = H5Dget_space (dset_id);
H5Sselect_hyperslab (dataspace_id, H5S_SELECT_SET, global_offset, NULL, global_count, NULL);
H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, dataspace_id, plist_id, temp_buf);
H5Sclose(dataspace_id);
H5Dclose(dset_id);


dset_id = H5Dcreate(group_id, "jy", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
temp_buf_index = 0;
for(size_t i(1); i<grid->nx+1; i++) {
    for(size_t j(1); j<grid->ny+1; j++) {
        for(size_t k(1); k<grid->nz+1; k++) {
            temp_buf[temp_buf_index] = hydro(i,j,k).jy;
            temp_buf_index = temp_buf_index + 1;
}}} 
dataspace_id = H5Dget_space (dset_id);
H5Sselect_hyperslab (dataspace_id, H5S_SELECT_SET, global_offset, NULL, global_count, NULL);
H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, dataspace_id, plist_id, temp_buf);
H5Sclose(dataspace_id);
H5Dclose(dset_id);

dset_id = H5Dcreate(group_id, "jz", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
temp_buf_index = 0;
for(size_t i(1); i<grid->nx+1; i++) {
    for(size_t j(1); j<grid->ny+1; j++) {
        for(size_t k(1); k<grid->nz+1; k++) {
            temp_buf[temp_buf_index] = hydro(i,j,k).jz;
            temp_buf_index = temp_buf_index + 1;
}}} 
dataspace_id = H5Dget_space (dset_id);
H5Sselect_hyperslab (dataspace_id, H5S_SELECT_SET, global_offset, NULL, global_count, NULL);
H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, dataspace_id, plist_id, temp_buf);
H5Sclose(dataspace_id);
H5Dclose(dset_id);

dset_id = H5Dcreate(group_id, "rho", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
temp_buf_index = 0;
for(size_t i(1); i<grid->nx+1; i++) {
    for(size_t j(1); j<grid->ny+1; j++) {
        for(size_t k(1); k<grid->nz+1; k++) {
            temp_buf[temp_buf_index] = hydro(i,j,k).rho;
            temp_buf_index = temp_buf_index + 1;
}}} 
dataspace_id = H5Dget_space (dset_id);
H5Sselect_hyperslab (dataspace_id, H5S_SELECT_SET, global_offset, NULL, global_count, NULL);
H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, dataspace_id, plist_id, temp_buf);
H5Sclose(dataspace_id);
H5Dclose(dset_id);

dset_id = H5Dcreate(group_id, "px", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
temp_buf_index = 0;
for(size_t i(1); i<grid->nx+1; i++) {
    for(size_t j(1); j<grid->ny+1; j++) {
        for(size_t k(1); k<grid->nz+1; k++) {
            temp_buf[temp_buf_index] = hydro(i,j,k).px;
            temp_buf_index = temp_buf_index + 1;
}}} 
dataspace_id = H5Dget_space (dset_id);
H5Sselect_hyperslab (dataspace_id, H5S_SELECT_SET, global_offset, NULL, global_count, NULL);
H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, dataspace_id, plist_id, temp_buf);
H5Sclose(dataspace_id);
H5Dclose(dset_id);

dset_id = H5Dcreate(group_id, "py", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
temp_buf_index = 0;
for(size_t i(1); i<grid->nx+1; i++) {
    for(size_t j(1); j<grid->ny+1; j++) {
        for(size_t k(1); k<grid->nz+1; k++) {
            temp_buf[temp_buf_index] = hydro(i,j,k).py;
            temp_buf_index = temp_buf_index + 1;
}}} 
dataspace_id = H5Dget_space (dset_id);
H5Sselect_hyperslab (dataspace_id, H5S_SELECT_SET, global_offset, NULL, global_count, NULL);
H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, dataspace_id, plist_id, temp_buf);
H5Sclose(dataspace_id);
H5Dclose(dset_id);

dset_id = H5Dcreate(group_id, "pz", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
temp_buf_index = 0;
for(size_t i(1); i<grid->nx+1; i++) {
    for(size_t j(1); j<grid->ny+1; j++) {
	for(size_t k(1); k<grid->nz+1; k++) {
            temp_buf[temp_buf_index] = hydro(i,j,k).pz;
            temp_buf_index = temp_buf_index + 1;
}}} 
dataspace_id = H5Dget_space (dset_id);
H5Sselect_hyperslab (dataspace_id, H5S_SELECT_SET, global_offset, NULL, global_count, NULL);
H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, dataspace_id, plist_id, temp_buf);
H5Sclose(dataspace_id);
H5Dclose(dset_id);

el2 = uptime() - el2;
sim_log("TimeHDF5Write: "<< el2 << " s"); 

double el3 = uptime();

free(temp_buf);
H5Sclose(filespace);
H5Sclose(memspace);
H5Pclose(plist_id);
H5Gclose(group_id);
H5Fclose(file_id);

el3 = uptime() - el3;
sim_log("TimeHDF5Close: "<< el3 << " s"); 


if(mpi_rank == 0){



const char *header = "<?xml version=\"1.0\"?>\n<!DOCTYPE Xdmf SYSTEM \"Xdmf.dtd\" []>\n<Xdmf xmlns:xi=\"http://www.w3.org/2001/XInclude\" Version=\"2.0\">\n\t<Domain>\n";
const char *header_topology = "\t\t<Topology Dimensions=\"%s\" TopologyType=\"3DCoRectMesh\" name=\"topo\"/>\n";
const char *header_geom  ="\t\t<Geometry Type=\"ORIGIN_DXDYDZ\" name=\"geo\">\n";
const char *header_origin = "\t\t\t<!-- Origin --> \n\t\t\t<DataItem Dimensions=\"3\" Format=\"XML\">%s</DataItem>\n";
const char *header_dxdydz = "\t\t\t<!-- DxDyDz --> \n\t\t\t<DataItem Dimensions=\"3\" Format=\"XML\">%s</DataItem>\n";
const char *footer_geom  ="\t\t</Geometry>\n";
const char *grid_line = "\t\t<Grid CollectionType=\"Temporal\" GridType=\"Collection\" Name=\"TimeSeries\"> \n \
\t\t\t<Time TimeType=\"HyperSlab\"> \n \
\t\t\t\t<DataItem Dimensions=\"%d\" Format=\"XML\" NumberType=\"Float\">";
const char *grid_line_footer = "</DataItem> \n\
\t\t\t</Time>\n";
const char *footer = "\t\t</Grid>\n\t</Domain>\n</Xdmf>\n";

const char *main_body = "\t\t\t<Grid GridType=\"Uniform\" Name=\"T%d\"> \n \
\t\t\t\t<Topology Reference=\"/Xdmf/Domain/Topology[1]\"/>   \n \
\t\t\t\t<Geometry Reference=\"/Xdmf/Domain/Geometry[1]\"/>  \n \
\t\t\t\t<Attribute AttributeType=\"Vector\" Center=\"Node\" Name=\"J\">  \n  \
\t\t\t\t\t<DataItem Dimensions=\" %s \" Function=\"JOIN($0, $1, $2)\" ItemType=\"Function\">  \n \
\t\t\t\t\t\t<DataItem ItemType=\"Uniform\" Dimensions=\" %s \" DataType=\"Float\" Precision=\"4\" Format=\"HDF\"> T.%d/hydro_%s_%d.h5:/Timestep_%d/jx </DataItem>  \n \
\t\t\t\t\t\t<DataItem ItemType=\"Uniform\" Dimensions=\" %s \" DataType=\"Float\" Precision=\"4\" Format=\"HDF\"> T.%d/hydro_%s_%d.h5:/Timestep_%d/jy </DataItem> \n \
\t\t\t\t\t\t<DataItem ItemType=\"Uniform\" Dimensions=\" %s \" DataType=\"Float\" Precision=\"4\" Format=\"HDF\"> T.%d/hydro_%s_%d.h5:/Timestep_%d/jz </DataItem>  \n \
\t\t\t\t\t</DataItem>  \n \
\t\t\t\t</Attribute>  \n \
\t\t\t\t<Attribute AttributeType=\"Vector\" Center=\"Node\" Name=\"P\">  \n \
\t\t\t\t\t<DataItem Dimensions=\" %s \" Function=\"JOIN($0, $1, $2)\" ItemType=\"Function\">  \n \
\t\t\t\t\t\t<DataItem ItemType=\"Uniform\" Dimensions=\" %s \" DataType=\"Float\" Precision=\"4\" Format=\"HDF\"> T.%d/hydro_%s_%d.h5:/Timestep_%d/px </DataItem>  \n \
\t\t\t\t\t\t<DataItem ItemType=\"Uniform\" Dimensions=\" %s \" DataType=\"Float\" Precision=\"4\" Format=\"HDF\"> T.%d/hydro_%s_%d.h5:/Timestep_%d/py </DataItem>  \n \
\t\t\t\t\t\t<DataItem ItemType=\"Uniform\" Dimensions=\" %s \" DataType=\"Float\" Precision=\"4\" Format=\"HDF\"> T.%d/hydro_%s_%d.h5:/Timestep_%d/pz </DataItem>  \n \
\t\t\t\t\t</DataItem>  \n \
\t\t\t\t</Attribute>  \n \
\t\t\t\t<Attribute AttributeType=\"Scalar\" Center=\"Node\" Name=\"N\">  \n \
\t\t\t\t\t<DataItem ItemType=\"Uniform\" Dimensions=\" %s \" DataType=\"Float\" Precision=\"4\" Format=\"HDF\"> T.%d/hydro_%s_%d.h5:/Timestep_%d/rho </DataItem>  \n \
\t\t\t\t</Attribute>  \n \
\t\t\t</Grid>  \n";

#define create_file_with_header(xml_file_name, dimensions, orignal, dxdydz, nframes, fields_interval) ({\
    FILE * fp;\
    fp = fopen(xml_file_name,"w");\
    fputs(header, fp);\
    fprintf(fp, header_topology, dimensions);\
    fputs(header_geom, fp);\
    fprintf(fp, header_origin, orignal);\
    fprintf(fp, header_dxdydz, dxdydz);\
    fputs(footer_geom, fp);\
    fprintf(fp, grid_line, nframes);\
    int i;\
    for ( i = 0; i < nframes; i++ )\
        fprintf(fp, "%d ", i*fields_interval);\
    fputs(grid_line_footer, fp);\
    fclose (fp);\
})


#define invert_xml_item(xml_file_name, speciesname, time_step, dims_4d, dims_3d,  add_footer_flag) ({\
    FILE * fp;\
    fp = fopen(xml_file_name,"a");\
    fprintf(fp, main_body,time_step, dims_4d, dims_3d, time_step, speciesname, time_step, time_step,\
                                              dims_3d, time_step, speciesname, time_step, time_step,\
                                              dims_3d, time_step, speciesname, time_step, time_step,\
                                     dims_4d, dims_3d, time_step, speciesname, time_step, time_step,\
                                              dims_3d, time_step, speciesname, time_step, time_step,\
                                              dims_3d, time_step, speciesname, time_step, time_step,\
                                              dims_3d, time_step, speciesname, time_step, time_step); \
    if(add_footer_flag) fputs(footer, fp); \
    fclose (fp); \
    })


char output_xml_file[128]; sprintf(output_xml_file, "./%s/%s%s%s", "hydro_hdf5", "hydro-", speciesname, ".xdmf");
char dimensions_3d[128];  sprintf(dimensions_3d, "%lld %lld %lld", hydro_global_size[0],hydro_global_size[1], hydro_global_size[2]);
char dimensions_4d[128];  sprintf(dimensions_4d, "%lld %lld %lld %d", hydro_global_size[0],hydro_global_size[1], hydro_global_size[2], 3);
char orignal[128];    sprintf(orignal, "%f %f %f", grid->x0,grid->y0, grid->z0);
char dxdydz[128];   sprintf(dxdydz, "%f %f %f", grid->dx,grid->dy, grid->dz); 


int  nframes         = num_step/global->fields_interval + 1;
int  fields_interval = global->fields_interval;
static int  tframe = 0 ;

#ifdef DUMP_INFO_DEBUG
printf("         meta file : %s \n", output_xml_file);
printf(" array dims per var: %s \n", dimensions_3d);
printf("array dims all vars: %s \n", dimensions_4d);
printf("            orignal: %s \n", orignal);
printf("             dxdydz: %s \n", dxdydz);
printf("            nframes: %d \n", nframes);
printf("    fields_interval: %d \n", fields_interval);
printf("       current step: %ld \n", grid->step);
printf("    Simulation time: %f \n", grid->t0);
printf("             tframe: %d \n", tframe);
#endif

if(tframe >= 1) {
    if( tframe == (nframes - 1)){
        invert_xml_item(output_xml_file, speciesname, grid->step, dimensions_4d, dimensions_3d, 1);
    }else{
        invert_xml_item(output_xml_file, speciesname, grid->step, dimensions_4d, dimensions_3d, 0);
    }
} else {
    create_file_with_header(output_xml_file, dimensions_3d, orignal,dxdydz, nframes, fields_interval);
    if( tframe == (nframes - 1)){
        invert_xml_item(output_xml_file, speciesname, grid->step, dimensions_4d, dimensions_3d, 1);
    }else{
        invert_xml_item(output_xml_file, speciesname, grid->step, dimensions_4d, dimensions_3d, 0);
    }
}
tframe++;
}

#undef create_file_with_header
#undef invert_xml_item
