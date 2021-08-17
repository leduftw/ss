#include "instruction.hpp"

unordered_map<string, Opcode> Instruction::opcode({
    { "halt", Opcode::HALT },
    { "int", Opcode::INT },
    { "iret", Opcode::IRET },
    { "call", Opcode::CALL },
    { "ret", Opcode::RET },
    { "jmp", Opcode::JMP },
    { "jeq", Opcode::JEQ },
    { "jne", Opcode::JNE },
    { "jgt", Opcode::JGT },
    { "push", Opcode::PUSH },
    { "pop", Opcode::POP },
    { "xchg", Opcode::XCHG },
    { "add", Opcode::ADD },
    { "sub", Opcode::SUB },
    { "mul", Opcode::MUL },
    { "div", Opcode::DIV },
    { "cmp", Opcode::CMP },
    { "not", Opcode::NOT },
    { "and", Opcode::AND },
    { "or", Opcode::OR },
    { "xor", Opcode::XOR },
    { "test", Opcode::TEST },
    { "shl", Opcode::SHL },
    { "shr", Opcode::SHR },
    { "ldr", Opcode::LDR },
    { "str", Opcode::STR },
    });

Opcode Instruction::get_instruction_opcode(string name) {
    if (opcode.find(name) != opcode.end()) {
        return opcode[name];
    }

    return Opcode::ERROR;
}

Instruction::InstructionMetadata Instruction::get_instruction_metadata(Opcode opcode) {
    switch (opcode) {
        case Opcode::HALT:
            return InstructionMetadata(Opcode::HALT);

        case Opcode::INT:
            return InstructionMetadata(Opcode::INT);

        case Opcode::IRET:
            return InstructionMetadata(Opcode::IRET);

        case Opcode::CALL:
            return InstructionMetadata(Opcode::CALL);

        case Opcode::RET:
            return InstructionMetadata(Opcode::RET);

        case Opcode::JMP:
            return InstructionMetadata(Opcode::JMP);

        case Opcode::JEQ:
            return InstructionMetadata(Opcode::JEQ);

        case Opcode::JNE:
            return InstructionMetadata(Opcode::JNE);

        case Opcode::JGT:
            return InstructionMetadata(Opcode::JGT);

        case Opcode::PUSH:
            return InstructionMetadata(Opcode::PUSH);

        case Opcode::POP:
            return InstructionMetadata(Opcode::POP);

        case Opcode::XCHG:
            return InstructionMetadata(Opcode::XCHG);

        case Opcode::ADD:
            return InstructionMetadata(Opcode::ADD);

        case Opcode::SUB:
            return InstructionMetadata(Opcode::SUB);

        case Opcode::MUL:
            return InstructionMetadata(Opcode::MUL);

        case Opcode::DIV:
            return InstructionMetadata(Opcode::DIV);

        case Opcode::CMP:
            return InstructionMetadata(Opcode::CMP);

        case Opcode::NOT:
            return InstructionMetadata(Opcode::NOT);

        case Opcode::AND:
            return InstructionMetadata(Opcode::AND);

        case Opcode::OR:
            return InstructionMetadata(Opcode::OR);

        case Opcode::XOR:
            return InstructionMetadata(Opcode::XOR);

        case Opcode::TEST:
            return InstructionMetadata(Opcode::TEST);

        case Opcode::SHL:
            return InstructionMetadata(Opcode::SHL);

        case Opcode::SHR:
            return InstructionMetadata(Opcode::SHR);

        case Opcode::LDR:
            return InstructionMetadata(Opcode::LDR);

        case Opcode::STR:
            return InstructionMetadata(Opcode::STR);

        case Opcode::ERROR:
            break;

        default:
            break;
    }

    return InstructionMetadata();
}

size_t Instruction::size() const {
    return 0;
}
