#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include "../src/lexer.h"
#include "../src/token.h"
#include "../src/parser.h"

void printVec(std::vector<std::string> vec) {
    std::cout << "{";
    for (std::string s : vec) {
	std::cout << s << ", ";
    }
    std::cout << "}";
}

void runLinterTest(std::string fileName, std::vector<std::string> expectedWarnings) {
    std::ifstream inFile(fileName);
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

    if (li->linterWarnings != expectedWarnings) {
	std::cout << "LinterWarnings: ";
	printVec(li->linterWarnings);
	std::cout << ", ExpectedWarnings: ";
	printVec(expectedWarnings);
	std::cout << "\n";

	assert(false);
    }

    delete d;
    delete li;
    delete l;
    delete p;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> expectedWarnings;
	// Test control flow
	expectedWarnings = { "Missing control flow jump before label _random_label (line number 7)" };
	runLinterTest("linter-examples/control-flow.S", expectedWarnings);	

	// Test register safety
	expectedWarnings = { "Trying to read from an unsafe register t0 in operation mv (line number 19)" };
	runLinterTest("linter-examples/register-safety.S", expectedWarnings);	

	return 0;
}

