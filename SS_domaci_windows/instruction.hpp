#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include <string>

using namespace std;

class Instruction {

    string label;

    void copy(const Instruction &other);
    void move(Instruction &other);
    void erase();

public:
    Instruction() { }

    Instruction(const Instruction &other) {
        copy(other);
    }

    Instruction(Instruction &&other) noexcept {
        move(other);
    }

    ~Instruction() {
        erase();
    }

    Instruction &operator=(const Instruction &other) {
        if (this == &other) {
            return *this;
        }

        label = other.label;

        return *this;
    }

    Instruction &operator=(Instruction &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        label = other.label;

        return *this;
    }

    bool has_label() const {
        return label != "";
    }

    string get_label() const {
        return label;
    }

    size_t size() const {
        return 0;
    }

    string get_operation() const {
        return "";
    }
};

#endif
