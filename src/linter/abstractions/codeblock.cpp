#include <iostream>
#import "codeblock.h"

// class Linter;
#import "../linter.h"

CodeBlock::CodeBlock(Linter* li) {
    this->li = li;
    Reset();
    hasSeenLabel = false;
}

void CodeBlock::Reset() {
    hasSeenLabel = true;
    isInCodeBlock = false;
    hasSeenJump = false;
    stackSavedValues = {};

    // Because we consider our linter to be out of any previous codeblock, we reset all volatile registers to unsafe
    for (std::string reg : RiscvDict::volatileRegisters) {
        this->regs[reg] = UNSAFE;
    }
    // Reset all saved registers to themselves
    for (std::string reg : RiscvDict::nonVolatileRegisters) {
        setRegisterValue(reg, reg);
    } 
    for (std::string reg : RiscvDict::volatileRegisters) {
        setRegisterValue(reg, reg);
    }
}

bool CodeBlock::isRegisterSafe(std::string reg) {
    return this->regs[reg] == SAFE;
}

void CodeBlock::setRegisterStatus(std::string reg, RegisterState status) {
    this->regs[reg] = status;
}

void CodeBlock::CheckEndOfCodeBlock(std::string label, TokenMetadata metadata) {
    if (hasSeenLabel && isInCodeBlock && !hasSeenJump) {
        std::string msg = "Missing control flow jump before label " + label;
        li->LogWarning(msg, metadata);
    }

    for (std::string reg : RiscvDict::nonVolatileRegisters) {
        if (savedRegisterValues[reg] != reg) {
            std::string msg = "Non-volatile register " + reg + " is not restored before end of code block";
            li->LogWarning(msg, metadata);
        }
    }
    
}

void CodeBlock::setIsInCodeBlock() {
    this->isInCodeBlock = true;
}

void CodeBlock::setHasSeenJump() {
    this->hasSeenJump = true;
}

void CodeBlock::setRegisterValue(std::string reg, std::string value){
    this->savedRegisterValues[reg] = value;
}

void CodeBlock::moveRegisterValue(std::string source, std::string dest) {
    this->savedRegisterValues[source] = this->savedRegisterValues[dest];
}

void CodeBlock::pushToStack(Token reg){
    this->stackSavedValues.push_back(reg.literal);
}

void CodeBlock::popFromStack(Token reg) {
    std::string topOfStack = this->stackSavedValues.back();
    this->stackSavedValues.pop_back();
    if (reg.literal != topOfStack) {
        std::string msg = "Trying to pop register " + reg.literal + " off stack but top of stack actually contains " + topOfStack;
        li->LogWarning(msg, reg.metadata);
    }
}
