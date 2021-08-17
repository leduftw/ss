#include "parser.hpp"
#include "utils.h"

Parser::Parser(ifstream& in_file) : input_file(in_file) {
    cur_line = 0;

    /*
        Label is optional and it can start with . or any other letter or digit or underscore.
        After that, it can only contain letters, digits and underscores.
        Before label name, after label name (before :), and after : we can have whitespaces.
        Only the label name is matched, but whole label regex is not saved as group in smatch object => (?:subpattern), passive group
    */
    string label_regex = "\\s*(?:([A-Za-z0-9_\\.][A-Za-z0-9_]*)\\s*:\\s*)?";

    // Command, e.g. "cmp".
    string command_regex = "([A-Za-z]+)";

    /*
         One operand, followed by zero or more whitespaces.
         Examples:
            1) r1
            2) $5
            3) $a
            4) 123
            5) %b
            6) [r1 + 123]
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

Instruction* Parser::get_next_instruction() {
    Instruction* instruction = new Instruction();
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
