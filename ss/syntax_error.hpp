#ifndef SYNTAX_ERROR_H
#define SYNTAX_ERROR_H

#include <iostream>
#include <exception>
#include <string>

using namespace std;

class SyntaxError : public exception {

    string msg;

public:

    SyntaxError(string message) {
        msg = message;
    }

    virtual const char* what() const noexcept {
        return msg.c_str();
    }
};

#endif
