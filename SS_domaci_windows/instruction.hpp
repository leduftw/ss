#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

enum class Opcode {
    ERROR = -1,

    HALT = 0,
    INT = 1,
    IRET = 2,
    CALL = 3,
    RET = 4,
    JMP = 5,
    JEQ = 6,
    JNE = 7,
    JGT = 8,
    PUSH = 9,
    POP = 10,
    XCHG = 11,
    ADD = 12,
    SUB = 13,
    MUL = 14,
    DIV = 15,
    CMP = 16,
    NOT = 17,
    AND = 18,
    OR = 19,
    XOR = 20,
    TEST = 21,
    SHL = 22,
    SHR = 23,
    LDR = 24,
    STR = 25,
};

class Instruction {

    static unordered_map<string, Opcode> opcode;

    struct InstructionMetadata {
        Opcode opcode;
        int number_of_operands;
        bool jump;

        InstructionMetadata(Opcode oc = Opcode::ERROR, int num_op = -1, bool jmp = false) : opcode(oc), number_of_operands(num_op), jump(jmp) { }
    };

    int line = 0;
    vector<string> labels;
    bool directive = false;

    string command_name = "";
    string operand1 = "";
    string operand2 = "";

    
    string directive_name = "";
    vector<string> directive_args;

public:

    friend class TwoPassAssembler;

    static Opcode get_instruction_opcode(string name);

    static InstructionMetadata get_instruction_metadata(Opcode opcode);

    int get_line() const {
        return line;
    }

    void set_line(int l) {
        line = l;
    }

    bool has_label() const {
        return !labels.empty();
    }

    vector<string>& get_labels() {
        return labels;
    }

    void set_labels(vector<string> ls) {
        labels = ls;
    }

    bool is_command() const {
        return !is_directive();
    }

    string get_command_name() const {
        return command_name;
    }

    void set_command_name(string c) {
        command_name = c;
    }

    bool has_operand1() const {
        return operand1 != "";
    }

    string get_operand1() const {
        return operand1;
    }

    void set_operand1(string op1) {
        operand1 = op1;
    }

    bool has_operand2() const {
        return operand2 != "";
    }

    string get_operand2() const {
        return operand2;
    }

    void set_operand2(string op2) {
        operand2 = op2;
    }

    bool is_directive() const {
        return directive;
    }

    void set_directive(bool d) {
        directive = d;
    }

    string get_directive_name() const {
        return directive_name;
    }

    void set_directive_name(string dir_name) {
        directive_name = dir_name;
    }

    vector<string>& get_directive_args() {
        return directive_args;
    }

    void set_directive_args(vector<string> dir_args) {
        directive_args = dir_args;
    }

    /* TODO */
    size_t size() const;
};

#endif
