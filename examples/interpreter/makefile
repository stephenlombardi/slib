all : interpreter

interpreter : main.cpp
	g++ -o interpreter -I../../include main.cpp ../../src/ScriptAST.cpp ../../src/ScriptError.cpp ../../src/ScriptFunctions.cpp ../../src/ScriptParse.cpp ../../src/Tokenizer.cpp

clean :
	rm interpreter
