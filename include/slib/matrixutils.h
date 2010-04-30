/****************************************************
* Author: Steve Lombardi
* Date: January, 2010
* Description: Utilities for creating view and
*    projection matrices
****************************************************/

#ifndef SLIB_MATRIXUTILS_H
#define SLIB_MATRIXUTILS_H

#include <cmath>

namespace slib {

	void normalizeVec3( float * vec );
	void crossProductVec3( float * vec1, float * vec2, float * out );
	void lookat( float * matrix, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ );
	void perspective( float * matrix, float fovy, float aspect, float nearp, float farp );
	void orthographic( float * matrix, float left, float right, float bottom, float top, float nearval, float farval );

}

#endif