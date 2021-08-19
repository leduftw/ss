#ifndef TWO_PASS_ASSEMBLER_H
#define TWO_PASS_ASSEMBLER_H

#include <vector>
#include <memory>

#include "assembler.hpp"
#include "parser.hpp"
#include "section.hpp"

using namespace std;

class TwoPassAssembler : public Assembler {

    vector<shared_ptr<Instruction>> instructions_outside_of_sections;

    shared_ptr<Section> current_section;
    vector<shared_ptr<Section>> sections;

    bool continuation_of_section = false;

    void process_directive_first_pass(shared_ptr<Instruction> instruction);
    void process_label_first_pass(shared_ptr<Instruction> instruction);
    void process_command_first_pass(shared_ptr<Instruction> instruction);

    void generate_machine_code_for_section(shared_ptr<Section> section);
    vector<char> generate_machine_code_for_instruction(shared_ptr<Instruction> instruction);

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
