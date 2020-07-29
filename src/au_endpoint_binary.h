

/**
 *
 * Email questions to dbin@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#ifndef END_POINT_BINARY_H
#define END_POINT_BINARY_H

#include "au_type.h"
#include "au_endpoint.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>

#define BINARY_SET_SIZE 0
#define BINARY_ENABLE_TRANSPOSE_ON_READ 1
#define BINARY_ENABLE_TRANSPOSE_ON_WRITE 2
#define BINARY_DISABLE_TRANSPOSE_ON_READ 3
#define BINARY_DISABLE_TRANSPOSE_ON_WRITE 4
//
//I/O layer
class EndpointBinary : public Endpoint
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

    long int seek_offset = 0; //SEEK_SET: from the beginning of file

    bool tranpose_on_read_flag = false;
    bool tranpose_on_write_flag = false;

    //std::vector<size_t> array_size;
public:
    /**
     * @brief Construct a new EndpointBinary object
     * 
     * @param data_endpoint contains the info of the endpoint, e.g., file type + file info
     */
    EndpointBinary(std::string endpoint_info_p)
    {
        endpoint_info = endpoint_info_p;
        ParseEndpointInfo();
        SetOpenFlag(false);
        SetEndpointType(EP_BINARY);
    }
    /**
     * @brief Construct a new Endpoint in Binary 
     *         Nothing to do there, can be used as sub-endpoint of directory
     */
    EndpointBinary()
    {
        SetOpenFlag(false);
        SetEndpointType(EP_BINARY);
    }

    virtual ~EndpointBinary()
    {
        Close();
    }

    void SetMode(std::string model_str_p)
    {
        model_str = model_str_p;
    }

    std::string GetMode()
    {
        return model_str;
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
     * @brief update the seek_offset
     * 
     */
    virtual void UpdateSeekOffset();

    /**
     * @brief call a special operator on binary endpoint
     *        such as OP_SET_BINARY_SIZE
     * @param opt_code, specially defined code 
     */
    int SpecialOperator(int opt_code, std::string parameter) override;

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
