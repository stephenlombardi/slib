#ifndef SLIB_OPENGLTEXT_H
#define SLIB_OPENGLTEXT_H

#include <string>

#include <GL/freeglut.h>
#include <GL/glext.h>

#include <slib/extensions.h>

#include <slib/Matrix.hpp>

namespace slib {

	float * createTextVerts( const std::string & str );
	float * createTextTexcoords( const std::string & str );
	unsigned int * createTextIndices( const std::string & str );

}

#endif
