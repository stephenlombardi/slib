#include <slib/drawableobject.h>

namespace slib {

	void DrawableObject::initVAO( ) {
		glGenVertexArrays( 1, &vao );
	}

	void DrawableObject::destroyVAO( ) {
		for( int i = 0; i < vbos.size( ); i++ ) {
			glDeleteBuffers( 1, &vbos[ i ] );
		}
		glDeleteVertexArrays( 1, &vao );
	}

	void DrawableObject::addBuffers( int count ) {
		glBindVertexArray( vao );
		for( int i = 0; i < count; i++ ) {
			vbos.push_back( 0 );
			glGenBuffers( 1, &vbos.back( ) );
		}
	}

	void DrawableObject::setBuffer( int index, const float * buffer, int _nvertices, int componentspervertex, int usage ) {
		glBindVertexArray( vao );
		glBindBuffer( GL_ARRAY_BUFFER, vbos[ index ] );
		glBufferData( GL_ARRAY_BUFFER, componentspervertex * _nvertices * sizeof(float), buffer, usage );
		glVertexAttribPointer( index, componentspervertex, GL_FLOAT, GL_FALSE, 0, 0 );
		glEnableVertexAttribArray( index );
		nvertices = _nvertices;
	}

	void DrawableObject::setBuffer( int index, const unsigned int * buffer, int _nindices, int usage ) {
		glBindVertexArray( vao );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbos[ index ] );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, _nindices * sizeof(GLuint), buffer, usage );
		nindices = _nindices;
	}

	void DrawableObject::bindBuffers( ) const {
		glBindVertexArray( vao );
	}

	void DrawableObject::draw( int primitive ) const {
		glDrawArrays( primitive, 0, nvertices );
	}

	void DrawableObject::drawIndexed( int primitive ) const {
		glDrawElements( primitive, nindices, GL_UNSIGNED_INT, 0 );
	}

}
