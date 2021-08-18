#include "section.hpp"

ostream& operator<<(ostream& os, const Section& s) {
    os << s.section_name;
    return os;
}
