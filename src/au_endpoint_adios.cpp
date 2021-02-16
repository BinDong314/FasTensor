
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

#include "au_endpoint_adios.h"

#ifdef HAS_ADIOS_END_POINT

void check_adios_handler(const void *handler, const char *message, int lineno)
{
    if (handler == NULL)
    {
        printf("ERROR: invalid %s handler at line %d  \n", message, lineno);
        exit(EXIT_FAILURE);
    }
}

void check_adios_error(const int error, int lineno)
{
    if (error)
    {
        printf("ERROR: %d at line %d \n", error, lineno);
        exit(error);
    }
}

int EndpointADIOS::ParseEndpointInfo()
{
    //std::cout << endpoint_info << "\n";
    std::stringstream ss(endpoint_info);
    if (!std::getline(ss, fn_str, ':'))
    {
        AU_EXIT("Invalued endpoint_info");
    }

    if (!std::getline(ss, vn_str, ':'))
    {
        AU_EXIT("Invalued endpoint_info");
    }

    std::cout << "fn_str =" << fn_str << ", vn_str = " << vn_str << std::endl;
    return 0;
}

//bindings/C/adios2/c/adios2_c_types.h
void EndpointADIOS::Map2MyType()
{
    switch (data_element_type)
    {
    case AU_SHORT:
        adios2_data_element_type = adios2_type_int16_t;
        return;
    case AU_INT:
        adios2_data_element_type = adios2_type_int32_t;
        return;
    case AU_LONG:
        adios2_data_element_type = adios2_type_int64_t;
        return;
    case AU_LONG_LONG:
        adios2_data_element_type = adios2_type_int64_t;
        return;
    case AU_USHORT:
        adios2_data_element_type = adios2_type_uint16_t;
        return;
    case AU_UINT:
        adios2_data_element_type = adios2_type_uint32_t;
        return;
    case AU_ULONG:
        adios2_data_element_type = adios2_type_uint64_t;
        return;
    case AU_ULLONG:
        adios2_data_element_type = adios2_type_uint64_t;
        return;
    case AU_FLOAT:
        adios2_data_element_type = adios2_type_float;
        return;
    case AU_DOUBLE:
        adios2_data_element_type = adios2_type_double;
        return;
    default:
        std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
        std::flush(std::cout);
        std::exit(EXIT_FAILURE);
    }
};

/**
     * @brief print information about the endpoint
     * 
     * @return < 0 error, >= 0 works 
     */
int EndpointADIOS::PrintInfo()
{
    std::cout << "fn_str : " << fn_str << ", vn_str :" << vn_str << "\n";
    return 0;
}

/**
    * @brief extracts metadata, possbile endpoint_ranks/endpoint_dim_size/data_element_type
    * 
    * @return int < 0 error, >= 0 works 
    */
int EndpointADIOS::ExtractMeta()
{
    return 0;
}

/**
     * @brief write the data to the end-point
     * 
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data 
     * @return int < 0 error, >= 0 works
     */
int EndpointADIOS::Write(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{
    Map2MyType();
    int endpoint_dim_rank = start.size();
    adios_start.resize(endpoint_dim_rank);
    adios_count.resize(endpoint_dim_rank);
    for (int i = 0; i < endpoint_dim_rank; i++)
    {
        adios_start[i] = start[i];
        adios_count[i] = end[i] - start[i] + 1;
    }

    if (!GetOpenFlag())
    {
        SetRwFlag(adios2_mode_write);
        Open(); //open for write
    }
    else if (GetRwFlag() != adios2_mode_write)
    {
        Close();
        SetRwFlag(adios2_mode_write);
        Open();
    }

    int errio = adios2_set_selection(rw_variable, endpoint_dim_rank, adios_start.data(), adios_count.data());
    check_adios_error(errio, __LINE__);

    errio = adios2_put(rw_engine, rw_variable, data, adios2_mode_sync);
    check_adios_error(errio, __LINE__);
    //adios2_perform_puts(write_engine);
    //adios2_close(write_engine);

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
int EndpointADIOS::Read(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{
    Map2MyType();
    int adios_rank = start.size();
    adios_start.resize(adios_rank);
    adios_count.resize(adios_rank);
    for (int i = 0; i < adios_rank; i++)
    {
        adios_start[i] = start[i];
        adios_count[i] = end[i] - start[i] + 1;
        //cout << "Read, adios_start: " << adios_start[0] << ", " << adios_start[1] << ", adios_count: " << adios_count[0] << ", " << adios_count[1] << "\n";
    }

    if (!GetOpenFlag())
    {
        SetRwFlag(adios2_mode_read);
        Open();
    }
    else if (GetRwFlag() != adios2_mode_read)
    {
        Close();
        SetRwFlag(adios2_mode_read);
        Open();
    }

    adios2_set_selection(rw_variable, adios_rank, adios_start.data(), adios_count.data());
    adios2_get(rw_engine, rw_variable, data, adios2_mode_sync);
    return 0;
}

int EndpointADIOS::Create()
{
    int endpoint_dim_rank = endpoint_dim_size.size();
    adios_start.resize(endpoint_dim_rank);
    adios_count.resize(endpoint_dim_rank);
    if (set_endpoint_dim_size_flag)
    {
        adios_shape.resize(endpoint_dim_rank);
    }
    for (int i = 0; i < endpoint_dim_rank; i++)
    {
        adios_start[i] = 0;
        adios_count[i] = endpoint_dim_size[i] - 1;
        if (set_endpoint_dim_size_flag)
        {
            adios_shape[i] = endpoint_dim_size[i];
        }
    }
    //std::cout << "define variable : " << vn_str << ", adios_shape : " << adios_shape[0] << ", " << adios_shape[1] << " in Create\n";

    rw_variable = adios2_define_variable(rw_io, vn_str.c_str(), adios2_data_element_type, endpoint_dim_rank, adios_shape.data(), adios_start.data(), adios_count.data(), adios2_constant_dims_false);
    check_adios_handler(rw_variable, "adios2_define_variable", __LINE__);

    //Assume create file to write
    rw_engine = adios2_open(rw_io, fn_str.c_str(), adios2_mode_write);
    check_adios_handler(rw_engine, "adios2_open", __LINE__);

    //Do the first write to create the
    SetCreateFlag(true);
    SetOpenFlag(true);
    SetRwFlag(adios2_mode_write);
    return 0;
}

int EndpointADIOS::Open()
{
    if (file_exist(fn_str.c_str()) == 0)
    {
        //create the file it does not exits
        cout << "create file \n";
        return Create();
    }

    //Assume create file to write
    if (GetRwFlag() == adios2_mode_read)
    {
        rw_engine = adios2_open(rw_io, fn_str.c_str(), adios2_mode_read);
    }
    else
    {
        //rw_engine = adios2_open(rw_io, fn_str.c_str(), adios2_mode_append);

        rw_engine = adios2_open(rw_io, fn_str.c_str(), adios2_mode_read);
        rw_engine = adios2_open(rw_io, fn_str.c_str(), adios2_mode_write);
    }
    check_adios_handler(rw_engine, "adios2_open", __LINE__);

    rw_variable = adios2_inquire_variable(rw_io, vn_str.c_str());
    check_adios_handler(rw_variable, "adios2_inquire_variable", __LINE__);

    SetOpenFlag(true);
    return 0;
}

int EndpointADIOS::Close()
{
    if (GetRwFlag() == adios2_mode_write)
    {
        adios2_perform_puts(rw_engine);
        adios2_flush(rw_engine);
    }
    adios2_close(rw_engine);
    std::cout << "call close ! \n";
    return -1;
}

#endif