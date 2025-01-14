#include <stdexcept>

#include <string>
#include <iostream>
#include <cstdlib>
#include "parser.h"
#include "lexer.h"
#include "token.h"
#include "linter/exception.h"

Parser::Parser(Lexer* l, Linter* li) {
    this->l = l;
    this->li = li;
    token = NULL;
    peek = NULL;
    NextToken(); // Call twice to initialize token and peek.
    NextToken();
}

bool Parser::CheckToken(TokenType type) {
    return token->type == type;
}

bool Parser::CheckPeek(TokenType type) {
    return peek->type == type;
}

void Parser::Match(TokenType type) {
    if(!CheckToken(type)) {
        std::cout << "Expected " << TokenTypeStrings[type] << " but found " << token->literal << ".\n";
        abort();
    }
    NextToken();
} 

void Parser::NextToken() {
    token = peek;
    peek = l->NextToken();
}

void Parser::Parse() {
    while(!CheckToken(TokenType::EndOfFile)) {
        // Is it a label?
        if(CheckPeek(TokenType::Colon)) {
            Label();
        }
        
        // Is it a directive or instruction?
        if(CheckToken(TokenType::Dot)) {
            Directive();
        }
        else if(CheckToken(TokenType::Symbol)) {
            Instruction();
        }

        // Is it a comment?
        if(CheckToken(TokenType::Comment)) {
            NextToken();
        }
        else if(CheckToken(TokenType::Annotation)) {
            Annotation();
        }

        Match(TokenType::Newline);
    }

    li->Finish(*token);
}

void Parser::Annotation() {
    // Consume '#'
    NextToken();
    // Since it's an annotation, we should already know the next token is a colon, but verify anyway
    CheckToken(TokenType::Colon);
    NextToken();
    if (CheckToken(TokenType::Plus)) {
        NextToken();
        li->PushValueOntoStack(*token);
        NextToken();
    } else if (CheckToken(TokenType::Minus)) {
        NextToken();
        li->PopValueOffStack(*token);
        NextToken();
    } else {
        throw LinterException("The notation '#:' is reserved for the linter and expects '+' or '-' after it");
    }
}

void Parser::Label() {
    li->CheckLabel(token->literal, token->metadata);
    Match(TokenType::Symbol);
    NextToken();
    
    // TODO: Why?
    NextToken();
}

void Parser::Instruction() {
    std::string instruction = token->literal;
    TokenMetadata metadata = token->metadata;
    std::vector<std::string> operands;
    
    NextToken();

    if(!CheckToken(TokenType::Newline) && !CheckToken(TokenType::Comment)) {
        operands.push_back(token->literal);
        Operand();
        // Zero or more operands.
        while(CheckToken(TokenType::Comma)) {
            NextToken();
            operands.push_back(token->literal);
            Operand();
        }

    }
    
    li->CheckInstruction(instruction, operands, metadata);
}

void Parser::Directive() {
    NextToken(); // Consume the leading dot.
    Match(TokenType::Symbol);

    // Is there at least one operand?
    if(!CheckToken(TokenType::Newline) && !CheckToken(TokenType::Comment)) {
        // Does the first operand have a dot? Plus or minus?
        if(CheckToken(TokenType::Dot) || CheckToken(TokenType::Plus) || CheckToken(TokenType::Minus)) {
            NextToken();
        }
        Match(TokenType::Symbol);
        // Zero or more operands.
        while(CheckToken(TokenType::Comma)) {
            NextToken();
            // Is there a plus or minus?
            if(CheckToken(TokenType::Plus) || CheckToken(TokenType::Minus)) {
                NextToken();
            }
            Match(TokenType::Symbol);
        }
    }
}

void Parser::Operand() {
    // Optional sign.
    if(CheckToken(TokenType::Plus) || CheckToken(TokenType::Minus)) {
        NextToken();
    }

    // Only a symbol.
    if(CheckToken(TokenType::Symbol) && !CheckPeek(TokenType::Lparen)) {
        NextToken();
    }
    // symbol '(' symbol ')'
    else {
        Match(TokenType::Symbol); 
        Match(TokenType::Lparen);
        Match(TokenType::Symbol);
        Match(TokenType::Rparen);
    }
}
