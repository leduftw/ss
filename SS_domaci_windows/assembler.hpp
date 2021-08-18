#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include "parser.hpp"
#include "symbol_table.hpp"
#include "instruction.hpp"

using namespace std;

class Assembler {

protected:

    ifstream input_file;
    ofstream output_file;

    shared_ptr<Parser> parser;
    shared_ptr<SymbolTable> symbol_table;

public:

    virtual ~Assembler() { }

    virtual void assemble(string input_file_name, string output_file_name) = 0;

};

#endif
