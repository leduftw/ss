#include "symbol_table.hpp"

ostream& operator<<(ostream& os, const SymbolTable& symbol_table) {
    os << "============================ SYMBOL TABLE ============================\n";
    os << "Name" << "\t" << "Value" << "\t" << "Section" << "\t" << "Global" << "\t" << "Entry" << "\n";
    for (auto& p : symbol_table.symbols) {
        os << p.first << "\t" << p.second->value << "\t" << *(p.second->section) << "\t" << 
            p.second->is_global << "\t" << p.second->entry_number << "\n";
    }

    return os;
}
