#ifndef DBCB_H
#define DBCB_H
#include "directory_view.h"
#include <atomic>
#include <functional>
#include <queue>
#include <condition_variable>
#include <thread>

class DirectoryMonitor {
public:
    using Callback = std::function<void(const DirectoryView&)>;

    DirectoryMonitor(const std::string& path, std::chrono::seconds interval)
        : monitored_path(path), scan_interval(interval), shouldStop(false) {}

    ~DirectoryMonitor() {
        stop();
    }    

    void start() {
        // We keep a separate monitoring and callback thread to prevent long-running 
        // or blocking callbacks from impacting the monitoring loop.
        std::lock_guard<std::mutex> lock(thread_mutex);
        if (isRunning) {
            throw std::runtime_error("DirectoryMonitor is already running");
        }
        monitorThread = std::thread(&DirectoryMonitor::monitorLoop, this);
        callbackThread = std::thread(&DirectoryMonitor::callbackLoop, this);
    }

    void stop() {
        std::lock_guard<std::mutex> lock(thread_mutex);
        if (!isRunning) {
            return;  // Already stopped, no action needed
        }
        shouldStop = true;
        callbackCV.notify_one();
        if (monitorThread.joinable()) {
            monitorThread.join();
        }
        if (callbackThread.joinable()) {
            callbackThread.join();
        }
        isRunning = false;
    }

    void registerCallback(Callback callback) {
        std::lock_guard<std::mutex> lock(callback_mutex);
        callbacks.push_back(std::move(callback));
    }

private:
    std::string monitored_path;
    std::chrono::seconds scan_interval;
    DirectoryView previous_view;

    std::vector<Callback> callbacks;
    std::mutex callback_mutex;
    std::queue<DirectoryView> callback_queue;
    std::condition_variable callbackCV;

    bool isRunning = false;
    std::atomic<bool> shouldStop;
    std::thread monitorThread;
    std::thread callbackThread;

    std::mutex queue_mutex;
    std::mutex thread_mutex;

    static std::chrono::system_clock::time_point 
    file_time_to_system_clock(const std::filesystem::file_time_type& ft)
    {
        using namespace std::chrono;
        using namespace std::filesystem;

        // Get the duration since the file_time_type epoch
        auto ft_dur = ft.time_since_epoch();

        // Get the current time points for both clocks
        auto now_ft = file_time_type::clock::now();
        auto now_sys = system_clock::now();

        // Calculate the offset between the two clocks
        auto offset = now_sys.time_since_epoch() - now_ft.time_since_epoch();

        // Convert file_time to system_clock time, adjusting for the offset
        return system_clock::time_point(ft_dur + offset);
    }
    void monitorLoop() {
        while (!shouldStop) {
            DirectoryView latestView;
            // Optimisation opportunity:: Seeif we can use sets here
            for (const auto& entry : fs::recursive_directory_iterator(monitored_path)) {
                if (fs::is_regular_file(entry)) {
                    latestView.updateFile(entry.path().string(), file_time_to_system_clock(fs::last_write_time(entry)));
                }
            }

            auto differences = previous_view.diff(latestView);
            if (!differences.empty()) {
                std::lock_guard<std::mutex> lock(queue_mutex);
                callback_queue.push(latestView);
                callbackCV.notify_one();
            }

            previous_view = std::move(latestView);
            std::this_thread::sleep_for(scan_interval);
        }
    }

    void callbackLoop() {
        while (!shouldStop) {
            std::unique_lock<std::mutex> lock(queue_mutex);
            callbackCV.wait(lock, [this] { return !callback_queue.empty() || shouldStop; });

            if (shouldStop) break;

            auto view = std::move(callback_queue.front());
            callback_queue.pop();
            lock.unlock();

            std::lock_guard<std::mutex> callbackLock(callback_mutex);
            for (const auto& callback : callbacks) {
                callback(view);
            }
        }
    }
};
#endif