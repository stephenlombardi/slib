#include <slib/OpenGLText.h>

namespace slib {

	float * createTextVerts( const std::string & str ) {
		float * verts = new float[ str.size( ) * 4 * 3 ];
		for( size_t i = 0; i < str.size( ); i++ ) {
			Vec< 3 >::Tor( &verts[ i * 4 * 3 + 0 ] ) = Vector3( (float)i, 1.0f, 0.0f );
			Vec< 3 >::Tor( &verts[ i * 4 * 3 + 3 ] ) = Vector3( (float)i, 0.0f, 0.0f );
			Vec< 3 >::Tor( &verts[ i * 4 * 3 + 6 ] ) = Vector3( (float)( i + 1 ), 0.0f, 0.0f );
			Vec< 3 >::Tor( &verts[ i * 4 * 3 + 9 ] ) = Vector3( (float)( i + 1 ), 1.0f, 0.0f );
		}
		return verts;
	}

	float * createTextTexcoords( const std::string & str ) {
		float * texcoords = new float[ str.size( ) * 4 * 2 ];
		for( size_t i = 0; i < str.size( ); i++ ) {
			char c = str[ i ];
			int x = c % 16, y = c / 16;
			Vec< 2 >::Tor( &texcoords[ i * 4 * 2 + 0 ] ) = Vector2( ( x + 0 ) * 16.0f / 256.0f, ( y + 0 ) * 16.0f / 256.0f );
			Vec< 2 >::Tor( &texcoords[ i * 4 * 2 + 2 ] ) = Vector2( ( x + 0 ) * 16.0f / 256.0f, ( y + 1 ) * 16.0f / 256.0f );
			Vec< 2 >::Tor( &texcoords[ i * 4 * 2 + 4 ] ) = Vector2( ( x + 1 ) * 16.0f / 256.0f, ( y + 1 ) * 16.0f / 256.0f );
			Vec< 2 >::Tor( &texcoords[ i * 4 * 2 + 6 ] ) = Vector2( ( x + 1 ) * 16.0f / 256.0f, ( y + 0 ) * 16.0f / 256.0f );
		}
		return texcoords;
	}

	unsigned int * createTextIndices( const std::string & str ) {
		unsigned int * indices = new unsigned int[ str.size( ) * 6 ];
		for( size_t i = 0; i < str.size( ); i++ ) {
			indices[ i * 6 + 0 ] = i * 4 + 0;
			indices[ i * 6 + 1 ] = i * 4 + 1;
			indices[ i * 6 + 2 ] = i * 4 + 2;
			indices[ i * 6 + 3 ] = i * 4 + 2;
			indices[ i * 6 + 4 ] = i * 4 + 3;
			indices[ i * 6 + 5 ] = i * 4 + 0;
		}
		return indices;
	}

}
