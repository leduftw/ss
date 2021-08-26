#include "two_pass_assembler.hpp"
#include "file_error.hpp"
#include "syntax_error.hpp"
#include "semantic_error.hpp"

void TwoPassAssembler::assemble(string input_file_name, string output_file_name) {
    cout << "Assembling " + input_file_name + " in two passes.\n";

    input_file.open(input_file_name, ios::in);
    if (!input_file.is_open()) {
        throw FileError(input_file_name);
    }

    parser = make_shared<Parser>(input_file);
    symbol_table = make_shared<SymbolTable>();
    relocation_table = make_shared<RelocationTable>();
    current_section = nullptr;
    sections.clear();

    first_pass();

    input_file.close();

    // Create output file only if syntax and semantic analysis didn't throw any exceptions in first pass
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
        // Parser by definition returns correct instruction, throws error otherwise
        while (!(instruction = parser->get_next_instruction()));

        if (instruction->is_directive() && instruction->get_directive_name() == "end") {
            break;
        }

        if (instruction->is_directive()) {
            process_directive_first_pass(instruction);
        } else {
            process_command_first_pass(instruction);
        }

        // All commands are saved, but only word and skip directives are saved since 
        // only they generate code. All other directives are not saved in instruction buffer.
        if (instruction->is_command()) {
            if (!current_section) {
                throw logic_error("Logic error at line " + to_string(instruction->get_line()) + ": Command not in section.");
            }

            current_section->get_instructions().push_back(instruction);
        }

        if (instruction->is_directive() &&
            (instruction->get_directive_name() == "word" || instruction->get_directive_name() == "skip")
            ) {

            if (!current_section) {
                throw logic_error("Logic error at line " + to_string(instruction->get_line()) + ": Directive '" + instruction->get_directive_name() + "' not in section.");
            }

            current_section->get_instructions().push_back(instruction);
        }

        if (current_section) {
            int sz = instruction->get_size();
            if (sz < 0) {
                string name = instruction->is_command() ? instruction->get_command_name() : instruction->get_directive_name();
                throw logic_error("Instruction '" + name + "' at line " + to_string(instruction->get_line()) + " returned size " + to_string(sz) + ".");
            }

            current_section->increment_location_counter(sz);
        }
    }

    if (!continuation_of_section && current_section) {
        sections.push_back(current_section);
    }

    continuation_of_section = false;
    current_section = nullptr;

    for (auto& section : sections) {
        vector<byte>& machine_code_for_section = section->get_machine_code();
        machine_code_for_section.resize(section->get_location_counter());
    }

    cout << "First pass done. Symbol table created.\n\n";
}

void TwoPassAssembler::process_directive_first_pass(shared_ptr<Instruction> directive) {
    process_label_first_pass(directive);

    string directive_name = directive->get_directive_name();
    if (directive_name == "section") {
        string section_name = directive->get_directive_args()[0];
        if (symbol_table->contains(section_name)) {
            auto symbol_info = symbol_table->get(section_name);
            if (section_name != symbol_info->section->get_section_name()) {
                throw SemanticError("Semantic error at line " + to_string(directive->get_line()) + ": Symbol '" + section_name + "' is already defined.");
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
            symbol_info->symbol_type = SymbolTable::SymbolType::SECTION_NAME;
            symbol_info->entry_number = symbol_table->get_size() + 1;

            symbol_table->insert(section_name, symbol_info);
        }

    } else if (directive_name == "equ") {
        vector<string>& args = directive->get_directive_args();
        string symbol_name = args[0];

        if (symbol_table->contains(symbol_name)) {
            auto symbol_info = symbol_table->get(symbol_name);
            if (symbol_info->is_defined) {
                throw SemanticError("Semantic error at line " + to_string(directive->get_line()) + ": Symbol '" + symbol_name + "' is already defined.");
            }

            symbol_info->value = stoi(args[1], nullptr, 0);
            symbol_info->is_defined = true;
            symbol_info->symbol_type = SymbolTable::SymbolType::EQU_SYMBOL;

        } else {
            // Other fields are not important
            auto symbol_info = make_shared< SymbolTable::SymbolInfo>();
            // Third argument 0 means that it should automatically deduce base
            symbol_info->value = stoi(args[1], nullptr, 0);
            symbol_info->symbol_type = SymbolTable::SymbolType::EQU_SYMBOL;
            symbol_info->entry_number = symbol_table->get_size() + 1;

            symbol_table->insert(symbol_name, symbol_info);
        }

    } else if (directive_name == "global") {
        vector<string>& args = directive->get_directive_args();
        for (string& arg : args) {
            if (symbol_table->contains(arg)) {
                auto symbol_info = symbol_table->get(arg);
                if (!symbol_info->is_global) {
                    symbol_info->is_global = true;
                } else {
                    throw SemanticError("Semantic error at line " + to_string(directive->get_line()) + ": Symbol '" + arg + "' is already global.");
                }

            } else {
                auto symbol_info = make_shared<SymbolTable::SymbolInfo>();
                symbol_info->is_global = true;
                symbol_info->entry_number = symbol_table->get_size() + 1;

                symbol_table->insert(arg, symbol_info);
            }
        }
        
    } else if (directive_name == "extern") {
        vector<string>& args = directive->get_directive_args();
        for (string& arg : args) {
            if (symbol_table->contains(arg)) {
                auto symbol_info = symbol_table->get(arg);
                if (!symbol_info->is_global) {
                    symbol_info->is_global = true;
                } else {
                    throw SemanticError("Semantic error at line " + to_string(directive->get_line()) + ": Symbol '" + arg + "' is already global.");
                }

            } else {
                auto symbol_info = make_shared<SymbolTable::SymbolInfo>();
                symbol_info->is_global = true;
                symbol_info->entry_number = symbol_table->get_size() + 1;

                symbol_table->insert(arg, symbol_info);
            }
        }

    } else if (!current_section && (directive_name == "word" || directive_name == "skip")) {
        throw SemanticError("Semantic error at line " + to_string(directive->get_line()) + ": Directive '" + directive_name + "' must be inside a section.");
    }
}

void TwoPassAssembler::process_label_first_pass(shared_ptr<Instruction> instruction) {
    // Label must be in section
    if (!current_section && instruction->has_label()) {
        throw SyntaxError("Syntax error at line " + to_string(instruction->get_line()) + ": Label must be placed inside a section.");
    }

    for (string& label : instruction->get_labels()) {
        if (!symbol_table->contains(label)) {
            shared_ptr<SymbolTable::SymbolInfo> symbol_info = make_shared<SymbolTable::SymbolInfo>();
            symbol_info->value = current_section->get_location_counter();
            symbol_info->section = current_section;
            symbol_info->is_defined = true;
            symbol_info->is_global = false;
            symbol_info->symbol_type = SymbolTable::SymbolType::LABEL;
            symbol_info->entry_number = symbol_table->get_size() + 1;

            symbol_table->insert(label, symbol_info);

        } else {
            auto symbol_info = symbol_table->get(label);
            if (symbol_info->is_defined) {
                throw SyntaxError("Syntax error at line " + to_string(instruction->get_line()) + ": Symbol '" + label + "' is already defined.");
            }

            symbol_info->value = current_section->get_location_counter();
            symbol_info->section = current_section;
            symbol_info->is_defined = true;
            symbol_info->symbol_type = SymbolTable::SymbolType::LABEL;
        }
    }
}

void TwoPassAssembler::process_command_first_pass(shared_ptr<Instruction> command) {
    if (!current_section) {
        throw SyntaxError("Syntax error at line " + to_string(command->get_line()) + ": Instruction '" + command->get_command_name() + "' must be specified inside a section.");
    }

    process_label_first_pass(command);
}

void TwoPassAssembler::second_pass() {
    cout << "Second pass...\n";

    for (auto& section : sections) {
        section->reset_location_counter();
        generate_machine_code_section(section);
        section->set_machine_code_ready(true);
    }

    cout << "Second pass done. Generated machine code for each section.\n\n";
}

void TwoPassAssembler::generate_machine_code_section(shared_ptr<Section> section) {
    int idx = 0;
    vector<byte>& section_machine_code = section->get_machine_code();
    for (auto& instruction : section->get_instructions()) {
        /*
        // Commands can only be in text section
        if (instruction->is_command() && !section->is_text_section()) {
            throw SemanticError("Semantic error at line " + to_string(instruction->get_line()) + ": Commands must be placed inside text section.");
        }
        */

        vector<byte> instruction_machine_code = generate_machine_code_instruction(instruction);
        for (byte b : instruction_machine_code) {
            section_machine_code[idx++] = b;
        }

        /*
        if (instruction->get_size() != instruction_machine_code.size()) {
            throw logic_error("Instruction and generated machine code for instruction do not have equal size!");
        }
        */

        section->increment_location_counter(instruction->get_size());
    }
}

vector<byte> TwoPassAssembler::generate_machine_code_instruction(shared_ptr<Instruction> instruction) const {
    if (instruction->is_command()) {
        return generate_machine_code_command(instruction);
    } else {
        return generate_machine_code_directive(instruction);
    }
}

vector<byte> TwoPassAssembler::generate_machine_code_command(shared_ptr<Instruction> command) const {
    switch (Instruction::get_command_code(command->get_command_name())) {
        case Command::HALT:
        {
            byte instr_descr = 0x00;

            return { instr_descr };
            break;
        }

        case Command::INT:
        {
            byte reg_ind = get_register_index(command->get_operand1());

            byte instr_descr = 0x10;
            byte regs_descr = (reg_ind << 4) | 0x0F;

            return { instr_descr, regs_descr };
            break;
        }

        case Command::IRET:
        {
            byte instr_descr = 0x20;

            return { instr_descr };
            break;
        }

        case Command::CALL:
        {
            /* TODO */
            break;
        }

        case Command::RET:
        {
            byte instr_descr = 0x40;

            return { instr_descr };
            break;
        }

        case Command::JMP:
        case Command::JEQ:
        case Command::JNE:
        case Command::JGT:
        {
            /* TODO */
            break;
        }

        case Command::PUSH:
        case Command::POP:
        {
            /* TODO */
            break;
        }

        case Command::XCHG:
        {
            byte reg_ind_dst = get_register_index(command->get_operand1());
            byte reg_ind_src = get_register_index(command->get_operand2());

            byte instr_descr = 0x60;
            byte regs_descr = (reg_ind_dst << 4) | reg_ind_src;

            return { instr_descr, regs_descr };
            break;
        }

        case Command::ADD:
        case Command::SUB:
        case Command::MUL:
        case Command::DIV:
        case Command::CMP:
        {
            byte nibble_hi = 0x70;
            byte nibble_lo = get_lower_nibble_arithmetic_operation(command);

            byte reg_ind_dst = get_register_index(command->get_operand1());
            byte reg_ind_src = get_register_index(command->get_operand2());

            byte instr_descr = nibble_hi | nibble_lo;
            byte regs_descr = (reg_ind_dst << 4) | reg_ind_src;

            return { instr_descr, regs_descr };
            break;
        }

        case Command::NOT:
        case Command::AND:
        case Command::OR:
        case Command::XOR:
        case Command::TEST:
        {
            byte nibble_hi = 0x80;
            byte nibble_lo = get_lower_nibble_logical_operation(command);

            byte reg_ind_dst = get_register_index(command->get_operand1());
            byte reg_ind_src = 0x0F;
            if (command->get_command_name() != "not") {
                reg_ind_src = get_register_index(command->get_operand2());
            }

            byte instr_descr = nibble_hi | nibble_lo;
            byte regs_descr = (reg_ind_dst << 4) | reg_ind_src;

            return { instr_descr, regs_descr };
            break;
        }

        case Command::SHL:
        case Command::SHR:
        {
            byte nibble_hi = 0x90;
            byte nibble_lo = get_lower_nibble_shift_operation(command);

            byte reg_ind_dst = get_register_index(command->get_operand1());
            byte reg_ind_src = get_register_index(command->get_operand2());

            byte instr_descr = nibble_hi | nibble_lo;
            byte regs_descr = (reg_ind_dst << 4) | reg_ind_src;

            return { instr_descr, regs_descr };
            break;
        }

        case Command::LDR:
        case Command::STR:
        {
            /* TODO */
            break;
        }
    }
}

byte TwoPassAssembler::get_register_index(string reg) const {
    byte reg_ind = 0;
    if (reg == "sp") {
        reg_ind = 6;
    } else if (reg == "pc") {
        reg_ind = 7;
    } else if (reg == "psw") {
        reg_ind = 8;
    } else {
        // General purpose register, e.g. "r4"
        reg_ind = reg[1] - '0';
    }

    return reg_ind;
}

byte TwoPassAssembler::get_lower_nibble_arithmetic_operation(shared_ptr<Instruction> command_arithmetic) const {
    byte nibble_lo_add = 0x0;
    byte nibble_lo_sub = 0x1;
    byte nibble_lo_mul = 0x2;
    byte nibble_lo_div = 0x3;
    byte nibble_lo_cmp = 0x4;

    string command_name = command_arithmetic->get_command_name();
    if (command_name == "add") {
        return nibble_lo_add;
    } else if (command_name == "sub") {
        return nibble_lo_sub;
    } else if (command_name == "mul") {
        return nibble_lo_mul;
    } else if (command_name == "div") {
        return nibble_lo_div;
    } else if (command_name == "cmp") {
        return nibble_lo_cmp;
    }

    throw logic_error("Instruction " + command_arithmetic->get_command_name() + " does not represent arithmetic instruction.");
}

byte TwoPassAssembler::get_lower_nibble_logical_operation(shared_ptr<Instruction> command_logical) const {
    byte nibble_lo_not = 0x0;
    byte nibble_lo_and = 0x1;
    byte nibble_lo_or = 0x2;
    byte nibble_lo_xor = 0x3;
    byte nibble_lo_test = 0x4;

    string command_name = command_logical->get_command_name();
    if (command_name == "not") {
        return nibble_lo_not;
    } else if (command_name == "and") {
        return nibble_lo_and;
    } else if (command_name == "or") {
        return nibble_lo_or;
    } else if (command_name == "xor") {
        return nibble_lo_xor;
    } else if (command_name == "test") {
        return nibble_lo_test;
    }

    throw logic_error("Instruction " + command_logical->get_command_name() + " does not represent logical instruction.");
}

byte TwoPassAssembler::get_lower_nibble_shift_operation(shared_ptr<Instruction> command_shift) const {
    byte nibble_lo_shl = 0x0;
    byte nibble_lo_shr = 0x1;

    string command_name = command_shift->get_command_name();
    if (command_name == "shl") {
        return nibble_lo_shl;
    } else if (command_name == "shr") {
        return nibble_lo_shr;
    }

    throw logic_error("Instruction " + command_shift->get_command_name() + " does not represent shift instruction.");
}

vector<byte> TwoPassAssembler::generate_machine_code_directive(shared_ptr<Instruction> directive) const {
    // Only .word and .skip directive generate machine code
    size_t number_of_bytes = directive->get_size();
    vector<byte> code(number_of_bytes, 0);
    if (directive->get_directive_name() == "skip") {
        return code;
    }

    if (directive->get_directive_name() == "word") {
        size_t idx = 0;
        vector<string>& args = directive->get_directive_args();
        for (string& arg : args) {
            word data = 0;
            if (parser->is_literal(arg)) {
                data = stoi(arg, nullptr, 0);
            } else {
                if (!symbol_table->contains(arg)) {
                    throw SemanticError("Semantic error at line " + to_string(directive->get_line()) + ": Symbol '" + arg + "' is undefined.");
                }

                auto symbol_info = symbol_table->get(arg);
                data = symbol_info->value;
            }

            // Little-endian
            code[idx++] = data & 0xFF;
            code[idx++] = (data >> 8) & 0xFF;
        }

        return code;
    }

    return { };
}

void TwoPassAssembler::create_obj_file() {
    output_file << "Symbol table\n";
    output_file << "============\n";
    output_file << *symbol_table << "\n";

    output_file << "Sections\n";
    output_file << "========\n";
    for (auto& section : sections) {
        output_file << *section << "\n\n";
    }

    output_file << "Relocation records\n";
    output_file << "==================\n";
    output_file << *relocation_table << "\n";
}
