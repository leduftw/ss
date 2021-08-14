#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <string>

using namespace std;

class Assembler {
public:

    virtual ~Assembler() { }

    virtual bool assemble(string input_file_name, string output_file_name) = 0;

};

#endif
