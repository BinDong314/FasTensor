
#include "ft_type.h"
private:
#define MONITOR_RANK 0
#define READY_SIGNAL 1
#define TERMINATION_SIGNAL -1
#define NO_WORK_SIGNAL 0

template<typename vec_t>
std::string prettyPrintVector(const std::vector<vec_t>& vec) {
    std::ostringstream oss;
    oss << "[";
    
    if (!vec.empty()) {
        // Print all elements except the last one with a comma and space
        std::copy(vec.begin(), vec.end() - 1, 
                  std::ostream_iterator<vec_t>(oss, ", "));
        
        // Print the last element without a trailing comma
        oss << vec.back();
    }
    
    oss << "]";
    return oss.str();
}
bool divibleWorkload() {
   
    auto  divisibleByDimension = std::all_of(data_size.begin(), data_size.end(), 
                            [=](int num) { return num % worker_size == 0; });
    auto  divisibleByChunkSize = std::all_of(data_chunk_size.begin(), data_chunk_size.end(), 
                            [=](int num) { return num % worker_size == 0; });
    return divisibleByDimension && divisibleByChunkSize;                    
}
template <class UDFOutputType, class BType = UDFOutputType>
void streamingTransform(Stencil<UDFOutputType> (*UDF)(const Stencil<T> &), Array<BType> *B = nullptr){      
          
      // First check MPI rank of itself
      // Here we are checking ft_rank not worker_rank because
      /// Only the main process with MPI_COMM_WORLD communicator will monitor directory
      if(0 == ft_rank){

        monitorDirectory();
      }
      else{
        processFiles(UDF,B,io_comm);
      }

}

// File monitoring process
// @TODO: provide termination signal
void monitorDirectory(){    
    if (0 == ft_rank) {
        // std::cout << "Rank " << ft_rank << ": Says Hi" << std::endl;
        auto dir_mon = DirectoryMonitor(stream_dir,std::chrono::seconds(1));        
        StreamView streamView(stream_input_regex);
        //std::cout << "Register StreamView callback" << std::endl;
        dir_mon.registerCallback([&streamView](const DirectoryView& view) {
        try {
            streamView.updateView(view);            
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
           
        }
        });
       // std::cout << "No. of processes (ft_size)" <<ft_size <<std::endl;
       // std::cout << "No. of worker processes(worker_size)" <<worker_size <<std::endl;
        dir_mon.start();
        //std::cout << "Directory Monitor process started." << std::endl;
        
    int file_count = 0;
    std::queue<std::string> processingQueue;
    // @TODO: how to provide termination signal mechanism
    while(!terminationRequested){
        
        while (streamView.HasNextFile()) {
                auto it = streamView.getNextFile();
            //  std::cout << "Found file: " << it->path << std::endl;
                processingQueue.push(it->path);
        }
        if(!processingQueue.empty()){
            distributeWork(processingQueue,file_count);
        }
        else{
            // No-escape from busy-waiting :(
            // I tried many approached to avoid busy-waiting ultimately it depends on internal implementation of MPI 
        }
    }

    // Send termination signal
    int terminationSignal = -1;
    MPI_Bcast(&terminationSignal, 1, MPI_INT, 0, MPI_COMM_WORLD);

    }
}


template <class UDFOutputType, class BType = UDFOutputType>
void processFiles(Stencil<UDFOutputType> (*UDF)(const Stencil<T> &), Array<BType> *B = nullptr,MPI_Comm comm = MPI_COMM_WORLD_DEFAULT) {
   // std::cout << "Rank " << worker_rank << " Worker: Says Hi" << std::endl;
    
    while (!terminationRequested) {
        // Synchronize all processes before receiving new data
        MPI_Barrier(comm);
       
        int readySignal;
        MPI_Bcast(&readySignal, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (readySignal == TERMINATION_SIGNAL)             
            break;
        
        // Send acknowledgment
        MPI_Gather(&readySignal, 1, MPI_INT, NULL, 1, MPI_INT, MONITOR_RANK, MPI_COMM_WORLD);
        // Receive file path from monitoring process
        int pathLength;
        MPI_Bcast(&pathLength, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Receive file path from monitoring process
        std::vector<char> buffer(pathLength);
        MPI_Bcast(buffer.data(), pathLength, MPI_CHAR, 0, MPI_COMM_WORLD);      
        std::string recv_file_path(buffer.begin(), buffer.end());

        int file_count = 0;
        MPI_Bcast(&file_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Process the file
        try {
        //    std::cout << "Worker Process " << worker_rank << " received file for processing: " << recv_file_path << std::endl;
            Array<T>* inputArray;
            if(!data_chunk_size.empty()){
                inputArray = new Array<T>(ep_name + ":" + recv_file_path + ":" + dataset, data_chunk_size,{}, comm);
            }
            else{
                throw std::runtime_error("Chunk Size must be set before processing files");
            }

            if (!divibleWorkload()) {
                std::vector<unsigned long long> array_size;
                inputArray->GetArraySize(array_size);
                std::cout << "Error: Array Dimension or chunk size is not divisible by the number of worker processes." << std::endl
                          << "Total Process(ft_rank) : " << ft_size << std::endl
                          << "Worker Process:" << worker_size << std::endl 
                          << "Array Dimension size" << prettyPrintVector(array_size)<< std::endl 
                          << "Data chunk size" << prettyPrintVector(data_chunk_size)<< std::endl;
                std::cout.flush();
                
                
                
                 MPI_Barrier(comm);  
                 MPI_Abort(MPI_COMM_WORLD, 1); 
            }
        //    std::cout << "[prepping output array ]" << std::endl;
        //    std::string new_file_path = recv_file_path.substr(0, recv_file_path.find_last_of('.')) + "_out.h5";
            int width = static_cast<int>(std::log10(file_count)) + 1;
            std::ostringstream endpoint_cfg;
            // What should be reasonable default for when B is a nullptr?
            if(B != nullptr)
            {
                auto outputEndpointName = B->getEndpointName();
            //    std::cout << "outputEndpointName: " << outputEndpointName << std::endl;
                
                endpoint_cfg << outputEndpointName << ":" 
                            << B->getDirectory() 
                            << B->getStreamBaseName() 
                            << std::setw(width) 
                            << std::setfill('0') << file_count
                            << getEndpointExtension(outputEndpointName)<<":"
                            << dataset;
            }
        
            Array<BType>* outputArray = new Array<BType>(endpoint_cfg.str(),{},{},comm);       
      
         //   Process file using Transform
            inputArray->Transform(UDF, outputArray);

            // Clean up
            delete inputArray;
            delete outputArray;
            

        //    std::cout << "Worker Process " << worker_rank << " completed processing file: " << recv_file_path << std::endl;
        //    std::cout.flush();
        } catch (const std::exception& e) {
            std::cerr << "Error in worker process " << worker_rank << " processing file " << recv_file_path << ": " << e.what() << std::endl;
        }
       
    }
 
}

void distributeWork(std::queue<std::string>& processingQueue, int& file_count) {
   // MPI_Barrier(MPI_COMM_WORLD);
    // Send a ready signal to all processes
    int readySignal = READY_SIGNAL;
    MPI_Bcast(&readySignal, 1, MPI_INT, MONITOR_RANK, MPI_COMM_WORLD);

    // Wait for acknowledgment from all processes
    std::vector<int> acknowledgments(ft_size);
    MPI_Gather(&readySignal, 1, MPI_INT, acknowledgments.data(), 1, MPI_INT, MONITOR_RANK, MPI_COMM_WORLD);

    // Check if all processes are ready
    bool allReady = std::all_of(acknowledgments.begin(), acknowledgments.end(),
                                    [](int ack) { return ack == READY_SIGNAL; });

    if (allReady) {
        std::string filePath = processingQueue.front();
        processingQueue.pop();

        // Broadcast file path to all processes
        int pathLength = filePath.length();
        MPI_Bcast(&pathLength, 1, MPI_INT, MONITOR_RANK, MPI_COMM_WORLD);
        MPI_Bcast(const_cast<char*>(filePath.c_str()), pathLength, MPI_CHAR, MONITOR_RANK, MPI_COMM_WORLD);

        file_count++;
        MPI_Bcast(&file_count, 1, MPI_INT, MONITOR_RANK, MPI_COMM_WORLD);

         std::cout << "Sent file for processing: " << filePath << std::endl;
    } 
    else {
        //@Investigate: We actually never hit this code path 
        std::cout << "Not all processes are ready. Waiting..." << std::endl;
    }
}