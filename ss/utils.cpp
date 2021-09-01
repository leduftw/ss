#include "utils.hpp"
#include "usage_error.hpp"

void Utils::process_input(int argc, char** argv, string& input_file_name, string& output_file_name) {
    if (argc < 2 || argc > 4) {
        throw UsageError();
    }

    if (string(argv[1]) == "-o") {
        if (argc != 4) {
            throw UsageError();
        }

        output_file_name = string(argv[2]);
        input_file_name = string(argv[3]);

    } else {
        if (argc != 2) {
            throw UsageError();
        }

        input_file_name = string(argv[1]);
    }

    // If you just want input file name as argument without typing whole path, uncomment next line
    //input_file_name = "./test/" + input_file_name;
}

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
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !isspace(ch);
    }));
}

void Utils::right_trim_string(string& s) {
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !isspace(ch);
    }).base(), s.end());
}

string Utils::trim_string(string s) {
    left_trim_string(s);
    right_trim_string(s);

    return s;
}

string Utils::int_to_hex(int i) {
    stringstream stream;
    stream << "0x"
        << setfill('0') << setw(4)
        << hex << i;

    return stream.str();
}
