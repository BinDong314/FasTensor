

/**
 *
 * Email questions to dbin@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#ifndef END_POINT_H
#define END_POINT_H

#include "au_utility.h"
#include "au_type.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

/**
 * @brief Define the class for the Endpoint used by ArrayUDF 
 * to store the data. It contains basic infomation for the endpoint
 * as well as the operations supported by the endpoint
 * 
 */
class Endpoint
{
protected:
    AuEndpointType endpoint_type;
    std::string endpoint_info;
    std::vector<unsigned long long> endpoint_dim_size;
    int endpoint_ranks;
    AuEndpointDataType data_element_type;
    std::string data_endpoint_orig;

    bool set_endpoint_dim_size_flag = false;
    bool open_flag = false;
    bool create_flag = false;
    unsigned read_write_flag;

public:
    Endpoint(){};
    virtual ~Endpoint(){};

    /**
     * @brief Get the Dimensions of the data
     * 
     * @return vector for the size of data  endpoint_dim_size.size() is the rank
     */
    std::vector<unsigned long long> GetDimensions();

    /**
     * @brief Set the Dimensions 
     * 
     * @return < 0 error, works otherwise
     */
    void SetDimensions(std::vector<unsigned long long> endpoint_dim_size_p);

    /**
     * @brief set the type of data element
     * 
     * @param data_element_type_p 
     */
    void SetDataElementType(AuEndpointDataType data_element_type_p);

    /**
     * @brief Get the Type of Data Element 
     * 
     * @return AuEndpointDataType 
     */
    AuEndpointDataType GetDataElementType();

    /**
     * @brief Get the size of the type for the element
     * 
     * @return int 
     */
    int GetDataElementTypeSize();

    /**
     * @brief Set the Endpoint Type object
     * 
     * @param endpoint_type_p 
     */
    void SetEndpointType(AuEndpointType endpoint_type_p);

    /**
     * @brief Get the Endpoint Type object
     * 
     * @return AuEndpointType 
     */
    AuEndpointType GetEndpointType();

    bool GetOpenFlag();

    void SetOpenFlag(bool open_flag_p);

    bool GetCreateFlag();

    void SetCreateFlag(bool open_flag_p);

    void SetRwFlag(unsigned read_write_flag_p);

    unsigned GetRwFlag();

    /**
     * @brief convert my data in (void *) type to Union type
     * 
     * @param vp : pointer to data (after read)
     * @return std::vector<AuEndpointDataTypeUnion> : return value
     */
    std::vector<AuEndpointDataTypeUnion> Void2Union(void *vp, size_t n_elements);

    /**
     * @brief convert data from union to void type 
     * 
     * @param data_vector_in_union_type : vector of data in union type
     * @return void* : pointer to data (for write)
     */
    void *Union2Void(std::vector<AuEndpointDataTypeUnion> &data_vector_in_union_type);

    /**
     * @brief set the endpoint_info string 
     * 
     * @param endpoint_info 
     */
    void SetEndpointInfo(std::string endpoint_info_p);

    /**
     * @brief Get the endpoint_info string 
     * 
     * @return std::string 
     */
    std::string GetEndpointInfo();

    /**
     * @brief parse endpoint_info to my own info
     *        
     * @return int: 0 works,  < 0 error,
     */
    virtual int ParseEndpointInfo() = 0;

    /**
     * @brief extracts metadata, possbile endpoint_ranks/endpoint_dim_size/other ep_type dependents ones
     * 
     * @return int < 0 error, >= 0 works 
     */
    virtual int ExtractMeta() = 0;
    /**
     * @brief print information about the endpoint
     * 
     * @return < 0 error, >= 0 works 
     */
    virtual int PrintInfo() = 0;

    /**
     * @brief create the endpoint
     * 
     * @return  < 0 error, >= 0 works 
     */
    virtual int Create() = 0;

    /**
     * @brief open the endpoint
     * 
     * @return < 0 error, >= 0 works 
     */
    virtual int Open() = 0;

    /**
     * @brief read the data from end-point
     * 
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data 
     * @return int < 0 error, >= 0 works
     */
    virtual int Read(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data) = 0;

    /**
     * @brief write the data to the end-point
     * 
     * @param start, coordinates of the cell to start (including)
     * @param end , coordinates of the cell to end (including)
     * @param data, store the result data 
     * @return int < 0 error, >= 0 works
     */
    virtual int Write(std::vector<unsigned long long> start, std::vector<unsigned long long> end, void *data) = 0;

    /**
     * @brief Set the Attribute object
     *   Do not need to be pure virtual method
     * @param name 
     * @param data 
     * @return int 
     */
    virtual int WriteAttribute(const std::string &name, const void *data, FTDataType data_type_p, const size_t &data_length_p = 0);

    /**
     * @brief Get the Attribute object
     *  Do not need to be pure virtual method
     * @param name 
     * @param data 
     * @return int 
     */
    virtual int ReadAttribute(const std::string &name, void *data, FTDataType data_type_p, const size_t &data_length_p = 0);

    virtual size_t GetAttributeSize(const std::string &name, FTDataType data_type_p);

    /**
     * @brief close the end-point
     * 
     * @return int int < 0 error, >= 0 works
     */
    virtual int Close() = 0;

    /**
     * @brief call the finalize to close everything (like call Destractor)
     * 
     * @return int 
     */
    //virtual int Finalize() = 0;

    virtual void Map2MyType() = 0;

    virtual void EnableCollectiveIO();

    virtual void DisableCollectiveIO();

    /**
    * @brief Get the Dir File Vector object
    * 
    * @return std::vector<std::string> 
    */
    virtual std::vector<std::string> GetDirFileVector();

    /**
     * @brief Set the Dir File Vector object
     * 
     * @param file_list 
     */
    virtual void SetDirFileVector(std::vector<std::string> &file_list);

    /**
     * @brief Get the Dir Chunk Size object
     * 
     * @return std::vector<int> 
     */
    virtual std::vector<int> GetDirChunkSize();

    /**
     * @brief Set the Dir Chunk Size object
     * 
     * @param dir_chunk_size_p 
     */
    virtual void SetDirChunkSize(std::vector<int> &dir_chunk_size_p);

    /**
     * @brief call a special operator on endpoint
     *        such as, enable collective I/O for HDF5
     *                 dump file from MEMORY to HDF5
     * @param opt_code, specially defined code 
     */
    //virtual int SpecialOperator(int opt_code, std::string parameter);

    /**
     * @brief call a special operator on endpoint
     *        such as, enable collective I/O for HDF5
     *                 dump file from MEMORY to HDF5
     * @param opt_code, specially defined code 
     * @param parameter_v, vector of argument
     */
    virtual int SpecialOperator(int opt_code, std::vector<std::string> parameter_v);

    /**
     * @brief map a op_cmd_str to int as input of SpecialOperator
     * 
     * @param op_cmd_str cmd string 
     * @return int 
     */
    virtual int MapOpStr2Int(std::string op_cmd_str);

    /**
     * @brief map op_int to string 
     * 
     * @param op_int 
     * @return std::string 
     */
    virtual std::string MapOpInt2Str(int op_int);
};

#endif
