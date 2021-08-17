#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <regex>

#include "instruction.hpp"
#include "syntax_error.hpp"

using namespace std;

class Parser {

    ifstream& input_file;

    int cur_line;

    regex operation_regex;
    regex directive_regex;

public:

    Parser(ifstream& in);

    Instruction* get_next_instruction();
};

#endif
