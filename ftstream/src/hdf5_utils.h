#ifndef HDF5_UTILS_H
#define HDF5_UTILS_H
#include <iostream>
#include <stdexcept>
#include "H5Ipublic.h"
#include "hdf5.h"

class H5Exception : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class H5Handle {
public:

    H5Handle() : m_id(-1), m_owns_handle(false) {}
    // Constructor for handles we own
/*     explicit H5Handle(hid_t id, bool take_ownership = true) : m_id(id), m_owns_handle(take_ownership) {
        // (m_owns_handle && m_id >= 0 && H5Iinc_ref(m_id)
        if (m_owns_handle && m_id >= 0 && H5Iinc_ref(m_id) < 0) {
                throw H5Exception("Failed to increment reference count");

        } 
    } */
        // Constructor for raw hid_t
    explicit H5Handle(hid_t id, bool take_ownership = true)
    : m_id(id), m_owns_handle(take_ownership) 
    {
        if (!isValid()) {
            throw H5Exception("Invalid HDF5 object ID");
        }
        // Don't increment ref count here, as it's a new or existing handle
    
    }
    // Constructor for handles we don't own
    static H5Handle from_existing(hid_t id) {
        return H5Handle(id, false);
    }

    ~H5Handle() {
        releaseAndReset();
    }

    void close() {
        releaseAndReset();
    }


    H5Handle(const H5Handle& other) : m_id(other.m_id), m_owns_handle(other.m_owns_handle) {
        if (m_owns_handle && m_id >= 0 && H5Iinc_ref(m_id) < 0) {
            throw H5Exception("Failed to increment reference count");
        }
    }

    H5Handle(H5Handle&& other) noexcept 
        : m_id(other.m_id), m_owns_handle(other.m_owns_handle) {
        other.m_id = -1;
        other.m_owns_handle = false;
    }

    H5Handle& operator=(const H5Handle& other) {
        if (this != &other) {
            releaseAndReset();
            m_id = other.m_id;
            m_owns_handle = other.m_owns_handle;
            if (m_owns_handle && m_id >= 0 && H5Iinc_ref(m_id) < 0) {
                throw H5Exception("Failed to increment reference count");
            }
        }
        return *this;
    }

    H5Handle& operator=(H5Handle&& other) noexcept {
        if (this != &other) {
            releaseAndReset();
            m_id = other.m_id;
            m_owns_handle = other.m_owns_handle;
            other.m_id = -1;
            other.m_owns_handle = false;
        }
        return *this;
    }

    operator hid_t() const { return m_id; }
    bool isValid() const { return m_id >= 0; }

    // Take ownership of the handle
    void take_ownership() {
        if (!m_owns_handle && m_id >= 0) {
            if (H5Iinc_ref(m_id) < 0) {
                throw H5Exception("Failed to take ownership of handle");
            }
            m_owns_handle = true;
        }
    }

    // Release ownership of the handle
    hid_t release() {
        hid_t temp = m_id;
        if (m_owns_handle && m_id >= 0) {
            if (H5Idec_ref(m_id) < 0) {
                throw H5Exception("Failed to release ownership of handle");
            }
        }
        m_id = -1;
        m_owns_handle = false;
        return temp;
    }

    void swap(H5Handle& other) noexcept {
        std::swap(m_id, other.m_id);
        std::swap(m_owns_handle, other.m_owns_handle);
    }

private:
    hid_t m_id;
    bool m_owns_handle;

    void releaseAndReset() {
        auto copy_id = m_id;
        if (m_owns_handle && m_id >= 0) {
            H5I_type_t type = H5Iget_type(m_id);
            herr_t status;
            /*Do not remove H5D,H5G close functions
            * you might think H5O will handle this
            * But, Call to H5Iget_type is expensive
            * H5O internally makes the same call
            * making a redundant expensive call to H5Iget_type again 
            */
            switch(type) {
                case H5I_FILE: status = H5Fclose(m_id);/* std::cout<<"file closed"<<copy_id <<std::endl */; break;
                case H5I_GROUP: status = H5Gclose(m_id); break;
                case H5I_DATATYPE: status = H5Tclose(m_id); break;
                case H5I_DATASPACE: status = H5Sclose(m_id); break;
                case H5I_DATASET: status = H5Dclose(m_id); break;
                case H5I_GENPROP_LST: status = H5Pclose(m_id); break;
                case H5I_ATTR: status = H5Aclose(m_id); break;
                default: status = H5Oclose(m_id); break;
            }
            if (status < 0) {
                std::cerr << "Warning: Failed to close HDF5 object" << copy_id<< std::endl;

            }
        }
        m_id = -1;
        m_owns_handle = false;
    }
};

#endif
