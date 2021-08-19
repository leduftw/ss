#include "two_pass_assembler.hpp"
#include "file_error.hpp"
#include "syntax_error.hpp"

void TwoPassAssembler::assemble(string input_file_name, string output_file_name) {
    cout << "Assembling " + input_file_name + " in two passes.\n";

    input_file.open(input_file_name, ios::in);
    if (!input_file.is_open()) {
        throw FileError(input_file_name);
    }

    symbol_table = make_shared<SymbolTable>();
    parser = make_shared<Parser>(input_file);
    instructions_outside_of_sections.clear();
    current_section = nullptr;
    sections.clear();

    first_pass();

    input_file.close();

    // Create output file only if syntax analysis didn't throw any exceptions in first pass 
    output_file.open(output_file_name, ios::out | ios::trunc);
    if (!output_file.is_open()) {
        throw FileError(output_file_name);
    }

    second_pass();

    create_obj_file();
    output_file.close();

    cout << "Assembling finished. Created object file " + output_file_name + "\n";
}

void TwoPassAssembler::first_pass() {
    cout << "First pass...\n";

    shared_ptr<Instruction> instruction;
    while (true) {
        // Find first line which isn't empty and isn't comment-only
        /* PARSER BY DEFINITION RETURNS VALID INSTRUCTION (TODO IN PARSER) */
        while (!(instruction = parser->get_next_instruction()));

        if (instruction->is_directive() && instruction->get_directive_name() == "end") {
            break;
        }

        if (instruction->is_directive()) {
            process_directive_first_pass(instruction);
        } else {
            process_command_first_pass(instruction);
        }

        if (instruction->get_directive_name() != "section" && current_section) {
            current_section->get_instructions().push_back(instruction);
        } else {
            instructions_outside_of_sections.push_back(instruction);
        }

        if (current_section) {
            current_section->increment_location_counter(instruction->get_size());
        } else {
            cout << "Warning: Instruction " << instruction << " is not in any section.\n";
        }
    }

    if (!continuation_of_section && current_section) {
        sections.push_back(current_section);
    }

    continuation_of_section = false;

    for (auto& section : sections) {
        vector<char>& machine_code_for_section = section->get_machine_code();
        machine_code_for_section.resize(section->get_location_counter());
    }
}

void TwoPassAssembler::process_directive_first_pass(shared_ptr<Instruction> instruction) {
    /* CHECK WHICH DIRECTIVES CANNOT CONTAIN LABELS! */
    string directive_name = instruction->get_directive_name();
    if ((directive_name == "section" || directive_name == "global" || directive_name == "extern") && instruction->has_label()) {
        throw SyntaxError("Syntax error at line " + to_string(instruction->get_line()) + ": Directive '" + directive_name + "' cannot contain labels.");
    }

    process_label_first_pass(instruction);

    if (instruction->get_directive_name() == "section") {
        string section_name = instruction->get_directive_args()[0];
        if (symbol_table->contains(section_name)) {
            auto symbol_info = symbol_table->get(section_name);
            if (section_name != symbol_info->section->get_section_name()) {
                throw SyntaxError("Syntax error at line " + to_string(instruction->get_line()) + ": Symbol '" + section_name + "' is already defined.");
            } else {
                if (!continuation_of_section) {
                    sections.push_back(current_section);
                }

                continuation_of_section = true;
                current_section = symbol_info->section;
            }
        } else {
            if (!continuation_of_section && current_section) {
                sections.push_back(current_section);
            }

            continuation_of_section = false;
            current_section = make_shared<Section>(section_name);

            auto symbol_info = make_shared<SymbolTable::SymbolInfo>();
            symbol_info->value = 0;
            symbol_info->section = current_section;
            symbol_info->is_defined = true;
            symbol_info->is_global = false;
            symbol_info->is_external = false;
            symbol_info->entry_number = symbol_table->get_size() + 1;

            symbol_table->insert(section_name, symbol_info);
        }
    }
}

void TwoPassAssembler::process_label_first_pass(shared_ptr<Instruction> instruction) {
    for (string& label : instruction->get_labels()) {
        if (!symbol_table->contains(label)) {
            shared_ptr<SymbolTable::SymbolInfo> symbol_info = make_shared<SymbolTable::SymbolInfo>();
            symbol_info->value = current_section->get_location_counter();
            symbol_info->section = current_section;
            symbol_info->is_defined = true;
            symbol_info->is_global = false;
            symbol_info->is_external = false;
            symbol_info->entry_number = symbol_table->get_size() + 1;

            symbol_table->insert(label, symbol_info);
        } else {
            throw SyntaxError("Syntax error at line " + to_string(instruction->get_line()) + ": Symbol '" + label + "' is already defined.");
        }
    }
}

void TwoPassAssembler::process_command_first_pass(shared_ptr<Instruction> instruction) {
    if (!current_section) {
        throw SyntaxError("Syntax error at line " + to_string(instruction->get_line()) + ": Instruction '" + instruction->get_command_name() + "' must be specified inside a section.");
    }

    process_label_first_pass(instruction);
}

void TwoPassAssembler::second_pass() {
    cout << "Second pass...\n";
    for (auto& section : sections) {
        section->reset_location_counter();
    }
}

void TwoPassAssembler::create_obj_file() {
    output_file << *symbol_table << "\n";
}
