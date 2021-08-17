#include "two_pass_assembler.hpp"
#include "file_error.hpp"
#include "syntax_error.hpp"

#include "LineParser.h"

void TwoPassAssembler::erase() {
    for (Instruction* instruction : instructions) {
        delete instruction;
    }

    instructions.clear();
    buffer.clear();

    delete parser;
}

void TwoPassAssembler::first_pass() {
    cout << "First pass...\n";

    int current_section = -1;
    size_t location_counter = 0;
    Instruction* next_instruction = parser->get_next_instruction();

    /*
    string line;
    getline(input_file, line);
    Instruction *next_instruction = LineParser::parse(line);
    */

    instructions.push_back(next_instruction);

    while (true) {
        if (next_instruction->is_directive() && next_instruction->get_directive_name() == "end") {
            break;
        }

        if (next_instruction->has_label()) {
            /* TODO */
            string label = next_instruction->get_labels()[0];
            if (symbol_table->contains(label)) {
                /* TODO */
                cerr << "Symbol '" + label + "' is already declared.\n";
            } else {
                /* TODO */
                SymbolTable::SymbolInfo* symbol_info = new SymbolTable::SymbolInfo();
                symbol_info->value = location_counter;
                symbol_info->section = current_section;
                symbol_info->is_defined = true;
                symbol_info->is_global = false;
                symbol_info->entry_number = ++symbol_table->last_entry;

                symbol_table->insert(label, symbol_info);
            }
        }

        location_counter += next_instruction->size();

        while (!(next_instruction = parser->get_next_instruction()));

        /*
        string line;
        getline(input_file, line);
        Instruction *next_instruction = LineParser::parse(line);
        */

        instructions.push_back(next_instruction);
    }

    buffer = vector<char>(location_counter);
}

void TwoPassAssembler::second_pass() {
    cout << "Second pass...\n";

    /*
    // Return to the beginning of the input file
    input_file.seekg(0);
    */

    size_t location_counter = 0;
    for (Instruction* next_instruction : instructions) {
        /* TODO */

        location_counter += next_instruction->size();
    }
}

void TwoPassAssembler::assemble(string input_file_name, string output_file_name) {
    cout << "Assembling " + input_file_name + " in two passes.\n";

    input_file.open(input_file_name, ios::in);
    if (!input_file.is_open()) {
        cerr << "Unable to open file " + input_file_name + ".\n";
        throw FileError(input_file_name);
    }

    parser = new Parser(input_file);

    symbol_table = new SymbolTable();

    first_pass();

    input_file.close();

    output_file.open(output_file_name, ios::out | ios::trunc);
    if (!output_file.is_open()) {
        cerr << "Unable to open file " + output_file_name + ".\n";
        throw FileError(output_file_name);
    }

    second_pass();

    output_file << symbol_table << "\n";

    output_file.close();

    erase();

    cout << "Assembling finished. Created object file " + output_file_name + "\n";
}
