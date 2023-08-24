#pragma once
#include <string>
#include "token.h"

class Lexer {
private:
    std::string source;
    std::string fileName;
    char ch; // Current character.
    int pos; // Position of next character to be read.
    int lineNumber; // Line number the lexer is currently on
    void NextChar();
    char PeekChar();
    bool IsPunct();
    bool IsAlpha();
    bool IsNum();
    bool IsNewline();
    bool IsColon();
    bool IsSpace();
    void SkipSpaces();

public:
    Lexer(std::string source, std::string fileName);
    Token* NextToken();
    int GetLineNumber();
};
