#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <memory>

#include "instruction.hpp"
#include "syntax_error.hpp"
#include "two_pass_assembler.hpp"

using namespace std;

typedef int8_t byte;

class Parser {

    static string literal;
    static string symbol;
    static string reg;

    ifstream& input_file;

    int cur_line = 0;

    regex operation_regex;
    regex directive_regex;

    shared_ptr<Instruction> _get_next_instruction();
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

public:

    friend class TwoPassAssembler;
    friend class Instruction;

    static bool is_symbol(string s);
    static bool is_literal(string s);
    static bool is_register(string s);
    static pair<bool, byte> is_operand(string s, bool is_jump);

    Parser(ifstream& in);

    shared_ptr<Instruction> get_next_instruction();
};

#endif
