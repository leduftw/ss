#include <fstream>

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

    ifstream input_file;
    input_file.open(input_file_name, ios::in);
    if (!input_file.is_open()) {
        cerr << "Unable to open file " + input_file_name + ".\n";
        return false;
    }

    ofstream output_file;
    output_file.open(output_file_name, ios::out | ios::trunc);
    if (!output_file.is_open()) {
        cerr << "Unable to open file " + output_file_name + ".\n";
        return false;
    }

    first_pass();

    second_pass();

    input_file.close();
    output_file.close();

    cout << "Assembling finished. Created object file " + output_file_name + "\n";

    return false;
}
