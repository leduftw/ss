#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <regex>
#include <memory>

#include "instruction.hpp"
#include "syntax_error.hpp"

using namespace std;

class Parser {

    ifstream& input_file;

    int cur_line;

    regex operation_regex;
    regex directive_regex;

    shared_ptr<Instruction> build_instruction();
    void check_syntax(shared_ptr<Instruction> instruction);

public:

    Parser(ifstream& in);

    shared_ptr<Instruction> get_next_instruction();
};

#endif
