#include "H5public.h"
#include "cmdline.h" // Contains usage string, docopt, and few utuilties
#include "fsutils.h"
#include "dataset_utils.h"
#include "hdf5_utils.h"
#include "fastflush.h"
#include <iostream>
#include <cstdlib>
#include <random>
#include <string>
#include <ctime>
#include <chrono>
#include <sys/stat.h>
#include <thread>
#include <vector>



#define MAX_FILENAME_LENGTH 255
std::vector<std::string> splitString(const std::string& input, char delimiter);
std::vector<std::string> generate_file_names(const std::string& base_name,const std::string& dir_path, int count, int count_from = 1);
std::vector<uint8_t> createTensorBuffer(TypeInfo type, const std::vector<hsize_t>& dimensions);
std::vector<hsize_t> convertToSizeT(const std::vector<std::string>& stringVector);

void repeatedWrites(const std::string& file_name, 
                    const std::string& output_dir,
                    size_t count, 
                    const std::vector<char>& file_buffer,
                    std::chrono::milliseconds interval,
                    std::chrono::milliseconds minInterval 
           );
int playbackMode(const std::string& seed_file, 
                  const std::string& directory, 
                  const std::string& file_name,
                  const size_t count,
                  const std::chrono::seconds interval,
                  const std::vector<std::string>& include_dataset, 
                  const std::vector<std::string>& exclude_dataset 
                 );
int 
randomMode(const std::string& dataset_path,
                  const std::string& directory, 
                  const std::string& base_name,
                  const size_t count,           
                  const std::chrono::seconds interval,
                  const TypeInfo& data_type,
                  std::vector<hsize_t> dimensions
             
                 );
/*
   Improved latency by 100 ms but we don't need it any more

*/
void optimizeFileAccess(const H5Handle& file_id, size_t L3cache_MiB) {
  H5AC_cache_config_t mdc_config;
  mdc_config.version = H5AC__CURR_CACHE_CONFIG_VERSION;
  H5Fget_mdc_config(file_id, &mdc_config);
  mdc_config.max_size = L3cache_MiB; 
  H5Fset_mdc_config(file_id, &mdc_config);
  // Set page buffer size
  H5Handle fapl(H5Fget_access_plist(file_id));
  H5Pset_page_buffer_size(fapl, L3cache_MiB, 0, 0); 
}

                 
int main(int argc, char * argv[]) 
{
    if(argc<2)
    {
        std::cout << usage_screen << std::endl;
        exit(0);
    }

    const std::string default_file_name("stream_001.h5"),
                      default_directory("current directory"),
                      default_include_dataset("all"),
                      default_exclude_dataset("none");
    const float INITIAL_VALUE = 1.0f;
    const long default_interval = 5; // Flush every 1 minute, minimum = 2sec.

    std::map<std::string, docopt::value> args
        = docopt::docopt(usage_screen,
                         { argv + 1, argv + argc },
                         true,               // show help if requested
                         "FTstream 1.0");  // version string
    
 
    const auto count = value_or(args["--count"], 0l);

    std::string directory = value_or(args["--directory"], default_directory);
    if (directory == default_directory)
        directory = ".";
    // Parse the directory and check if it exists
    try
    {
        directory = validateDirectory(directory);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cout <<"Attempting to create directory" << std::endl;
        mkdir(directory.c_str(), 0755);
    }
    const std::chrono::seconds interval(value_or(args["--interval"], default_interval));
    std::string fileName = value_or(args["--stream-name"], default_file_name);
    if (fileName == default_file_name)
        fileName = "stream";

    if (args["playback"].asBool()) {
        const std::string seedFile = args["--seed-file"].asString();

        // parameter = is_parameter_set ? parameter : default_value
        auto parse_include_dataset = value_or(args["--include-dataset"], default_include_dataset);
        auto parse_exclude_dataset = value_or(args["--exclude-dataset"], default_exclude_dataset);
        
        auto includeDataset = splitString(parse_include_dataset, ',');
        auto excludeDataset = splitString(parse_exclude_dataset, ',');
        playbackMode(seedFile,
                     directory, 
                     fileName,
                     count,
                     interval, 
                     includeDataset, 
                     excludeDataset
                    );
    }
    else if (args["random"].asBool()) {
        bool is_le = true;
        if(args["--big-endian"].isBool())
        {
            is_le = false;
        }
        const std::string dataset_path = value_or(args["--dataset"], "/test/testdata");
        const std::string parse_data_type = value_or(args["--datatype"], "float");
        const std::string parse_dimensions = value_or(args["--dimensions"], "1024,1024");
        auto dimensions = splitString(parse_dimensions, ',');   
        auto data_type = stringToTypeInfo   (parse_data_type, is_le);
        randomMode(dataset_path, 
                   directory, 
                   fileName, 
                   count, 
                   interval, 
                   data_type, 
                   convertToSizeT(dimensions)
                  );
    }
    std::cout << "Streaming completed to the count. Exiting" << std::endl;
    return 0;
}



int 
playbackMode(const std::string& seed_file, 
                  const std::string& directory, 
                  const std::string& file_name,
                  const size_t count,
                  const std::chrono::seconds interval,
                  const std::vector<std::string>& include_dataset, 
                  const std::vector<std::string>& exclude_dataset 
                 )
                  
{
    std::cout << "Playback mode" << std::endl
     << "Seed file: " << seed_file << std::endl
     << "Directory: " << directory << std::endl
     << "Interval: " << interval.count() << std::endl
     << "Stream name: " << file_name << std::endl;
    if(count == 0)
     {
        std::cout << "Count: 0 (Infinite Stream)" << std::endl;
     }else
     {
        std::cout << "Count: " << count << std::endl;
     }

    std::string output_directory(directory);  

    if(include_dataset.size() ==1 && include_dataset[0] == "all")
    {
        auto file_buffer = read_file_into_memory(seed_file);
        // Benchmark write to disk operation which is rounded off to nearest 100ms
        auto time_to_flush = benchmarkFlushTime(file_buffer);
        std::cout << "Min. Time to flush: " << time_to_flush.count() << " ms" << std::endl;
        std::cout << "Streaming started" << std::endl;
        repeatedWrites(file_name, output_directory,  count,file_buffer, interval, time_to_flush);
    }
    else {
        
        H5Handle input_file(H5Fopen(seed_file.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT));
        if (!input_file.isValid()) {
            std::cerr <<"Failed to open input file." <<std::endl;
            return 1;
        }
        // Copy the relevant datasets, to construct a new HDF5 file 
        auto datasets = prepareDatasetsCopy(input_file, include_dataset, exclude_dataset);
        input_file.close();
        // Create a new temporary HDF5 file and write the datasets to it
        buildHDF5File("/tmp/temp_base.h5", datasets);
        // Reload the newly created HDF5 file into memory
        auto file_buffer = read_file_into_memory("/tmp/temp_base.h5");
        // first we have to profile the write to disk operation which is rounded off to nearest 100ms
        auto time_to_flush = benchmarkFlushTime(file_buffer);
        std::cout << "Min. Time to flush: " << time_to_flush.count() << " ms" << std::endl;
        std::cout << "Streaming started" << std::endl;
        repeatedWrites(file_name, output_directory,  count,file_buffer, interval, time_to_flush);
        unlink("/tmp/temp_base.h5");
    }
         

    
    return 0;
}

int 
randomMode(const std::string& dataset_path,
                  const std::string& directory, 
                  const std::string& base_name,
                  const size_t count,           
                  const std::chrono::seconds interval,
                  const TypeInfo& data_type,
                  std::vector<hsize_t> dimensions
             
                 )
{
    std::cout << "Random mode" << std::endl
     << "Dataset: " << dataset_path << std::endl
     << "Directory: " << directory << std::endl
     << "Interval: " << interval.count() << std::endl
     << "Stream name: " << base_name << std::endl;
    if(count == 0)
     {
        std::cout << "Count: 0 (Infinite Stream)" << std::endl;
     }else
     {
        std::cout << "Count: " << count << std::endl;
     }
    // Generate a buffer based on data type
    auto tensor_buffer = createTensorBuffer(data_type, dimensions);
    auto totalElements = std::accumulate(dimensions.begin(), dimensions.end(), 
                                           1ULL, std::multiplies<hsize_t>());
    auto bufferSize = totalElements * data_type.size;
    // create a new HDF5 file
    H5Handle file_id(H5Fcreate((directory +"/stream_base.h5").c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT));
    if (!file_id.isValid()) {
        std::cerr << "Failed to create HDF5 file" << std::endl;
        return 1;
    }
    // create a new dataset with dataset path
    auto new_dataset = Dataset{
        dataset_path.c_str(),
        data_type,
        dimensions,
        tensor_buffer.data(),
        bufferSize
    };
    H5Handle dataset_id = createNewDataset(file_id, new_dataset);
    H5Fflush(file_id, H5F_SCOPE_GLOBAL);
    file_id.close();
    std::cout << "HDF5 file constructed" << std::endl;

    auto file_buffer = read_file_into_memory(directory + "/stream_base.h5");
    // first we have to profile the write to disk operation which is rounded off to nearest 100ms
    auto time_to_flush = benchmarkFlushTime(file_buffer);
    std::cout << "Average Time to flush: " << time_to_flush.count() << " ms" << std::endl;
    std::cout << "Streaming started" << std::endl;
    repeatedWrites(base_name, directory,  count,file_buffer, interval, time_to_flush);
    return 0;
}   

 


std::vector<std::string> generate_file_names(const std::string& base_name,const std::string& dir_path, int count, int count_from) {
    auto valid_dir = validateDirectory(dir_path);
    std::vector<std::string> result;
    result.reserve(count);  // Reserve space for efficiency

    for (int i = count_from; i < count_from + count; ++i) {
        std::stringstream ss;
        ss << valid_dir << "/" << base_name << "_" << std::setw(3) << std::setfill('0') << i << ".h5";
        result.push_back(ss.str());
    }

    return result;
}

void repeatedWrites(const std::string& file_name, 
                    const std::string& output_dir,
                    size_t count, 
                    const std::vector<char>& file_buffer,
                    std::chrono::milliseconds interval,
                      std::chrono::milliseconds minInterval 
           ) 
{

    if (interval < minInterval) {
        throw std::invalid_argument("Interval must be greater than or equal to minInterval");
    }
    
    auto output_file_names = generate_file_names(file_name, output_dir, count);

    auto nextExecutionTime = std::chrono::steady_clock::now() + interval;
    size_t executionCount = 0;
    FastFlush io_manager;

    while (count == 0 || executionCount < count) {
        auto currentTime = std::chrono::steady_clock::now();
        auto timeUntilExecution = std::chrono::duration_cast<std::chrono::milliseconds>(
            nextExecutionTime - currentTime);

        if (timeUntilExecution > minInterval) {
            std::this_thread::sleep_for(timeUntilExecution - minInterval);
        }
        io_manager.writeBufferToFile(file_buffer, output_file_names[executionCount]);
      //  std::cout << "Wrote to disk: " << output_file_names[executionCount] << std::endl;
        executionCount++;
        nextExecutionTime += interval;
        // Handle potential overflow
        if (nextExecutionTime < currentTime) {
            nextExecutionTime = currentTime + interval;
        }
    }
}

std::vector<std::string> splitString(const std::string& input, char delimiter) 
{
    std::vector<std::string> result;
    std::istringstream iss(input);
    std::string item;
    while (std::getline(iss, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}


template<typename T>
void fillBuffer(std::vector<uint8_t>& buffer, size_t totalElements) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    T* typedBuffer = reinterpret_cast<T*>(buffer.data());
    for (size_t i = 0; i < totalElements; ++i) {
        if constexpr (std::is_integral_v<T>) {
            typedBuffer[i] = static_cast<T>(dis(gen) * std::numeric_limits<T>::max());
        } else {
            typedBuffer[i] = static_cast<T>(dis(gen));
        }
    }
}

std::vector<uint8_t> createTensorBuffer(TypeInfo type, const std::vector<hsize_t>& dimensions) {
    if (dimensions.empty() || dimensions.size() > 4) {

        throw std::invalid_argument("Rank of dataset must be between 1 and 4");
    }

    size_t totalElements = std::accumulate(dimensions.begin(), dimensions.end(), 
                                           1ULL, std::multiplies<size_t>());
    
    if (totalElements > std::numeric_limits<size_t>::max() / type.size) {
        throw std::overflow_error("Tensor size too large");
    }

    size_t bufferSize = totalElements * type.size;
    std::vector<uint8_t> buffer(bufferSize);

    switch (type.basic_type) {
        case HDF5BasicType::Int8:
            fillBuffer<int8_t>(buffer, totalElements); break;
        case HDF5BasicType::UInt8:
            fillBuffer<uint8_t>(buffer, totalElements); break;
        case HDF5BasicType::Int16:
            fillBuffer<int16_t>(buffer, totalElements); break;
        case HDF5BasicType::UInt16:
            fillBuffer<uint16_t>(buffer, totalElements); break;
        case HDF5BasicType::Int32:
            fillBuffer<int32_t>(buffer, totalElements); break;
        case HDF5BasicType::UInt32:
            fillBuffer<uint32_t>(buffer, totalElements); break;
        case HDF5BasicType::Int64:
            fillBuffer<int64_t>(buffer, totalElements); break;
        case HDF5BasicType::UInt64:
            fillBuffer<uint64_t>(buffer, totalElements); break;
        case HDF5BasicType::Float32:
            fillBuffer<float>(buffer, totalElements); break;
        case HDF5BasicType::Float64:
            fillBuffer<double>(buffer, totalElements); break;
        default:
            throw std::invalid_argument("Unknown HDF5BasicType");
    }

    return buffer;
}

std::vector<hsize_t> convertToSizeT(const std::vector<std::string>& stringVector) {
    std::vector<hsize_t> result;
    result.reserve(stringVector.size());  // Preallocate for efficiency

    std::transform(stringVector.begin(), stringVector.end(), std::back_inserter(result),
        [](const std::string& str) {
            try {
                return std::stoull(str);
            } catch (const std::exception& e) {
                throw std::invalid_argument("Invalid number in input: " + str);
            }
        }
    );

    return result;
}
