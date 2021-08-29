#include "relocation_table.hpp"

ostream& operator<<(ostream& os, const RelocationTable& rt) {
    // Backup
    ios init(nullptr);
    init.copyfmt(os);

    for (auto& p : rt.records) {
        rt.print_header(os, p.first);
        rt.print_content(os, p.second);
    }

    // Restore
    os.copyfmt(init);

    return os;
}

void RelocationTable::print_header(ostream& os, string section_name) const {
    // For text we want .rel.text, but for .text we don't want .rel..text, but again .rel.text
    string rel_section_name = ".rel";
    if (section_name[0] == '.') {
        rel_section_name += section_name;
    } else {
        rel_section_name += "." + section_name;
    }

    os << rel_section_name << "\n";
    os << left << setw(10) << setfill(' ') << "Offset";
    os << left << setw(10) << setfill(' ') << "Type";
    os << left << setw(10) << setfill(' ') << "Entry";
    os << "\n";
}

void RelocationTable::print_content(ostream& os, const vector<shared_ptr<RelocationRecord>>& records_for_section) const {
    for (auto& r : records_for_section) {
        os << left << setw(10) << r->offset;

        string relocation_type = r->relocation_type == RelocationType::R_HYPO_PC16 ? "R_HYPO_PC16" : "R_HYPO_16";
        os << left << setw(10) << relocation_type;

        os << left << setw(10) << r->entry;
        os << "\n";
    }
}
