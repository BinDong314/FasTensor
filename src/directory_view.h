
#ifndef DIRECTORY_VIEW_H
#define DIRECTORY_VIEW_H
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <filesystem>
namespace fs = std::filesystem;

template<typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end) : begin_(begin), end_(end) {}
    Iterator begin() const { return begin_; }
    Iterator end() const { return end_; }

private:
    Iterator begin_;
    Iterator end_;
};
class DirectoryView {
public:
    struct FileInfo {
        std::chrono::system_clock::time_point lastModified;
        bool operator!=(const FileInfo& other) const {
            return lastModified != other.lastModified;
        }
    };

    using FileMap = std::unordered_map<std::string, FileInfo>;

    
    template<typename MapIterator>
    class IteratorBase {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = typename MapIterator::value_type;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        explicit IteratorBase(MapIterator it) : it_(it) {}

        IteratorBase& operator++() { ++it_; return *this; }
        IteratorBase operator++(int) { auto tmp = *this; ++(*this); return tmp; }
        bool operator==(const IteratorBase& other) const { return it_ == other.it_; }
        bool operator!=(const IteratorBase& other) const { return it_ != other.it_; }
        reference operator*() const { return *it_; }
        pointer operator->() const { return &(*it_); }

    private:
        MapIterator it_;
    };

    using Iterator = IteratorBase<FileMap::iterator>;
    using ConstIterator = IteratorBase<FileMap::const_iterator>;

    // Iterators
    Iterator begin() { return Iterator(files.begin()); }
    Iterator end() { return Iterator(files.end()); }
    ConstIterator begin() const { return ConstIterator(files.begin()); }
    ConstIterator end() const { return ConstIterator(files.end()); }
    ConstIterator cbegin() const { return ConstIterator(files.cbegin()); }
    ConstIterator cend() const { return ConstIterator(files.cend()); }

    FileInfo getFileInfo(const std::string& path) const {
        auto it = files.find(path);
        if (it != files.end()) {
            return it->second;
        }
        throw std::runtime_error("File not found in DirectoryView: " + path);
    }

    void updateFile(const std::string& path, const std::chrono::system_clock::time_point& lastModified) {
        files[path] = {lastModified};
    }

    void removeFile(const std::string& path) {
        files.erase(path);
    }

    bool containsFile(const std::string& path) const {
        return files.find(path) != files.end();
    }
    void dbg_print() const {
        if (!files.empty()) {
            std::cout << files.begin()->first << std::endl;
        }
    }

    std::vector<std::string> diff(const DirectoryView& other) const {
        std::vector<std::string> differences;
        differences.reserve(files.size() + other.files.size());

        
        for (const auto& [path, info] : files) {
            auto it = other.files.find(path);
            if (it == other.files.end()) {
                differences.push_back("Removed: " + path);
            } else if (info != it->second) {
                differences.push_back("Modified: " + path);
            }
        }
        for (const auto& [path, info] : other.files) {
            if (!containsFile(path)) {
                differences.push_back("Added: " + path);
            }
        }
        return differences;
    }

private:
    FileMap files;
};
#endif