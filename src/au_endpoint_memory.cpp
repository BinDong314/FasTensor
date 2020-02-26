#include "au_endpoint_memory.h"

//see au.h for its definations
extern int au_mpi_size_global;
extern int au_mpi_rank_global;

int EndpointMEMORY::ExtractMeta()
{

    return 0;
}

int EndpointMEMORY::Create()
{
    std::vector<unsigned long> endpoint_dim_size_ul;
    endpoint_dim_size_ul.resize(endpoint_dim_size.size());
    for (int i = 0; i < endpoint_dim_size.size(); i++)
    {
        endpoint_dim_size_ul[i] = endpoint_dim_size[i];
    }
    PrintVector("Create endpoint_dim_size_ul = ", endpoint_dim_size_ul);
    switch (endpoint_dim_size.size())
    {
    case 1:
    {
        CreateDashMatrix(dash_array_p, 1, data_element_type, endpoint_dim_size_ul);
        break;
    }
    case 2:
    {
        CreateDashMatrix(dash_array_p, 2, data_element_type, endpoint_dim_size_ul);
        break;
    }
    case 3:
    {
        CreateDashMatrix(dash_array_p, 3, data_element_type, endpoint_dim_size_ul);
        break;
    }
    default:
        AU_EXIT("Only support until 3D in memory data!");
        break;
    }

    SetOpenFlag(true);

    return 0;
}

int EndpointMEMORY::Open()
{
    AU_INFO("Memory Open");
    return 0;
}

/**
     * @brief read the data from end-point
     * 
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data 
     * @return int < 0 error, >= 0 works
     */
int EndpointMEMORY::Read(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{
    //AU_INFO("Memory read");
    dash::Team::All().barrier();

    if(!local_mirror_flag){
        std::vector<unsigned long> start_ul, end_ul;
        start_ul.resize(start.size());
        end_ul.resize(end.size());

        for (int i = 0; i < start.size(); i++)
        {
            start_ul[i] = start[i];
            end_ul[i] = end[i];
        }
        switch (endpoint_dim_size.size())
        {
        case 1:
        {
            AccessDashData1D(dash_array_p, start_ul, end_ul, data, data_element_type, DASH_READ_FLAG);
            break;
        }
        case 2:
        {
            AccessDashData2D(dash_array_p, start_ul, end_ul, data, data_element_type, DASH_READ_FLAG);
            break;
        }
        case 3:
        {
            AccessDashData3D(dash_array_p, start_ul, end_ul, data, data_element_type, DASH_READ_FLAG);
            break;
        }
        default:
            AU_EXIT("Only support until 3D in memory data!");
            break;
        }
        dash::Team::All().barrier();
        return 0;
    }else{
        float *local_mirror_buffer_typed = (float *) local_mirror_buffer;
        std::memcpy(data, &local_mirror_buffer_typed[start[0]], sizeof(float) );
        return 0;
    }
}

/**
     * @brief write the data to the end-point
     * 
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data 
     * @return int < 0 error, >= 0 works
     */
int EndpointMEMORY::Write(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{

    if(!local_mirror_flag){
        std::vector<unsigned long> start_ul, end_ul;
        start_ul.resize(start.size());
        end_ul.resize(end.size());

        for (int i = 0; i < start.size(); i++)
        {
            start_ul[i] = start[i];
            end_ul[i] = end[i];
        }


        switch (endpoint_dim_size.size())
        {
        case 1:
            AccessDashData1D(dash_array_p, start_ul, end_ul, data, data_element_type, DASH_WRITE_FLAG);
            break;
        case 2:
            AccessDashData2D(dash_array_p, start_ul, end_ul, data, data_element_type, DASH_WRITE_FLAG);
            break;
        case 3:
            AccessDashData3D(dash_array_p, start_ul, end_ul, data, data_element_type, DASH_WRITE_FLAG);
            break;
        default:
            AU_EXIT("Only support until 3D in memory data!");
            break;
        }
        return 0;
    }else{
        float *local_mirror_buffer_typed = (float *) local_mirror_buffer;
        std::memcpy(&local_mirror_buffer_typed[start[0]],  data, sizeof(float));
        return 0;
    }
}

/**
     * @brief close the end-point
     * 
     * @return int int < 0 error, >= 0 works
     */
int EndpointMEMORY::Close()
{
    AU_INFO("Memory Close");

    return 0;
};

void EndpointMEMORY::EnableCollectiveIO()
{
}

void EndpointMEMORY::DisableCollectiveIO()
{
}

int EndpointMEMORY::PrintInfo()
{
    std::cout << data_endpoint_orig << std::endl;
    return 0;
}

void EndpointMEMORY::Map2MyType()
{
}

int EndpointMEMORY::ParseEndpointInfo()
{
    return 0;
}

int EndpointMEMORY::SpecialOperator(int opt_code, std::string parameter)
{
    int ret =0;
    switch (opt_code)
    {
    case DASH_NONVOLATILE_CODE:
        ret = Nonvolatile(parameter);
        break;
    case DASH_VOLATILE_CODE:
        ret = Volatile(parameter);
        break;
    case DASH_ENABLE_LOCAL_MIRROR_CODE:
        local_mirror_flag = true;
        ret = CreateLocalMirror();
        break;
    case DASH_MERGE_MIRRORS_CODE:
        ret = MergeMirrors();
        break;
    default:
        AU_EXIT("Not supported operator on MEMORY endpoint");
        break;
    }

    return ret;
}

/**
     * @brief Nonvolatile the data in MEMORY to HDF5
     * 
     * @param parameter 
     * @return int 
     */
int EndpointMEMORY::Nonvolatile(std::string parameter)
{
    hdf5_options fopts;
	fopts.overwrite_file = false; // Do not overwrite existing file
    std::string fn_str, path_str;
    std::stringstream ss(parameter);
    if (!std::getline(ss, fn_str, ':'))
    {
        AU_EXIT("Invalued endpoint_info");
    }

    if (!std::getline(ss, path_str, ':'))
    {
        AU_EXIT("Invalued endpoint_info");
    }
    switch (endpoint_dim_size.size())
    {
       case 1:
       {
            if (data_element_type == AU_SHORT)                                                                                                             
            {                                                                                                                                                
                dash::Matrix<short, 1, unsigned long> *dash_array_typed = (dash::Matrix<short, 1, unsigned long> *)dash_array_p;                           
                StoreHDF::write(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_INT)                                                                                                          
            {                                                                                                                                                
                dash::Matrix<int, 1, unsigned long> *dash_array_typed = (dash::Matrix<int, 1, unsigned long> *)dash_array_p;                               
                StoreHDF::write(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                                                                                                                                                                     \
            }                                                                                                                                                
            else if (data_element_type == AU_LONG)                                                                                                         
            {                                                                                                                                                
                dash::Matrix<long, 1, unsigned long> *dash_array_typed = (dash::Matrix<long, 1, unsigned long> *)dash_array_p;                             
                StoreHDF::write(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_LONG_LONG)                                                                                                    
            {                                                                                                                                                
                dash::Matrix<long long, 1, unsigned long> *dash_array_typed = (dash::Matrix<long long, 1, unsigned long> *)dash_array_p;                   
                StoreHDF::write(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_USHORT)                                                                                                       
            {                                                                                                                                                
                dash::Matrix<unsigned short, 1, unsigned long> *dash_array_typed = (dash::Matrix<unsigned short, 1, unsigned long> *)dash_array_p;         
                StoreHDF::write(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_UINT)                                                                                                         
            {                                                                                                                                                
                dash::Matrix<unsigned int, 1, unsigned long> *dash_array_typed = (dash::Matrix<unsigned int, 1, unsigned long> *)dash_array_p;             
                StoreHDF::write(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_ULONG)                                                                                                        
            {                                                                                                                                                
                dash::Matrix<unsigned long, 1, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long, 1, unsigned long> *)dash_array_p;           
                StoreHDF::write(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_ULLONG)                                                                                                       
            {                                                                                                                                                
                dash::Matrix<unsigned long long, 1, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long long, 1, unsigned long> *)dash_array_p; 
                StoreHDF::write(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                                                                                                                                                           
            }                                                                                                                                                
            else if (data_element_type == AU_FLOAT)                                                                                                        
            {                                                                                                                                                
                dash::Matrix<float, 1, unsigned long> *dash_array_typed = (dash::Matrix<float, 1, unsigned long> *)dash_array_p;                           
                StoreHDF::write(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                                                                                                                                                              
            }                                                                                                                                                
            else if (data_element_type == AU_DOUBLE)                                                                                                       
            {                                                                                                                                               
                dash::Matrix<double, 1, unsigned long> *dash_array_typed = (dash::Matrix<double, 1, unsigned long> *)dash_array_p;                         
                StoreHDF::write(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                                                                                                                                                                    \
            }                                                                                                                                                
            else                                                                                                                                             
            {                                                                                                                                                
                AU_EXIT("Unsupported datatype in AccessDashData !");                                                                                         
            }   
            break;
        }         
    case 2:
    {
         if (data_element_type == AU_SHORT)                                                                                                             
        {                                                                                                                                                
            dash::Matrix<short, 2, unsigned long> *dash_array_typed = (dash::Matrix<short, 2, unsigned long> *)dash_array_p;                           
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_INT)                                                                                                          
        {                                                                                                                                                
            dash::Matrix<int, 2, unsigned long> *dash_array_typed = (dash::Matrix<int, 2, unsigned long> *)dash_array_p;                               
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                                                                                                                                                                     \
        }                                                                                                                                                
        else if (data_element_type == AU_LONG)                                                                                                         
        {                                                                                                                                                
            dash::Matrix<long, 2, unsigned long> *dash_array_typed = (dash::Matrix<long, 2, unsigned long> *)dash_array_p;                             
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_LONG_LONG)                                                                                                    
        {                                                                                                                                                
            dash::Matrix<long long, 2, unsigned long> *dash_array_typed = (dash::Matrix<long long, 2, unsigned long> *)dash_array_p;                   
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_USHORT)                                                                                                       
        {                                                                                                                                                
            dash::Matrix<unsigned short, 2, unsigned long> *dash_array_typed = (dash::Matrix<unsigned short, 2, unsigned long> *)dash_array_p;         
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_UINT)                                                                                                         
        {                                                                                                                                                
            dash::Matrix<unsigned int, 2, unsigned long> *dash_array_typed = (dash::Matrix<unsigned int, 2, unsigned long> *)dash_array_p;             
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                          
         }                                                                                                                                                
        else if (data_element_type == AU_ULONG)                                                                                                        
        {                                                                                                                                                
            dash::Matrix<unsigned long, 2, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long, 2, unsigned long> *)dash_array_p;           
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_ULLONG)                                                                                                       
        {                                                                                                                                                
            dash::Matrix<unsigned long long, 2, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long long, 2, unsigned long> *)dash_array_p; 
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                                                                                                                                                           
        }                                                                                                                                                
        else if (data_element_type == AU_FLOAT)                                                                                                        
        {                                                                                                                                                
            dash::Matrix<float, 2, unsigned long> *dash_array_typed = (dash::Matrix<float, 2, unsigned long> *)dash_array_p;                           
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                                                                                                                                                              
        }                                                                                                                                                
        else if (data_element_type == AU_DOUBLE)                                                                                                       
        {                                                                                                                                               
            dash::Matrix<double, 2, unsigned long> *dash_array_typed = (dash::Matrix<double, 2, unsigned long> *)dash_array_p;                         
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                                                                                                                                                                    \
        }                                                                                                                                                
        else                                                                                                                                             
        {                                                                                                                                                
            AU_EXIT("Unsupported datatype in AccessDashData !");                                                                                         
        }   
        break;
    }
    case 3:
    {
         if (data_element_type == AU_SHORT)                                                                                                             
        {                                                                                                                                                
            dash::Matrix<short, 3, unsigned long> *dash_array_typed = (dash::Matrix<short, 3, unsigned long> *)dash_array_p;                           
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_INT)                                                                                                          
        {                                                                                                                                                
            dash::Matrix<int, 3, unsigned long> *dash_array_typed = (dash::Matrix<int, 3, unsigned long> *)dash_array_p;                               
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                                                                                                                                                                     \
        }                                                                                                                                                
        else if (data_element_type == AU_LONG)                                                                                                         
        {                                                                                                                                                
            dash::Matrix<long, 3, unsigned long> *dash_array_typed = (dash::Matrix<long, 3, unsigned long> *)dash_array_p;                             
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                          
                                                                                                                                           
        }                                                                                                                                                
        else if (data_element_type == AU_LONG_LONG)                                                                                                    
        {                                                                                                                                                
            dash::Matrix<long long, 3, unsigned long> *dash_array_typed = (dash::Matrix<long long, 3, unsigned long> *)dash_array_p;                   
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                          
                                                                                                                                           
        }                                                                                                                                                
        else if (data_element_type == AU_USHORT)                                                                                                       
        {                                                                                                                                                
            dash::Matrix<unsigned short, 3, unsigned long> *dash_array_typed = (dash::Matrix<unsigned short, 3, unsigned long> *)dash_array_p;         
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                          
                                                                                                                                           
        }                                                                                                                                                
        else if (data_element_type == AU_UINT)                                                                                                         
        {                                                                                                                                                
            dash::Matrix<unsigned int, 3, unsigned long> *dash_array_typed = (dash::Matrix<unsigned int, 3, unsigned long> *)dash_array_p;             
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                          
                                                                                                                                           
        }                                                                                                                                                
        else if (data_element_type == AU_ULONG)                                                                                                        
        {                                                                                                                                                
            dash::Matrix<unsigned long, 3, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long, 3, unsigned long> *)dash_array_p;           
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                          
                                                                                                                                           
        }                                                                                                                                                
        else if (data_element_type == AU_ULLONG)                                                                                                       
        {                                                                                                                                                
            dash::Matrix<unsigned long long, 3, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long long, 3, unsigned long> *)dash_array_p; 
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                          
                                                                                                                                           
        }                                                                                                                                                
        else if (data_element_type == AU_FLOAT)                                                                                                        
        {                                                                                                                                                
            dash::Matrix<float, 3, unsigned long> *dash_array_typed = (dash::Matrix<float, 3, unsigned long> *)dash_array_p;                           
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                          
                                                                                                                                           
        }                                                                                                                                                
        else if (data_element_type == AU_DOUBLE)                                                                                                       
        {                                                                                                                                               
            dash::Matrix<double, 3, unsigned long> *dash_array_typed = (dash::Matrix<double, 3, unsigned long> *)dash_array_p;                         
            StoreHDF::write(*dash_array_typed, fn_str, path_str);                                                                                                                                                                                                                                                                                                                                                                    \
        }                                                                                                                                                
        else                                                                                                                                             
        {                                                                                                                                                
            AU_EXIT("Unsupported datatype in AccessDashData !");                                                                                         
        }   
        break; 
    }       
    default:
        AU_EXIT("Only support until 3D in memory data!");
        break;
    }

    //DashHDF5Transfer2D(dash_array_p, data_element_type, DASH_WRITE_FLAG, fn_str, path_str);

    //dash::Matrix<float, 2, unsigned long> *dash_array_typed = (dash::Matrix<float, 2, unsigned long> *)dash_array_p;
    //dash::Matrix<float, 2, unsigned long> &dash_array_typed_ref = *dash_array_typed;
    //StoreHDF::write(*dash_array_typed, fn_str, path_str);

    return 0;
}

/**
     * @brief Volatile the data from HDF5 to MEMORY
     * 
     * @param parameter 
     * @return int 
     */
int EndpointMEMORY::Volatile(std::string parameter)
{

    std::string fn_str, path_str;
    std::stringstream ss(parameter);
    if (!std::getline(ss, fn_str, ':'))
    {
        AU_EXIT("Invalued endpoint_info");
    }

    if (!std::getline(ss, path_str, ':'))
    {
        AU_EXIT("Invalued endpoint_info");
    }

    /*
    switch (endpoint_dim_size.size())
    {
    case 1:
        DashHDF5Transfer1D(dash_array_p, data_element_type, DASH_READ_FLAG, fn_str, path_str);
        break;
    case 2:
        DashHDF5Transfer2D(dash_array_p, data_element_type, DASH_READ_FLAG, fn_str, path_str);
        break;
    case 3:
        DashHDF5Transfer3D(dash_array_p, data_element_type, DASH_READ_FLAG, fn_str, path_str);
        break;
    default:
        AU_EXIT("Only support until 3D in memory data!");
        break;
    }*/

    dash::Matrix<float, 2> *dash_array_typed = (dash::Matrix<float, 2> *)dash_array_p;
    dash::Matrix<float, 2> &dash_array_typed_ref = *dash_array_typed;

    StoreHDF::read(dash_array_typed_ref, fn_str, fn_str);

    return 0;
}



  /**
     * @brief Merger mirrors on all processes
     * 
     * @return int 
     */
int EndpointMEMORY::MergeMirrors(){
    void *reduced_mirror_buffer;
    if(!au_mpi_rank_global){
        reduced_mirror_buffer = (float *)malloc( local_mirror_size * sizeof(float));
    }
    MPI_Reduce(&local_mirror_buffer, &reduced_mirror_buffer, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(!au_mpi_rank_global){
        std::vector<unsigned long> start_ul, end_ul;
        start_ul.resize(endpoint_dim_size.size());
        end_ul.resize(endpoint_dim_size.size());

        for (int i = 0; i < endpoint_dim_size.size(); i++)
        {
            start_ul[i] = 0;
            end_ul[i] = endpoint_dim_size[i];
        }
        AccessDashData1D(dash_array_p, start_ul, end_ul, local_mirror_buffer, data_element_type, DASH_WRITE_FLAG);
    }
}

    /**
     * @brief Create a Local Mirror object
     * 
     * @return int 
     */
int EndpointMEMORY::CreateLocalMirror(){
    local_mirror_size = 1;
    for (int i = 0; i < endpoint_dim_size.size(); i++){
        local_mirror_size = local_mirror_size * endpoint_dim_size[i];
    }
    local_mirror_buffer = (float *)malloc( local_mirror_size * sizeof(float));                                                                                                       
}
