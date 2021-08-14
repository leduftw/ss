#include "symbol_table.hpp"

ostream &operator<<(ostream &os, const SymbolTable &symbol_table) {
    os << "============================ SYMBOL TABLE ============================\n";
    os << "Name" << "\t" << "Value" << "\t" << "Section" << "\t" << "Global" << "\t" << "Entry" << "\n";



    return os;
}
