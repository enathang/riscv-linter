#include <vector>
#include <string>
#include <map>
#include "riscv-dict.h"


class Linter {
  private:
    RiscvDict* dict;
    std::map<std::string, RegisterState> regs;
    bool isInCodeBlock;
    bool hasSeenLabel;
    bool hasSeenJump;

   public:
    std::vector<std::string> volatileRegisters = { "t0","t1","t2","t3","t4","t5","t6" };
    std::vector<std::string> nonVolatileRegisters = { "s0","s1","s2","s3","s4" };
    std::vector<std::string> undefinedRegisters = { "a0", "a1", "a2" };
    Linter(RiscvDict*);
    InstructionType LookupInstructionType(std::string);
    Instruction ParseInstruction(std::string, std::vector<std::string>);
    void CheckInstruction(std::string, std::vector<std::string>);
    void CheckLabel(std::string);
};

