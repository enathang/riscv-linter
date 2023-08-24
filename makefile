linter: src/* linter-examples/*
	# Build linter
	clang++ src/*.cpp src/linter/*.cpp src/linter/abstractions/*.cpp -std=c++20 -o linter.out

	# Build unit tests
	clang++ tests/LinterTest.cpp src/lexer.cpp src/linter/linter.cpp src/parser.cpp src/linter/exception.cpp \
		src/linter/riscv-dict.cpp src/linter/abstractions/codeblock.cpp -std=c++20 -o linter-tests.out
	
	# Run unit tests
	./linter-tests.out
