#ifndef TWO_PASS_ASSEMBLER_H
#define TWO_PASS_ASSEMBLER_H

#include <vector>

#include "assembler.hpp"
#include "parser.hpp"

using namespace std;

class TwoPassAssembler : public Assembler {

    Parser* parser = nullptr;

    vector<Instruction*> instructions;

    vector<char> buffer;

    void erase();

protected:

    /*
        First pass builds the symbol table and report syntax errors, if any.
    */
    virtual void first_pass();

    /*
        Second pass generates machine code.
    */
    virtual void second_pass();

public:

    virtual ~TwoPassAssembler() {
        //erase();
    }

    virtual void assemble(string input_file_name, string output_file_name) override;
};

#endif
