#include "instruction.hpp"
#include "parser.hpp"

unordered_map<string, Command> Instruction::command_code({
    { "halt", Command::HALT },
    { "int", Command::INT },
    { "iret", Command::IRET },
    { "call", Command::CALL },
    { "ret", Command::RET },
    { "jmp", Command::JMP },
    { "jeq", Command::JEQ },
    { "jne", Command::JNE },
    { "jgt", Command::JGT },
    { "push", Command::PUSH },
    { "pop", Command::POP },
    { "xchg", Command::XCHG },
    { "add", Command::ADD },
    { "sub", Command::SUB },
    { "mul", Command::MUL },
    { "div", Command::DIV },
    { "cmp", Command::CMP },
    { "not", Command::NOT },
    { "and", Command::AND },
    { "or", Command::OR },
    { "xor", Command::XOR },
    { "test", Command::TEST },
    { "shl", Command::SHL },
    { "shr", Command::SHR },
    { "ldr", Command::LDR },
    { "str", Command::STR },
    });

unordered_map<string, Directive> Instruction::directive_code({
    { "global", Directive::GLOBAL },
    { "extern", Directive::EXTERN },
    { "section", Directive::SECTION },
    { "word", Directive::WORD },
    { "skip", Directive::SKIP },
    { "equ", Directive::EQU },
    { "end", Directive::END },
    });

Command Instruction::get_command_code(string command_name) {
    if (command_code.find(command_name) != command_code.end()) {
        return command_code[command_name];
    }

    return Command::ERROR;
}

Directive Instruction::get_directive_code(string directive_name) {
    if (directive_code.find(directive_name) != directive_code.end()) {
        return directive_code[directive_name];
    }

    return Directive::ERROR;
}

ostream& operator<<(ostream& os, const Instruction& i) {
    if (i.directive) {
        os << i.directive_name;
        for (string dir_arg : i.directive_args) {
            os << " ";
        }
    } else {
        os << i.command_name;
        if (i.has_operand1()) {
            os << " " << i.operand1;
        }

        if (i.has_operand2()) {
            os << ", " << i.operand2;
        }
    }

    return os;
}
