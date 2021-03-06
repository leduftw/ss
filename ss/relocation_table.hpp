#ifndef RELOCATION_TABLE
#define RELOCATION_TABLE

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <memory>
#include <unordered_map>

#include "section.hpp"

using namespace std;

class RelocationTable {
public:

    enum class RelocationType {
        UNDEFINED,

        R_HYPO_16,
        R_HYPO_PC16,
        R_HYPO_PC16_ABS,  // for %symbol in jump instructions if the symbol is defined with equ directive
    };

    struct RelocationRecord {
        // What?
        string symbol_name = "";
        size_t entry = 0;

        // Where?
        shared_ptr<Section> section;
        int offset = -1;  // position inside a section

        // How?
        RelocationType relocation_type = RelocationType::UNDEFINED;
    };

    friend ostream& operator<<(ostream& os, const RelocationTable& rt);

    void insert(string section_name, shared_ptr<RelocationRecord> record) {
        records[section_name].push_back(record);
        size++;
    }

    size_t get_size() const {
        return size;
    }

    bool empty() const {
        return get_size() == 0;
    }

private:

    size_t size = 0;

    /*
        For each section name, we have vector of relocation records.
        E.g. for .text section we have .rel.text, for .data .rel.data etc.
    */
    unordered_map<string, vector<shared_ptr<RelocationRecord>>> records;

    void print_header(ostream& os, string section_name) const;
    void print_content(ostream& os, const vector<shared_ptr<RelocationRecord>>& records_for_section) const;

};

#endif
