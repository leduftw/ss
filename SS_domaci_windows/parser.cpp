#include "parser.hpp"
#include "utils.h"

Parser::Parser(ifstream& in_file) : input_file(in_file) {
    /*
        Label is optional and it can start with . or any other letter or digit or underscore.
        After that, it can only contain letters, digits and underscores.
        Before label name, after label name (before :), and after : we can have whitespaces.
        Only the label name is matched, but whole label regex is not saved as group in smatch object => (?:subpattern), passive group
    */
    string label_regex = "\\s*(?:([A-Za-z0-9_\\.][A-Za-z0-9_]*)\\s*:\\s*)?";

    // Command name, e.g. "cmp".
    string command_regex = "([A-Za-z]+)";

    /*
         One operand, followed by zero or more whitespaces.
         Examples:
            1) r1
            2) $5
            3) $a
            4) 123
            5) %b
            6) *[r1 + 123]
            7) *r6
    */
    string operand_regex = "([^,#]+)\\s*";

    /*
        Zero, one or two operands, chained with comma. There can be any number of whitespaces before and after a comma.
    */
    string operands_regex = "(?:\\s+" + operand_regex + "(?:,\\s*" + operand_regex + ")?)?";

    /*
        Comment in the form "# comment until the end of line"
    */
    string comment_regex = "(?:\\s*#.*)?";

    operation_regex = regex(label_regex + "(?:" + command_regex + operands_regex + ")?" + comment_regex);

    /*
        Directive shall start with period (".").
        After that it shall have at least one letter, digit or underscore.
        After directive name, there can be any number of whitespaces.
    */
    string directive_name_regex = "\\.([A-Za-z0-9_\\.]+)\\s*";

    string directive_arg_regex = "([^,\\s#]+)";

    //                                                                                                           old regex (.*)
    directive_regex = regex(label_regex + directive_name_regex + "(?:\\s+" + directive_arg_regex + "\\s*(?:,\\s*" + "([^#]*)" + "\\s*)*)*" + comment_regex);
}

shared_ptr<Instruction> Parser::get_next_instruction() {
    shared_ptr<Instruction> instruction = build_instruction();
    if (instruction) {
        check_syntax(instruction);
    }

    return instruction;
}

shared_ptr<Instruction> Parser::build_instruction() {
    auto instruction = make_shared<Instruction>();
    while (true) {
        string line;
        if (!getline(input_file, line)) {
            throw SyntaxError("Syntax error: Input file does not have .end directive.");
        }

        cur_line++;

        smatch sm;
        bool is_directive = false;

        if (!regex_match(line, sm, operation_regex)) {
            if (!regex_match(line, sm, directive_regex)) {
                throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Line does not represent one valid instruction.");
            }

            if (sm[1].length() != 0 || sm[2].length() != 0 || sm[3].length() != 0 || sm[4].length() != 0) {
                is_directive = true;
            }
        }

        // Empty line or comment-only line
        if (sm[1].length() == 0 && sm[2].length() == 0 && sm[3].length() == 0 && sm[4].length() == 0) {
            return nullptr;
        }

        // Label-only line
        if (sm[1].length() != 0 && sm[2].length() == 0 && sm[3].length() == 0 && sm[4].length() == 0) {
            if (sm[1].matched) {
                instruction->get_labels().push_back(sm[1]);
                continue;
            } else {
                throw SyntaxError("Unknown syntax error. Line should be label-only, but it is not matched.");
            }
        }

        instruction->set_line(cur_line);
        if (sm[1].matched) {
            instruction->get_labels().push_back(sm[1]);
        }

        if (is_directive) {
            instruction->set_directive(true);
            instruction->set_directive_name(sm[2]);

            vector<string> args;

            if (sm[3].matched) {
                args.push_back(Utils::trim_string(sm[3]));
            }

            if (sm[4].matched) {
                vector<string> v = Utils::split_string(sm[4], ",");
                for (string& s : v) {
                    args.push_back(Utils::trim_string(s));
                }
            }

            for (string& arg : args) {
                instruction->get_directive_args().push_back(arg);
            }

        } else {
            instruction->set_directive(false);

            instruction->set_command_name(sm[2]);
            if (sm[3].matched) {
                instruction->set_operand1(sm[3]);
            }

            if (sm[4].matched) {
                instruction->set_operand2(sm[4]);
            }
        }

        break;
    }

    return instruction;
}

void Parser::check_syntax(shared_ptr<Instruction> instruction) {
    if (instruction->is_directive()) {
        // Directive syntax check
        switch (Instruction::get_directive_code(instruction->get_directive_name())) {
            case Directive::GLOBAL:
            case Directive::EXTERN:
                check_syntax_directive_symbol_list(instruction);
                break;

            case Directive::SECTION:
                check_syntax_directive_section_name(instruction);
                break;

            case Directive::WORD:
                check_syntax_directive_symbol_or_literal_list(instruction);
                break;

            case Directive::SKIP:
                check_syntax_directive_literal(instruction);
                break;

            case Directive::EQU:
                check_syntax_directive_symbol_literal(instruction);
                break;

            case Directive::END:
                check_syntax_directive_no_arguments(instruction);
                break;

            case Directive::ERROR:
                throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Directive '" + instruction->get_directive_name() + "' not recognized.");
                break;

            default:
                throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Directive '" + instruction->get_directive_name() + "' not recognized.");
                break;
        }

    } else {
        // Command syntax check
        switch (Instruction::get_command_code(instruction->get_command_name())) {
            case Command::HALT:
                check_syntax_command_no_arguments(instruction);
                break;

            case Command::INT:
                check_syntax_command_reg(instruction);
                break;

            case Command::IRET:
                check_syntax_command_no_arguments(instruction);
                break;

            case Command::CALL:
                instruction->set_jump(true);
                check_syntax_command_operand(instruction);
                break;

            case Command::RET:
                check_syntax_command_no_arguments(instruction);
                break;

            case Command::JMP:
            case Command::JEQ:
            case Command::JNE:
            case Command::JGT:
                instruction->set_jump(true);
                check_syntax_command_operand(instruction);
                break;

            case Command::PUSH:
            case Command::POP:
                check_syntax_command_reg(instruction);
                break;

            case Command::XCHG:
            case Command::ADD:
            case Command::SUB:
            case Command::MUL:
            case Command::DIV:
            case Command::CMP:
                check_syntax_command_reg_reg(instruction);
                break;

            case Command::NOT:
                check_syntax_command_reg(instruction);
                break;

            case Command::AND:
            case Command::OR:
            case Command::XOR:
            case Command::TEST:
            case Command::SHL:
            case Command::SHR:
                check_syntax_command_reg_reg(instruction);
                break;

            case Command::LDR:
            case Command::STR:
                check_syntax_command_reg_op(instruction);
                break;

            case Command::ERROR:
                throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Command '" + instruction->get_command_name() + "' not recognized.");
                break;

            default:
                throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Command '" + instruction->get_command_name() + "' not recognized.");
                break;
        }
    }
}

void Parser::check_syntax_directive_symbol_list(shared_ptr<Instruction> instruction) {
    vector<string>& args = instruction->get_directive_args();

    if (args.empty()) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": '" + instruction->get_directive_name() + "' directive must have at least one argument.");
    }

    for (string& arg : args) {
        if (!is_symbol(arg)) {
            throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Argument '" + arg + "' in '" + instruction->get_directive_name() + "' directive is not valid symbol.");
        }
    }
}

void Parser::check_syntax_directive_section_name(shared_ptr<Instruction> instruction) {
    vector<string>& args = instruction->get_directive_args();

    if (args.empty()) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Name of section missing in 'section' directive.");
    }

    if (args.size() > 1) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": 'section' directive must not have more than one argument.");
    }

    if (!is_symbol(args[0])) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Argument '" + args[0] + "' in 'section' directive is not valid symbol.");
    }
}

void Parser::check_syntax_directive_symbol_or_literal_list(shared_ptr<Instruction> instruction) {
    vector<string>& args = instruction->get_directive_args();

    if (args.empty()) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": '" + instruction->get_directive_name() + "' directive must have at least one argument.");
    }

    for (string& arg : args) {
        if (!is_symbol(arg) && !is_literal(arg)) {
            throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Argument '" + arg + "' in '" + instruction->get_directive_name() + "' directive is neither valid symbol nor valid literal.");
        }
    }
}

void Parser::check_syntax_directive_literal(shared_ptr<Instruction> instruction) {
    vector<string>& args = instruction->get_directive_args();

    if (args.empty()) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Argument of '" + instruction->get_directive_name() + "' directive not found.");
    }

    if (args.size() > 1) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": '" + instruction->get_directive_name() + "' directive must have only one argument.");
    }

    if (!is_literal(args[0])) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Argument '" + args[0] + "' in '" + instruction->get_directive_name() + "' is not valid literal.");
    }
}

void Parser::check_syntax_directive_symbol_literal(shared_ptr<Instruction> instruction) {
    vector<string>& args = instruction->get_directive_args();

    if (args.empty()) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Arguments of '" + instruction->get_directive_name() + "' directive not found.");
    }

    if (args.size() > 2) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": '" + instruction->get_directive_name() + "' directive must have only two arguments.");
    }

    if (!is_symbol(args[0])) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Argument '" + args[0] + "' in '" + instruction->get_directive_name() + "' directive is not valid symbol name.");
    }

    if (!is_literal(args[1])) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Argument '" + args[1] + "' in '" + instruction->get_directive_name() + "' directive is not valid literal.");
    }
}

void Parser::check_syntax_directive_no_arguments(shared_ptr<Instruction> instruction) {
    if (!instruction->get_directive_args().empty()) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": '" + instruction->get_directive_name() + "' directive must not have any arguments.");
    }
}

void Parser::check_syntax_command_no_arguments(shared_ptr<Instruction> instruction) {
    if (instruction->has_operand1() || instruction->has_operand2()) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Command '" + instruction->get_command_name() + "' must not have operands.");
    }
}

void Parser::check_syntax_command_reg(shared_ptr<Instruction> instruction) {
    if (!instruction->has_operand1()) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Command '" + instruction->get_command_name() + "' must have one operand.");
    }

    if (instruction->has_operand2()) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Command '" + instruction->get_command_name() + "' cannot have more than one operand.");
    }

    if (!is_register(instruction->get_operand1())) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": '" + instruction->get_operand1() + "' is not valid register.");
    }
}

void Parser::check_syntax_command_operand(shared_ptr<Instruction> instruction) {
    if (!instruction->has_operand1()) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Command '" + instruction->get_command_name() + "' must have one operand.");
    }

    if (instruction->has_operand2()) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Command '" + instruction->get_command_name() + "' cannot have more than one operand.");
    }

    if (!is_operand(instruction->get_operand1(), instruction->is_jump())) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": '" + instruction->get_operand1() + "' is not valid operand.");
    }
}

void Parser::check_syntax_command_reg_reg(shared_ptr<Instruction> instruction) {
    if (!instruction->has_operand1() || !instruction->has_operand2()) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Command '" + instruction->get_command_name() + "' must have two operands.");
    }

    if (!is_register(instruction->get_operand1())) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": '" + instruction->get_operand1() + "' is not valid register.");
    }

    if (!is_register(instruction->get_operand2())) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": '" + instruction->get_operand2() + "' is not valid register.");
    }
}

void Parser::check_syntax_command_reg_op(shared_ptr<Instruction> instruction) {
    if (!instruction->has_operand1() || !instruction->has_operand2()) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": Command '" + instruction->get_command_name() + "' must have two operands.");
    }

    if (!is_register(instruction->get_operand1())) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": '" + instruction->get_operand1() + "' is not valid register.");
    }

    if (!is_operand(instruction->get_operand2(), instruction->is_jump())) {
        throw SyntaxError("Syntax error at line " + to_string(cur_line) + ": '" + instruction->get_operand2() + "' is not valid operand.");
    }
}

bool Parser::is_symbol(string s) {
    return regex_match(s, regex(symbol));
}

bool Parser::is_literal(string s) {
    return regex_match(s, regex(literal));
}

bool Parser::is_register(string s) {
    return regex_match(s, regex(reg));
}

bool Parser::is_operand(string s, bool is_jump) {
    if (!is_jump) {
        return regex_match(s, regex("\\$" + literal)) ||
            regex_match(s, regex("\\$" + symbol)) ||
            regex_match(s, regex(literal)) ||
            regex_match(s, regex(symbol)) ||
            regex_match(s, regex("%" + symbol)) ||
            regex_match(s, regex(reg)) ||
            regex_match(s, regex("\\[\\s*" + reg + "\\s*\\]")) ||
            regex_match(s, regex("\\[\\s*" + reg + "\\s* \\+ \\s*" + literal + "\\s*\\]")) ||
            regex_match(s, regex("\\[\\s*" + reg + "\\s* \\+ \\s*" + symbol + "\\s*\\]"));
    }

    return regex_match(s, regex(literal)) ||
        regex_match(s, regex(symbol)) ||
        regex_match(s, regex("%" + symbol)) ||
        regex_match(s, regex("\\*" + literal)) ||
        regex_match(s, regex("\\*" + symbol)) ||
        regex_match(s, regex("\\*" + reg)) ||
        regex_match(s, regex("\\*\\[\\s*" + reg + "\\s*\\]")) ||
        regex_match(s, regex("\\*\\[\\s*" + reg + "\\s* \\+ \\s*" + literal + "\\s*\\]")) ||
        regex_match(s, regex("\\*\\[\\s*" + reg + "\\s* \\+ \\s*" + symbol + "\\s*\\]"));
}
