#ifndef AU_XDMF_H

#define AU_XDMF_H

#include <vector>
#include <type_traits>
#include <cstring>
#include <cmath>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <tuple>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <utility>
#include <variant>
#include <dirent.h>

#include "au_type.h"
#include "au_utility_macro.h"
#include "au_utility.h"

int create_xdmf(std::string file_name, std::string dset_name, std::vector<unsigned long long> dimensions, FTType data_element_type);

#endif