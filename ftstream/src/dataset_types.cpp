#include "dataset_types.h"
#include <algorithm>
#include <unordered_map>
// Function to get TypeInfo from an HDF5 datatype
TypeInfo get_type_info(hid_t type_id)
{
    H5T_class_t type_class = H5Tget_class(type_id);
    size_t type_size = H5Tget_size(type_id);
    bool is_le = (H5Tget_order(type_id) == H5T_ORDER_LE);

    // We will only support 4 NATIVE Types
    // They are deduced at runtime 
    if (H5Tequal(type_id, H5T_NATIVE_FLOAT) > 0)
        return TypeInfo(HDF5BasicType::NativeFloat, type_size, is_le);
    if (H5Tequal(type_id, H5T_NATIVE_DOUBLE) > 0)
        return TypeInfo(HDF5BasicType::NativeDouble, type_size, is_le);
    if (H5Tequal(type_id, H5T_NATIVE_INT) > 0)
        return TypeInfo(HDF5BasicType::NativeInt, type_size, is_le);
    if (H5Tequal(type_id, H5T_NATIVE_UINT) > 0)
        return TypeInfo(HDF5BasicType::NativeUInt, type_size, is_le);

    switch (type_class) {
        case H5T_INTEGER: {
            if (H5Tequal(type_id, H5T_STD_I8LE) > 0 || H5Tequal(type_id, H5T_STD_I8BE) > 0)
                return TypeInfo(HDF5BasicType::Int8, type_size, is_le);
            if (H5Tequal(type_id, H5T_STD_U8LE) > 0 || H5Tequal(type_id, H5T_STD_U8BE) > 0)
                return TypeInfo(HDF5BasicType::UInt8, type_size, is_le);
            if (H5Tequal(type_id, H5T_STD_I16LE) > 0 || H5Tequal(type_id, H5T_STD_I16BE) > 0)
                return TypeInfo(HDF5BasicType::Int16, type_size, is_le);
            if (H5Tequal(type_id, H5T_STD_U16LE) > 0 || H5Tequal(type_id, H5T_STD_U16BE) > 0)
                return TypeInfo(HDF5BasicType::UInt16, type_size, is_le);
            if (H5Tequal(type_id, H5T_STD_I32LE) > 0 || H5Tequal(type_id, H5T_STD_I32BE) > 0)
                return TypeInfo(HDF5BasicType::Int32, type_size, is_le);
            if (H5Tequal(type_id, H5T_STD_U32LE) > 0 || H5Tequal(type_id, H5T_STD_U32BE) > 0)
                return TypeInfo(HDF5BasicType::UInt32, type_size, is_le);
            if (H5Tequal(type_id, H5T_STD_I64LE) > 0 || H5Tequal(type_id, H5T_STD_I64BE) > 0)
                return TypeInfo(HDF5BasicType::Int64, type_size, is_le);
            if (H5Tequal(type_id, H5T_STD_U64LE) > 0 || H5Tequal(type_id, H5T_STD_U64BE) > 0)
                return TypeInfo(HDF5BasicType::UInt64, type_size, is_le);
            break;
        }
        case H5T_FLOAT: {
            if (H5Tequal(type_id, H5T_IEEE_F32LE) > 0 || H5Tequal(type_id, H5T_IEEE_F32BE) > 0)
                return TypeInfo(HDF5BasicType::Float32, type_size, is_le);
            if (H5Tequal(type_id, H5T_IEEE_F64LE) > 0 || H5Tequal(type_id, H5T_IEEE_F64BE) > 0)
                return TypeInfo(HDF5BasicType::Float64, type_size, is_le);
            break;
        }
        case H5T_STRING:
            return TypeInfo(HDF5BasicType::String, type_size, is_le);
        case H5T_COMPOUND:
            return TypeInfo(HDF5BasicType::Compound, type_size, is_le, "Compound");
        default:
            return TypeInfo(HDF5BasicType::Other, type_size, is_le, "Custom");
    }

    return TypeInfo(HDF5BasicType::Other, type_size, is_le, "Unknown");
}

// Function to recreate HDF5 datatype from TypeInfo
hid_t recreate_datatype(const TypeInfo& type_info) {
    hid_t type_id;

    switch (type_info.basic_type) {
        case HDF5BasicType::Float32:
            type_id = type_info.is_little_endian ? H5T_IEEE_F32LE : H5T_IEEE_F32BE;
            break;
        case HDF5BasicType::Float64:
            type_id = type_info.is_little_endian ? H5T_IEEE_F64LE : H5T_IEEE_F64BE;
            break;
        case HDF5BasicType::Int64:
            type_id = type_info.is_little_endian ? H5T_STD_I64LE : H5T_STD_I64BE;
            break;
        case HDF5BasicType::UInt64:
            type_id = type_info.is_little_endian ? H5T_STD_U64LE : H5T_STD_U64BE;
            break;
        case HDF5BasicType::Int32:
            type_id = type_info.is_little_endian ? H5T_STD_I32LE : H5T_STD_I32BE;
            break;
        case HDF5BasicType::UInt32:
            type_id = type_info.is_little_endian ? H5T_STD_U32LE : H5T_STD_U32BE;
            break;
        case HDF5BasicType::Int16:
            type_id = type_info.is_little_endian ? H5T_STD_I16LE : H5T_STD_I16BE;
            break;
        case HDF5BasicType::UInt16:
            type_id = type_info.is_little_endian ? H5T_STD_U16LE : H5T_STD_U16BE;
            break;
        case HDF5BasicType::Int8:
            type_id = type_info.is_little_endian ? H5T_STD_I8LE : H5T_STD_I8BE;
            break;
        case HDF5BasicType::UInt8:
            type_id = type_info.is_little_endian ? H5T_STD_U8LE : H5T_STD_U8BE;
            break;
        case HDF5BasicType::NativeFloat:
            type_id = H5Tcopy(H5T_NATIVE_FLOAT);
            break;
        case HDF5BasicType::NativeDouble:
            type_id = H5Tcopy(H5T_NATIVE_DOUBLE);
            break;
        case HDF5BasicType::NativeInt:
            type_id = H5Tcopy(H5T_NATIVE_INT);
            break;
        case HDF5BasicType::NativeUInt:
            type_id = H5Tcopy(H5T_NATIVE_UINT);
            break;
        case HDF5BasicType::String:
            type_id = H5Tcopy(H5T_C_S1);
            H5Tset_size(type_id, type_info.size);
            break;
        case HDF5BasicType::Compound:
        case HDF5BasicType::Other:
        // We will not be supporting compound types, here only for the completeness
            type_id = H5Tcreate(H5T_COMPOUND, type_info.size);
            break;
        default:
            throw std::runtime_error("Unknown datatype");
    }

    return type_id;
}

TypeInfo stringToTypeInfo(std::string type_string, bool is_le) {
    // Convert to lowercase for case-insensitive comparison
      std::string lowercase_type = type_string;
    std::transform(lowercase_type.begin(), lowercase_type.end(), lowercase_type.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    static const std::unordered_map<std::string, std::pair<HDF5BasicType, size_t>> type_map {
        {"int8", {HDF5BasicType::Int8, 1}},
        {"uint8", {HDF5BasicType::UInt8, 1}},
        {"int16", {HDF5BasicType::Int16, 2}},
        {"uint16", {HDF5BasicType::UInt16, 2}},
        {"int32", {HDF5BasicType::Int32, 4}},
        {"uint32", {HDF5BasicType::UInt32, 4}},
        {"int64", {HDF5BasicType::Int64, 8}},
        {"uint64", {HDF5BasicType::UInt64, 8}},
        {"float", {HDF5BasicType::Float32, 4}},
        {"float32", {HDF5BasicType::Float32, 4}},
        {"fp32", {HDF5BasicType::Float32, 4}},
        {"double", {HDF5BasicType::Float64, 8}},
        {"float64", {HDF5BasicType::Float64, 8}},
        {"fp64", {HDF5BasicType::Float64, 8}}
    };

    auto it = type_map.find(lowercase_type);
    if (it != type_map.end()) {
        const auto& [basic_type, size] = it->second;
        return TypeInfo(basic_type, size, is_le);
    } else {
        throw std::invalid_argument("Unknown type string: " + type_string);
    }
}