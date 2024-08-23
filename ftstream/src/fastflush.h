#ifndef FASTFLUSH_H
#define FASTFLUSH_H
#include <chrono>
#include <string>
#include <unistd.h>
#include <vector>
#include <sys/syscall.h>
#include <linux/aio_abi.h>

std::vector<char> read_file_into_memory(const std::string& input_filename);
void setCpuAffinity(int target_cpu = -1);
void high_performance_copy(const std::string& input_filename);
std::chrono::milliseconds benchmarkFlushTime(const std::vector<char>& file_buffer) ;

// Learned this RAII trick from chromium project
class ScopedFd {
public:
    explicit ScopedFd(int fd) : fd_(fd) {}
    ~ScopedFd() { if (fd_ != -1) close(fd_); }
    int get() const { return fd_; }
    ScopedFd(const ScopedFd&) = delete;
    ScopedFd& operator=(const ScopedFd&) = delete;
private:
    int fd_;
};

class LinuxAIO {
public:
    static int io_setup(unsigned nr_events, aio_context_t* ctx_idp) {
        return syscall(__NR_io_setup, nr_events, ctx_idp);
    }

    static int io_submit(aio_context_t ctx_id, long nr, struct iocb** iocbpp) {
        return syscall(__NR_io_submit, ctx_id, nr, iocbpp);
    }

    static int io_getevents(aio_context_t ctx_id, long min_nr, long nr, struct io_event* events, struct timespec* timeout) {
        return syscall(__NR_io_getevents, ctx_id, min_nr, nr, events, timeout);
    }

    static int io_destroy(aio_context_t ctx_id) {
        return syscall(__NR_io_destroy, ctx_id);
    }

private:
    // Syscall numbers for x86_64
    #if defined(__x86_64__)
        static const int NR_IO_SETUP = 206;
        static const int NR_IO_DESTROY = 207;
        static const int NR_IO_SUBMIT = 209;
        static const int NR_IO_GETEVENTS = 208;
    #else
        #error "Unsupported architecture"
    #endif
};

class FastFlush {
public:
    // Profile to find optimal queue depth, depth=8 is a sane default
    explicit FastFlush(int queue_depth = 8);
    ~FastFlush();

    void writeBufferToFile(const std::vector<char>& buffer, const std::string& output_filename);

private:
    static const size_t BUFFER_SIZE = 1024 * 1024 * 1; // 1 MB buffer
    static const size_t ALIGNMENT = 4096; // 4KB alignment for direct I/O

    int queue_depth_;
    aio_context_t aio_ctx_ = 0;
    std::vector<char*> aligned_buffers_;
    std::vector<struct iocb> iocbs_;
    std::vector<struct iocb*> iocbps_;

    void prep_pwrite(struct iocb* iocb, int fd, void* buf, size_t count, long long offset);
};

#endif
