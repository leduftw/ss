#include <iostream>
#include <memory>

#include "two_pass_assembler.hpp"
#include "utils.hpp"

using namespace std;

int main(int argc, char** argv) {
    try {
        string input_file_name = "", output_file_name = "";
        Utils::process_input(argc, argv, input_file_name, output_file_name);
        if (output_file_name == "") {
            output_file_name = Utils::get_file_name_without_extension(input_file_name) + ".o.txt";
        }

        // RAII
        unique_ptr<Assembler> assembler = make_unique<TwoPassAssembler>();
        assembler->assemble(input_file_name, output_file_name);

    } catch (exception& e) {
        cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
