#include "relocation_table.hpp"
#include "utils.h"

ostream& operator<<(ostream& os, const RelocationTable& rt) {
    for (auto& p : rt.records) {
        rt.print_header(os, p.first);
        rt.print_content(os, p.second);
    }

    return os;
}

void RelocationTable::print_header(ostream& os, string section_name) const {
    // Backup
    ios init(nullptr);
    init.copyfmt(os);

    // For text we want .rel.text, but for .text we don't want .rel..text, but again .rel.text
    string rel_section_name = ".rel";
    if (section_name[0] == '.') {
        rel_section_name += section_name;
    } else {
        rel_section_name += "." + section_name;
    }

    os << rel_section_name << "\n";
    os << left << setw(12) << setfill(' ') << "Offset";
    os << left << setw(16) << setfill(' ') << "Type";
    os << left << setw(16) << setfill(' ') << "Entry";
    os << "\n";

    // Restore
    os.copyfmt(init);
}

void RelocationTable::print_content(ostream& os, const vector<shared_ptr<RelocationRecord>>& records_for_section) const {
    // Backup
    ios init(nullptr);
    init.copyfmt(os);

    for (auto& r : records_for_section) {
        string off = Utils::int_to_hex(r->offset);
        os << left << setw(12) << hex << off;

        string relocation_type;
        switch (r->relocation_type) {
            case RelocationTable::RelocationType::R_HYPO_16:
                relocation_type = "R_HYPO_16";
                break;

            case RelocationTable::RelocationType::R_HYPO_PC16:
                relocation_type = "R_HYPO_PC16";
                break;

            case RelocationTable::RelocationType::R_HYPO_PC16_ABS:
                relocation_type = "R_HYPO_PC16_ABS";
                break;

            default:
                break;
        }

        os << left << setw(16) << relocation_type;

        os << left << setw(16) << r->entry;
        os << "\n";
    }

    // Restore
    os.copyfmt(init);

    os << "\n";
}
