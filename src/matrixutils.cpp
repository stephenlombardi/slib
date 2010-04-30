#include <slib/matrixutils.h>

namespace slib {

	void normalizeVec3( float * vec ) {
		float sqrtnorm = sqrt( vec[ 0 ] * vec[ 0 ] + vec[ 1 ] * vec[ 1 ] + vec[ 2 ] * vec[ 2 ] );
		vec[ 0 ] /= sqrtnorm;
		vec[ 1 ] /= sqrtnorm;
		vec[ 2 ] /= sqrtnorm;
	}

	void crossProductVec3( float * vec1, float * vec2, float * out ) {
		out[ 0 ] = vec1[ 1 ] * vec2[ 2 ] - vec1[ 2 ] * vec2[ 1 ];
		out[ 1 ] = vec1[ 2 ] * vec2[ 0 ] - vec1[ 0 ] * vec2[ 2 ];
		out[ 2 ] = vec1[ 0 ] * vec2[ 1 ] - vec1[ 1 ] * vec2[ 0 ];
	}

	void lookat( float * matrix, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ ) {
		float f[] = { centerX - eyeX, centerY - eyeY, centerZ - eyeZ };
		normalizeVec3( f );
		float up[] = { upX, upY, upZ };
		normalizeVec3( up );
		float s[ 3 ];
		crossProductVec3( f, up, s );
		normalizeVec3( s );
		float u[ 3 ];
		crossProductVec3( s, f, u );
		normalizeVec3( u );
		matrix[ 0 ] = s[ 0 ]; matrix[ 1 ] = s[ 1 ]; matrix[ 2 ] = s[ 2 ]; matrix[ 3 ] = -s[ 0 ] * eyeX - s[ 1 ] * eyeY - s[ 2 ] * eyeZ;
		matrix[ 4 ] = u[ 0 ]; matrix[ 5 ] = u[ 1 ]; matrix[ 6 ] = u[ 2 ]; matrix[ 7 ] = -u[ 0 ] * eyeX - u[ 1 ] * eyeY - u[ 2 ] * eyeZ;
		matrix[ 8 ] = -f[ 0 ]; matrix[ 9 ] = -f[ 1 ]; matrix[ 10 ] = -f[ 2 ]; matrix[ 11 ] = f[ 0 ] * eyeX + f[ 1 ] * eyeY + f[ 2 ] * eyeZ;
		matrix[ 12 ] = 0.0f; matrix[ 13 ] = 0.0f; matrix[ 14 ] = 0.0f; matrix[ 15 ] = 1.0f;
	}

	void perspective( float * matrix, float fovy, float aspect, float nearp, float farp ) {
		float f = 1.0f / tan( 0.5f * fovy );
		matrix[ 0 ] = f / aspect; matrix[ 1 ] = 0.0f; matrix[ 2 ] = 0.0f; matrix[ 3 ] = 0.0f;
		matrix[ 4 ] = 0.0f; matrix[ 5 ] = f; matrix[ 6 ] = 0.0f, matrix[ 7 ] = 0.0f;
		matrix[ 8 ] = 0.0f; matrix[ 9 ] = 0.0f; matrix[ 10 ] = ( farp + nearp ) / ( nearp - farp ); matrix[ 11 ] = 2.0f * farp * nearp / ( nearp - farp );
		matrix[ 12 ] = 0.0f; matrix[ 13 ] = 0.0f; matrix[ 14 ] = -1.0f; matrix[ 15 ] = 0.0f;
	}

	void orthographic( float * matrix, float left, float right, float bottom, float top, float nearval, float farval ) {
		matrix[ 0 ] = 2.0f / ( right - left ); matrix[ 1 ] = 0.0f; matrix[ 2 ] = 0.0f; matrix[ 3 ] = -( right + left ) / ( right - left );
		matrix[ 4 ] = 0.0f; matrix[ 5 ] = 2.0f / ( top - bottom ); matrix[ 6 ] = 0.0f; matrix[ 7 ] = -( top + bottom ) / ( top - bottom );
		matrix[ 8 ] = 0.0f; matrix[ 9 ] = 0.0f; matrix[ 10 ] = -2.0f / ( farval - nearval ); matrix[ 11 ] = -( farval + nearval ) / ( farval - nearval );
		matrix[ 12 ] = 0.0f; matrix[ 13 ] = 0.0f; matrix[ 14 ] = 0.0f; matrix[ 15 ] = 1.0f;
	}

}
