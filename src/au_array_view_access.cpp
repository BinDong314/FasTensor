#include "au_array_view_access.h"

/**
 * @brief Access an view (subset) of an array 
 * 
 * @param view_buffer : pointer to the buffer for the view
 * @param array_buffer : pointer to the buffer for the array
 * @param array_size : the size for the array
 * @param start : the start address of the view
 * @param end : the end address of the view
 * @param read_write_code : mode to read or write, e.g., ARRAY_VIEW_READ , ARRAY_VIEW_WRITE 1
 * @param element_size : the size of each element data
 * @return int : non zero if works, otherwise failed
 */

inline int ArrayViewAccess(void *view_buffer, void *array_buffer, std::vector<unsigned long long> &array_size, std::vector<unsigned long long> &start, std::vector<unsigned long long> &end, int read_write_code, int element_size)
{
    std::vector<unsigned long long> view_size;
    unsigned long long element_count = 1;
    view_size.resize(array_size.size());
    for (int i = 0; i < array_size.size(); i++)
    {
        view_size[i] = end[i] - start[i] + 1;
        element_count = element_count * view_size[i];
    }

    if (view_size == array_size)
    {
        MEMCPY_ACCESS_HELP(view_buffer, 0, array_buffer, 0, element_count, element_size, read_write_code);
        return 0;
    }

    if (view_size.size() == 1)
    {
        MEMCPY_ACCESS_HELP(view_buffer, 0, array_buffer, start[0], element_count, element_size, read_write_code);
        return 0;
    }

    if (view_size.size() == 2)
    {
        for (int i = 0; i < view_size[0]; i++)
        {
            //Copy each row
            MEMCPY_ACCESS_HELP(view_buffer, view_size[1] * i, array_buffer, array_size[1] * i + start[0], view_size[1], element_size, read_write_code);
        }
        return 0;
    }

    //Generic version
    unsigned long long array_buffer_offset = 0, view_buffer_offset = 0;

    std::vector<unsigned long long> ord(start.begin(), start.end());
    for (unsigned long long i = 0; i < element_count; i++)
    {
        ROW_MAJOR_ORDER_MACRO(array_size, array_size.size(), ord, array_buffer_offset);
        MEMCPY_ACCESS_HELP(view_buffer, view_buffer_offset, array_buffer, array_buffer_offset, 1, element_size, read_write_code);
        view_buffer_offset++;
        ITERATOR_MACRO(ord, start, end); //update ord by increasing "1", row-major order
    }

    return 0;
}
