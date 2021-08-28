#include "section.hpp"

ostream& operator<<(ostream& os, const Section& s) {
    os << ".section " << s.section_name << "\n";
    if (!s.is_machine_code_ready()) {
        return os;
    }

    /*
    string b = s.machine_code.size() != 1 ? "bytes" : "byte";
    os << "size: " << s.machine_code.size() << " " << b << "\n";
    */

    size_t address = 0x0;
    for (int i = 0; i < (int) s.machine_code.size(); i++) {
        if (address % 8 == 0) {
            if (address != 0) {
                os << "\n";
            }

            // Backup
            ios init(nullptr);
            init.copyfmt(os);

            os << "0x" << setfill('0') << setw(4) << hex << address << ":";

            // Restore
            os.copyfmt(init);
        }

        // Backup
        ios init(nullptr);
        init.copyfmt(os);

        // Does not work! Tries to print character, but most of them are not printable.
        //os << " " << setfill('0') << setw(2) << hex << s.machine_code[i];

        // This works
        os << hex << " " << ((s.machine_code[i] >> 4) & 0x0F);
        os << hex << (s.machine_code[i] & 0x0F);

        // Restore
        os.copyfmt(init);

        address++;
    }

    return os;
}
