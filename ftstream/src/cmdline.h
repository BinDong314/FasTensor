// commandline 
#include "docopt.h"
#include <vector>

static const char usage_screen[] =
R"(ftstream: A tool for generating and playingback HDF5 streams

Usage:
  ftstream playback (--seed-file=<file>) [--directory=<dir>] [--include-dataset=<dataset>]
                               [--exclude-dataset=<dataset>] [--interval=<sec>] [--stream-name=<name>] [--count=<count>]
  ftstream random [--dimensions=<dims>] [--directory=<dir>] [--dataset=<dataset>]
                             [--group=<group>] [--datatype=<type>] [--interval=<sec>] [--stream-name=<name>] [--count=<count>]
  ftstream (-h | --help)
  ftstream (-v | --version)

Options:
  -h --help                   Show this screen.
  -v --version                Show version.
  --directory=<dir>           Directory to save the generated files [default: current directory].
  --stream-name=<name>        Base file name for the generated files [default: stream_001.h5].
  --interval=<sec>            Time interval between file generations in seconds [default: 60].
  --count=<count>             Number of files to generate, zero means infinite [default: 1].
 
Playback Mode Options:
  --seed-file=<file>          Seed HDF5 file for playback mode.
  --include-dataset=<dataset> Comma-separated list of datasets to include in the generated files. [default: all]
  --exclude-dataset=<dataset> Comma-separated list of datasets to exclude from the generated files. [default:none]

Random Value Generation Mode Options:
  --dimensions=<dims>         Comma-separated list of tensor dimensions for random value generation mode.[default: 1024,1024]
  --dataset=<dataset>         Dataset name for the generated tensors [default: /testg/testd].
  --datatype=<type>           Data type for the generated tensors [default: float].
  --big-endian                Use big endian encoding
  )"; 

std::string value_or(const docopt::value& val, const std::string& default_value);
std::vector<std::string> value_or(const docopt::value& val, std::vector<std::string> default_value);
int value_or(const docopt::value& val, int default_value);
u_long value_or(const docopt::value& val, u_long default_value);


    /* Docopt populates the 'args' map with all the parameters defined in the usage string,
       regardless of whether they are provided by the user or not.
       As a result, 'args.count("--param") > 0' will always be true, even if the parameter is not set by the user.
       If a parameter is not provided by the user, 'args["--param"]' will contain an empty value.
       Calling 'asString()' on an empty value will result in a runtime error.
       To check if a parameter is actually set by the user, we use 'args["--param"].isString()'.
       If 'args["--param"].isString()' returns true, it means the parameter was provided by the user,
       and we can safely call 'asString()' to retrieve its value.*/
    // You are basically wishing for value_or support in C++11   
    // parameter = is_parameter_set ? parameter : default_value

// String specialization
std::string value_or(const docopt::value& val, const std::string& default_value) {
    return val.isString() ? val.asString() : default_value;
}

// int specialization
long value_or(const docopt::value& val, long default_value) {
    return val.isString() ? (val.asLong()) : default_value;
}

// u_long specialization
/* u_long value_or(const docopt::value& val, u_long default_value) {
    return val.isLong() ? static_cast<u_long>(val.asLong()) : default_value;
} */

std::vector<std::string> value_or(const docopt::value& val, std::vector<std::string> default_value)
{
  return val.isStringList() ? val.asStringList() : default_value;
}