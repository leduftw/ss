#include "LineParser.h"

Instruction* LineParser::parse(string line) {
    smatch sm;
    bool isDirective = false;

    regex operation_regex("\\s*(?:([A-Za-z0-9_\\.][A-Za-z0-9_]*)\\s*:\\s*){0,1}(?:([A-Za-z0-9_]+)(?:\\s+([^,\\s]+)\\s*(?:,\\s*([^,\\s]+)\\s*)*){0,1}){0,1}");
    regex directive_regex("\\s*(?:([A-Za-z0-9_\\.][A-Za-z0-9_]*)\\s*:\\s*){0,1}(?:\\.([A-Za-z0-9_\\.]+)\\s*(?:(?:\\s+([^,\\s]+)\\s*,{0,1})(?:\\s*(.+)\\s*)*)*){0,1}");

    if (!regex_match(line, sm, operation_regex)) {
        if (!regex_match(line, sm, directive_regex)) {
            cerr << "Syntax error\n";
        }

        if (sm[1].length() != 0 || sm[2].length() != 0 || sm[3].length() != 0 || sm[4].length() != 0) isDirective = true;
    }

    if (sm[1].length() == 0 && sm[2].length() == 0 && sm[3].length() == 0 && sm[4].length() == 0) return nullptr;

    Instruction* i = new Instruction();

    return i;
}
