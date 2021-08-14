#include "two_pass_assembler.hpp"

bool TwoPassAssembler::first_pass() {
    cout << "First pass...\n";

    return false;
}

bool TwoPassAssembler::second_pass() {
    cout << "Second pass...\n";

    return false;
}

bool TwoPassAssembler::assemble(string input_file_name, string output_file_name) {
    cout << "Assembling " + input_file_name + " in two passes.\n";

    first_pass();

    second_pass();

    cout << "Assembling finished. Created object file " + output_file_name + "\n";

    return false;
}
