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
        bool is_external = false;
        SymbolType symbol_type = SymbolType::UNDEFINED;

        int entry_number = -1;
    };

    friend ostream& operator<<(ostream& os, const SymbolTable& symbol_table);

    SymbolTable() {
        string section_name = "UND";
        auto symbol_info = make_shared<SymbolInfo>();
        symbol_info->value = 0;

        und_section = make_shared<Section>(section_name);
        symbol_info->section = und_section;

        symbol_info->is_defined = true;
        symbol_info->is_global = false;
        symbol_info->is_external = false;
        symbol_info->symbol_type = SymbolType::SECTION_NAME;
        symbol_info->entry_number = 0;

        insert(section_name, symbol_info);
    }

    bool contains(string symbol) const {
        return symbols.find(symbol) != symbols.end();
    }

    void insert(string symbol, shared_ptr<SymbolInfo> symbol_info) {
        symbols[symbol] = symbol_info;
    }

    shared_ptr<SymbolInfo> get(string key) {
        return symbols[key];
    }

    shared_ptr<Section> get_und_section() const {
        return und_section;
    }

    size_t get_size() const {
        return symbols.size();
    }

    pair<string, shared_ptr<SymbolInfo>> get_symbol_by_entry_number(size_t entry_no) const;

private:

    shared_ptr<Section> und_section;

    unordered_map<string, shared_ptr<SymbolInfo>> symbols;

    size_t get_max_width() const;

    void print_header(ostream& os, size_t width) const;
    void print_content(ostream&, size_t width) const;
};

#endif
