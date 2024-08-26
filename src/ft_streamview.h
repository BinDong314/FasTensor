#ifndef STREAMVIEW_H
#define STREAMVIEW_H
#include "directory_view.h"
#include <atomic>
#include <condition_variable>
#include <fcntl.h>
#include <filesystem>
#include <mutex>
#include <optional>
#include <queue>
#include <regex>
#include <set>
#include <thread>
#include <unistd.h>
#include <sys/file.h>

// We will remove locking, they are not useful on linux/UNIX
// https://gavv.net/articles/file-locks/
class StreamView {
public:
    StreamView(const std::regex& filePattern)
        : pattern(filePattern), isLocked(false), should_stop(false) {}
        

    struct FileEntry {
        std::string path;
        DirectoryView::FileInfo info;
        std::chrono::system_clock::time_point addedTime;

        FileEntry(std::string p, DirectoryView::FileInfo i)
            : path(std::move(p)), info(std::move(i)), 
              addedTime(std::chrono::system_clock::now()) {}
        // I refactored the code to use std::chrono instead of std::filesystem::file_time_type   
        // Please don't go back to file_time_type.
        FileEntry(std::string p, DirectoryView::FileInfo i, std::chrono::system_clock::time_point t)
        : path(std::move(p)), info(std::move(i)), 
          addedTime(t) {}
    };
    // This function exits if we don't detect a new file in next 100 ms
    bool HasNextFile() 
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
            
        cleanupYieldedFiles();

        if (!allFilesYielded())
                return true;
            
        // Wait for new files
        cv.wait_for(lock, std::chrono::milliseconds(100), [this]
        {
            cleanupYieldedFiles();
            return should_stop || !allFilesYielded();
         });

            // Check again after waiting
            return !should_stop && !allFilesYielded();
    }

    std::optional<FileEntry> getNextFile() 
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        
        cleanupYieldedFiles();

        for (const auto& entry : sorted_set) {
            if (yielded_files.find(entry.path) == yielded_files.end()) {
                yielded_files.insert(entry.path);
                return entry;
            }
        }

        return std::nullopt;
    }

    template<typename Comparator>
    class SortedInfiniteIterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = FileEntry;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        SortedInfiniteIterator(StreamView& view, Comparator comp, bool end = false)
            : view(view), comp(comp), end(end), 
            current(view.sorted_set.end()),
            initialScanComplete(false) {
            if (!end) {
                advance();
            }
        }
        SortedInfiniteIterator& operator++() {
            
            advance();
            return *this;
        }

        SortedInfiniteIterator operator++(int) {
            SortedInfiniteIterator tmp = *this;
            advance();
            return tmp;
        }

        bool operator==(const SortedInfiniteIterator& other) const {
            return end == other.end;
        }

        bool operator!=(const SortedInfiniteIterator& other) const {
            return !(*this == other);
        }

        reference operator*() const { 
            
            return *current; 
        }
        pointer operator->() const { return &(*current); }

        StreamView& view;
        Comparator comp;
        bool end;
        typename std::multiset<FileEntry, Comparator>::const_iterator current;
        bool initialScanComplete;
        std::set<std::string> yielded_files;

        void advance() {
            
            std::unique_lock<std::mutex> lock(view.queue_mutex);
            
            if (view.sorted_set.empty()) {                
                view.cv.wait(lock, [this] { return !view.sorted_set.empty() || view.should_stop; });
            }

            if (!initialScanComplete) {                
                for (auto it = view.sorted_set.begin(); it != view.sorted_set.end(); ++it) {
                    if (yielded_files.find(it->path) == yielded_files.end()) {
                        current = it;
                        yielded_files.insert(it->path);                        
                        return;
                    }
                }                
                initialScanComplete = true;
            }

            
            while (!view.should_stop) {
                bool newFileFound = false;
                for (auto it = view.sorted_set.begin(); it != view.sorted_set.end(); ++it) {
                    if (yielded_files.find(it->path) == yielded_files.end()) {
                        current = it;
                        yielded_files.insert(it->path);                       
                        newFileFound = true;
                        return;
                    }
                }

                if (!newFileFound) {
                    auto status = view.cv.wait_for(lock, std::chrono::seconds(1));
                    if (status == std::cv_status::timeout) {
                        // std::cout << "Wait timed out" << std::endl;
                        if (view.should_stop) {
                            // std::cout << "Iterator stopping" << std::endl;
                            end = true;
                            return;
                        }
                    } 
                }
            }

            end = true;
        }
    };

    // Comparators
    // The alphabetical comparator eventual limits to be a time_of_arrival comparator for slow streams
    // It will return files in order of time of arrival for slow streams
    struct AlphabeticalComparator {
        bool operator()(const FileEntry& a, const FileEntry& b) const {
            return a.path < b.path;
        }
    };

    struct TimeOfArrivalComparator {
        bool operator()(const FileEntry& a, const FileEntry& b) const {
            return a.info.lastModified < b.info.lastModified;
        }
    };

    // Yes I want my code to look cleaner
    using AlphabeticalIterator = SortedInfiniteIterator<AlphabeticalComparator>;
    using TimeOfArrivalIterator = SortedInfiniteIterator<TimeOfArrivalComparator>;

    AlphabeticalIterator alphabetical_begin() {
        return AlphabeticalIterator(*this, AlphabeticalComparator());
    }

    AlphabeticalIterator alphabetical_end() {
        return AlphabeticalIterator(*this, AlphabeticalComparator(), true);
    }

    TimeOfArrivalIterator time_of_arrival_begin() {
        return TimeOfArrivalIterator(*this, TimeOfArrivalComparator());
    }

    TimeOfArrivalIterator time_of_arrival_end() {
        return TimeOfArrivalIterator(*this, TimeOfArrivalComparator(), true);
    }

    // I wanted boost::make_iterator_range without including boost
    IteratorRange<AlphabeticalIterator> alphabetical_range()  {
        return IteratorRange<AlphabeticalIterator>(alphabetical_begin(), alphabetical_end());
    }

    IteratorRange<TimeOfArrivalIterator> time_of_arrival_range()  {
        return IteratorRange<TimeOfArrivalIterator>(time_of_arrival_begin(), time_of_arrival_end());
    }


void updateView(const DirectoryView& newView) {
    std::lock_guard<std::mutex> lock(view_mutex);
    auto differences = current_view.diff(newView);
    bool notifyIterators = false;

    {
        std::lock_guard<std::mutex> queueLock(queue_mutex);
        
        for (const auto& diff : differences) {
            if (diff.substr(0, 7) == "Added: " || diff.substr(0, 9) == "Modified: ") {
                std::string path = diff.substr(diff.find(": ") + 2);
                if (std::regex_match(path, pattern)) {
                    DirectoryView::FileInfo fileInfo;
                    try {
                        fileInfo = newView.getFileInfo(path);
                    } catch (const std::runtime_error& e) {
                        std::cerr << "Error getting file info: " << e.what() << std::endl;
                        continue;
                    }
                    current_view.updateFile(path, fileInfo.lastModified);
                    
                    auto now = std::chrono::system_clock::now();
                    
                    // Use lower_bound for more efficient lookup
                    auto it = sorted_set.lower_bound(FileEntry{path, fileInfo, now});
                    if (it != sorted_set.end() && it->path == path) {
                        // Update existing entry
                        sorted_set.erase(it);
                    }
                    sorted_set.emplace(FileEntry{path, fileInfo, now});
                    notifyIterators = true;
                }
            } else if (diff.substr(0, 9) == "Removed: ") {
                std::string path = diff.substr(9);
                auto it = sorted_set.find(FileEntry{path, {}, {}});
                if (it != sorted_set.end()) {
                    sorted_set.erase(it);
                    yielded_files.erase(path);
                    notifyIterators = true;
                }
            }
        }
    }

    if (notifyIterators) {
        cv.notify_all();
    }
}

    void stop() {
        should_stop = true;
        cv.notify_all();
    }


    void lock() {
        std::lock_guard<std::mutex> lock(view_mutex);
        if (!isLocked) {
            isLocked = true;
            for (const auto& diff : current_view.diff(DirectoryView())) {
                if (diff.substr(0, 7) == "Added: ") {
                    lockFile(diff.substr(7));
                }
            }
        }
    }

    void unlock() {
        std::lock_guard<std::mutex> lock(view_mutex);
        if (isLocked) {
            isLocked = false;
            for (const auto& [path, fd] : locked_files) {
                unlockFile(path);
            }
            locked_files.clear();
        }
    }

    void dbg_print(){
        current_view.dbg_print();
    }

private:
    std::regex pattern;
    DirectoryView current_view;
    bool isLocked;
    std::atomic<bool> should_stop;
    std::unordered_map<std::string, int> locked_files;
    std::mutex view_mutex;
    std::multiset<FileEntry, AlphabeticalComparator> sorted_set;
    std::mutex queue_mutex;
    std::condition_variable cv;
    
    std::set<std::string> yielded_files;
    bool allFilesYielded() const {
        return yielded_files.size() >= sorted_set.size();
    }

    void cleanupYieldedFiles() {
        for (auto it = yielded_files.begin(); it != yielded_files.end(); ) {
            if (sorted_set.find(FileEntry{*it, {}, {}}) == sorted_set.end()) {
                it = yielded_files.erase(it);
            } else {
                ++it;
            }
        }
    }

    void lockFile(const std::string& path) {
        int fd = open(path.c_str(), O_RDONLY);
        if (fd == -1) {
            throw std::runtime_error("Failed to open file: " + path);
        }
        if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
            close(fd);
            throw std::runtime_error("Failed to lock file: " + path);
        }
        locked_files[path] = fd;
    }

    void unlockFile(const std::string& path) {
        auto it = locked_files.find(path);
        if (it != locked_files.end()) {
            flock(it->second, LOCK_UN);
            close(it->second);
            locked_files.erase(it);
        }
    }
};
#endif