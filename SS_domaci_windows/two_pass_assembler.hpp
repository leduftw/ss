#ifndef TWO_PASS_ASSEMBLER_H
#define TWO_PASS_ASSEMBLER_H

#include <vector>

#include "assembler.hpp"

using namespace std;

class TwoPassAssembler : public Assembler {

    vector<char> buffer;

protected:

    /*
        First pass builds the symbol table and report syntax errors, if any.
    */
    virtual bool first_pass();

    /*
        Second pass generates machine code.
    */
    virtual bool second_pass();

    virtual Instruction get_next_instruction() override;

public:

    virtual ~TwoPassAssembler() { }

    virtual bool assemble(string input_file_name, string output_file_name) override;

};

#endif
