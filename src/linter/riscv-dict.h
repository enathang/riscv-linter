#import <string>
#import <vector>
#import <map>

enum InstructionType { REGULAR, CALL, JUMP, MOVE };
enum OperandRole { READ, WRITE, CONSTANT, UNKNOWN };

struct InstructionSignature {
    std::string name;
    std::vector<OperandRole> operandRoles;
};

struct Instruction {
    InstructionSignature sig;
    InstructionType type;
    std::vector<std::string> operands;
};


class RiscvDict {
private:
    std::map<std::string, InstructionSignature>* dict;
    OperandRole DetermineOperandRole(std::string);
    void Dump();

public:
    inline static const std::vector<std::string> volatileRegisters = { "t0","t1","t2","t3","t4","t5","t6","a2","a3","a4","a5","a6","a7" };
    // Note: Probably need to add a mapping of ABI name to register name (s0 -> x8) because both are usable, as well as multiple
    // ABI names can map to the same register name (s0 and fp both point to x8)
    inline static const std::vector<std::string> nonVolatileRegisters = { "s0","s1","s2","s3","s4","s5","s6","s7","s8","s9","s10","s11" };
    inline static const std::vector<std::string> undefinedRegisters = { "a0","a1","zero","gp","tp" }; 

    RiscvDict(std::string);
    void ConstructFromFile(std::string);
    InstructionSignature GetInstructionSignature(std::string);
};

