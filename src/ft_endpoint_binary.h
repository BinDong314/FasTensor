

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

#ifndef END_POINT_BINARY_H
#define END_POINT_BINARY_H

#include "ft_endpoint.h"
#include "ft_type.h"
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#define BINARY_SET_SIZE (OP_USER_DEFINED_START + 0)
#define BINARY_ENABLE_TRANSPOSE_ON_READ (OP_USER_DEFINED_START + 1)
#define BINARY_ENABLE_TRANSPOSE_ON_WRITE (OP_USER_DEFINED_START + 2)
#define BINARY_DISABLE_TRANSPOSE_ON_READ (OP_USER_DEFINED_START + 3)
#define BINARY_DISABLE_TRANSPOSE_ON_WRITE (OP_USER_DEFINED_START + 4)

#define BINARY_ENABLE_TRAN_READ BINARY_ENABLE_TRANSPOSE_ON_READ
#define BINARY_ENABLE_TRAN_WRITE BINARY_ENABLE_TRANSPOSE_ON_WRITE
#define BINARY_DISABLE_TRAN_READ BINARY_DISABLE_TRANSPOSE_ON_READ
#define BINARY_DISABLE_TRAN_WRITE BINARY_DISABLE_TRANSPOSE_ON_WRITE
//
// I/O layer
class EndpointBinary : public Endpoint {
private:
  std::string fn_str;
  FILE *fp = NULL;

  /**
   * @brief
   * "r" :  Opens a file for reading. The file must exist.
   * "w" :  Creates an empty file for writing. If a file with the same name
   * already exists, its content is erased and the file is considered as a new
   * empty file. "a" : Appends to a file. Writing operations, append data at the
   * end of the file. The file is created if it does not exist. "r+" : Opens a
   * file to update both reading and writing. The file must exist. "w+" :
   * Creates an empty file for both reading and writing. "a+" : Opens a file for
   * reading and appending.
   */
  std::string model_str = "r";

  size_t rw_type_size;

  long int seek_offset = 0; // SEEK_SET: from the beginning of file

  bool tranpose_on_read_flag = false;
  bool tranpose_on_write_flag = false;

  // std::vector<size_t> array_size;
public:
  /**
   * @brief Construct a new EndpointBinary object
   *
   * @param data_endpoint contains the info of the endpoint, e.g., file type +
   * file info
   */
  EndpointBinary(std::string endpoint_info_p) {
    endpoint_info = endpoint_info_p;
    ParseEndpointInfo();
    SetOpenFlag(false);
    SetEndpointType(EP_BINARY);
  }
  /**
   * @brief Construct a new Endpoint in Binary
   *         Nothing to do there, can be used as sub-endpoint of directory
   */
  EndpointBinary() {
    SetOpenFlag(false);
    SetEndpointType(EP_BINARY);
  }

  virtual ~EndpointBinary() { Close(); }

  void SetMode(std::string model_str_p) { model_str = model_str_p; }

  std::string GetMode() { return model_str; }
  /**
   * @brief extracts metadata, possbile
   * endpoint_ranks/endpoint_dim_size/data_element_type
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
  int Read(std::vector<unsigned long long> start,
           std::vector<unsigned long long> end, void *data) override;

  /**
   * @brief write the data to the end-point
   *
   * @param start, coordinates of the cell to start (including)
   * @param end , coordinates of the cell to end (including)
   * @param data, store the result data
   * @return int < 0 error, >= 0 works
   */
  int Write(std::vector<unsigned long long> start,
            std::vector<unsigned long long> end, void *data) override;

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
   * @brief update the seek_offset
   *
   */
  virtual void UpdateSeekOffset();

  /**
   * @brief call a special operator on binary endpoint
   *        such as OP_SET_BINARY_SIZE
   * @param opt_code, specially defined code
   */
  int Control(int opt_code, std::vector<std::string> &parameter_v) override;

  /**
   * @brief Get file point
   *
   * @return FILE* return null if nothing opened
   */
  FILE *GetFP();

  /**
   * @brief Set the Seek Offset
   *
   * @param seek_offset_p : set offset to be used for reading
   */
  void SetSeekOffset(long int seek_offset_p);

  /**
   * @brief map a op_cmd_str to int as input of SpecialOperator
   *
   * @param op_cmd_str cmd string
   * @return int
   */
  int MapOpStr2Int(std::string op_cmd_str) override;

  /**
   * @brief map op_int to string
   *
   * @param op_int
   * @return std::string
   */
  std::string MapOpInt2Str(int op_int) override;

  /**
   * @brief EnableTranposeOnRead
   *
   */
  void EnableTranposeOnRead();
  /**
   * @brief EnableTranposeOnWrite
   *
   */
  void EnableTranposeOnWrite();

  /**
   * @brief DisableTranposeOnRead
   *
   */
  void DisableTranposeOnRead();

  /**
   * @brief DisableTranposeOnWrite
   *
   */
  void DisableTranposeOnWrite();
};
#endif
