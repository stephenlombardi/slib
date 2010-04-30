#include <cassert>

#include <slib/Matrix.hpp>

using namespace slib;

void test0( ) {
	float d0[] = { 1.0f, 2.0f, 3.0f, 4.0f };
	Matrix< 2, 2, float > m0( &d0[ 0 ], &d0[ 4 ] );

	assert( m0( 0, 0 ) == 1.0f && m0( 0, 1 ) == 2.0f && m0( 1, 0 ) == 3.0f && m0( 1, 1 ) == 4.0f );

	float d1[] = { 2.0f, -1.0f };
	Matrix< 2, 1, float > m1( &d1[ 0 ], &d1[ 2 ] );

	assert( m1( 0, 0 ) == 2.0f && m1( 1, 0 ) == -1.0f );
	assert( m1[ 0 ] == 2.0f && m1[ 1 ] == -1.0f );

	Matrix< 2, 1, float > m2 = m0 * m1;

	assert( m2( 0, 0 ) == 0.0f && m2( 1, 0 ) == 2.0f );
	assert( m2[ 0 ] == 0.0f && m2[ 1 ] == 2.0f );
}

void test1( ) {
	mat4f m0 = MultiplicitiveIdentity< mat4f >::Identity( );

	assert( m0( 0, 0 ) == 1.0f && m0( 0, 1 ) == 0.0f && m0( 0, 2 ) == 0.0f && m0( 0, 3 ) == 0.0f &&
		m0( 1, 0 ) == 0.0f && m0( 1, 1 ) == 1.0f && m0( 1, 2 ) == 0.0f && m0( 1, 3 ) == 0.0f &&
		m0( 2, 0 ) == 0.0f && m0( 2, 1 ) == 0.0f && m0( 2, 2 ) == 1.0f && m0( 2, 3 ) == 0.0f &&
		m0( 3, 0 ) == 0.0f && m0( 3, 1 ) == 0.0f && m0( 3, 2 ) == 0.0f && m0( 3, 3 ) == 1.0f );

	Sub< 2, 2 >::Matrix( m0, 0, 2 ) = Sub< 2, 2 >::Matrix( m0 );

	assert( m0( 0, 0 ) == 1.0f && m0( 0, 1 ) == 0.0f && m0( 0, 2 ) == 1.0f && m0( 0, 3 ) == 0.0f &&
		m0( 1, 0 ) == 0.0f && m0( 1, 1 ) == 1.0f && m0( 1, 2 ) == 0.0f && m0( 1, 3 ) == 1.0f &&
		m0( 2, 0 ) == 0.0f && m0( 2, 1 ) == 0.0f && m0( 2, 2 ) == 1.0f && m0( 2, 3 ) == 0.0f &&
		m0( 3, 0 ) == 0.0f && m0( 3, 1 ) == 0.0f && m0( 3, 2 ) == 0.0f && m0( 3, 3 ) == 1.0f );
}

void test2( ) {
	float d0 [] = { 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

	Sub< 3, 3 >::MatrixVal( RowMajor< 4, 4, float >( d0 ) ) = MultiplicitiveIdentity< mat3f >::Identity( );

	assert( d0[ 0 ] == 1.0f && d0[ 1 ] == 0.0f && d0[ 2 ] == 0.0f && d0[ 3 ] == 0.0f &&
		d0[ 4 ] == 0.0f && d0[ 5 ] == 1.0f && d0[ 6 ] == 0.0f && d0[ 7 ] == 0.0f &&
		d0[ 8 ] == 0.0f && d0[ 9 ] == 0.0f && d0[ 10 ] == 1.0f && d0[ 11 ] == 0.0f &&
		d0[ 12 ] == 0.0f && d0[ 13 ] == 0.0f && d0[ 14 ] == 0.0f && d0[ 15 ] == 1.0f );
}

void test3( ) {
	float d0 [] = { 1.0f, 2.0f,
		3.0f, 4.0f };

	RowMajor< 2, 2, float >( d0 ) = Matrix< 2, 2, float >( TransposeVal( RowMajor< 2, 2, float >( d0 ) ) );

	assert( d0[ 0 ] == 1.0f && d0[ 1 ] == 3.0f && 
		d0[ 2 ] == 2.0f && d0[ 3 ] == 4.0f );
}

void test4( ) {
	float d0 [] = { 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f };

	mat3f m0( &d0[ 0 ], &d0[ 9 ] );

	vec3f v0 = m0 * Vector3( 1.0f, 2.0f, 3.0f );

	assert( v0[ 0 ] == 1.0f && v0[ 1 ] == 3.0f && v0[ 2 ] == 2.0f );

	vec3f v1 = Solve( m0, v0 );

	assert( v1[ 0 ] == 1.0f && v1[ 1 ] == 2.0f && v1[ 2 ] == 3.0f );
}

void test5( ) {
	float d0 [] = { 1.0f, 2.0f,
		3.0f, 4.0f };

	mat2f m0( &d0[ 0 ], &d0[ 4 ] );

	float d1 [] = { -3.0f, 1.0f,
		1.0f, -2.0f };

	mat2f m1( &d1[ 0 ], &d1[ 4 ] );

	mat2f m2 = m0 + m1;

	assert( m2( 0, 0 ) == -2.0f && m2( 0, 1 ) == 3.0f &&
		m2( 1, 0 ) == 4.0f && m2( 1, 1 ) == 2.0f );

	mat2f m3 = m0 * m1;

	assert( m3( 0, 0 ) == -1.0f && m3( 0, 1 ) == -3.0f &&
		m3( 1, 0 ) == -5.0f && m3( 1, 1 ) == -5.0f );
}

void test6( ) {
	float d0 [] = { 1.0f, -2.0f, 1.0f };

	assert( NormSquared( Vec< 3 >::Tor( d0 ) ) == 6.0f );

	float d1 [] = { 1.0f, 2.0f, -2.0f };

	assert( Norm( Vec< 3 >::Tor( d1 ) ) == 3.0f );

	vec3f v0 = Vec< 3 >::Tor( d0 ) + Vec< 3 >::Tor( d1 );

	assert( v0[ 0 ] == 2.0f && v0[ 1 ] == 0.0f && v0[ 2 ] == -1.0f );

	vec3f v1 = Vec< 3 >::Tor( d0 ) - Vec< 3 >::Tor( d1 );

	assert( v1[ 0 ] == 0.0f && v1[ 1 ] == -4.0f && v1[ 2 ] == 3.0f );

	assert( DotProduct( v0, v1 ) == -3.0f );

	vec3f v2 = CrossProduct( v0, v1 );

	assert( v2[ 0 ] == -4.0f && v2[ 1 ] == -6.0f && v2[ 2 ] == -8.0f );
}

int main( ) {
	test0( );
	test1( );
	test2( );
	test3( );
	test4( );
	test5( );
	test6( );

	return 0;
}
