#include "lexer.h"
#include "linter.h"

class Parser {
private:
    Lexer* l;
    Linter* li;
    Token* token;
    Token* peek;

public:
    // Helper functions.
    Parser(Lexer* l, Linter* li);
    bool CheckToken(TokenType type);
    bool CheckPeek(TokenType type);
    void Match(TokenType type);
    void NextToken();

    // Grammar functions.
    void Parse();
    void Label();
    void Directive();
    void Instruction();
    void Operand();
};
