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
    //PrintVector("Create endpoint_dim_size_ul = ", endpoint_dim_size_ul);
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
    SetCreateFlag(true);
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
    //dash::Team::All().barrier();
    if (!GetCreateFlag())
    {
        Create();
        std::cout << "Create in Read !\n";
    }

    if (!local_mirror_flag)
    {
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
    }
    else
    {
        switch (data_element_type)
        {
        case AU_SHORT:
        {
            AccessLocalMirrorHelp<short>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_READ_FLAG);
            break;
        }
        case AU_INT:
        {
            AccessLocalMirrorHelp<int>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_READ_FLAG);
            break;
        }
        case AU_LONG:
        {
            AccessLocalMirrorHelp<long>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_READ_FLAG);
            break;
        }
        case AU_LONG_LONG:
        {
            AccessLocalMirrorHelp<long long>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_READ_FLAG);
            break;
        }
        case AU_USHORT:
        {
            AccessLocalMirrorHelp<unsigned short>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_READ_FLAG);
            break;
        }
        case AU_UINT:
        {
            AccessLocalMirrorHelp<unsigned int>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_READ_FLAG);
            break;
        }
        case AU_ULONG:
        {
            AccessLocalMirrorHelp<unsigned long>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_READ_FLAG);
            break;
        }
        case AU_ULLONG:
        {
            AccessLocalMirrorHelp<unsigned long long>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_READ_FLAG);
            break;
        }
        case AU_FLOAT:
        {
            AccessLocalMirrorHelp<float>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_READ_FLAG);
            break;
        }
        case AU_DOUBLE:
        {
            AccessLocalMirrorHelp<double>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_READ_FLAG);
            break;
        }
        case AU_DOUBLE_COMPLEX:
        {
            AccessLocalMirrorHelp<std::complex<double>>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_READ_FLAG);
            break;
        }
        default:
        {
            AU_EXIT("Not supported type in memory data!");
            break;
        }
        }

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
    //dash::Team::All().barrier();

    if (!GetCreateFlag())
    {
        Create();
        std::cout << "\nCreate in write !\n";
    }

    if (!local_mirror_flag)
    {
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
    }
    else
    {
        switch (data_element_type)
        {
        case AU_SHORT:
        {
            AccessLocalMirrorHelp<short>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_WRITE_FLAG);
            break;
        }
        case AU_INT:
        {
            AccessLocalMirrorHelp<int>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_WRITE_FLAG);
            break;
        }
        case AU_LONG:
        {
            AccessLocalMirrorHelp<long>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_WRITE_FLAG);
            break;
        }
        case AU_LONG_LONG:
        {
            AccessLocalMirrorHelp<long long>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_WRITE_FLAG);
            break;
        }
        case AU_USHORT:
        {
            AccessLocalMirrorHelp<unsigned short>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_WRITE_FLAG);
            break;
        }
        case AU_UINT:
        {
            AccessLocalMirrorHelp<unsigned int>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_WRITE_FLAG);
            break;
        }
        case AU_ULONG:
        {
            AccessLocalMirrorHelp<unsigned long>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_WRITE_FLAG);
            break;
        }
        case AU_ULLONG:
        {
            AccessLocalMirrorHelp<unsigned long long>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_WRITE_FLAG);
            break;
        }
        case AU_FLOAT:
        {
            AccessLocalMirrorHelp<float>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_WRITE_FLAG);
            break;
        }
        case AU_DOUBLE:
        {
            AccessLocalMirrorHelp<double>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_WRITE_FLAG);
            break;
        }
        case AU_DOUBLE_COMPLEX:
        {
            AccessLocalMirrorHelp<std::complex<double>>(local_mirror_buffer, endpoint_dim_size, start, end, data, LOCAL_MIRROR_WRITE_FLAG);
            break;
        }
        default:
        {
            AU_EXIT("Not supported type in memory data!");
            break;
        }
        }
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
    //AU_INFO("Memory Close");

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
    int ret = 0;
    switch (opt_code)
    {
    case DASH_NONVOLATILE_CODE:
        std::cout << "Call Nonvolatile \n ";
        ret = Nonvolatile(parameter);
        break;
    case DASH_VOLATILE_CODE:
        ret = Volatile(parameter);
        break;
    case DASH_ENABLE_LOCAL_MIRROR_CODE:
        local_mirror_flag = true;
        ret = CreateLocalMirror(parameter);
        break;
    case DASH_MERGE_MIRRORS_CODE:
        ret = MergeMirrors(parameter);
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
    fopts.overwrite_file = true; // Do not overwrite existing file
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

    std::cout << "Nonvolatile \n";
    if (!au_mpi_rank_global)
    {
        std::vector<unsigned long long> start(endpoint_dim_size.size()), end(endpoint_dim_size.size());

        for (int i = 0; i < endpoint_dim_size.size(); i++)
        {
            start[i] = 0;
            end[i] = endpoint_dim_size[i] - 1;
        }
        //Pass 1 to require no Collective IO, i.e. no MPI
        //Todo: need a better way to do that
        //This is needed when it called only from on processes
        Endpoint *sub_endpoint = new EndpointHDF5(1);
        sub_endpoint->SpecialOperator(OP_DISABLE_MPI_IO, "");
        sub_endpoint->SpecialOperator(OP_DISABLE_COLLECTIVE_IO, "");

        std::cout << "Nonvolatile : after OP_DISABLE_COLLECTIVE_IO \n";
        sub_endpoint->SetDataElementType(data_element_type);
        sub_endpoint->SetDimensions(endpoint_dim_size);
        sub_endpoint->SetEndpointInfo(parameter);

        std::cout << sub_endpoint->GetEndpointInfo() << "\n";
        PrintVector("endpoint_dim_size", endpoint_dim_size);
        PrintVector("start =", start);
        PrintVector("end   =", end);

        //std::cout << "Create pre \n";
        sub_endpoint->Create();
        //std::cout << "Write pre \n";
        sub_endpoint->Write(start, end, local_mirror_buffer);
    }

    return 0;

    /*
    //if(!local_mirror_flag){
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
                StoreHDF::write(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_INT)                                                                                                          
            {                                                                                                                                                
                dash::Matrix<int, 2, unsigned long> *dash_array_typed = (dash::Matrix<int, 2, unsigned long> *)dash_array_p;                               
                StoreHDF::write(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                                                                                                                                                                     \
            }                                                                                                                                                
            else if (data_element_type == AU_LONG)                                                                                                         
            {                                                                                                                                                
                dash::Matrix<long, 2, unsigned long> *dash_array_typed = (dash::Matrix<long, 2, unsigned long> *)dash_array_p;                             
                StoreHDF::write(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_LONG_LONG)                                                                                                    
            {                                                                                                                                                
                dash::Matrix<long long, 2, unsigned long> *dash_array_typed = (dash::Matrix<long long, 2, unsigned long> *)dash_array_p;                   
                StoreHDF::write(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_USHORT)                                                                                                       
            {                                                                                                                                                
                dash::Matrix<unsigned short, 2, unsigned long> *dash_array_typed = (dash::Matrix<unsigned short, 2, unsigned long> *)dash_array_p;         
                StoreHDF::write(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_UINT)                                                                                                         
            {                                                                                                                                                
                dash::Matrix<unsigned int, 2, unsigned long> *dash_array_typed = (dash::Matrix<unsigned int, 2, unsigned long> *)dash_array_p;             
                StoreHDF::write(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_ULONG)                                                                                                        
            {                                                                                                                                                
                dash::Matrix<unsigned long, 2, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long, 2, unsigned long> *)dash_array_p;           
                StoreHDF::write(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_ULLONG)                                                                                                       
            {                                                                                                                                                
                dash::Matrix<unsigned long long, 2, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long long, 2, unsigned long> *)dash_array_p; 
                StoreHDF::write(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                                                                                                                                                           
            }                                                                                                                                                
            else if (data_element_type == AU_FLOAT)                                                                                                        
            {                                                                                                                                                
                dash::Matrix<float, 2, unsigned long> *dash_array_typed = (dash::Matrix<float, 2, unsigned long> *)dash_array_p;                           
                StoreHDF::write(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                                                                                                                                                              
            }                                                                                                                                                
            else if (data_element_type == AU_DOUBLE)                                                                                                       
            {                                                                                                                                               
                dash::Matrix<double, 2, unsigned long> *dash_array_typed = (dash::Matrix<double, 2, unsigned long> *)dash_array_p;                         
                StoreHDF::write(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                                                                                                                                                                    \
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
                StoreHDF::write(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_INT)                                                                                                          
            {                                                                                                                                                
                dash::Matrix<int, 3, unsigned long> *dash_array_typed = (dash::Matrix<int, 3, unsigned long> *)dash_array_p;                               
                StoreHDF::write(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                                                                                                                                                                     \
            }                                                                                                                                                
            else if (data_element_type == AU_LONG)                                                                                                         
            {                                                                                                                                                
                dash::Matrix<long, 3, unsigned long> *dash_array_typed = (dash::Matrix<long, 3, unsigned long> *)dash_array_p;                             
                StoreHDF::write(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_LONG_LONG)                                                                                                    
            {                                                                                                                                                
                dash::Matrix<long long, 3, unsigned long> *dash_array_typed = (dash::Matrix<long long, 3, unsigned long> *)dash_array_p;                   
                StoreHDF::write(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_USHORT)                                                                                                       
            {                                                                                                                                                
                dash::Matrix<unsigned short, 3, unsigned long> *dash_array_typed = (dash::Matrix<unsigned short, 3, unsigned long> *)dash_array_p;         
                StoreHDF::write(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_UINT)                                                                                                         
            {                                                                                                                                                
                dash::Matrix<unsigned int, 3, unsigned long> *dash_array_typed = (dash::Matrix<unsigned int, 3, unsigned long> *)dash_array_p;             
                StoreHDF::write(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_ULONG)                                                                                                        
            {                                                                                                                                                
                dash::Matrix<unsigned long, 3, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long, 3, unsigned long> *)dash_array_p;           
                StoreHDF::write(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_ULLONG)                                                                                                       
            {                                                                                                                                                
                dash::Matrix<unsigned long long, 3, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long long, 3, unsigned long> *)dash_array_p; 
                StoreHDF::write(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_FLOAT)                                                                                                        
            {                                                                                                                                                
                dash::Matrix<float, 3, unsigned long> *dash_array_typed = (dash::Matrix<float, 3, unsigned long> *)dash_array_p;                           
                StoreHDF::write(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_DOUBLE)                                                                                                       
            {                                                                                                                                               
                dash::Matrix<double, 3, unsigned long> *dash_array_typed = (dash::Matrix<double, 3, unsigned long> *)dash_array_p;                         
                StoreHDF::write(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                                                                                                                                                                    \
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
  */

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
    hdf5_options fopts;
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

    AU_EXIT("Not supported at this time because the error of DASH\n");
    /*
    switch (endpoint_dim_size.size())
    {
       case 1:
       { 
            if (data_element_type == AU_SHORT)                                                                                                             
            {                                                                                                                                                
                dash::Matrix<short, 1, unsigned long> *dash_array_typed = (dash::Matrix<short, 1, unsigned long> *)dash_array_p;                           
                StoreHDF::read(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_INT)                                                                                                          
            {                                                                                                                                                
                dash::Matrix<int, 1, unsigned long> *dash_array_typed = (dash::Matrix<int, 1, unsigned long> *)dash_array_p;                               
                StoreHDF::read(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                                                                                                                                                                     \
            }                                                                                                                                                
            else if (data_element_type == AU_LONG)                                                                                                         
            {                                                                                                                                                
                dash::Matrix<long, 1, unsigned long> *dash_array_typed = (dash::Matrix<long, 1, unsigned long> *)dash_array_p;                             
                StoreHDF::read(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_LONG_LONG)                                                                                                    
            {                                                                                                                                                
                dash::Matrix<long long, 1, unsigned long> *dash_array_typed = (dash::Matrix<long long, 1, unsigned long> *)dash_array_p;                   
                StoreHDF::read(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_USHORT)                                                                                                       
            {                                                                                                                                                
                dash::Matrix<unsigned short, 1, unsigned long> *dash_array_typed = (dash::Matrix<unsigned short, 1, unsigned long> *)dash_array_p;         
                StoreHDF::read(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_UINT)                                                                                                         
            {                                                                                                                                                
                dash::Matrix<unsigned int, 1, unsigned long> *dash_array_typed = (dash::Matrix<unsigned int, 1, unsigned long> *)dash_array_p;             
                StoreHDF::read(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_ULONG)                                                                                                        
            {                                                                                                                                                
                dash::Matrix<unsigned long, 1, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long, 1, unsigned long> *)dash_array_p;           
                StoreHDF::read(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
            }                                                                                                                                                
            else if (data_element_type == AU_ULLONG)                                                                                                       
            {                                                                                                                                                
                dash::Matrix<unsigned long long, 1, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long long, 1, unsigned long> *)dash_array_p; 
                StoreHDF::read(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                                                                                                                                                           
            }                                                                                                                                                
            else if (data_element_type == AU_FLOAT)                                                                                                        
            {                                                                                                                                                
                dash::Matrix<float, 1, unsigned long> *dash_array_typed = (dash::Matrix<float, 1, unsigned long> *)dash_array_p;                           
                StoreHDF::read(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                                                                                                                                                              
            }                                                                                                                                                
            else if (data_element_type == AU_DOUBLE)                                                                                                       
            {                                                                                                                                               
                dash::Matrix<double, 1, unsigned long> *dash_array_typed = (dash::Matrix<double, 1, unsigned long> *)dash_array_p;                         
                StoreHDF::read(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                                                                                                                                                                    \
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
            StoreHDF::read(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_INT)                                                                                                          
        {                                                                                                                                                
            dash::Matrix<int, 2, unsigned long> *dash_array_typed = (dash::Matrix<int, 2, unsigned long> *)dash_array_p;                               
            StoreHDF::read(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                                                                                                                                                                     \
        }                                                                                                                                                
        else if (data_element_type == AU_LONG)                                                                                                         
        {                                                                                                                                                
            dash::Matrix<long, 2, unsigned long> *dash_array_typed = (dash::Matrix<long, 2, unsigned long> *)dash_array_p;                             
            StoreHDF::read(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_LONG_LONG)                                                                                                    
        {                                                                                                                                                
            dash::Matrix<long long, 2, unsigned long> *dash_array_typed = (dash::Matrix<long long, 2, unsigned long> *)dash_array_p;                   
            StoreHDF::read(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_USHORT)                                                                                                       
        {                                                                                                                                                
            dash::Matrix<unsigned short, 2, unsigned long> *dash_array_typed = (dash::Matrix<unsigned short, 2, unsigned long> *)dash_array_p;         
            StoreHDF::read(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_UINT)                                                                                                         
        {                                                                                                                                                
            dash::Matrix<unsigned int, 2, unsigned long> *dash_array_typed = (dash::Matrix<unsigned int, 2, unsigned long> *)dash_array_p;             
            StoreHDF::read(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
         }                                                                                                                                                
        else if (data_element_type == AU_ULONG)                                                                                                        
        {                                                                                                                                                
            dash::Matrix<unsigned long, 2, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long, 2, unsigned long> *)dash_array_p;           
            StoreHDF::read(*dash_array_typed, fn_str, path_str, fopts);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_ULLONG)                                                                                                       
        {                                                                                                                                                
            dash::Matrix<unsigned long long, 2, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long long, 2, unsigned long> *)dash_array_p; 
            StoreHDF::read(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                                                                                                                                                           
        }                                                                                                                                                
        else if (data_element_type == AU_FLOAT)                                                                                                        
        {                                                                                                                                                
            dash::Matrix<float, 2, unsigned long> *dash_array_typed = (dash::Matrix<float, 2, unsigned long> *)dash_array_p;                           
            StoreHDF::read(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                                                                                                                                                              
        }                                                                                                                                                
        else if (data_element_type == AU_DOUBLE)                                                                                                       
        {                                                                                                                                               
            dash::Matrix<double, 2, unsigned long> *dash_array_typed = (dash::Matrix<double, 2, unsigned long> *)dash_array_p;                         
            StoreHDF::read(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                                                                                                                                                                    \
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
            StoreHDF::read(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_INT)                                                                                                          
        {                                                                                                                                                
            dash::Matrix<int, 3, unsigned long> *dash_array_typed = (dash::Matrix<int, 3, unsigned long> *)dash_array_p;                               
            StoreHDF::read(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                                                                                                                                                                     \
        }                                                                                                                                                
        else if (data_element_type == AU_LONG)                                                                                                         
        {                                                                                                                                                
            dash::Matrix<long, 3, unsigned long> *dash_array_typed = (dash::Matrix<long, 3, unsigned long> *)dash_array_p;                             
            StoreHDF::read(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_LONG_LONG)                                                                                                    
        {                                                                                                                                                
            dash::Matrix<long long, 3, unsigned long> *dash_array_typed = (dash::Matrix<long long, 3, unsigned long> *)dash_array_p;                   
            StoreHDF::read(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_USHORT)                                                                                                       
        {                                                                                                                                                
            dash::Matrix<unsigned short, 3, unsigned long> *dash_array_typed = (dash::Matrix<unsigned short, 3, unsigned long> *)dash_array_p;         
            StoreHDF::read(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_UINT)                                                                                                         
        {                                                                                                                                                
            dash::Matrix<unsigned int, 3, unsigned long> *dash_array_typed = (dash::Matrix<unsigned int, 3, unsigned long> *)dash_array_p;             
            StoreHDF::read(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_ULONG)                                                                                                        
        {                                                                                                                                                
            dash::Matrix<unsigned long, 3, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long, 3, unsigned long> *)dash_array_p;           
            StoreHDF::read(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_ULLONG)                                                                                                       
        {                                                                                                                                                
            dash::Matrix<unsigned long long, 3, unsigned long> *dash_array_typed = (dash::Matrix<unsigned long long, 3, unsigned long> *)dash_array_p; 
            StoreHDF::read(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_FLOAT)                                                                                                        
        {                                                                                                                                                
            dash::Matrix<float, 3, unsigned long> *dash_array_typed = (dash::Matrix<float, 3, unsigned long> *)dash_array_p;                           
            StoreHDF::read(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                          
        }                                                                                                                                                
        else if (data_element_type == AU_DOUBLE)                                                                                                       
        {                                                                                                                                               
            dash::Matrix<double, 3, unsigned long> *dash_array_typed = (dash::Matrix<double, 3, unsigned long> *)dash_array_p;                         
            StoreHDF::read(*dash_array_typed, fn_str, path_str,fopts);                                                                                                                                                                                                                                                                                                                                                                    \
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
    */

    return 0;
}

/**
     * @brief Merger mirrors on all processes
     * 
     * @return int 
     */
int EndpointMEMORY::MergeMirrors(std::string op_str)
{
    void *reduced_mirror_buffer;

    switch (data_element_type)
    {
    case AU_SHORT:
    {
        reduced_mirror_buffer = MergeMirrorsHelp<short>(local_mirror_buffer, local_mirror_size, op_str);
        break;
    }
    case AU_INT:
    {
        reduced_mirror_buffer = MergeMirrorsHelp<int>(local_mirror_buffer, local_mirror_size, op_str);
        break;
    }
    case AU_LONG:
    {
        reduced_mirror_buffer = MergeMirrorsHelp<long>(local_mirror_buffer, local_mirror_size, op_str);
        break;
    }
    case AU_LONG_LONG:
    {
        reduced_mirror_buffer = MergeMirrorsHelp<long long>(local_mirror_buffer, local_mirror_size, op_str);
        break;
    }
    case AU_USHORT:
    {
        reduced_mirror_buffer = MergeMirrorsHelp<unsigned short>(local_mirror_buffer, local_mirror_size, op_str);
        break;
    }
    case AU_UINT:
    {
        reduced_mirror_buffer = MergeMirrorsHelp<unsigned int>(local_mirror_buffer, local_mirror_size, op_str);
        break;
    }
    case AU_ULONG:
    {
        reduced_mirror_buffer = MergeMirrorsHelp<unsigned long>(local_mirror_buffer, local_mirror_size, op_str);
        break;
    }
    case AU_ULLONG:
    {
        reduced_mirror_buffer = MergeMirrorsHelp<unsigned long long>(local_mirror_buffer, local_mirror_size, op_str);
        break;
    }
    case AU_FLOAT:
    {
        reduced_mirror_buffer = MergeMirrorsHelp<float>(local_mirror_buffer, local_mirror_size, op_str);
        break;
    }
    case AU_DOUBLE:
    {
        reduced_mirror_buffer = MergeMirrorsHelp<double>(local_mirror_buffer, local_mirror_size, op_str);
        break;
    }
    case AU_DOUBLE_COMPLEX:
    {
        reduced_mirror_buffer = MergeMirrorsHelp<std::complex<double>>(local_mirror_buffer, local_mirror_size, op_str);
        break;
    }
    default:
    {
        AU_EXIT("Not supported type in memory data!");
        break;
    }
    }

    //if(!au_mpi_rank_global) std::cout << "Pre write merged result to dash disabled\n" ;
    if (!au_mpi_rank_global && false)
    {
        std::vector<unsigned long> start_ul, end_ul;
        start_ul.resize(endpoint_dim_size.size());
        end_ul.resize(endpoint_dim_size.size());

        for (int i = 0; i < endpoint_dim_size.size(); i++)
        {
            start_ul[i] = 0;
            end_ul[i] = endpoint_dim_size[i] - 1;
        }
        //PrintScalar("local_mirror_size = ", local_mirror_size);
        //PrintVector("endpoint_dim_size = ", endpoint_dim_size);

        switch (endpoint_dim_size.size())
        {
        case 1:
            AccessDashData1D(dash_array_p, start_ul, end_ul, reduced_mirror_buffer, data_element_type, DASH_WRITE_FLAG);
            break;
        case 2:
            AccessDashData2D(dash_array_p, start_ul, end_ul, reduced_mirror_buffer, data_element_type, DASH_WRITE_FLAG);
            break;
        case 3:
            AccessDashData3D(dash_array_p, start_ul, end_ul, reduced_mirror_buffer, data_element_type, DASH_WRITE_FLAG);
            break;
        default:
            AU_EXIT("AU_MEMORY only support until 3D data");
            break;
        }
    }

    return 0;
}

/**
     * @brief Create a Local Mirror object (Clone)
     * 
     * @return int 
     */
int EndpointMEMORY::CreateLocalMirror(std::string init_value_str)
{
    local_mirror_size = 1;
    for (int i = 0; i < endpoint_dim_size.size(); i++)
    {
        local_mirror_size = local_mirror_size * endpoint_dim_size[i];
    }

    switch (data_element_type)
    {
    case AU_SHORT:
    {
        local_mirror_buffer = CreateLocalMirrorHelp<short>(init_value_str, local_mirror_size);
        break;
    }
    case AU_INT:
    {
        local_mirror_buffer = CreateLocalMirrorHelp<int>(init_value_str, local_mirror_size);
        break;
    }
    case AU_LONG:
    {
        local_mirror_buffer = CreateLocalMirrorHelp<long>(init_value_str, local_mirror_size);
        break;
    }
    case AU_LONG_LONG:
    {
        local_mirror_buffer = CreateLocalMirrorHelp<long long>(init_value_str, local_mirror_size);
        break;
    }
    case AU_USHORT:
    {
        local_mirror_buffer = CreateLocalMirrorHelp<unsigned short>(init_value_str, local_mirror_size);
        break;
    }
    case AU_UINT:
    {
        local_mirror_buffer = CreateLocalMirrorHelp<unsigned int>(init_value_str, local_mirror_size);
        break;
    }
    case AU_ULONG:
    {
        local_mirror_buffer = CreateLocalMirrorHelp<unsigned long>(init_value_str, local_mirror_size);
        break;
    }
    case AU_ULLONG:
    {
        local_mirror_buffer = CreateLocalMirrorHelp<unsigned long long>(init_value_str, local_mirror_size);
        break;
    }
    case AU_FLOAT:
    {
        local_mirror_buffer = CreateLocalMirrorHelp<float>(init_value_str, local_mirror_size);
        break;
    }
    case AU_DOUBLE:
    {
        local_mirror_buffer = CreateLocalMirrorHelp<double>(init_value_str, local_mirror_size);
        break;
    }
    case AU_DOUBLE_COMPLEX:
    {
        local_mirror_buffer = CreateLocalMirrorHelp<std::complex<double>>(init_value_str, local_mirror_size);
        break;
    }
    default:
    {
        AU_EXIT("Only support until 3D in memory data!");
        break;
    }
    }

    //std::cout << "Pre init_value_str at " << au_mpi_rank_global << "\n" <<  std::flush;

    //Error:
    //Disable them for DASH distribution issue.
    //AccessDashData1D may access data from different nodes
    if (init_value_str == "" && false)
    {
        if (!au_mpi_rank_global)
        {
            std::vector<unsigned long> start_ul, end_ul;
            start_ul.resize(endpoint_dim_size.size());
            end_ul.resize(endpoint_dim_size.size());

            for (int i = 0; i < endpoint_dim_size.size(); i++)
            {
                start_ul[i] = 0;
                end_ul[i] = endpoint_dim_size[i] - 1;
            }
            //PrintScalar("local_mirror_size = ", local_mirror_size);
            //PrintVector("endpoint_dim_size = ", endpoint_dim_size);

            switch (endpoint_dim_size.size())
            {
            case 1:
                AccessDashData1D(dash_array_p, start_ul, end_ul, local_mirror_buffer, data_element_type, DASH_READ_FLAG);
                break;
            case 2:
                AccessDashData2D(dash_array_p, start_ul, end_ul, local_mirror_buffer, data_element_type, DASH_READ_FLAG);
                break;
            case 3:
                AccessDashData3D(dash_array_p, start_ul, end_ul, local_mirror_buffer, data_element_type, DASH_READ_FLAG);
                break;
            default:
                AU_EXIT("AU_MEMORY only support until 3D data");
                break;
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);

        switch (data_element_type)
        {
        case AU_SHORT:
        {
            BcastHelp<short>(local_mirror_buffer, local_mirror_size);
            break;
        }
        case AU_INT:
        {
            BcastHelp<int>(local_mirror_buffer, local_mirror_size);
            break;
        }
        case AU_LONG:
        {
            BcastHelp<long>(local_mirror_buffer, local_mirror_size);
            break;
        }
        case AU_LONG_LONG:
        {
            BcastHelp<long long>(local_mirror_buffer, local_mirror_size);
            break;
        }
        case AU_USHORT:
        {
            BcastHelp<unsigned short>(local_mirror_buffer, local_mirror_size);
            break;
        }
        case AU_UINT:
        {
            BcastHelp<unsigned int>(local_mirror_buffer, local_mirror_size);
            break;
        }
        case AU_ULONG:
        {
            BcastHelp<unsigned long>(local_mirror_buffer, local_mirror_size);
            break;
        }
        case AU_ULLONG:
        {
            BcastHelp<unsigned long long>(local_mirror_buffer, local_mirror_size);
            break;
        }
        case AU_FLOAT:
        {
            BcastHelp<float>(local_mirror_buffer, local_mirror_size);
            break;
        }
        case AU_DOUBLE:
        {
            BcastHelp<double>(local_mirror_buffer, local_mirror_size);
            break;
        }
        case AU_DOUBLE_COMPLEX:
        {
            std::cout << "Pre BcastHelp at " << au_mpi_rank_global << "\n";
            BcastHelp<std::complex<double>>(local_mirror_buffer, local_mirror_size);
            break;
        }
        default:
        {
            AU_EXIT("Only support until 3D in memory data!");
            break;
        }
        }
    }

    return 0;
}
