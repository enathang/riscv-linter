#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "riscv-dict.h"
#include "exception.h"

RiscvDict::RiscvDict(std::string fileContents) {
    this->dict = new std::map<std::string, InstructionSignature>();
    ConstructFromFile(fileContents);

    // Note: Hack-ily adding pseudo ops because they're not contained in the opcodes definition file. In the future, should parse them from another file
    // Also defining macro names for now because I can't figure out how to expand them in assembly yet :/ will need to fix cause that's important
    (*dict)["csrr"] = InstructionSignature {
        "csrr",
        { WRITE, READ }
    };
    
    (*dict)["bnez"] = InstructionSignature {
        "bnez",
        { READ, CONSTANT }
    };
    
    (*dict)["j"] = InstructionSignature {
        "j",
        { CONSTANT }
    };

    (*dict)["la"] = InstructionSignature {
        "la",
        { WRITE, CONSTANT }
    };
    
    (*dict)["li"] = InstructionSignature {
        "li",
        { WRITE, CONSTANT }
    };
    
    (*dict)["csrw"] = InstructionSignature {
        "csrw",
        { WRITE, READ }
    };
    (*dict)["print"] = InstructionSignature {
        "print",
        { CONSTANT }
    };
    (*dict)["push"] = InstructionSignature {
        "push",
        { CONSTANT }
    };
    
    (*dict)["pop"] = InstructionSignature {
        "pop",
        { CONSTANT }
    };
    (*dict)["call"] = InstructionSignature {
        "call",
        { CONSTANT }
    };
    (*dict)["ret"] = InstructionSignature {
        "ret",
        { CONSTANT }
    };
    (*dict)["bgt"] = InstructionSignature {
        "bgt",
        { READ, READ, CONSTANT }
    };
    (*dict)["not"] = InstructionSignature {
        "not",
        { WRITE, READ }
    };
    (*dict)["and"] = InstructionSignature {
        "and",
        { WRITE, READ, READ }
    };
    (*dict)["mv"] = InstructionSignature {
        "mv",
        { WRITE, READ }
    };
    (*dict)["add"] = InstructionSignature {
        "add",
        { WRITE, READ, READ }
    };

    // Dump();
}

void RiscvDict::Dump() {
    static const char * EnumStrings[] = { "READ", "WRITE", "CONSTANT", "UNKNOWN" };
    
    for(const auto& elem : *this->dict) {
        std::cout << "Key: " << elem.first << ", Signature {" << elem.second.name;
        for (const auto& role : elem.second.operandRoles) {
            std::cout << ", " << EnumStrings[role];
        }
        std::cout << "} " << "\n";
    }
}

OperandRole RiscvDict::DetermineOperandRole(std::string operand) {
    std::vector<std::string> writeRegisters = { "rd" };
    std::vector<std::string> readRegisters = { "rs1", "rs2" };
    std::vector<std::string> constantOperands = { 
        "imm20", "oimm20", "jimm20", "imm12", "oimm12", "sbimm12", "simm12", "shamt5", "shamt6", "shamt7" 
    };

    if (std::find(writeRegisters.begin(), writeRegisters.end(), operand) != writeRegisters.end()) {
        return WRITE;
    } else if (std::find(readRegisters.begin(), readRegisters.end(), operand) != readRegisters.end()) {
        return READ;
    } else if (std::find(constantOperands.begin(), constantOperands.end(), operand) != constantOperands.end()) {
        return CONSTANT;
    }

    return UNKNOWN;

}

void RiscvDict::ConstructFromFile(std::string fileContents) {
    std::istringstream iss(fileContents);
    std::string line;

    while (std::getline(iss, line)) {
        if (line[0] == '#') continue;

        std::istringstream lineIss(line);
        std::vector<OperandRole> roles;
        std::string operation;
        std::string operand;

        lineIss >> operation;

        // Here we assume that an instruction will have no more than 5 operands, and if we encounter an unknown operand we assume that 
        // we're encountering some text after the definition and consider the instruction signature completely read
        for (int i=0; i<5 && lineIss >> operand; ++i) {
            OperandRole role = DetermineOperandRole(operand);
            if (role == UNKNOWN) break;
            roles.push_back(role);
        }

        InstructionSignature operationSig = {
            operation,
            roles
        };

        (*dict)[operation] = operationSig;
    }
}

InstructionSignature RiscvDict::GetInstructionSignature(std::string name) {
    // Check key exists in map, otherwise directly accessing it will segfault
    if ((*dict).find(name) == (*dict).end()) {
        std::string msg = "Could not find instruction signature for " + name;
        throw LinterException(msg.c_str());
    }

    return (*dict)[name];

}
