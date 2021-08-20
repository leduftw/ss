#include "symbol_table.hpp"

ostream& operator<<(ostream& os, const SymbolTable& symbol_table) {
    // Backup
    ios init(nullptr);
    init.copyfmt(os);

    symbol_table.print_header(os);
    symbol_table.print_content(os);

    // Restore
    os.copyfmt(init);

    return os;
}

void SymbolTable::print_header(ostream& os) const {
    os << left << setw(10) << setfill(' ') << "Name";
    os << left << setw(10) << setfill(' ') << "Value";
    os << left << setw(10) << setfill(' ') << "Section";
    os << left << setw(10) << setfill(' ') << "Global";
    os << left << setw(10) << setfill(' ') << "External";
    os << left << setw(10) << setfill(' ') << "Entry";
    os << "\n";
}

void SymbolTable::print_content(ostream& os) const {
    for (auto& p : symbols) {
        os << left << setw(10) << p.first;
        os << left << setw(10) << p.second->value;
        os << left << setw(10) << (p.second->section ? p.second->section->get_section_name() : "ABS");
        os << left << setw(10) << p.second->is_global;
        os << left << setw(10) << p.second->is_external;
        os << left << setw(10) << p.second->entry_number;
        os << "\n";
    }
}
