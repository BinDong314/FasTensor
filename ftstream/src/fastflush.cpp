#include "fastflush.h"

#include <iostream>
#include <numeric>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <chrono>
#include <vector>


FastFlush::FastFlush(int queue_depth) : queue_depth_(queue_depth) {
    if (LinuxAIO::io_setup(queue_depth_, &aio_ctx_) < 0) {
        throw std::runtime_error("io_setup failed: " + std::string(strerror(errno)));
    }

    aligned_buffers_.resize(queue_depth_);
    iocbs_.resize(queue_depth_);
    iocbps_.resize(queue_depth_);

    for (int i = 0; i < queue_depth_; ++i) {
        if (posix_memalign(reinterpret_cast<void**>(&aligned_buffers_[i]), ALIGNMENT, BUFFER_SIZE) != 0) {
            throw std::runtime_error("Unable to allocate aligned memory");
        }
        iocbps_[i] = &iocbs_[i];
    }
}

FastFlush::~FastFlush() {
    LinuxAIO::io_destroy(aio_ctx_);
    for (int i = 0; i < queue_depth_; ++i) {
        free(aligned_buffers_[i]);
    }
}

void FastFlush::writeBufferToFile(const std::vector<char>& buffer, const std::string& output_filename) {
    int output_fd = open(output_filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_DIRECT, 0644);
    if (output_fd == -1) {
        throw std::runtime_error("Unable to open output file: " + std::string(strerror(errno)));
    }
    ScopedFd output_fd_guard(output_fd);

    // Preallocate the file
    if (posix_fallocate(output_fd, 0, buffer.size()) != 0) {
        throw std::runtime_error("File preallocation failed: " + std::string(strerror(errno)));
    }

    size_t total_written = 0;
    const size_t file_size = buffer.size();
    int current_queue = 0;
    int in_flight = 0;

    while (total_written < file_size) {
        // Submit I/O requests
        while (in_flight < queue_depth_ && total_written < file_size) {
            size_t bytes_to_write = std::min(BUFFER_SIZE, file_size - total_written);
            size_t aligned_write_size = (bytes_to_write / ALIGNMENT) * ALIGNMENT;
            
            if (aligned_write_size > 0) {
                std::memcpy(aligned_buffers_[current_queue], buffer.data() + total_written, aligned_write_size);
                prep_pwrite(&iocbs_[current_queue], output_fd, aligned_buffers_[current_queue], aligned_write_size, total_written);
                
                int ret = LinuxAIO::io_submit(aio_ctx_, 1, &iocbps_[current_queue]);
                if (ret != 1) {
                    throw std::runtime_error("io_submit failed: " + std::string(strerror(-ret)));
                }

                total_written += aligned_write_size;
                current_queue = (current_queue + 1) % queue_depth_;
                ++in_flight;
            }

            // Handle unaligned portion immediately
            if (bytes_to_write > aligned_write_size) {
                size_t remaining = bytes_to_write - aligned_write_size;
                int flags = fcntl(output_fd, F_GETFL);
                fcntl(output_fd, F_SETFL, flags & ~O_DIRECT);
                
                ssize_t written = pwrite(output_fd, buffer.data() + total_written, remaining, total_written);
                if (written == -1) {
                    throw std::runtime_error("Unaligned write error: " + std::string(strerror(errno)));
                }
                total_written += written;
                
                fcntl(output_fd, F_SETFL, flags);
            }
        }

        // Wait for I/O completions
        struct io_event events[queue_depth_];
        int min_complete = (total_written == file_size) ? in_flight : 1;
        int ret = LinuxAIO::io_getevents(aio_ctx_, min_complete, queue_depth_, events, nullptr);
        if (ret < 0) {
            throw std::runtime_error("io_getevents failed: " + std::string(strerror(-ret)));
        }

        for (int i = 0; i < ret; ++i) {
            if (events[i].res < 0) {
                std::cerr << "AIO error details:" << std::endl;
                std::cerr << "  Event index: " << i << std::endl;
                std::cerr << "  Error code: " << -events[i].res << std::endl;
                std::cerr << "  Error message: " << strerror(-events[i].res) << std::endl;
    
                // Cast the data pointer back to iocb*
                const struct iocb* iocb = static_cast<const struct iocb*>(reinterpret_cast<const void*>(events[i].obj));
                if (iocb) {
                    std::cerr << "  Bytes requested: " << iocb->aio_nbytes << std::endl;
                    std::cerr << "  File offset: " << iocb->aio_offset << std::endl;
                } else {
                    std::cerr << "  Unable to retrieve iocb information" << std::endl;
                }

                throw std::runtime_error("AIO write error: " + std::string(strerror(-events[i].res)));
            }
        }

        in_flight -= ret;
    }

    // Ensure all data is written to disk
    if (fdatasync(output_fd) == -1) {
        throw std::runtime_error("Fdatasync error: " + std::string(strerror(errno)));
    }

    if (total_written != file_size) {
        throw std::runtime_error("File size mismatch. Written: " + std::to_string(total_written) + 
                                 ", Expected: " + std::to_string(file_size));
    }
}

void FastFlush::prep_pwrite(struct iocb* iocb, int fd, void* buf, size_t count, long long offset) {
    memset(iocb, 0, sizeof(*iocb));
    iocb->aio_fildes = fd;
    iocb->aio_lio_opcode = IOCB_CMD_PWRITE;
    iocb->aio_buf = reinterpret_cast<uint64_t>(buf);
    iocb->aio_nbytes = count;
    iocb->aio_offset = offset;
}

std::vector<char> read_file_into_memory(const std::string& input_filename) {
    int input_fd = open(input_filename.c_str(), O_RDONLY);
    if (input_fd == -1) {
        throw std::runtime_error("Unable to open input file: " + std::string(strerror(errno)));
    }
    ScopedFd input_fd_guard(input_fd);

    struct stat sb;
    if (fstat(input_fd, &sb) == -1) {
        throw std::runtime_error("Unable to get file size: " + std::string(strerror(errno)));
    }
    size_t file_size = sb.st_size;

    std::vector<char> buffer(file_size);
    ssize_t bytes_read = read(input_fd, buffer.data(), file_size);
    if (bytes_read == -1 || static_cast<size_t>(bytes_read) != file_size) {
        throw std::runtime_error("Failed to read entire file: " + std::string(strerror(errno)));
    }

    return buffer;
}

void set_cpu_affinity(int target_cpu = -1) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);

    int num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    if (num_cpus <= 0) {
        throw std::runtime_error("Failed to determine the number of CPUs");
    }

    if (target_cpu < 0 || target_cpu >= num_cpus) {
        target_cpu = num_cpus - 1;  // Default to last CPU if not specified or invalid
    }

    CPU_SET(target_cpu, &cpuset);

    if (sched_setaffinity(0, sizeof(cpuset), &cpuset) == -1) {
        std::cerr << "Warning: Could not set CPU affinity" << std::endl;
    } else {
        std::cout << "Process pinned to CPU " << target_cpu << std::endl;
    }
}


void warm_up_run(FastFlush& io_manager, const std::vector<char>& buffer) {
    io_manager.writeBufferToFile(buffer, "/tmp/warmup_file.h5");
    unlink("/tmp/warmup_file");
}

std::chrono::milliseconds benchmarkFlushTime(const std::vector<char>& file_buffer) 
{
    /* set_cpu_affinity(): this optimisation is sensitive to cpu load , profile first */;
    
    FastFlush io_manager;
    warm_up_run(io_manager, file_buffer);
    const std::vector<std::string> output_filenames = {
        "/tmp/output1.h5", "/tmp/output2.h5",
        "/tmp/output3.h5", "/tmp/output4.h5"
    };

    std::vector<std::chrono::milliseconds> durations;
    durations.reserve(output_filenames.size());

    for (const auto& output_filename : output_filenames) {
        auto start = std::chrono::high_resolution_clock::now();
        io_manager.writeBufferToFile(file_buffer, output_filename);
        auto end = std::chrono::high_resolution_clock::now();
        durations.emplace_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
    }

    auto total_duration = std::accumulate(durations.begin(), durations.end(), 
                                          std::chrono::milliseconds(0));
    auto average_duration = total_duration / durations.size();

    constexpr auto rounding_factor = std::chrono::milliseconds(100);
     for (const auto& filename : output_filenames) {
        unlink(filename.c_str());
    }
    return rounding_factor * ((average_duration + rounding_factor - std::chrono::milliseconds(1)) / rounding_factor);
}