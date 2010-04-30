#include "Brush.h"

Brush::Brush( const PlanarVolume< 3, float, Plane< 3, float > > & _brush ) : brush( _brush ) {
}

float Brush::GreatestDistance( const kd::Plane< 3, float > & plane ) const {
	std::list< vec3f > points = GeneratePoints( brush );

	float max = -std::numeric_limits< float >::infinity( );
	for( std::list< vec3f >::iterator iter = points.begin( ); iter != points.end( ); ++iter ) {
		if( plane.Distance( *iter ) > max ) {
			max = plane.Distance( *iter );
		}
	}

	return max;
}

float Brush::LeastDistance( const kd::Plane< 3, float > & plane ) const {
	std::list< vec3f > points = GeneratePoints( brush );

	float min = std::numeric_limits< float >::infinity( );
	for( std::list< vec3f >::iterator iter = points.begin( ); iter != points.end( ); ++iter ) {
		if( plane.Distance( *iter ) < min ) {
			min = plane.Distance( *iter );
		}
	}

	return min;
}
