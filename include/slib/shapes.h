/****************************************************
* Author: Steve Lombardi
* Date: January, 2010
* Description: shape generation functions
****************************************************/

#ifndef SLIB_SHAPES_H
#define SLIB_SHAPES_H

#include <cmath>

namespace slib {

	// input: xradius, yradius, zradius
	// output: vertex array, vertex count
	void genCube( float * & verts, int & nverts, float xradius, float yradius, float zradius );

	// input: bottomradius, topradius, height, slices, stacks
	// output: vertex array, vertex count
	void genCylinder( float * & verts, int & nverts, float bottomradius, float topradius, float height, int slices, int stacks );

	// input: radius, slices, stacks
	// output: vertex array, vertex count
	void genSphere( float * & verts, int & nverts, float radius, int slices, int stacks );

	// input: inner radius, outer radius, slices, rings
	// output: vertex array, vertex count
	void genDisk( float * & verts, int & nverts, float innerradius, float outerradius, int slices, int rings );

}

#endif