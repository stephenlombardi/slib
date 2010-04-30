#ifndef _OpenGLText_h
#define _OpenGLText_h

#include <string>

#include <gl/freeglut.h>
#include <slib/glext.h>
#include <slib/extensions.h>

#include <slib/Matrix.hpp>

namespace slib {

	float * createTextVerts( const std::string & str );
	float * createTextTexcoords( const std::string & str );
	unsigned int * createTextIndices( const std::string & str );

}

#endif