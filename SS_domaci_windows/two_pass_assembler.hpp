#ifndef TWO_PASS_ASSEMBLER_H
#define TWO_PASS_ASSEMBLER_H

#include "assembler.hpp"

class TwoPassAssembler : public Assembler {
protected:

    virtual bool first_pass();

    virtual bool second_pass();

public:

    virtual ~TwoPassAssembler() { }

    virtual bool assemble(string input_file_name, string output_file_name) override;

};

#endif
