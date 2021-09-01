#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <iostream>

using namespace std;

class Utils {

    static void left_trim_string(string& s);

    static void right_trim_string(string& s);

public:

    static void process_input(int argc, char** argv, string& input_file_name, string& output_file_name);

    static string get_file_name_without_extension(string& file_name);

    static vector<string> split_string(string s, string delimiter);

    static string trim_string(string s);

    static string int_to_hex(int i);
};

#endif
