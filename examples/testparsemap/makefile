all : test

test : test.cpp ../../include/slib/ParseMap.h ../../src/ParseMap.cpp
	g++ -o test -I../../include/ test.cpp ../../src/ParseMap.cpp

clean :
	rm test

