#ifndef LINE_PARSER_H_
#define LINE_PARSER_H_

#include <string>
#include <regex>

#include "instruction.hpp"

using namespace std;

class LineParser {
public:
    static Instruction* parse(string line);
};

#endif