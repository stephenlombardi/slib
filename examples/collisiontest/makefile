all : collisiontest

collisiontest : collisiontest.cpp Brush.h Brush.cpp
	g++ -o collisiontest -I../../include -lGL -lglut collisiontest.cpp Brush.cpp ../../src/ParseMap.cpp ../../src/TGA.cpp

clean :
	rm collisiontest
