#ifndef SLIB_BRUSH_H
#define SLIB_BRUSH_H

#include <slib/kdTree.hpp>
#include <slib/Matrix.hpp>
#include <slib/PlanarVolume.hpp>
#include <slib/Plane.hpp>

using namespace slib;

struct BrushIntersection {
	BrushIntersection( ) : brush( 0 ), intersection( PlanarVolumeIntersection< 3, float, Plane< 3, float > >( -std::numeric_limits< float >::infinity( ), std::numeric_limits< float >::infinity( ), 0, 0 ) ) {
	}

	BrushIntersection( const PlanarVolume< 3, float, Plane< 3, float > > * _brush, const PlanarVolumeIntersection< 3, float, Plane< 3, float > > & _intersection ) : brush( _brush ), intersection( _intersection ) {
	}

	bool Intersection( ) const {
		return intersection.enterplane;
	}

	float RayDistance( ) const {
		return intersection.enterplane ? intersection.enterfraction : std::numeric_limits< float >::infinity( );
	}

	const PlanarVolume< 3, float, Plane< 3, float > > * brush;
	PlanarVolumeIntersection< 3, float, Plane< 3, float > > intersection;
};

class Brush {
public:
	Brush( const PlanarVolume< 3, float, Plane< 3, float > > & _brush );

	template< class OffsetCalcT >
	BrushIntersection IntersectLine( const vec3f & pointa, const vec3f & pointb, const OffsetCalcT & offsetcalc ) const {
		return BrushIntersection( &brush, brush.IntersectLine( pointa, pointb, offsetcalc ) );
	}
	float GreatestDistance( const kd::Plane< 3, float > & plane ) const;
	float LeastDistance( const kd::Plane< 3, float > & plane ) const;
private:
	PlanarVolume< 3, float, Plane< 3, float > > brush;
};

#endif
