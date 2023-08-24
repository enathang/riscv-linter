#pragma once
#include <string>

enum TokenType {
    Symbol = 0,
    Lparen,
    Rparen,
    Plus,
    Minus,
    Dot,
    Colon,
    Comma,
    Newline,
    Comment,
    Annotation,
    EndOfFile
};

struct TokenMetadata {
    std::string fileName;
    int lineNumber;
};

const char* const TokenTypeStrings[] = {"Symbol", "Left-Parenthesis", "Right-Parenthesis", "Plus", "Minus", "Dot", "Colon", "Comma", "Newline", "Comment", "Annotation", "End-Of-File"};

class Token {
public:
    TokenType type;
    std::string literal;
    TokenMetadata metadata;
};
