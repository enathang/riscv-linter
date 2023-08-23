#include <stdexcept>

#include <string>
#include <iostream>
#include <cstdlib>
#include "parser.h"
#include "lexer.h"
#include "token.h"

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

        Match(TokenType::Newline);
    }
}

void Parser::Label() {
    li->CheckLabel(token->literal);
    Match(TokenType::Symbol);
    NextToken();
    //Match(TokenType::Colon);
  
    /*while (!CheckPeek(TokenType::Label)) {
        NextToken();
    }

    if (!CheckToken(TokenType::Symbol)) {
        // Throw error
        throw std::invalid_argument( "received negative value" );
    }*/

    NextToken();
}


/*void Parser::Label() {
    Match(TokenType::Symbol);
    NextToken();
}*/

void Parser::Instruction() {
    std::string instruction = token->literal;
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

    li->CheckInstruction(instruction, operands);
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
/*
void Parser::Instruction() {
    NextToken();

    // Is there at least one operand?
    if(!CheckToken(TokenType::Newline) && !CheckToken(TokenType::Comment)) {
        Operand();
        // Zero or more operands.
        while(CheckToken(TokenType::Comma)) {
            NextToken();
            Operand();
        }
    }
}*/

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
