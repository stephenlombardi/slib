/****************************************************
* Author: Steve Lombardi
* Date: January, 2010
* Description: Wrapper class for constructing
*    vertex array object and vertex buffer objects
*
* How to use:
*    After an OpenGL context has been created, the
*    object must be initialized by calling 
*
*       DrawableObject::initVAO( )
*
*    The arrays can be freed at the end of execution
*    with 
*
*       DrawableObject::destroyVAO( )
*
*    After initializing the vertex array object,
*
*       DrawableObject::addBuffers( int )
*
*    should be called with the number of vertex
*    buffer objects you intend to use. After adding
*    the buffers, you can fill them by calling one
*    of the
*
*       DrawableObject::setBuffer( ... )
*
*    methods. The method that takes five parameters
*    is for vertex attributes (vertex positions,
*    colors, normals, etc). The method that takes
*    four parameters is for index buffers. Finally,
*    to draw the buffers, use
*
*       DrawableObject::draw( ... )
*
*    for objects without index buffers and use
*
*       DrawableObject::drawIndexed( ... )
*
*    for objects with index buffers
*
* Example:
*
*    // global scope
*    DrawableObject object;
*    ...
*
*    // in initialization method
*    object.initVAO( );
*    object.addBuffers( 2 );
*
*    float vertices [] = { 0.5f, -0.5f, 0.0f,
*                          0.0f, 0.5f, 0.0f,
*                         -0.5f, -0.5f, 0.0f };
*    int nvertices = 3;
*    // GL_STATIC_DRAW indicates that this buffer will
*    // not be updated during execution.
*    // componentspervertex indicates how many values
*    // are associated with each vertex. In this case, there
*    // is an x, y, and z value for each vertex, coming to
*    // three components per vertex.
*    object.setBuffer( 0, vertices, nvertices, 3, GL_STATIC_DRAW );
*    float colors [] = { 1.0f, 0.0f, 0.0f,
*                        0.0f, 1.0f, 0.0f,
*                        0.0f, 0.0f, 1.0f };
*    object.setBuffer( 0, colors, nvertices, 3, GL_STATIC_DRAW );
*    ...
*
*    // in drawing method
*    object.draw( GL_TRIANGLES );
*    ...
*
*    // in uninitialization method
*    object.destroyVAO( );
****************************************************/

#ifndef SLIB_DRAWABLEOBJECT_H
#define SLIB_DRAWABLEOBJECT_H

#include <vector>

#include <gl/freeglut.h>
#include <slib/glext.h>
#include <slib/extensions.h>

namespace slib {

	class DrawableObject {
	public:
		void initVAO( );
		void destroyVAO( );
		void addBuffers( int count );
		void setBuffer( int index, const float * buffer, int _nvertices, int componentspervertex, int usage );
		void setBuffer( int index, const unsigned int * buffer, int _nindices, int usage );
		void bindBuffers( ) const;
		void draw( int primitive ) const;
		void drawIndexed( int primitive ) const;
	private:
		unsigned int vao;
		std::vector< unsigned int > vbos;
		int nvertices, nindices;
	};

}

#endif