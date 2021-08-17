#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class SymbolTable {

    struct SymbolInfo {
        int value;
        int section;
        bool is_defined;
        bool is_global;
        int entry_number;
    };

    size_t last_entry = 0;
    unordered_map<string, SymbolInfo*> symbols;

    void erase();

public:

    friend class TwoPassAssembler;

    friend ostream& operator<<(ostream& os, const SymbolTable& symbol_table);

    ~SymbolTable() {
        erase();
    }

    bool contains(string symbol) const {
        return symbols.find(symbol) != symbols.end();
    }

    void insert(string symbol, SymbolInfo* symbol_info) {
        symbols[symbol] = symbol_info;
    }
};

#endif
