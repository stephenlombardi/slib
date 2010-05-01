#include <cassert>
#include <iostream>
#include <iterator>
#include <limits>

#include <slib/Matrix.hpp>
#include <slib/PlanarVolume.hpp>
#include <slib/Plane.hpp>

namespace slib {
	template< int dim, class FieldT >
	std::ostream & operator<<( std::ostream & out, const Matrix< dim, 1, FieldT > & rhs ) {
		for( int i = 0; i < dim; i++ ) {
			out << ( i == 0 ? "" : ", " ) << rhs[ i ];
		}
		return out;
	}

	template< int dim, class FieldT >
	bool operator==( const Matrix< dim, 1, FieldT > & lhs, const Matrix< dim, 1, FieldT > & rhs ) {
		for( int i = 0; i < dim; i++ ) {
			if( lhs[ i ] != rhs[ i ] ) {
				return false;
			}
		}
		return true;
	}
}

using namespace slib;

template< int dim, class FieldT, class PlaneT >
std::ostream & operator<<( std::ostream & out, const PlanarVolumeIntersection< dim, FieldT, PlaneT > & pvi ) {
	out << pvi.enterfraction << " (" << ( pvi.enterplane ? pvi.enterplane->GetNormal( ) : vec3f( 0.0f ) ) << ") ";
	out << pvi.exitfraction << " (" << ( pvi.exitplane ? pvi.exitplane->GetNormal( ) : vec3f( 0.0f ) ) << ")";
	return out;
}

template< int dim, class FieldT, class PlaneT, template< int, class > class OffsetCalcT >
void printIntersectionLine( const PlanarVolume< dim, FieldT, PlaneT > & pv, const vec3f & start, const vec3f & end, const OffsetCalcT< dim, FieldT > & offsetcalc ) {
	PlanarVolumeIntersection< dim, FieldT, PlaneT > pvi = pv.IntersectLine( start, end, offsetcalc );
	std::cout << start << " to " << end << ( pv.Within( start ) ? ": starts inside " : ": starts outside " ) << pvi << std::endl;
}

template< int dim, class FieldT, class PlaneT, template< int, class > class OffsetCalcT >
void checkIntersectionLine( const PlanarVolume< dim, FieldT, PlaneT > & pv, const vec3f & start, const vec3f & end, const OffsetCalcT< dim, FieldT > & offsetcalc, FieldT startdist, const Matrix< dim, 1, FieldT > & startnormal, FieldT enddist, const Matrix< dim, 1, FieldT > & endnormal ) {
	PlanarVolumeIntersection< dim, FieldT, PlaneT > pvi = pv.IntersectLine( start, end, offsetcalc );
	//std::cout << start << " to " << end << ( pv.Within( start ) ? ": starts inside " : ": starts outside " ) << pvi << std::endl;
	assert( pvi.enterfraction == startdist );
	assert( !pvi.enterplane && !Norm( startnormal ) || pvi.enterplane && pvi.enterplane->GetNormal( ) == startnormal );
	assert( pvi.exitfraction == enddist );
	assert( !pvi.exitplane && !Norm( endnormal ) || pvi.exitplane && pvi.exitplane->GetNormal( ) == endnormal );
}

template< int dim, class FieldT, class PlaneT, template< int, class > class OffsetCalcT >
void printIntersectionRay( const PlanarVolume< dim, FieldT, PlaneT > & pv, const vec3f & start, const vec3f & dir, const OffsetCalcT< dim, FieldT > & offsetcalc ) {
	PlanarVolumeIntersection< dim, FieldT, PlaneT > pvi = pv.IntersectRay( start, dir, offsetcalc );
	std::cout << start << " in direction " << dir << ( pv.Within( start ) ? ": starts inside " : ": starts outside " ) << pvi << std::endl;
}

template< int dim, class FieldT, class PlaneT, template< int, class > class OffsetCalcT >
void checkIntersectionRay( const PlanarVolume< dim, FieldT, PlaneT > & pv, const vec3f & start, const vec3f & dir, const OffsetCalcT< dim, FieldT > & offsetcalc, FieldT startdist, const Matrix< dim, 1, FieldT > & startnormal, FieldT enddist, const Matrix< dim, 1, FieldT > & endnormal ) {
	PlanarVolumeIntersection< dim, FieldT, PlaneT > pvi = pv.IntersectRay( start, dir, offsetcalc );
	//std::cout << start << " in direction " << dir << ( pv.Within( start ) ? ": starts inside " : ": starts outside " ) << pvi << std::endl;
	assert( pvi.enterfraction == startdist );
	assert( !pvi.enterplane && !Norm( startnormal ) || pvi.enterplane && pvi.enterplane->GetNormal( ) == startnormal );
	assert( pvi.exitfraction == enddist );
	assert( !pvi.exitplane && !Norm( endnormal ) || pvi.exitplane && pvi.exitplane->GetNormal( ) == endnormal );
}

void test0( ) {
	// cube of width 2 centered at origin
	Plane< 3, float > p0 [] = { Plane< 3, float >( Vector3( 1.0f, 0.0f, 0.0f ), 1.0f ),
		Plane< 3, float >( Vector3( -1.0f, 0.0f, 0.0f ), 1.0f ),
		Plane< 3, float >( Vector3( 0.0f, 1.0f, 0.0f ), 1.0f ),
		Plane< 3, float >( Vector3( 0.0f, -1.0f, 0.0f ), 1.0f ),
		Plane< 3, float >( Vector3( 0.0f, 0.0f, 1.0f ), 1.0f ),
		Plane< 3, float >( Vector3( 0.0f, 0.0f, -1.0f ), 1.0f ) };
	PlanarVolume< 3, float, Plane< 3, float > > pv0( &p0[ 0 ], &p0[ 6 ] );

	//std::cout << "testing line segment intersections" << std::endl;

	// no intersection (outside)
	// -inf ( 0 0 0 ) inf ( 0 0 0 )
	//printIntersectionLine( pv0, Vector3( -2.0f, 0.0f, 0.0f ), Vector3( -1.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ) );
	checkIntersectionLine( pv0, Vector3( -2.0f, 0.0f, 0.0f ), Vector3( -1.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ),
		//-std::numeric_limits< float >::infinity( ), vec3f( 0.0f ), std::numeric_limits< float >::infinity( ), vec3f( 0.0f ) );
		1.0f, Vector3( -1.0f, 0.0f, 0.0f ), std::numeric_limits< float >::infinity( ), vec3f( 0.0f ) );

	// front intersection
	// 0.5 ( -1 0 0 ) inf ( 0 0 0 )
	//printIntersectionLine( pv0, Vector3( -2.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ) );
	checkIntersectionLine( pv0, Vector3( -2.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ),
		0.5f, Vector3( -1.0f, 0.0f, 0.0f ), std::numeric_limits< float >::infinity( ), vec3f( 0.0f ) );

	// front and back intersection
	// 0.25 ( -1 0 0 ) 0.75 ( 1 0 0 )
	//printIntersectionLine( pv0, Vector3( -2.0f, 0.0f, 0.0f ), Vector3( 2.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ) );
	checkIntersectionLine( pv0, Vector3( -2.0f, 0.0f, 0.0f ), Vector3( 2.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ),
		0.25f, Vector3( -1.0f, 0.0f, 0.0f ), 0.75f, Vector3( 1.0f, 0.0f, 0.0f ) );

	// back intersection
	// -inf ( 0 0 0 ) 0.5 ( 1 0 0 )
	//printIntersectionLine( pv0, Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 2.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ) );
	checkIntersectionLine( pv0, Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 2.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ),
		-std::numeric_limits< float >::infinity( ), vec3f( 0.0f ), 0.5f, Vector3( 1.0f, 0.0f, 0.0f ) );

	// no intersection (inside)
	// -inf ( 0 0 0 ) inf ( 0 0 0 )
	//printIntersectionLine( pv0, Vector3( -0.5f, 0.0f, 0.0f ), Vector3( 0.5f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ) );
	checkIntersectionLine( pv0, Vector3( -0.5f, 0.0f, 0.0f ), Vector3( 0.5f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ),
		-std::numeric_limits< float >::infinity( ), vec3f( 0.0f ), std::numeric_limits< float >::infinity( ), vec3f( 0.0f ) );

	// 
	//printIntersectionLine( pv0, Vector3( -1.5f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.5f ), ZeroOffsetCalc< 3, float >( ) );
	//checkIntersectionLine( pv0, Vector3( -1.5f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -1.5f ), ZeroOffsetCalc< 3, float >( ) );

	// 
	// -inf ( 0 0 0 ) inf ( 0 0 0 )
	//printIntersectionLine( pv0, Vector3( -2.5f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -2.5f ), ZeroOffsetCalc< 3, float >( ) );
	checkIntersectionLine( pv0, Vector3( -2.5f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, -2.5f ), ZeroOffsetCalc< 3, float >( ),
		-std::numeric_limits< float >::infinity( ), vec3f( 0.0f ), std::numeric_limits< float >::infinity( ), vec3f( 0.0f ) );

	// corner case
	//printIntersectionLine( pv0, Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ) );
	//checkIntersectionLine( pv0, Vector3( -1.0f, 0.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ) );

	// corner case
	//printIntersectionLine( pv0, Vector3( -1.0f, 0.0f, 0.0f ), Vector3( -1.0f, 0.5f, 0.0f ), ZeroOffsetCalc< 3, float >( ) );
	//checkIntersectionLine( pv0, Vector3( -1.0f, 0.0f, 0.0f ), Vector3( -1.0f, 0.5f, 0.0f ), ZeroOffsetCalc< 3, float >( ) );

	// corner case
	//printIntersectionLine( pv0, Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ) );
	//checkIntersectionLine( pv0, Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ) );
}

void test1( ) {
	// cube of width 2 centered at origin
	Plane< 3, float > p0 [] = { Plane< 3, float >( Vector3( 1.0f, 0.0f, 0.0f ), 1.0f ),
		Plane< 3, float >( Vector3( -1.0f, 0.0f, 0.0f ), 1.0f ),
		Plane< 3, float >( Vector3( 0.0f, 1.0f, 0.0f ), 1.0f ),
		Plane< 3, float >( Vector3( 0.0f, -1.0f, 0.0f ), 1.0f ),
		Plane< 3, float >( Vector3( 0.0f, 0.0f, 1.0f ), 1.0f ),
		Plane< 3, float >( Vector3( 0.0f, 0.0f, -1.0f ), 1.0f ) };
	PlanarVolume< 3, float, Plane< 3, float > > pv0( &p0[ 0 ], &p0[ 6 ] );

	//std::cout << "testing ray intersections" << std::endl;

	// intersection (starts outside)
	// 1 (-1 0 0) 3 (1 0 0)
	//printIntersectionRay( pv0, Vector3( -2.0f, 0.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ) );
	checkIntersectionRay( pv0, Vector3( -2.0f, 0.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ),
		1.0f, Vector3( -1.0f, 0.0f, 0.0f ), 3.0f, Vector3( 1.0f, 0.0f, 0.0f ) );

	// intersection (starts inside)
	// -inf (0 0 0) 1 (1 0 0)
	//printIntersectionRay( pv0, Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ) );
	checkIntersectionRay( pv0, Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 1.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ),
		-std::numeric_limits< float >::infinity( ), vec3f( 0.0f ), 1.0f, Vector3( 1.0f, 0.0f, 0.0f ) );

	// no intersection
	// -inf (0 0 0) inf (0 0 0)
	//printIntersectionRay( pv0, Vector3( -2.0f, 0.0f, 0.0f ), Vector3( -1.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ) );
	checkIntersectionRay( pv0, Vector3( -2.0f, 0.0f, 0.0f ), Vector3( -1.0f, 0.0f, 0.0f ), ZeroOffsetCalc< 3, float >( ),
		-std::numeric_limits< float >::infinity( ), vec3f( 0.0f ), std::numeric_limits< float >::infinity( ), vec3f( 0.0f ) );

	//
	//printIntersectionRay( pv0, Vector3( -2.5f, 0.0f, 0.0f ), Vector3( 1.0f, 0.0f, -1.0f ), ZeroOffsetCalc< 3, float >( ) );
	//checkIntersectionRay( pv0, Vector3( -2.5f, 0.0f, 0.0f ), Vector3( 1.0f, 0.0f, -1.0f ), ZeroOffsetCalc< 3, float >( ) );

	//
	//printIntersectionRay( pv0, Vector3( -1.5f, 0.0f, 0.0f ), Vector3( 1.0f, 0.0f, -1.0f ), ZeroOffsetCalc< 3, float >( ) );
	//checkIntersectionRay( pv0, Vector3( -1.5f, 0.0f, 0.0f ), Vector3( 1.0f, 0.0f, -1.0f ), ZeroOffsetCalc< 3, float >( ) );
}

void test2( ) {
	// cube of width 2 centered at origin
	Plane< 3, float > p0 [] = { Plane< 3, float >( Vector3( 1.0f, 0.0f, 0.0f ), 1.0f ),
		Plane< 3, float >( Vector3( -1.0f, 0.0f, 0.0f ), 1.0f ),
		Plane< 3, float >( Vector3( 0.0f, 1.0f, 0.0f ), 1.0f ),
		Plane< 3, float >( Vector3( 0.0f, -1.0f, 0.0f ), 1.0f ),
		Plane< 3, float >( Vector3( 0.0f, 0.0f, 1.0f ), 1.0f ),
		Plane< 3, float >( Vector3( 0.0f, 0.0f, -1.0f ), 1.0f ) };
	PlanarVolume< 3, float, Plane< 3, float > > pv0( &p0[ 0 ], &p0[ 6 ] );

	//std::cout << "Generated points" << std::endl;

	std::list< vec3f > points = GeneratePoints( pv0 );

	//std::cout << "(";
	//std::copy( points.begin( ), points.end( ), std::ostream_iterator< vec3f >( std::cout, ") (" ) );
	//std::cout << std::endl;
}

int main( ) {
	test0( );
	test1( );
	test2( );
	
	return 0;
}
