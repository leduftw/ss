#include "symbol_table.hpp"
#include "utils.hpp"

ostream& operator<<(ostream& os, const SymbolTable& symbol_table) {
    // Backup
    ios init(nullptr);
    init.copyfmt(os);

    size_t width = symbol_table.get_max_width();
    symbol_table.print_header(os, width);
    symbol_table.print_content(os, width);

    // Restore
    os.copyfmt(init);

    return os;
}

size_t SymbolTable::get_max_width() const {
    size_t mx = 10;
    for (auto& p : symbols) {
        if (p.first.length() > mx) {
            mx = p.first.length();
        }

        if (p.second->section) {
            if (p.second->section->get_section_name().length() > mx) {
                mx = p.second->section->get_section_name().length();
            }
        }
    }

    return mx + 1;
}

void SymbolTable::print_header(ostream& os, size_t width) const {
    os << left << setw(width) << setfill(' ') << "Name";
    os << left << setw(width) << setfill(' ') << "Value";
    os << left << setw(width) << setfill(' ') << "Section";
    os << left << setw(width) << setfill(' ') << "Global";
    os << left << setw(width) << setfill(' ') << "Entry";
    os << "\n";
}

void SymbolTable::print_content(ostream& os, size_t width) const {
    for (auto& p : symbols) {
        os << left << setw(width) << p.first;

        stringstream str_stream;
        str_stream << "0x" << hex << p.second->value;
        string val(str_stream.str());
        os << left << setw(width) << val;

        os << left << setw(width) << (p.second->section ? p.second->section->get_section_name() : "ABS");
        os << left << setw(width) << p.second->is_global;
        os << left << setw(width) << p.second->entry_number;
        os << "\n";
    }
}

pair<string, shared_ptr<SymbolTable::SymbolInfo>> SymbolTable::get_symbol_by_entry_number(size_t entry_no) const {
    for (auto& p : symbols) {
        if (p.second->entry_number == entry_no) {
            return p;
        }
    }

    return { "", nullptr };
}
