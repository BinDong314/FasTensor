// Example that shows simple usage of the INIReader class

#include <iostream>
#include <sstream>
#include <sstream>
#include <vector>
#include <string>
#include "INIReader.h"
#include <iomanip> // std::setprecision

extern double DT;
extern double DT_NEW;
extern double WINLEN_SEC;
extern std::vector<double> INTERP_Z;
extern std::vector<double> INTERP_ZF;
extern double eCoeff;
extern unsigned long long MASTER_INDEX;

extern std::string i_file;
extern std::string o_file;
extern std::string i_group;
extern std::string o_group;
extern std::string i_dataset;
extern std::string o_dataset;

extern bool enable_view_flag;
extern std::vector<unsigned long long> view_start;
extern std::vector<unsigned long long> view_count;

int read_config_file(std::string file_name, int mpi_rank)
{
    INIReader reader(file_name);

    if (reader.ParseError() < 0)
    {
        std::cout << "Can't load [" << file_name << " ]\n";
        return 1;
    }

    std::string temp_str;
    temp_str = reader.Get("parameter", "z", "0, 0.5, 1, 1, 0.5, 0");
    std::stringstream iss(temp_str);
    double number;
    std::vector<double> Z;
    while (iss >> number)
    {
        Z.push_back(number);
        if (iss.peek() == ',')
            iss.ignore();
    }
    DT = reader.GetReal("parameter", "dt", 0.002);
    DT_NEW = reader.GetReal("parameter", "dt_new", 0.008);
    WINLEN_SEC = reader.GetReal("parameter", "winLen_sec", 0.5);
    INTERP_Z = Z;
    INTERP_ZF[0] = reader.GetReal("parameter", "F1", 0);
    INTERP_ZF[1] = reader.GetReal("parameter", "F2", 0.002);
    INTERP_ZF[2] = reader.GetReal("parameter", "F3", 0.006);
    INTERP_ZF[3] = reader.GetReal("parameter", "F4", 14.5);
    INTERP_ZF[4] = reader.GetReal("parameter", "F5", 15);

    eCoeff = reader.GetReal("parameter", "eCoeff", 1.0);
    MASTER_INDEX = reader.GetInteger("parameter", "master_index", 0);
    if (!mpi_rank)
    {
        std::cout << "Parameters from file [" << file_name << "]: "
                  << "\n           dt=" << reader.GetReal("parameter", "dt", 0.002)
                  << "\n       dt_new=" << reader.GetReal("parameter", "dt_new", 0.002)
                  << "\n   winLen_sec=" << reader.GetReal("parameter", "winLen_sec", 0.5)
                  << "\n            z=";
        for (int i = 0; i < INTERP_Z.size(); i++)
        {
            std::cout << INTERP_Z[i] << ", ";
        }
        std::cout << "\n           zf=";
        for (int i = 0; i < INTERP_ZF.size(); i++)
        {
            std::cout << INTERP_ZF[i] << ", ";
        }
        std::cout << "\n            F1 = " << reader.GetReal("parameter", "F1", 0)
                  << "\n            F2 = " << reader.GetReal("parameter", "F2", 0.002)
                  << "\n            F3 = " << reader.GetReal("parameter", "F3", 0.006)
                  << "\n            F4 = " << reader.GetReal("parameter", "F4", 14.5)
                  << "\n            F5 = " << reader.GetReal("parameter", "F5", 15)
                  << "\n        eCoeff = " << reader.GetReal("parameter", "eCoeff", 1.0)
                  << "\n  master_index = " << reader.GetInteger("parameter", "master_index", 0);
    }
    std::string temp_str2 = reader.Get("parameter", "input_file", "UNKNOWN");
    if (temp_str2 != "UNKNOWN")
    {
        i_file = temp_str2;
        if (!mpi_rank)
            std::cout << "\n        input_file = " << i_file;
    }

    std::string temp_str3 = reader.Get("parameter", "output_file", "UNKNOWN");
    if (temp_str3 != "UNKNOWN")
    {
        o_file = temp_str3;
        if (!mpi_rank)
            std::cout << "\n       output_file = " << o_file;
    }
    std::string temp_str35 = reader.Get("parameter", "input_group", "UNKNOWN");
    if (temp_str35 != "UNKNOWN")
    {
        i_group = temp_str35;
        if (!mpi_rank)
            std::cout << "\n       input_group = " << i_group;
    }

    std::string temp_str36 = reader.Get("parameter", "output_group", "UNKNOWN");
    if (temp_str36 != "UNKNOWN")
    {
        o_group = temp_str36;
        if (!mpi_rank)
            std::cout << "\n      output_group = " << o_group;
    }

    std::string temp_str4 = reader.Get("parameter", "input_dataset", "UNKNOWN");
    if (temp_str4 != "UNKNOWN")
    {
        i_dataset = temp_str4;
        if (!mpi_rank)
            std::cout << "\n     input_dataset = " << i_dataset;
    }
    std::string temp_str5 = reader.Get("parameter", "output_dataset", "UNKNOWN");
    if (temp_str5 != "UNKNOWN")
    {
        o_dataset = temp_str5;
        if (!mpi_rank)
            std::cout << "\n    output_dataset = " << o_dataset << std::endl;
    }

    enable_view_flag = reader.GetBoolean("parameter", "view_enable_flag", false);
    if (enable_view_flag)
    {
        if (!mpi_rank)
            std::cout << "      View_enabled = true";
        std::string temp_str_veiw_start = reader.Get("parameter", "view_start", "0,0");
        std::stringstream iss(temp_str_veiw_start);
        unsigned long long number;
        std::vector<unsigned long long> Z;
        while (iss >> number)
        {
            Z.push_back(number);
            if (iss.peek() == ',')
                iss.ignore();
        }
        view_start = Z;

        std::string temp_str_veiw_count = reader.Get("parameter", "view_count", "30000, 11648");
        std::stringstream iss2(temp_str_veiw_count);
        unsigned long long number2;
        std::vector<unsigned long long> C;
        while (iss2 >> number2)
        {
            C.push_back(number2);
            if (iss2.peek() == ',')
                iss2.ignore();
        }
        view_count = C;

        if (!mpi_rank)
        {
            std::cout << "\n        View_start = ";
            for (int i = 0; i < view_start.size(); i++)
            {
                std::cout << view_start[i] << ", ";
            }

            std::cout << "\n        View_count = ";
            for (int i = 0; i < view_count.size(); i++)
            {
                std::cout << view_count[i] << ", ";
            }
            std::cout << "\n\n";
        }
    }

    fflush(stdout);

    return 0;
}