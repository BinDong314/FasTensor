// Example that shows simple usage of the INIReader class

#include <iostream>
#include <sstream>
#include <sstream>
#include <vector>
#include <string>
#include "INIReader.h"
#include <iomanip> // std::setprecision

int read_config_file(std::string file_name)
{
    INIReader reader(file_name);

    if (reader.ParseError() < 0)
    {
        std::cout << "Can't load 'das-fft-full.config'\n";
        return 1;
    }

    std::string temp_str;
    temp_str = reader.Get("paramter", "z", "UNKNOWN");
    std::cout << temp_str << std::endl;
    std::stringstream iss(temp_str);
    double number;
    std::vector<double> Z;
    while (iss >> number)
    {
        Z.push_back(number);
        if (iss.peek() == ',')
            iss.ignore();
    }
    for (int i = 0; i < Z.size(); i++)
        std::cout << std::setprecision(15) << "Z: " << Z[i] << std::endl;

    std::cout << "Config loaded from 'das-fft-full.config': "
              << "\n, dt=" << reader.GetReal("paramter", "dt", 0.002)
              << "\n, dt_new=" << reader.GetReal("paramter", "dt_new", 0.002)
              << "\n, winLen_sec=" << reader.GetReal("paramter", "winLen_sec", 0.5)
              << "\n, F1=" << reader.GetReal("paramter", "F1", 0)
              << "\n, F2=" << reader.GetReal("paramter", "F2", 0.002)
              << "\n, F3=" << reader.GetReal("paramter", "F3", 0.006)
              << "\n, F4=" << reader.GetReal("paramter", "F4", 14.5)
              << "\n, F5=" << reader.GetReal("paramter", "F5", 15)
              << "\n, eCoeff=" << reader.GetReal("paramter", "eCoeff", 1.0)
              << "\n, master_index=" << reader.GetInteger("paramter", "master_index", 0)
              << "\n";
    return 0;
}
