#import <string>
#import <map>
#import <vector>
#import "../../token.h"

enum RegisterState { SAFE, UNSAFE };

// We do some forward defintions to prevent cicular dependency
class Linter;

class CodeBlock {
private:
    Linter* li;
    bool isInCodeBlock;
    bool hasSeenLabel;
    bool hasSeenJump;
    std::map<std::string, RegisterState> regs;
    std::map<std::string, std::string> savedRegisterValues;
    std::vector<std::string> stackSavedValues;
    

public:
    CodeBlock(Linter*);
    void Reset();
    bool isRegisterSafe(std::string);
    void setRegisterStatus(std::string, RegisterState);
    void setIsInCodeBlock();
    void setHasSeenJump();
    void setRegisterValue(std::string, std::string);
    void moveRegisterValue(std::string, std::string);
    void pushToStack(Token);
    void popFromStack(Token);
    void CheckEndOfCodeBlock(std::string, TokenMetadata);
};

