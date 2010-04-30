all: testimage

testimage: main.cpp ../../include/slib/Image.hpp ../../include/slib/TGA.h ../../src/TGA.cpp
	g++ -o test -I../../include main.cpp ../../src/TGA.cpp

clean:
	rm test
	
