linter: src/* linter-examples/*
	# Build linter
	clang++ src/*.cpp -std=c++20 -o linter.out

	# Run unit tests
	clang++ tests/LinterTest.cpp src/lexer.cpp src/linter.cpp src/parser.cpp src/exception.cpp src/riscv-dict.cpp -std=c++20 -o linter-tests.out
	./linter-tests.out
