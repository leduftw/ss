#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <fstream>
#include <string>

#include "symbol_table.hpp"
#include "instruction.hpp"

using namespace std;

class Assembler {

protected:

    ifstream input_file;
    ofstream output_file;

    SymbolTable symbol_table;

    virtual Instruction get_next_instruction() = 0;

public:

    virtual ~Assembler() { }

    virtual bool assemble(string input_file_name, string output_file_name) = 0;

};

#endif
