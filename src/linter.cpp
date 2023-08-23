#include "linter.h"
#include "exception.h"
#include <vector>
#include <string>

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

void Linter::CheckInstruction(std::string operation, std::vector<std::string> operands) {
    isInCodeBlock = true;
    Instruction instruction = Linter::ParseInstruction(operation, operands);
    
    if (instruction.type == REGULAR) {
        for (int i=0; i<instruction.operands.size(); i++) {
            if (instruction.sig.operandRoles[i] == READ && this->regs[instruction.sig.name] == UNSAFE) {
                throw LinterException("Trying to read from an unsafe register");
            }
        }
 
        for (int i=0; i<instruction.operands.size(); i++) {
            if (instruction.sig.operandRoles[i] == WRITE) {
                this->regs[instruction.sig.name] = SAFE;
            }
        }

    } else if (instruction.type == CALL) {
        for (std::string reg : volatileRegisters) {
            this->regs[reg] = UNSAFE;
        }
    } else if (instruction.type == JUMP) {
        this->hasSeenJump = true;
    } else {

    };
};

void Linter::CheckLabel(std::string label) {
    // TODO: Add checks to see if code block or other label
    if (isInCodeBlock && !hasSeenJump) {
        std::string msg = "Missing control flow jump before label " + label; 
        throw LinterException(msg.c_str());
    }

    hasSeenLabel = true;
    isInCodeBlock = false;
    hasSeenJump = false;
};

