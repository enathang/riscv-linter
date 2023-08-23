#pragma once
#include <string>
#include "token.h"

class Lexer {
private:
    std::string source;
    char ch; // Current character.
    int pos; // Position of next character to be read.
    int lineNumber; // Line number the lexer is currently on
    void NextChar();
    char PeekChar();
    bool IsPunct();
    bool IsAlpha();
    bool IsNum();
    bool IsNewline();
    bool IsSpace();
    void SkipSpaces();

public:
    Lexer(std::string source);
    Token* NextToken();
    int GetLineNumber();
};
