#include "linter.h"
#include "token.h"
#include "exception.h"
#include <vector>
#include <string>
#include <iostream>

Linter::Linter(RiscvDict* dict) {
    this->dict = dict;
    isInCodeBlock = false;
    hasSeenLabel = false;
    hasSeenJump = false;
};

InstructionType Linter::LookupInstructionType(std::string operation) {
    if (operation == "call") {
        return CALL;
    } else if (operation == "j" || operation == "ret") {
        return JUMP;
    } else {
        return REGULAR;
    }
}

Instruction Linter::ParseInstruction(std::string operation, std::vector<std::string> operands) {
    InstructionType type = Linter::LookupInstructionType(operation);
    InstructionSignature instrSignature = dict->GetInstructionSignature(operation);

    return Instruction { instrSignature, type, operands } ;
}

void Linter::CheckInstruction(std::string operation, std::vector<std::string> operands, TokenMetadata metadata) {
    isInCodeBlock = true;
    Instruction instruction = Linter::ParseInstruction(operation, operands);
    
    if (instruction.type == REGULAR) {
        for (int i=0; i<instruction.operands.size(); i++) {
            if (instruction.sig.operandRoles[i] == READ && this->regs[operands[i]] == UNSAFE) {
                LogWarning("Trying to read from an unsafe register " + operands[i] + " in operation " + operation, metadata);
            }
        }
 
        for (int i=0; i<instruction.operands.size(); i++) {
            if (instruction.sig.operandRoles[i] == WRITE) {
                this->regs[operands[i]] = SAFE;
            }
        }

    } else if (instruction.type == CALL) {
        for (std::string reg : volatileRegisters) {
            this->regs[reg] = UNSAFE;
        }
    } else if (instruction.type == JUMP) {
        this->hasSeenJump = true;
    } else {
        throw LinterException("Unknown instruction type");
    };
};

// Note: probably want to convert from tokenmetadata to a linter warning metadata
void Linter::LogWarning(std::string msg, TokenMetadata metadata) {
    // Can also throw LinterException(msg.c_str()) if you want to fail on the first error message
    std::string msg_with_metadata = msg + " (line number " + std::to_string(metadata.lineNumber) + ")";

    linterWarnings.push_back(msg_with_metadata);
    std::cout << "Linter warning: " << msg_with_metadata << "\n";
}

void Linter::CheckLabel(std::string label, TokenMetadata metadata) {
    if (isInCodeBlock && !hasSeenJump) {
        std::string msg = "Missing control flow jump before label " + label;
        LogWarning(msg, metadata);
    }

    hasSeenLabel = true;
    isInCodeBlock = false;
    hasSeenJump = false;

    for (std::string reg : volatileRegisters) {
        this->regs[reg] = UNSAFE;
    }
    
};

