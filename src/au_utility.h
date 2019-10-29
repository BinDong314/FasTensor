/**
 *ArrayUDF Copyright (c) 2017, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy).  All rights reserved.
 *
 *If you have questions about your rights to use or distribute this software, please contact Berkeley Lab's Innovation & Partnerships Office at  IPO@lbl.gov.
 *
 * NOTICE. This Software was developed under funding from the U.S. Department of Energy and the U.S. Government consequently retains certain rights. As such, the U.S. Government has been granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software to reproduce, distribute copies to the public, prepare derivative works, and perform publicly and display publicly, and to permit other to do so. 
 *
 */

/**
 *
 * Email questions to {dbin, kwu, sbyna}@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#ifndef ARRAY_UDF_UTILITY
#define ARRAY_UDF_UTILITY

#include <complex> // std::complex
#include <vector>
#include <type_traits>
#include <cstring>
#include <cmath>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <tuple>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "au_attribute.h"

int ExtractFileTypeInfo(std::string data_endpoint_info, AuEndpointType &endpoint_type, std::vector<std::string> &endpoint_info)
{

  //Get the AuDataEndpointType
  std::stringstream ss(data_endpoint_info);
  std::string token;
  if (!std::getline(ss, token, ':'))
  {
    std::cout << "Error: invalued data_endpoint_info " << std::endl;
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
    std::cout << "Error: No AuDataEndpointType found in data_endpoint_info " << std::endl;
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

#endif
