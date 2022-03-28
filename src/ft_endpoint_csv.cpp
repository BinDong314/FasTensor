

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
#include "ft_endpoint_csv.h"
#include "ft_array_transpose.h"

#include <string>

std::istream &operator>>(std::istream &str, CSVRow &data)
{
    data.readNextRow(str);
    return str;
}

int EndpointCSV::ExtractMeta()
{
    // AU_EXIT("Not support EndpointCSV::Write \n ");
    // Find the size;
    // Number of ROWS
    if (is_ExtractMeta)
        return 0;
    int number_of_rows = 0, number_of_columns = 0;

    std::ifstream file(fn_str);
    CSVRow row;

    if (is_set_delimiter)
    {
        row.SetDelimiter(delimiter);
    }
    while (file >> row)
    {
        if (number_of_rows == 0)
        {
            number_of_columns = row.size();
        }
        else
        {
            if (number_of_columns != row.size())
            {
                AU_EXIT("Rows in CSW does not have equal number of columns:");
            }
        }
        number_of_rows++;
    }

    endpoint_size.resize(2);
    endpoint_size[0] = number_of_rows;
    endpoint_size[1] = number_of_columns;
    PrintVector("EndpointCSV::ExtractMeta: endpoint_size = ", endpoint_size);
    set_endpoint_dim_size_flag = true;
    endpoint_ranks = 2;

    is_ExtractMeta = true;
    return 0;
}

int EndpointCSV::Create()
{
    AU_EXIT("Not support EndpointCSV::Create \n ");
    return 0;
}

int EndpointCSV::Open()
{

    if (file_exist(fn_str.c_str()) == 0)
    {
        AU_EXIT("File does not exits: " + fn_str);
    }
    SetOpenFlag(true);
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
int EndpointCSV::Read(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{
    int ret;
    if (GetOpenFlag() == false)
    {
        Open();
    }

    Map2MyType();

    size_t count = 1;
    int data_rank = start.size();
    for (int i = 0; i < data_rank; i++)
    {
        count = count * (end[i] - start[i] + 1);
    }

    std::ifstream file(fn_str);
    CSVRow row;
    if (is_set_delimiter)
    {
        row.SetDelimiter(delimiter);
    }
    // std::cout << "Elements : " << row[0] << " , " << row[1] << "\n";

    switch (data_element_type)
    {
    case AU_SHORT:
        return 0;
    case AU_INT:
        return 0;
    case AU_LONG:
        return 0;
    case AU_LONG_LONG:
        return 0;
    case AU_USHORT:
        return 0;
    case AU_UINT:
        return 0;
    case AU_ULONG:
        return 0;
    case AU_ULLONG:
        return 0;
    case AU_FLOAT:
        return 0;
    case AU_DOUBLE:
    {
        std::vector<double> temp_vector;
        temp_vector.reserve(count);
        while (file >> row)
        {
            for (int i = 0; i < endpoint_size[1]; i++)
            {
                temp_vector.push_back(std::stod(std::string{row[i]}));
            }
        }
        // for (auto e : temp_vector)
        // {
        //     std::cout << " , " << e;
        // }
        // std::cout << " \n ";
        memcpy(data, temp_vector.data(), count * sizeof(double));
        temp_vector.clear();
        return 0;
    }
    default:
        std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
        std::flush(std::cout);
        std::exit(EXIT_FAILURE);
    }

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
int EndpointCSV::Write(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data)
{
    AU_EXIT("Not support EndpointCSV::Write \n ");
    return 0;
}

/**
 * @brief close the end-point
 *
 * @return int int < 0 error, >= 0 works
 */
int EndpointCSV::Close()
{
    if (fp != NULL)
        fclose(fp);
    fp = NULL;
    SetOpenFlag(false);
    return 0;
};

int EndpointCSV::PrintInfo()
{
    std::cout << data_endpoint_orig << std::endl;
    return 0;
}

void EndpointCSV::Map2MyType(){
    // switch (data_element_type)
    // {
    // case AU_SHORT:
    //     rw_type_size = sizeof(short);
    //     return;
    // case AU_INT:
    //     rw_type_size = sizeof(int);
    //     return;
    // case AU_LONG:
    //     rw_type_size = sizeof(long);
    //     return;
    // case AU_LONG_LONG:
    //     rw_type_size = sizeof(long long);
    //     return;
    // case AU_USHORT:
    //     rw_type_size = sizeof(unsigned short);
    //     return;
    // case AU_UINT:
    //     rw_type_size = sizeof(unsigned int);
    //     return;
    // case AU_ULONG:
    //     rw_type_size = sizeof(unsigned long);
    //     return;
    // case AU_ULLONG:
    //     rw_type_size = sizeof(unsigned long long);
    //     return;
    // case AU_FLOAT:
    //     rw_type_size = sizeof(float);
    //     return;
    // case AU_DOUBLE:
    //     rw_type_size = sizeof(double);

    //     return;
    // default:
    //     std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__ << std::endl;
    //     std::flush(std::cout);
    //     std::exit(EXIT_FAILURE);
    // }
    // return 0;
};

/**
 * @brief parse endpoint_info to my own info
 *        In TMDS, it map endpoint_info to filename
 * @return int: 0 works,  < 0 error,
 */
int EndpointCSV::ParseEndpointInfo()
{
    // std::cout << endpoint_info << "\n";
    std::stringstream ss(endpoint_info);
    if (!std::getline(ss, fn_str, ':'))
    {
        AU_EXIT("Invalued endpoint_info");
    }

    return 0;
}

int EndpointCSV::Control(int opt_code, std::vector<std::string> &parameter_v)
{
    switch (opt_code)
    {
    case CSV_SET_DELIMITER:
        assert(parameter_v.size() >= 1);
        delimiter = parameter_v[0];
        is_set_delimiter = true;
        break;
    default:
        break;
    }
    return 0;
}
