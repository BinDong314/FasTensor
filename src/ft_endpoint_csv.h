

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

#ifndef END_POINT_CSV_H
#define END_POINT_CSV_H

#include "ft_type.h"
#include "ft_endpoint.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>

#define CSV_SET_DELIMITER (OP_USER_DEFINED_START + 0)

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class CSVRow
{
public:
    std::string_view operator[](std::size_t index) const
    {
        return std::string_view(&m_line[m_data[index] + 1], m_data[index + 1] - (m_data[index] + 1));
    }
    std::size_t size() const
    {
        return m_data.size() - 1;
    }
    void readNextRow(std::istream &str)
    {
        std::getline(str, m_line);

        m_data.clear();
        m_data.emplace_back(-1);
        std::string::size_type pos = 0;
        while ((pos = m_line.find(delimiter, pos)) != std::string::npos)
        {
            m_data.emplace_back(pos);
            ++pos;
        }
        // This checks for a trailing comma with no data after it.
        pos = m_line.size();
        m_data.emplace_back(pos);
    }
    void SetDelimiter(const std::string &delimiter_p)
    {
        delimiter = delimiter_p;
    }

private:
    std::string m_line;
    std::vector<int> m_data;
    std::string delimiter = ",";
};

// I/O layer
class EndpointCSV : public Endpoint
{
private:
    std::string fn_str;
    FILE *fp = NULL;

    /**
     * @brief
     * "r" :  Opens a file for reading. The file must exist.
     * "w" :  Creates an empty file for writing. If a file with the same name already exists, its content is erased and the file is considered as a new empty file.
     * "a" : Appends to a file. Writing operations, append data at the end of the file. The file is created if it does not exist.
     * "r+" : Opens a file to update both reading and writing. The file must exist.
     * "w+" : Creates an empty file for both reading and writing.
     * "a+" : Opens a file for reading and appending.
     */
    std::string model_str = "r";

    size_t rw_type_size;

    std::vector<size_t> array_size;

    std::string delimiter = ",";
    bool is_set_delimiter = false;

    bool is_ExtractMeta = false;

public:
    /**
     * @brief Construct a new EndpointCSV object
     *
     * @param data_endpoint contains the info of the endpoint, e.g., file type + file info
     */
    EndpointCSV(std::string endpoint_info_p)
    {
        endpoint_info = endpoint_info_p;
        ParseEndpointInfo();
        SetOpenFlag(false);
        SetEndpointType(EP_CSV);
    }
    /**
     * @brief Construct a new Endpoint in Binary
     *         Nothing to do there, can be used as sub-endpoint of directory
     */
    EndpointCSV()
    {
        SetOpenFlag(false);
        SetEndpointType(EP_CSV);
    }

    virtual ~EndpointCSV()
    {
        Close();
    }

    /**
     * @brief extracts metadata, possbile endpoint_ranks/endpoint_dim_size/data_element_type
     *
     * @return int < 0 error, >= 0 works
     */
    int ExtractMeta() override;
    /**
     * @brief print information about the endpoint
     *
     * @return < 0 error, >= 0 works
     */
    int PrintInfo() override;

    /**
     * @brief create the endpoint
     *
     * @return  < 0 error, >= 0 works
     */
    int Create() override;

    /**
     * @brief open the endpoint
     *
     * @return < 0 error, >= 0 works
     */
    int Open() override;

    /**
     * @brief read the data from end-point
     *
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data
     * @return int < 0 error, >= 0 works
     */
    int Read(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data) override;

    /**
     * @brief write the data to the end-point
     *
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data
     * @return int < 0 error, >= 0 works
     */
    int Write(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data) override;

    /**
     * @brief close the end-point
     *
     * @return int int < 0 error, >= 0 works
     */
    int Close() override;

    void Map2MyType() override;

    /**
     * @brief parse endpoint_info to my own info
     *        In binary, it map endpoint_info to filename,
     * @return int: 0 works,  < 0 error,
     */
    int ParseEndpointInfo() override;

    /**
     * @brief call a special operator on binary endpoint
     *        such as OP_SET_BINARY_SIZE
     * @param opt_code, specially defined code
     */
    int Control(int opt_code, std::vector<std::string> &parameter_v) override;
};
#endif
