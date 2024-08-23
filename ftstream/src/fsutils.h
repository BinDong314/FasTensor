// Support for std::filesystem constructs used in the codebase for C++11 compiling environment
// Namely: is_directory, is_regular_file, directory_iterator
// Since Dr.Bin has confirmed we will be using C++17 and later, this file will be deprectaed in future
#ifndef FSUTILS_H
#define FSUTILS_H

#include <string>

// We haven't checked which filesystem to include yet
#ifndef INCLUDE_STD_FILESYSTEM_EXPERIMENTAL

// Check for feature test macro for <filesystem>
#   if defined(__cpp_lib_filesystem)
#       define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 0

// Check for feature test macro for <experimental/filesystem>
#   elif defined(__cpp_lib_experimental_filesystem)
#       define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1

// We can't check if headers exist...
// Let's assume experimental to be safe
#   elif !defined(__has_include)
#       define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1

// Check if the header "<filesystem>" exists
#   elif __has_include(<filesystem>)
#       define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 0
// Check if the header "<filesystem>" exists
#   elif __has_include(<experimental/filesystem>)
#       define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1

// Fail if neither header is available with a nice error message

#   endif

// We priously determined that we need the exprimental version
#   if INCLUDE_STD_FILESYSTEM_EXPERIMENTAL
// Include it
#       include <experimental/filesystem>
#       define FS_AVAILABLE 1

// We need the alias from std::experimental::filesystem to std::filesystem
    namespace fs = experimental::filesystem;

// We have a decent compiler and can use the normal version
#   else
// Include it
#       include <filesystem>
#       define FS_AVAILABLE 1
    namespace fs = std::filesystem;
#   endif

#endif // #ifndef INCLUDE_STD_FILESYSTEM_EXPERIMENTAL

#ifndef FS_AVAILABLE
#   include <dirent.h>
#   include <sys/stat.h>
#endif
bool directoryExists(const std::string& directory_path);
std::string getCurrentDirectory();
std::string validateDirectory(const std::string& directory_path);

bool directoryExists(const std::string& directory_path) {
#ifdef FS_AVAILABLE
    fs::path path(directory_path);
    return fs::exists(path);
#else
    return is_directory(directory_path);
#endif
}

std::string getCurrentDirectory() {
#ifdef FS_AVAILABLE
    return fs::current_path().string();
#else
    char current_path[PATH_MAX];
    if (getcwd(current_path, sizeof(current_path)) != nullptr) {
        return std::string(current_path);
    }
    throw std::runtime_error("Failed to get current directory");
#endif
}

std::string validateDirectory(const std::string& directory_path)
{
    if (directory_path == ".") {
        return getCurrentDirectory();
    }

    if (!directoryExists(directory_path)) {
        throw std::runtime_error("Directory does not exist: " + directory_path);
    }

    return directory_path;
}
#endif // #ifndef FSUTILS_H