#include "utils.h"

string Utils::get_file_name_without_extension(string &file_name) {
    size_t last_index = file_name.find_last_of(".");

    // If last_index == string::npos, then substring is the whole string
    return file_name.substr(0, last_index);
}
