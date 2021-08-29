#ifndef TWO_PASS_ASSEMBLER_H
#define TWO_PASS_ASSEMBLER_H

#include <vector>
#include <memory>
#include <regex>

#include "assembler.hpp"
#include "parser.hpp"
#include "section.hpp"

using namespace std;

typedef int8_t byte;
typedef int16_t word;

struct AddressingMode {
    static const byte IMMEDIATE = 0x0;
    static const byte REGISTER_DIRECT = 0x1;
    static const byte REGISTER_INDIRECT = 0x2;
    static const byte REGISTER_INDIRECT_WITH_DISPLACEMENT = 0x3;
    static const byte DIRECT_MEMORY = 0x4;
    static const byte REGISTER_DIRECT_WITH_OPERAND = 0x5;  // used in jump instructions with operand %symbol

    static const byte ERROR = -1;
};

struct UpdateMode {
    static const byte NO_UPDATE = 0x0;
    static const byte DECREMENT_BY_TWO_BEFORE = 0x1;
    static const byte INCREMENT_BY_TWO_BEFORE = 0x2;
    static const byte DECREMENT_BY_TWO_AFTER = 0x3;
    static const byte INCREMENT_BY_TWO_AFTER = 0x4;
};

class TwoPassAssembler : public Assembler {

    shared_ptr<Section> current_section;
    vector<shared_ptr<Section>> sections;

    /*
        Support for continuation of a section.
        For example, program:
        {
        .section .text  # start of text section
            add r1, r2

        .section .bss
            .skip 8

        .section .text  # continuation of text section
            xor r2, r2
        }
        will be assembled as if it was written like this:
        {
        .section .text
            add r1, r2
            xor r2, r2

        .section .bss
            .skip 8
        }
    */
    bool continuation_of_section = false;

    void process_directive_first_pass(shared_ptr<Instruction> directive);
    void process_label_first_pass(shared_ptr<Instruction> instruction);
    void process_command_first_pass(shared_ptr<Instruction> command);

    void generate_machine_code_section(shared_ptr<Section> section);
    vector<byte> generate_machine_code_instruction(shared_ptr<Instruction> instruction) const;

    vector<byte> generate_machine_code_command(shared_ptr<Instruction> command) const;
    vector<byte> generate_machine_code_command_with_jump_operand1(shared_ptr<Instruction> command) const;
    vector<byte> generate_machine_code_command_with_data_operand2(shared_ptr<Instruction> command) const;

    vector<byte> generate_machine_code_directive(shared_ptr<Instruction> directive) const;

    byte get_register_index(string reg) const;
    byte get_lower_nibble_arithmetic_operation(shared_ptr<Instruction> command_arithmetic) const;
    byte get_lower_nibble_logical_operation(shared_ptr<Instruction> command_logical) const;
    byte get_lower_nibble_shift_operation(shared_ptr<Instruction> command_shift) const;

    byte get_lower_nibble_jump_command(shared_ptr<Instruction> command_shift) const;

    void create_relocation_record(string symbol_name, shared_ptr<SymbolTable::SymbolInfo> symbol_info, int& value, bool pc_relative) const;

protected:

    /*
        First pass builds the symbol table and report syntax errors, if any.
    */
    virtual void first_pass();

    /*
        Second pass generates machine code.
    */
    virtual void second_pass();

    virtual void create_obj_file();

public:

    virtual ~TwoPassAssembler() { }

    virtual void assemble(string input_file_name, string output_file_name) override;
};

#endif
