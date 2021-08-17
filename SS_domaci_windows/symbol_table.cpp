#include "symbol_table.hpp"

void SymbolTable::erase() {
    for (auto& it : symbols) {
        delete it.second;
    }
}

ostream& operator<<(ostream& os, const SymbolTable& symbol_table) {
    os << "============================ SYMBOL TABLE ============================\n";
    os << "Name" << "\t" << "Value" << "\t" << "Section" << "\t" << "Global" << "\t" << "Entry" << "\n";

    /* TODO */

    return os;
}
