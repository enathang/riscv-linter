#include <vector>
#include <string>
#include <map>
#include "riscv-dict.h"
#include "../token.h"

class CodeBlock;

class Linter {
  private:
    RiscvDict* dict;
    CodeBlock* codeBlock;

   public:
    std::vector<std::string> linterWarnings;

    Linter(RiscvDict*);
    InstructionType LookupInstructionType(std::string);
    Instruction ParseInstruction(std::string, std::vector<std::string>);
    void CheckInstruction(std::string, std::vector<std::string>, TokenMetadata);
    void CheckLabel(std::string, TokenMetadata);
    void PushValueOntoStack(Token);
    void PopValueOffStack(Token);
    void LogWarning(std::string, TokenMetadata);
    void Finish(Token);
};

