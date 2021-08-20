#ifndef FILE_ERROR_H
#define FILE_ERROR_H

#include <iostream>
#include <exception>
#include <string>

using namespace std;

class FileError : public exception {

    string msg;

public:

    FileError(string f_name) {
        msg = "Error occured while opening file " + f_name;
    }

    virtual const char* what() const noexcept {
        return msg.c_str();
    }
};

#endif
