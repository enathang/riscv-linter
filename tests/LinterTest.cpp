#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include "../src/lexer.h"
#include "../src/token.h"
#include "../src/parser.h"

void printVec(std::vector<std::string> vec) {
    std::cout << "{\n";
    for (std::string s : vec) {
	std::cout << "\t" << s << ", \n";
    }
    std::cout << "\n}";
}

void runLinterTest(std::string fileName, std::vector<std::string> expectedWarnings) {
    std::ifstream inFile(fileName);
    std::string source((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    source += '\n';
    
    std::ifstream inDictionaryFile("src/linter/opcodes");
    std::string dictionarySource((std::istreambuf_iterator<char>(inDictionaryFile)), std::istreambuf_iterator<char>());
    dictionarySource += '\n';

    RiscvDict *d = new RiscvDict(dictionarySource);
    Linter *li = new Linter(d);
    Lexer *l = new Lexer(source, fileName);
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
	expectedWarnings = { 
	    "Missing control flow jump before label _random_label (linter-examples/control-flow.S:7)",
	    "Missing control flow jump before label EOF (linter-examples/control-flow.S:18)"
	};
	std::cout << "Running test linter-examples/control-flow.S\n";
	runLinterTest("linter-examples/control-flow.S", expectedWarnings);	

	// Test register safety
	expectedWarnings = {
	    "Trying to read from an unsafe register t0 in operation mv (linter-examples/register-safety.S:19)",
	    "Non-volatile register s0 is not restored before end of code block (linter-examples/register-safety.S:42)"
	};
	std::cout << "Running test linter-examples/register-safety.S\n";
	runLinterTest("linter-examples/register-safety.S", expectedWarnings);	

	// Test stack safety
	expectedWarnings = {"Trying to read from an unsafe register t0 in operation mv (linter-examples/stack-safety.S:30)"};
	std::cout << "Running test linter-examples/stack-safety.S\n";
	runLinterTest("linter-examples/stack-safety.S", expectedWarnings);
	
	// Test macros
	expectedWarnings = {
	    "Trying to read from an unsafe register t0 in operation mv (linter-examples/macros.S:2)",
	    "Could not find instruction signature for macro_name (it might be a macro definition that hasn't been expanded) (linter-examples/macros.S:10)"
	};
	std::cout << "Running test linter-examples/macros.S\n";
	runLinterTest("linter-examples/macros.S", expectedWarnings);

	return 0;
}

