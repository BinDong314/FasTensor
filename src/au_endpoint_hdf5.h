

/**
 *
 * Email questions to dbin@lbl.gov
 * Scientific Data Management Research Group
 * Lawrence Berkeley National Laboratory
 *
 */

#ifndef END_POINT_HDF5_H
#define END_POINT_HDF5_H

#include "au_type.h"
#include "au_endpoint.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>

#include "hdf5.h" //right now, we only have code for HDF5

#define HDF5_ENABLE_MPI_IO 0
#define HDF5_DISABLE_MPI_IO 1
#define HDF5_ENABLE_COLLECTIVE_IO 2
#define HDF5_DISABLE_COLLECTIVE_IO 3

//For some old code
#define OP_ENABLE_MPI_IO HDF5_ENABLE_MPI_IO
#define OP_DISABLE_MPI_IO HDF5_DISABLE_MPI_IO
#define OP_ENABLE_COLLECTIVE_IO HDF5_ENABLE_COLLECTIVE_IO
#define OP_DISABLE_COLLECTIVE_IO HDF5_DISABLE_COLLECTIVE_IO

//
//I/O layer
class EndpointHDF5 : public Endpoint
{
private:
    hid_t fid = -1, gid = -1, did = -1;
    hid_t dataspace_id = -1;
    std::string fn_str, gn_str, dn_str;
    hid_t plist_id = H5P_DEFAULT, plist_cio_id = H5P_DEFAULT;
    hid_t mem_type, disk_type;
    bool is_mpi_enabled = false;

public:
    /**
     * @brief Construct a new EndpointHDF5 object
     * 
     * @param data_endpoint contains the info of the endpoint, e.g., file type + file info
     */
    EndpointHDF5(std::string endpoint_info_p)
    {
        endpoint_info = endpoint_info_p;
        ParseEndpointInfo();
        SetOpenFlag(false);
        SetRwFlag(H5F_ACC_RDONLY);
        SetEndpointType(EP_HDF5);
        EnableMPIIO();
        EnableCollectiveIO();
        is_mpi_enabled = true;
    }
    /**
     * @brief Construct a new Endpoint in HDF5 
     *         Nothing to do there, can be used as sub-endpoint of directory
     */
    EndpointHDF5()
    {
        SetOpenFlag(false);
        SetRwFlag(H5F_ACC_RDONLY);
        SetEndpointType(EP_HDF5);
        EnableMPIIO();
        EnableCollectiveIO();
        is_mpi_enabled = true;
    }

    /**
     * @brief Construct a new EndpointHDF5 object without MPI
     * 
     * @param no_mpi  any number should work
     */
    EndpointHDF5(int no_mpi)
    {
        //endpoint_info = endpoint_info_p;
        //ParseEndpointInfo();
        SetOpenFlag(false);
        SetRwFlag(H5F_ACC_RDONLY);
        SetEndpointType(EP_HDF5);
        //EnableMPIIO();
        //EnableCollectiveIO();
    }

    ~EndpointHDF5()
    {
        Close();
        if (is_mpi_enabled)
        {
            DisableMPIIO();
            DisableCollectiveIO();
        }
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

    void Map2MyTypeParameters(FTDataType ft_type, hid_t &mem_type, hid_t &disk_type);

    void EnableCollectiveIO() override;

    void DisableCollectiveIO() override;

    void EnableMPIIO();

    void DisableMPIIO();

    /**
     * @brief parse endpoint_info to my own info
     *        In HDF5, it map endpoint_info to filename, group name and datasetname
     * @return int: 0 works,  < 0 error,
     */
    int ParseEndpointInfo() override;

    /**
     * @brief call a special operator on endpoint
     *        such as, enable collective I/O for HDF5
     *                 dump file from MEMORY to HDF5
     * @param opt_code, specially defined code 
     */
    //int SpecialOperator(int opt_code, std::string parameter) override;

    /**
     * @brief call a special operator on endpoint
     *        such as, enable collective I/O for HDF5
     *                 dump file from MEMORY to HDF5
     * @param opt_code, specially defined code 
     */
    int SpecialOperator(int opt_code, std::vector<std::string> parameter_v) override;

    /**
     * @brief Set the Attribute object
     * 
     * @param name 
     * @param data 
     * @return int 
     */
    int WriteAttribute(const std::string &name, const void *data, FTDataType data_type_p, const size_t &data_length_p = 0) override;

    /**
     * @brief Get the Attribute object
     * 
     * @param name 
     * @param data 
     * @return int 
     */
    int ReadAttribute(const std::string &name, void *data, FTDataType data_type_p, const size_t &data_length_p = 0) override;

    size_t GetAttributeSize(const std::string &name, FTDataType data_type_p) override;
};
#endif
