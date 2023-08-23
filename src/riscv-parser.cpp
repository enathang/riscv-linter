// Example parser for RISC-V assembly.
// Austin Henley
// 10/5/2020

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include "lexer.h"
#include "token.h"
#include "parser.h"

int main(int argc, char* argv[]) {
    if(argc != 2) { 
        std::cout << "Needs one file path." << std::endl;
        return -1;
    }

    std::ifstream inFile(argv[1]);
    std::string source((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    source += '\n';
    
    std::ifstream inDictionaryFile("src/opcodes");
    std::string dictionarySource((std::istreambuf_iterator<char>(inDictionaryFile)), std::istreambuf_iterator<char>());
    dictionarySource += '\n';

    RiscvDict *d = new RiscvDict(dictionarySource);
    Linter *li = new Linter(d);
    Lexer *l = new Lexer(source);
    Parser *p = new Parser(l, li);
    p->Parse();
    std::cout << "Parsed successfully (" << l->GetLineNumber() << " lines)" << std::endl;

    delete d;
    delete li;
    delete l;
    delete p;
    return 0;
}
