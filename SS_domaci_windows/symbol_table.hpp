#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "section.hpp"

using namespace std;

class SymbolTable {
public:
    struct SymbolInfo {
        int value = -1;
        shared_ptr<Section> section = nullptr;
        bool is_defined = false;
        bool is_global = false;
        bool is_external = false;
        int entry_number = -1;
    };

    friend ostream& operator<<(ostream& os, const SymbolTable& symbol_table);

    bool contains(string symbol) const {
        return symbols.find(symbol) != symbols.end();
    }

    void insert(string symbol, shared_ptr<SymbolInfo> symbol_info) {
        symbols[symbol] = symbol_info;
    }

    shared_ptr<SymbolInfo> get(string key) {
        return symbols[key];
    }

    size_t get_size() const {
        return symbols.size();
    }

private:
    unordered_map<string, shared_ptr<SymbolInfo>> symbols;
};

#endif