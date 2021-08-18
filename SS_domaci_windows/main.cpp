#include <iostream>
#include <memory>

#include "two_pass_assembler.hpp"
#include "utils.h"
#include "usage_error.hpp"
#include "syntax_error.hpp"

using namespace std;

void process_input(int argc, char** argv, string& input_file_name, string& output_file_name);

int main(int argc, char** argv) {
    try {
        string input_file_name = "", output_file_name = "";
        process_input(argc, argv, input_file_name, output_file_name);
        if (output_file_name == "") {
            output_file_name = Utils::get_file_name_without_extension(input_file_name) + ".o.txt";
        }

        // RAII
        unique_ptr<Assembler> assembler = make_unique<TwoPassAssembler>();
        assembler->assemble(input_file_name, output_file_name);

    } catch (exception& e) {
        cerr << e.what() << "\n";
    }

    return 0;
}

void process_input(int argc, char** argv, string& input_file_name, string& output_file_name) {
    if (argc < 2 || argc > 4) {
        throw UsageError();
    }

    if (string(argv[1]) == "-o") {
        if (argc != 4) {
            throw UsageError();
        }

        output_file_name = string(argv[2]);
        input_file_name = string(argv[3]);

    } else {
        if (argc != 2) {
            throw UsageError();
        }

        input_file_name = string(argv[1]);
    }

    input_file_name = "./test/" + input_file_name;
}
