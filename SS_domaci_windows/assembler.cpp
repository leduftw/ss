#include "assembler.hpp"

bool Assembler::first_pass() {
    cout << "First pass...\n";

    return true;
}

bool Assembler::second_pass() {
    cout << "Second pass...\n";

    return true;
}

bool Assembler::assemble(string input_file_name, string output_file_name) {
    cout << "Assembling " + input_file_name + "\n";

    first_pass();

    second_pass();

    cout << "Assembling finished. Created object file " + output_file_name + "\n";

    return true;
}
