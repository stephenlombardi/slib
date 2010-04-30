all : test

test : test.cpp ../../include/slib/TGA.h ../../src/TGA.cpp
	g++ -o test -I../../include/ test.cpp ../../src/TGA.cpp

clean :
	rm test
