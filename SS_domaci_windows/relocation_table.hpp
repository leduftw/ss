#ifndef RELOCATION_TABLE
#define RELOCATION_TABLE

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "section.hpp"

using namespace std;

class RelocationTable {

    enum class RelocationType {
        R_HYPO_PC16,
        R_HYPO_16,
    };

    struct RelocationRecord {
        // What?
        string symbol_name;

        // Where?
        Section* section;
        int offset;  // position inside a section

        // How?
        RelocationType relocation_type;
    };

public:

    friend ostream& operator<<(ostream& os, const RelocationTable& rt);

};

#endif
