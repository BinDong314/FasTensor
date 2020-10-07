#ifndef ARRAY_UDF_UTILITY_MACRO_H
#define ARRAY_UDF_UTILITY_MACRO_H

/**
 * @brief help function to counts cells between start/end
 * 
 */
#define COUNT_CELLS(start_address_p, end_address_p, cells_count_p)                       \
    {                                                                                    \
        assert(start_address_p.size() == end_address_p.size());                          \
        cells_count_p = 1;                                                               \
        for (int i = 0; i < start_address_p.size(); i++)                                 \
        {                                                                                \
            cells_count_p = cells_count_p * (end_address_p[i] - start_address_p[i] + 1); \
        }                                                                                \
    }

/**
 * @brief help function to counts cells between start/end
 * 
 */
#define COUNT_RANGES(start_address_p, end_address_p, count_p)       \
    {                                                               \
        assert(start_address_p.size() == end_address_p.size());     \
        if (count_p.size() != start_address_p.size())               \
        {                                                           \
            count_p.resize(start_address_p.size());                 \
        }                                                           \
        for (int i = 0; i < start_address_p.size(); i++)            \
        {                                                           \
            count_p[i] = end_address_p[i] - start_address_p[i] + 1; \
        }                                                           \
    }

/**
 * @brief macro version of above two functions for speed
 * 
 */
#ifndef ROW_MAJOR_ORDER_MACRO
#define ROW_MAJOR_ORDER_MACRO(dsize, dsize_len, coordinate, offset) \
    {                                                               \
        offset = coordinate[0];                                     \
        for (int iii = 1; iii < dsize_len; iii++)                   \
        {                                                           \
            offset = offset * dsize[iii] + coordinate[iii];         \
        }                                                           \
    }
#endif

#ifndef ROW_MAJOR_ORDER_REVERSE_MACRO
#define ROW_MAJOR_ORDER_REVERSE_MACRO(offset, dsize, dsize_len, result_coord_v) \
    {                                                                           \
        unsigned long long temp_offset = offset;                                \
        for (int iii = dsize_len - 1; iii >= 1; iii--)                          \
        {                                                                       \
            result_coord_v[iii] = temp_offset % dsize[iii];                     \
            temp_offset = temp_offset / dsize[iii];                             \
        }                                                                       \
        result_coord_v[0] = temp_offset;                                        \
    }
#endif

#define AU_EXIT(info)                                                                                                            \
    {                                                                                                                            \
        std::cout << "Exit happens at file: " << __FILE__ << ",  function: " << __func__ << ", line: " << __LINE__ << std::endl; \
        std::cout << "Log : " << info << std::endl;                                                                              \
        std::exit(EXIT_FAILURE);                                                                                                 \
    }

#define AU_INFO(info)                                                                             \
    {                                                                                             \
        std::cout << "Info at " << __FILE__ << ", " << __func__ << ", " << __LINE__ << std::endl; \
        std::cout << "Info : " << info << std::endl;                                              \
    }

#endif