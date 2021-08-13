#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <string>

using namespace std;

class Assembler {
public:

    bool first_pass();

    bool second_pass();

    bool assemble(string input_file_name, string output_file_name);

};

#endif
