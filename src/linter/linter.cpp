#include "linter.h"
#include "exception.h"
#include <vector>
#include <string>
#include <iostream>
#include "abstractions/codeblock.h"

Linter::Linter(RiscvDict* dict) {
    this->dict = dict;
    this->codeBlock = new CodeBlock(this); 
};

// Note: probably would be useful to convert from tokenmetadata to a linter warning metadata
void Linter::LogWarning(std::string msg, TokenMetadata metadata) {
    std::string msg_with_metadata = msg + " (" + metadata.fileName + ":" + std::to_string(metadata.lineNumber) + ")";
    linterWarnings.push_back(msg_with_metadata);
}

InstructionType Linter::LookupInstructionType(std::string operation) {
    if (operation == "call") {
        return CALL;
    } else if (operation == "j" || operation == "ret") {
        return JUMP;
    } else if (operation == "mv") {
        return MOVE;
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
    codeBlock->setIsInCodeBlock();
    try {
        Instruction instruction = Linter::ParseInstruction(operation, operands);

        // Check the correct number of operands are present
        if (operands.size() != instruction.sig.operandRoles.size()) {
            std::string msg = "Instruction "+operation+" on line "+std::to_string(metadata.lineNumber)+" has "+std::to_string(operands.size())+ 
                " operands but was expecting "+std::to_string(instruction.sig.operandRoles.size()); 
            throw LinterException(msg.c_str());
        }
       
        switch(instruction.type) {
            case REGULAR:
                // See if any registers we read from are considered unsafe
                for (int i=0; i<instruction.operands.size(); i++) {
                    if (instruction.sig.operandRoles[i] == READ && !this->codeBlock->isRegisterSafe(operands[i])) {
                        LogWarning("Trying to read from an unsafe register " + operands[i] + " in operation " + operation, metadata);
                    }
                }
         
                // Since all registers and constants we read from are now assumed to be safe, we can declare all written registers are also safe
                for (int i=0; i<instruction.operands.size(); i++) {
                    if (instruction.sig.operandRoles[i] == WRITE) {
                        this->codeBlock->setRegisterStatus(operands[i], SAFE);
                    }
                }

                break;
            
            case MOVE:
                if (!this->codeBlock->isRegisterSafe(operands[1])) {
                        LogWarning("Trying to read from an unsafe register " + operands[1] + " in operation " + operation, metadata);
                }
                this->codeBlock->setRegisterStatus(operands[0], SAFE); 
                this->codeBlock->moveRegisterValue(operands[0], operands[1]);
                break;

            case CALL:
                // Since we just called another function, we can no longer guarantee the volatile registers are safe
                for (std::string reg : RiscvDict::volatileRegisters) {
                    this->codeBlock->setRegisterStatus(reg, UNSAFE);
                }
                break;

            case JUMP:
                this->codeBlock->setHasSeenJump();
                break;
        }
    } catch (LinterException e) {
        LogWarning(e.what(), metadata);
    }
};

void Linter::CheckLabel(std::string label, TokenMetadata metadata) {
    this->codeBlock->CheckEndOfCodeBlock(label, metadata);
    this->codeBlock->Reset();
};

void Linter::PushValueOntoStack(Token reg) {
    this->codeBlock->pushToStack(reg);
}

void Linter::PopValueOffStack(Token reg) {
    this->codeBlock->popFromStack(reg);
}

void Linter::Finish(Token eofToken) {
    this->codeBlock->CheckEndOfCodeBlock("EOF", eofToken.metadata);
}
