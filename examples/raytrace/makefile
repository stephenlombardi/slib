all : raytrace

raytrace : raytrace.cpp Brush.cpp ../../include/slib/kdTree.hpp ../../include/slib/Matrix.hpp ../../include/slib/ParseMap.h ../../src/ParseMap.cpp ../../include/slib/PlanarVolume.hpp ../../include/slib/TGA.h ../../src/TGA.cpp
	g++ -O2 -o raytrace -I../../include raytrace.cpp Brush.cpp ../../src/ParseMap.cpp ../../src/TGA.cpp

clean :
	rm raytrace

