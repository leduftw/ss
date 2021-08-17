#ifndef USAGE_ERROR_H
#define USAGE_ERROR_H

#include <iostream>
#include <exception>
#include <string>

using namespace std;

class UsageError : public exception {

    string msg;

public:

    UsageError() {
        string usage = "Usage: asembler [options] <input_file_name>\n";
        string options = "Options: -o <file> \t Place the output into <file>.\n";
        msg = usage + options;
    }

    virtual const char* what() const noexcept {
        return msg.c_str();
    }
};

#endif
