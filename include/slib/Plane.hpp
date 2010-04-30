#ifndef SLIB_PLANE_HPP
#define SLIB_PLANE_HPP

#include <limits>

#include <slib/Matrix.hpp>

namespace slib {

	template< int dimension, class FieldT >
	class Plane {
	public:
		Plane( const Matrix< dimension, 1, FieldT > & _normal, FieldT _distance ) : normal( _normal ), distance( _distance ) { }
		inline FieldT Distance( const Matrix< dimension, 1, FieldT > & point ) const {
			return DotProduct( this->normal, point ) - this->distance;
		}
		FieldT IntersectLine( const Matrix< dimension, 1, FieldT > & start, const Matrix< dimension, 1, FieldT > & end ) const {
			FieldT startdist = this->Distance( start ), enddist = this->Distance( end );
			if( startdist == enddist ) { // parallel to plane
				return std::numeric_limits< FieldT >::infinity( );
			}
			return startdist / ( startdist - enddist );
		}
		FieldT IntersectRay( const Matrix< dimension, 1, FieldT > & start, const Matrix< dimension, 1, FieldT > & direction ) const {
			FieldT denom = DotProduct( this->normal, direction );
			if( denom == 0 ) { // parallel to plane
				return std::numeric_limits< FieldT >::infinity( );
			}
			float t = -this->Distance( start ) / denom;
			if( t < (FieldT)0 ) {
				return std::numeric_limits< float >::infinity( );
			}
			return t;
		}
		const Matrix< dimension, 1, FieldT > & GetNormal( ) const {
			return this->normal;
		}
		const FieldT & GetDistance( ) const {
			return this->distance;
		}
	private:
		Matrix< dimension, 1, FieldT > normal;
		FieldT distance;
	};

}

#endif
