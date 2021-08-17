#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

using namespace std;

class Utils {
public:
    static string get_file_name_without_extension(string& file_name);

    static vector<string> split_string(string s, string delimiter);

    static void left_trim_string(string& s);

    static void right_trim_string(string& s);

    static string trim_string(string s);
};

#endif
