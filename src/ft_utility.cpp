/*
****************************

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy). 
All rights reserved.

If you have questions about your rights to use or distribute this software,
please contact Berkeley Lab's Intellectual Property Office at
IPO@lbl.gov.

NOTICE.  This Software was developed under funding from the U.S. Department
of Energy and the U.S. Government consequently retains certain rights.  As
such, the U.S. Government has been granted for itself and others acting on
its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the
Software to reproduce, distribute copies to the public, prepare derivative 
works, and perform publicly and display publicly, and to permit others to do so.


****************************


*** License Agreement ***

FasTensor (FT) Copyright (c) 2021, The Regents of the University of
California, through Lawrence Berkeley National Laboratory (subject to
receipt of any required approvals from the U.S. Dept. of Energy). 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

(1) Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

(2) Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

(3) Neither the name of the University of California, Lawrence Berkeley
National Laboratory, U.S. Dept. of Energy nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

You are under no obligation whatsoever to provide any bug fixes, patches,
or upgrades to the features, functionality or performance of the source
code ("Enhancements") to anyone; however, if you choose to make your
Enhancements available either publicly, or directly to Lawrence Berkeley
National Laboratory, without imposing a separate written license agreement
for such Enhancements, then you hereby grant the following license: a
non-exclusive, royalty-free perpetual license to install, use, modify,
prepare derivative works, incorporate into other computer software,
distribute, and sublicense such enhancements or derivative works thereof,
in binary and source code form.
*/

/**
 *
 * Author: Bin Dong dbin@lbl.gov
 * Web: https://crd.lbl.gov/bin-dong
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#include "ft_utility.h"
#include <limits.h>

std::string realpathEx(std::string path)
{
    const char *path_c_str = path.c_str();
    char *home;
    char buff[PATH_MAX]; // = (char *)malloc(path.size());
    if (*path_c_str == '~' && (home = getenv("HOME")))
    {
        char s[PATH_MAX];
        return std::string(realpath(strcat(strcpy(s, home), path_c_str + 1), buff));
    }
    else
    {
        return std::string(realpath(path_c_str, buff));
    }
    //free(buff);
}

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
    std::string dir_str_p_new = realpathEx(dir_str_p);
    //std::string dir_str_p_new = dir_str_p;
    int namelist_length = scandir(dir_str_p_new.c_str(), &namelist, 0, alphasort);
    std::vector<std::string> file_list;
    int temp_index = 0;
    if (namelist_length < 0)
    {
        AU_EXIT("Error in list directory at " + dir_str_p_new);
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
    else if (endpoint_type_str == "EP_PNETCDF")
    {
        endpoint_type = EP_PNETCDF;
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