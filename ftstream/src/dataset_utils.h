#ifndef DATASET_UTILS_H
#define DATASET_UTILS_H

#include "hdf5_utils.h"
#include "dataset_types.h"
#include <vector>

struct Dataset {
    std::string name;             // Dataset name
    TypeInfo type;                // Dataset datatype
    std::vector<hsize_t> dims;    // Array of dimensions
    void* buffer;                 // Pointer to the dataset buffer
    hsize_t buffer_size;          // Size of the dataset buffer
    hid_t dataspace;              // Dataset dataspace
    hid_t dataset_id;             // Dataset identifier

    // Default constructor
    Dataset() : buffer(nullptr), buffer_size(0), dataspace(H5I_INVALID_HID), dataset_id(H5I_INVALID_HID) {}
    
    // Main Constructor
    Dataset(const char* name, const TypeInfo& type_info,
            const std::vector<hsize_t>& dimensions, void* buffer, hsize_t buffer_size)
        : name(name),
          type(type_info),
          dims(dimensions),
          buffer(buffer),
          buffer_size(buffer_size),
          dataspace(H5Screate_simple(dims.size(), dims.data(), NULL)),
          dataset_id(H5I_INVALID_HID)
    {
        if (dataspace < 0) {
            throw std::runtime_error("Failed to create dataspace");
        }
    }

       // Delegating constructor for hid_t type
    Dataset(const char* name, hid_t type_id,
            const std::vector<hsize_t>& dimensions, void* buffer, hsize_t buffer_size)
        : Dataset(name, get_type_info(type_id), dimensions, buffer, buffer_size){}

};


Dataset readDataset(hid_t src_file, const std::string& dataset_name);
bool checkDatasetExists(hid_t file_id, const std::string& dataset_path);
H5Handle createIntermediateGroups(const H5Handle& file_id, const std::string& path);
H5Handle createNewDataset(const H5Handle& dst_file, const Dataset& src_dataset);
// H5Handle safeCopyDataset(const H5Handle& dst_file, const Dataset& src_dataset);
H5Handle buildHDF5File(std::string seed_file, std::vector<Dataset> datasets);

// To be compatible with HDF5 API
extern "C" herr_t H5literate_checkDatasetCallback(hid_t loc_id, const char *name, const H5L_info_t *linfo, void *opdata);
std::vector<std::string> getAllDatasetNames(const H5Handle& src_file);
std::vector<Dataset> prepareDatasetsCopy(const H5Handle& src_file,
                                         const std::vector<std::string>& include_list,
                                         const std::vector<std::string>& exclude_list);

#endif