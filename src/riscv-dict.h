#import <string>
#import <vector>
#import <map>

enum RegisterState { SAFE, UNSAFE };
enum InstructionType { REGULAR, CALL, JUMP };
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
    RiscvDict(std::string);
    void ConstructFromFile(std::string);
    InstructionSignature GetInstructionSignature(std::string);
};

