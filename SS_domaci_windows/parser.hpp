#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <memory>

#include "instruction.hpp"
#include "syntax_error.hpp"

using namespace std;

class Parser {

    ifstream& input_file;

    int cur_line = 0;

    string literal = "(\\d+)";

    // Warning: Following regex can also match registers, that's why we need additional processing if we get a match
    string symbol = "([\\.A-Za-z_][\\.A-Za-z0-9_]*)";

    string reg = "(?:(?:r[0-7])|(?:sp)|(?:pc)|(?:psw))";

    regex operation_regex;
    regex directive_regex;

    shared_ptr<Instruction> build_instruction();
    void check_syntax(shared_ptr<Instruction> instruction);

    void check_syntax_directive_symbol_list(shared_ptr<Instruction> instruction);
    void check_syntax_directive_section_name(shared_ptr<Instruction> instruction);
    void check_syntax_directive_symbol_or_literal_list(shared_ptr<Instruction> instruction);
    void check_syntax_directive_literal(shared_ptr<Instruction> instruction);
    void check_syntax_directive_symbol_literal(shared_ptr<Instruction> instruction);
    void check_syntax_directive_no_arguments(shared_ptr<Instruction> instruction);

    void check_syntax_command_no_arguments(shared_ptr<Instruction> instruction);
    void check_syntax_command_reg(shared_ptr<Instruction> instruction);
    void check_syntax_command_operand(shared_ptr<Instruction> instruction);
    void check_syntax_command_reg_reg(shared_ptr<Instruction> instruction);
    void check_syntax_command_reg_op(shared_ptr<Instruction> instruction);

    bool is_symbol(string s);
    bool is_literal(string s);
    bool is_register(string s);
    bool is_operand(string s, bool is_jump);

public:

    Parser(ifstream& in);

    shared_ptr<Instruction> get_next_instruction();
};

#endif
