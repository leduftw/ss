#include "utils.h"

string Utils::get_file_name_without_extension(string& file_name) {
    size_t last_index = file_name.find_last_of(".");

    // If last_index == string::npos, then substring is the whole string
    return file_name.substr(0, last_index);
}

vector<string> Utils::split_string(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));

    return res;
}

void Utils::left_trim_string(string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void Utils::right_trim_string(string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

string Utils::trim_string(string s) {
    left_trim_string(s);
    right_trim_string(s);

    return s;
}
