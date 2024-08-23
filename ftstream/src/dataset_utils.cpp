#include "dataset_utils.h"
#include "hdf5_utils.h"
#include <algorithm>
#include <numeric>
#include <string>
#include <unordered_set>
#include <vector>
#include <cstring>

struct TraverseData {
    std::vector<std::string>& dataset_names;
    std::string current_path;
};

Dataset 
readDataset(hid_t src_file, const std::string& dataset_name) 
{
    H5Handle src_dataset(H5Dopen(src_file, dataset_name.c_str(), H5P_DEFAULT));
    if (!src_dataset.isValid()) {
        throw H5Exception("Failed to open dataset. Check if Dataset exists");
    }
    H5Handle src_dataspace(H5Dget_space(src_dataset));
    
    auto src_dataset_rank = H5Sget_simple_extent_ndims(src_dataspace);
    std::vector<hsize_t> dims(src_dataset_rank);
    H5Sget_simple_extent_dims(src_dataspace, dims.data(), nullptr);
    hsize_t num_elements = std::accumulate(dims.begin(), dims.end(), 1ull, std::multiplies<hsize_t>());

    H5Handle src_dataset_type(H5Dget_type(src_dataset));
    TypeInfo infered_type = get_type_info(src_dataset_type);

    hsize_t buffer_size = num_elements * infered_type.size;
    void* buffer = (std::malloc(buffer_size));

    H5Dread(src_dataset,
             recreate_datatype(infered_type),
             H5S_ALL, 
             H5S_ALL, 
             H5P_DEFAULT, 
             buffer);
   
    return Dataset(dataset_name.c_str(),
                     infered_type, 
                     dims, 
                     buffer, 
                     buffer_size);
}

bool checkDatasetExists(hid_t file_id, const std::string& dataset_path) 
{
    std::vector<std::string> link_paths;
    std::string current_path = "";

    size_t start_pos = 1; // '/' always exist as per HDF5 1.10
    size_t end_pos = dataset_path.find('/', start_pos);
    /*
        Split the nested path into links

    */
    while (end_pos != std::string::npos) {
        current_path = dataset_path.substr(0, end_pos);
        /* std::cout << current_path << std::endl; */
        link_paths.push_back(current_path);
        start_pos = end_pos + 1;
        end_pos = dataset_path.find('/', start_pos);
    }
    link_paths.push_back(dataset_path);

    for (const std::string& link : link_paths) {
        auto link_exists = H5Lexists(file_id, link.c_str(), H5P_DEFAULT);
        if (!link_exists) {
        // Error occurred
            return false;
        }
    }

    // Check if the path is a dataset
    H5O_info_t info;
    herr_t status = H5Oget_info_by_name(file_id, dataset_path.c_str(), &info,H5O_INFO_ALL, H5P_DEFAULT);
    if (status < 0) {
        return false;
    }
    return info.type == H5O_TYPE_DATASET;
}
H5Handle createIntermediateGroups(const H5Handle& file_id, const std::string& group_path) 
{
    if (group_path.empty() || group_path[0] != '/') {
        throw H5Exception("Invalid path: " + group_path);
    }
    H5Handle plist(H5Pcreate(H5P_LINK_CREATE));
    H5Pset_create_intermediate_group(plist,1);
    H5Handle last_group(H5Gcreate2(file_id, group_path.c_str(), plist, H5P_DEFAULT, H5P_DEFAULT));
    if(!last_group.isValid()) {
        throw H5Exception("Failed to create intermediate groups");
    }
    return last_group;
}

H5Handle createNewDataset(const H5Handle& dst_file, const Dataset& src_dataset)
{
    auto split_idx = src_dataset.name.find_last_of('/');
    auto group_path = src_dataset.name.substr(0, split_idx);
    auto dataset_name = src_dataset.name.substr(split_idx + 1);
    H5Handle last_group;
    if (!group_path.empty()) {
        last_group = createIntermediateGroups(dst_file, group_path);
    }
    else{
        last_group = dst_file;
    }
    auto type_id = recreate_datatype(src_dataset.type);

    H5Handle dst_dataset(H5Dcreate2(last_group, 
                                   dataset_name.c_str(), 
                                   type_id, 
                                   src_dataset.dataspace, 
                                   H5P_DEFAULT, 
                                   H5P_DEFAULT, 
                                   H5P_DEFAULT));

    if (!dst_dataset.isValid()) {
        throw H5Exception("Failed to create dataset");
    }
     H5Handle dxpl(H5Pcreate(H5P_DATASET_XFER));
     size_t buf_size = 16*1024*1024;  // 16MB buffer Adjust as per your L3 Cache size
    void* tconv = malloc(buf_size);
    void* bkg = malloc(buf_size);
    
    if (tconv && bkg) {
        H5Pset_buffer(dxpl, buf_size, tconv, bkg);
    }
    
    H5Pset_hyper_vector_size(dxpl, 1024);

    auto status = H5Dwrite(dst_dataset, 
              type_id, 
              src_dataset.dataspace, 
              H5S_ALL, 
              dxpl, 
              src_dataset.buffer);
    if (status < 0) {
        throw H5Exception("Failed to write dataset");
    }
    return dst_dataset;
}

H5Handle buildHDF5File(std::string output_file_path, std::vector<Dataset> datasets){
    H5Handle build_file(H5Fcreate(output_file_path.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT));
        for (const auto& dataset : datasets) {
            createNewDataset(build_file, dataset);
        }
    H5Fflush(build_file, H5F_SCOPE_GLOBAL);
    return build_file;
}

/* H5Handle safeCopyDataset(const H5Handle& dst_file, const Dataset& src_dataset) 
{
    if(checkDatasetExists(dst_file, src_dataset.name))
    {
        // type check the dataset where rank, dims and data_space type are the same
        H5Handle dst_dataset(H5Dopen(dst_file, src_dataset.name.c_str(), H5P_DEFAULT));
        H5Handle dst_dataspace(H5Dget_space(dst_dataset));
        hsize_t dst_dims[2];
        H5Sget_simple_extent_dims(dst_dataspace, dst_dims, NULL);
        if(dst_dims[0] != src_dataset.dims[0] || dst_dims[1] != src_dataset.dims[1])
        {
           throw H5Exception("Error: Destinations Dataspace Dimesion mismatch with Source.");
        }

        H5Handle dst_datatype(H5Dget_type(dst_dataset));
      //  H5T_class_t dst_type_class = H5Tget_class(datatype); 
        if(src_dataset.type != dst_datatype)
        {
            throw H5Exception("Error: Destinations Dataspace Type Class mismatch with Source.");
        }

        auto status = H5Dwrite(dst_dataset, 
                                dst_datatype, 
                                dst_dataspace, 
                                H5S_ALL, 
                                H5P_DEFAULT, 
                                src_dataset.buffer);
        if (status < 0) {
            throw H5Exception("Error writing to destination dataset.");
        } 
        return dst_dataset;
    }
    else
    {
        return createNewDataset(dst_file, src_dataset);
    }
} */

void traverseGroup(hid_t loc_id, TraverseData& data) {
    H5Literate(loc_id, H5_INDEX_NAME, H5_ITER_NATIVE, nullptr, H5literate_checkDatasetCallback, &data);
}

// Callback function for H5Literate
extern "C" herr_t 
H5literate_checkDatasetCallback(hid_t loc_id, 
                                const char *name, 
                                const H5L_info_t *linfo, 
                                void *opdata) 
{
    auto data = static_cast<TraverseData*>(opdata);
    std::string full_path = data->current_path + "/" + name;

    H5O_info_t obj_info;
    H5Oget_info_by_name(loc_id, name, &obj_info, H5O_INFO_ALL, H5P_DEFAULT);

    if (obj_info.type == H5O_TYPE_DATASET) {
        data->dataset_names.push_back(full_path);
    } else if (obj_info.type == H5O_TYPE_GROUP) {
        hid_t group_id = H5Gopen2(loc_id, name, H5P_DEFAULT);
        if (group_id >= 0) {
            std::string old_path = data->current_path;
            data->current_path = full_path;
            traverseGroup(group_id, *data);
            data->current_path = old_path;
            H5Gclose(group_id);
        }
    }

    return 0;
}

std::vector<std::string> 
getAllDatasetNames(const H5Handle& src_file)
{
    std::vector<std::string> dataset_names;
    TraverseData data{dataset_names, ""};

    herr_t status = H5Literate(src_file, H5_INDEX_NAME, H5_ITER_NATIVE, nullptr, H5literate_checkDatasetCallback, &data);

    if (status < 0) {
        throw std::runtime_error("Error traversing HDF5 file structure");
    }

    return dataset_names;
}

std::vector<Dataset> 
prepareDatasetsCopy(const H5Handle& src_file,
                                         const std::vector<std::string>& include_list,
                                         const std::vector<std::string>& exclude_list)
{
    std::vector<std::string> final_list;
    // This is bit of premature optimisation on my end
    // But then you never know a scientist might treat a single HDF5 file  as a cabinet with many drawers.
    std::unordered_set<std::string> exclude_set(exclude_list.begin(), exclude_list.end());

    // If include_list is empty, we assume all datasets should be included
    if (include_list.empty()) {
        final_list = getAllDatasetNames(src_file);
        
    } else {
        final_list = include_list;
    }

    // Remove excluded datasets
    final_list.erase(
        std::remove_if(final_list.begin(), final_list.end(),
                       [&exclude_set](const std::string& dataset_name) {
                           return exclude_set.find(dataset_name) != exclude_set.end();
                       }),
        final_list.end());
    // Prepare the vector to hold the copied datasets
    std::vector<Dataset> copied_datasets;
    copied_datasets.reserve(final_list.size());

    // Read each dataset
    for (const auto& dataset_name : final_list) {
        try {
            copied_datasets.push_back(readDataset(src_file, dataset_name));
        } catch (const H5Exception& e) {
            // Log the error and continue with the next dataset
            std::cerr << "Error reading dataset '" << dataset_name << "': " << e.what() << std::endl;
        }
    }

    return copied_datasets;
}