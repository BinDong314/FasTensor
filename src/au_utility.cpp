#include "au_utility.h"

int ExtractEndpointTypeInfo(std::string endpoint_type_info, AuEndpointType &endpoint_type, std::string &endpoint_info)
{
    //Get the AuDataEndpointType
    std::stringstream ss(endpoint_type_info);
    std::string token;
    if (!std::getline(ss, token, ':'))
    {
        AU_EXIT("invalued data_endpoint_info");
    }
    endpoint_type = MapString2EndpointType(token);

    std::getline(ss, endpoint_info);
    return 0;
}

int file_exist(const char *filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

std::string ExtractFileName(const std::string &fullPath)
{
    const size_t lastSlashIndex = fullPath.find_last_of("/\\");
    return fullPath.substr(lastSlashIndex + 1);
}

std::string ExtractPath(const std::string &fullPath)
{
    const size_t lastSlashIndex = fullPath.find_last_of("/\\");
    return fullPath.substr(0, lastSlashIndex);
}

std::vector<std::string> GetDirFileList(std::string dir_str_p)
{
    struct dirent **namelist;
    int namelist_length = scandir(dir_str_p.c_str(), &namelist, 0, alphasort);
    std::vector<std::string> file_list;
    int temp_index = 0;
    if (namelist_length < 0)
    {
        AU_EXIT("Eroor in list directory");
    }
    else
    {
        while (temp_index < namelist_length)
        {
            if (strcmp(namelist[temp_index]->d_name, "..") != 0 && strcmp(namelist[temp_index]->d_name, ".") != 0)
                file_list.push_back(namelist[temp_index]->d_name);
            temp_index++;
        }
    }
    return file_list;
}

AuEndpointType MapString2EndpointType(std::string endpoint_type_str)
{
    AuEndpointType endpoint_type;
    if (endpoint_type_str == "EP_HDF5")
    {
        endpoint_type = EP_HDF5;
    }
    else if (endpoint_type_str == "EP_NETCDF")
    {
        endpoint_type = EP_NETCDF;
    }
    else if (endpoint_type_str == "EP_ADIOS")
    {
        endpoint_type = EP_ADIOS;
    }
    else if (endpoint_type_str == "EP_BINARY")
    {
        endpoint_type = EP_BINARY;
    }
    else if (endpoint_type_str == "EP_VIRTUAL")
    {
        endpoint_type = EP_VIRTUAL;
    }
    else if (endpoint_type_str == "EP_MEMORY")
    {
        endpoint_type = EP_MEMORY;
    }
    else if (endpoint_type_str == "EP_H5VDS")
    {
        endpoint_type = EP_H5VDS;
    }
    else if (endpoint_type_str == "EP_DIR")
    {
        endpoint_type = EP_DIR;
    }
    else if (endpoint_type_str == "EP_DASSA")
    {
        endpoint_type = EP_DASSA;
    }
    else if (endpoint_type_str == "EP_TDMS")
    {
        endpoint_type = EP_TDMS;
    }
    else
    {
        AU_EXIT("Not supported AuDataEndpointType");
    }
    return endpoint_type;
}