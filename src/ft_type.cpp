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

#include "ft_type.h"
#include "ft_utility.h"

AuEndpointType MapString2EndpointType(std::string endpoint_type_str) {
  AuEndpointType endpoint_type;
  if (endpoint_type_str == "EP_HDF5") {
    endpoint_type = EP_HDF5;
  } else if (endpoint_type_str == "EP_PNETCDF") {
    endpoint_type = EP_PNETCDF;
  } else if (endpoint_type_str == "EP_AUDIOS") {
    endpoint_type = EP_ADIOS;
  } else if (endpoint_type_str == "EP_BINARY") {
    endpoint_type = EP_BINARY;
  } else if (endpoint_type_str == "EP_VIRTUAL") {
    endpoint_type = EP_VIRTUAL;
  } else if (endpoint_type_str == "EP_MEMORY") {
    endpoint_type = EP_MEMORY;
  } else if (endpoint_type_str == "EP_H5VDS") {
    endpoint_type = EP_H5VDS;
  } else if (endpoint_type_str == "EP_DIR") {
    endpoint_type = EP_DIR;
  } else if (endpoint_type_str == "EP_DASSA") {
    endpoint_type = EP_DASSA;
  } else if (endpoint_type_str == "EP_TDMS") {
    endpoint_type = EP_TDMS;
  } else if (endpoint_type_str == "EP_CSV") {
    endpoint_type = EP_CSV;
  } else if (endpoint_type_str == "EP_RabbitMQ") {
    endpoint_type = EP_RabbitMQ;
  } else if (endpoint_type_str == "EP_DIR_STREAM") {
    endpoint_type = EP_DIR_STREAM;
  } else if (endpoint_type_str == "EP_RabbitMQ_RESTAPI") {
    endpoint_type = EP_RabbitMQ_RESTAPI;
  } else {
    AU_EXIT("Not supported AuDataEndpointType");
  }
  return endpoint_type;
}

size_t GetAuEndpointDataTypeSize(AuEndpointDataType type) {
  static const size_t sizes[] = {
      0,                            // AU_NO_TYPE
      sizeof(short),                // AU_SHORT
      sizeof(int),                  // AU_INT
      sizeof(long),                 // AU_LONG
      sizeof(long long),            // AU_LONG_LONG
      sizeof(unsigned short),       // AU_USHORT
      sizeof(unsigned int),         // AU_UINT
      sizeof(unsigned long),        // AU_ULONG
      sizeof(unsigned long long),   // AU_ULLONG
      sizeof(float),                // AU_FLOAT
      sizeof(double),               // AU_DOUBLE
      sizeof(std::complex<double>), // AU_DOUBLE_COMPLEX
      sizeof(const char *),         // AU_STRING
  };

  if (type < AU_NO_TYPE || type >= AU_NCLASSES) {
    throw std::invalid_argument("Invalid type");
  }

  return sizes[type + 1]; // +1 because AU_NO_TYPE is -1
}