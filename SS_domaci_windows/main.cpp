#include <iostream>
#include <memory>

#include "utils.h"
#include "assembler.hpp"

using namespace std;

void usage_error();
bool process_input(int argc, char **argv, string &input_file_name, string &output_file_name);

int main(int argc, char **argv) {
    string input_file_name = "", output_file_name = "";

    if (!process_input(argc, argv, input_file_name, output_file_name)) {
        usage_error();
        return 1;
    }

    if (output_file_name == "") {
        output_file_name = Utils::get_file_name_without_extension(input_file_name) + ".o";
    }

    auto assembler = make_unique<Assembler>();
    assembler->assemble(input_file_name, output_file_name);

    return 0;
}

void usage_error() {
    cout << "Usage: asembler [options] <input_file_name>\n";
    cout << "Options: -o <file>     Place the output into <file>.\n";
}

bool process_input(int argc, char **argv, string &input_file_name, string &output_file_name) {
    if (argc < 2 || argc > 4) {
        return false;
    }

    if (string(argv[1]) == "-o") {
        if (argc != 4) {
            return false;
        }

        output_file_name = string(argv[2]);
        input_file_name = string(argv[3]);

    } else {
        if (argc != 2) {
            return false;
        }

        input_file_name = string(argv[1]);
    }
}
