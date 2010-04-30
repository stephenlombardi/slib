#include "Brush.h"

/*BrushIntersection::BrushIntersection( ) : brush( 0 ), intersection( PlanarVolumeIntersection< 3, float, Plane< 3, float > >( -std::numeric_limits< float >::infinity( ), std::numeric_limits< float >::infinity( ), 0, 0 ) ) {
}

BrushIntersection::BrushIntersection( const PlanarVolume< 3, float, Plane< 3, float > > * _brush, const PlanarVolumeIntersection< 3, float, Plane< 3, float > > & _intersection ) : brush( _brush ), intersection( _intersection ) {
}

bool BrushIntersection::Intersection( ) const {
	//return intersection.Intersection( );
	return intersection.enterplane;
}

float BrushIntersection::RayDistance( ) const {
	//return intersection.t;
	return intersection.enterplane ? intersection.enterfraction : std::numeric_limits< float >::infinity( );
}*/


Brush::Brush( const PlanarVolume< 3, float, Plane< 3, float > > & _brush ) : brush( _brush ) {
}

/*template< class OffsetCalcT >
BrushIntersection Brush::IntersectLine( const vec3f & pointa, const vec3f & pointb, const OffsetCalcT & offsetcalc ) const {
	return BrushIntersection( &brush, brush.IntersectLine( pointa, pointb, offsetcalc ) );
}*/

float Brush::GreatestDistance( const kd::Plane< 3, float > & plane ) const {
	//std::list< vec3f > points = brush.GeneratePoints( );
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
	//std::list< vec3f > points = brush.GeneratePoints( );
	std::list< vec3f > points = GeneratePoints( brush );

	float min = std::numeric_limits< float >::infinity( );
	for( std::list< vec3f >::iterator iter = points.begin( ); iter != points.end( ); ++iter ) {
		if( plane.Distance( *iter ) < min ) {
			min = plane.Distance( *iter );
		}
	}

	return min;
}
