#ifndef SECTION_H
#define SECTION_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "instruction.hpp"

using namespace std;

class Section {

    size_t location_counter = 0;

    string section_name;

    vector<shared_ptr<Instruction>> instructions;
    vector<char> machine_code;

public:

    friend ostream& operator<<(ostream& os, const Section& s);

    Section(string name) {
        section_name = name;
    }

    size_t get_location_counter() const {
        return location_counter;
    }

    void set_location_counter(size_t lc) {
        location_counter = lc;
    }

    void reset_location_counter() {
        location_counter = 0;
    }

    void increment_location_counter(size_t inc) {
        location_counter += inc;
    }

    string get_section_name() const {
        return section_name;
    }

    vector<shared_ptr<Instruction>>& get_instructions() {
        return instructions;
    }

    vector<char>& get_machine_code() {
        return machine_code;
    }
};

#endif
