#include "au_utility.h"

int ExtractFileTypeInfo(std::string data_endpoint_info, AuEndpointType &endpoint_type, std::vector<std::string> &endpoint_info)
{
    //Get the AuDataEndpointType
    std::stringstream ss(data_endpoint_info);
    std::string token;
    if (!std::getline(ss, token, ':'))
    {
        std::cout << "Error: invalued data_endpoint_info: " << data_endpoint_info << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if (token == "EP_HDF5")
    {
        endpoint_type = EP_HDF5;
    }
    else if (token == "EP_NETCDF")
    {
        endpoint_type = EP_NETCDF;
    }
    else if (token == "EP_AUDIOS")
    {
        endpoint_type = EP_AUDIOS;
    }
    else if (token == "EP_BINARY")
    {
        endpoint_type = EP_BINARY;
    }
    else if (token == "EP_VIRTUAL")
    {
        endpoint_type = EP_VIRTUAL;
    }
    else if (token == "EP_IARRAY")
    {
        endpoint_type = EP_IARRAY;
    }
    else if (token == "EP_IVECTOR")
    {
        endpoint_type = EP_IVECTOR;
    }
    else
    {
        std::cout << "Error: No AuDataEndpointType found in: " << data_endpoint_info << std::endl;
        std::exit(EXIT_FAILURE);
    }

    while (std::getline(ss, token, ':'))
    {
        endpoint_info.push_back(token);
    }

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
