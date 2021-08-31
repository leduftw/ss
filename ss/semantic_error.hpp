#ifndef SEMANTIC_ERROR_H
#define SEMANTIC_ERROR_H

#include <iostream>
#include <exception>
#include <string>

using namespace std;

class SemanticError : public exception {

    string msg;

public:

    SemanticError(string message) {
        msg = message;
    }

    virtual const char* what() const noexcept {
        return msg.c_str();
    }
};

#endif
