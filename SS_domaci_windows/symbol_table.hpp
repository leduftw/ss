#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <iomanip>

#include "section.hpp"

using namespace std;

class SymbolTable {
public:

    enum class SymbolType {
        UNDEFINED = -1,

        SECTION_NAME,
        LABEL,
        EQU_SYMBOL,
    };

    struct SymbolInfo {
        int value = -1;
        shared_ptr<Section> section = nullptr;
        bool is_defined = false;
        bool is_global = false;
        SymbolType symbol_type = SymbolType::UNDEFINED;

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

    void print_header(ostream& os) const;
    void print_content(ostream& os) const;
};

#endif
