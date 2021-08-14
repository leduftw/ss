#include "two_pass_assembler.hpp"
#include "directive.hpp"

Instruction TwoPassAssembler::get_next_instruction() {

}

bool TwoPassAssembler::first_pass() {
    cout << "First pass...\n";
    bool status = true;

    int current_section = -1;
    size_t location_counter = 0;
    Instruction instruction = get_next_instruction();
    while (instruction.get_operation() != ".end") {
        if (instruction.has_label()) {
            string label = instruction.get_label();
            if (symbol_table.contains(label)) {
                cerr << "Symbol '" + label + "' is already declared.\n";
                status = false;
            } else {
                SymbolTable::SymbolInfo symbol_info;
                symbol_info.value = location_counter;
                symbol_info.section = current_section;
                symbol_info.is_defined = true;
                symbol_info.is_global = false;
                symbol_info.entry_number = ++symbol_table.last_entry;

                symbol_table.insert(label, symbol_info);
            }
        }

        location_counter += instruction.size();
        instruction = get_next_instruction();
    }

    buffer = vector<char>(location_counter);

    return status;
}

bool TwoPassAssembler::second_pass() {
    cout << "Second pass...\n";
    bool status = true;

    // Return to the beginning of the input file
    input_file.seekg(0);

    size_t location_counter = 0;
    Instruction instruction = get_next_instruction();
    while (instruction.get_operation() != ".end") {
        if (instruction.has_label()) {
            string label = instruction.get_label();
            if (symbol_table.contains(label)) {
                cerr << "Symbol '" + label + "' is already declared.\n";
                status = false;
            } else {
                symbol_table.insert(label, location_counter);
            }
        }

        location_counter += instruction.size();
        instruction = get_next_instruction();
    }


    return status;
}

bool TwoPassAssembler::assemble(string input_file_name, string output_file_name) {
    cout << "Assembling " + input_file_name + " in two passes.\n";

    input_file.open(input_file_name, ios::in);
    if (!input_file.is_open()) {
        cerr << "Unable to open file " + input_file_name + ".\n";
        return false;
    }

    output_file.open(output_file_name, ios::out | ios::trunc);
    if (!output_file.is_open()) {
        cerr << "Unable to open file " + output_file_name + ".\n";
        return false;
    }

    first_pass();

    second_pass();

    output_file << symbol_table << "\n";

    input_file.close();
    output_file.close();

    cout << "Assembling finished. Created object file " + output_file_name + "\n";

    return false;
}
