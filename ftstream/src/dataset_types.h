#ifndef DATASET_TYPES_H
#define DATASET_TYPES_H
#include "hdf5_utils.h"

// Enum to represent basic HDF5 datatypes
enum class HDF5BasicType {
    // NATURAL types
    Int8, UInt8, Int16, UInt16, Int32, UInt32, Int64, UInt64,
    Float32, Float64,  
    // Native types- deduced at runtime, we will support only 4
    NativeFloat, NativeDouble, NativeInt, NativeUInt,
    // Other types
    String, Compound, Other
};

// Struct to hold detailed type information
struct TypeInfo {
    HDF5BasicType basic_type;
    size_t size;
    bool is_little_endian;
    std::string custom_type_name;  // For compound or custom types

    // Default constructor
    TypeInfo()
        : basic_type(HDF5BasicType::Other),
          size(0),
          is_little_endian(true), // Because we are in x86 dominated world :P
          custom_type_name("Undefined")
    {}

    // Constructor for simple types
    TypeInfo(HDF5BasicType type, size_t s, bool le) 
        : basic_type(type), size(s), is_little_endian(le) {}

    // Constructor for compound or custom types
    // We will not be supporting compound types in this version
    TypeInfo(HDF5BasicType type, size_t s, bool le, const std::string& name) 
        : basic_type(type), size(s), is_little_endian(le), custom_type_name(name) {}
};

TypeInfo get_type_info(hid_t type_id);
hid_t recreate_datatype(const TypeInfo& type_info);
TypeInfo stringToTypeInfo(std::string type_string, bool is_le);


#endif