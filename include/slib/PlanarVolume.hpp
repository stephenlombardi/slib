/*****************************
Author: Steve Lombardi
Description: helper class for
detecting collisions against
convex polyhedra (defined by
a list of planes)
*****************************/

#ifndef SLIB_PLANARVOLUME_HPP
#define SLIB_PLANARVOLUME_HPP

#include <limits>
#include <list>

#include <slib/Matrix.hpp>

namespace slib {

	/*template< class _FieldT, template< int, class > class PlaneT >
	Matrix< 2, 1, _FieldT > IntersectPlanes( const PlaneT< 2, _FieldT > & p0, const PlaneT< 2, _FieldT > & p1 ) {
		_FieldT invdeterminant = MultiplicitiveIdentity< _FieldT >::Identity( ) / ( p0.GetNormal( )[ 0 ] * p1.GetNormal( )[ 1 ] - p0.GetNormal( )[ 1 ] * p1.GetNormal( )[ 0 ] );
		return Vector2( ( p1.GetNormal( )[ 1 ] * p0.GetDistance( ) - p0.GetNormal( )[ 1 ] * p1.GetDistance( ) ) * invdeterminant,
			( -p1.GetNormal( )[ 0 ] * p0.GetDistance( ) + p0.GetNormal( )[ 0 ] * p1.GetDistance( ) ) * invdeterminant );
	}

	template< class _FieldT, template< int, class > class PlaneT >
	Matrix< 3, 1, _FieldT > IntersectPlanes( const PlaneT< 3, _FieldT > & p0, const PlaneT< 3, _FieldT > & p1, const PlaneT< 3, _FieldT > & p2 ) {
		_FieldT invdeterminant = MultiplicitiveIdentity< _FieldT >::Identity( ) / DotProduct( p0.GetNormal( ), CrossProduct( p1.GetNormal( ), p2.GetNormal( ) ) );
		return ( p0.GetDistance( ) * CrossProduct( p1.GetNormal( ), p2.GetNormal( ) ) +
			p1.GetDistance( ) * CrossProduct( p2.GetNormal( ), p0.GetNormal( ) ) +
			p2.GetDistance( ) * CrossProduct( p0.GetNormal( ), p1.GetNormal( ) ) ) * invdeterminant;
	}*/

	template< class _FieldT, class PlaneT >
	Matrix< 2, 1, _FieldT > IntersectPlanes( const PlaneT & p0, const PlaneT & p1 ) {
		_FieldT invdeterminant = MultiplicitiveIdentity< _FieldT >::Identity( ) / ( p0.GetNormal( )[ 0 ] * p1.GetNormal( )[ 1 ] - p0.GetNormal( )[ 1 ] * p1.GetNormal( )[ 0 ] );
		return Vector2( ( p1.GetNormal( )[ 1 ] * p0.GetDistance( ) - p0.GetNormal( )[ 1 ] * p1.GetDistance( ) ) * invdeterminant,
			( -p1.GetNormal( )[ 0 ] * p0.GetDistance( ) + p0.GetNormal( )[ 0 ] * p1.GetDistance( ) ) * invdeterminant );
	}

	template< class _FieldT, class PlaneT >
	Matrix< 3, 1, _FieldT > IntersectPlanes( const PlaneT & p0, const PlaneT & p1, const PlaneT & p2 ) {
		_FieldT invdeterminant = MultiplicitiveIdentity< _FieldT >::Identity( ) / DotProduct( p0.GetNormal( ), CrossProduct( p1.GetNormal( ), p2.GetNormal( ) ) );
		return ( p0.GetDistance( ) * CrossProduct( p1.GetNormal( ), p2.GetNormal( ) ) +
			p1.GetDistance( ) * CrossProduct( p2.GetNormal( ), p0.GetNormal( ) ) +
			p2.GetDistance( ) * CrossProduct( p0.GetNormal( ), p1.GetNormal( ) ) ) * invdeterminant;
	}

	template< int dimension, class FieldT, class PlaneT >
	struct PlanarVolumeIntersection {
		PlanarVolumeIntersection( const FieldT & _enterfraction, const FieldT & _exitfraction, const PlaneT * _enterplane, const PlaneT * _exitplane ) : enterfraction( _enterfraction ), exitfraction( _exitfraction ), enterplane( _enterplane ), exitplane( _exitplane ) { }
		//PlanarVolumeIntersection( ) : t( std::numeric_limits< FieldT >::infinity( ) ), plane( 0 ) { }
		//PlanarVolumeIntersection( const FieldT & _t, const PlaneT< dimension, FieldT > * _plane ) : t( _t ), plane( _plane ) { }
		//bool Intersection( ) const { return -std::numeric_limits< FieldT >::infinity( ) < t && t < std::numeric_limits< FieldT >::infinity( ); }
		//bool Intersection( ) const { return t < std::numeric_limits< FieldT >::infinity( ); }
		//const PlaneT< dimension, FieldT > * plane;
		//FieldT t;
		FieldT enterfraction, exitfraction;
		const PlaneT * enterplane, * exitplane;
	};

	template< int dimension, class FieldT >
	class ZeroOffsetCalc {
	public:
		template< class PlaneT >
		Matrix< dimension, 1, FieldT > operator()( const PlaneT & plane ) const {
			return AdditiveIdentity< Matrix< dimension, 1, FieldT > >::Identity( );
		}
	};

	template< int dimension, class FieldT >
	class SphereOffsetCalc {
	public:
		SphereOffsetCalc( const FieldT & _radius ) : radius( _radius ) { }
		template< class PlaneT >
		Matrix< dimension, 1, FieldT > operator()( const PlaneT & plane ) const {
			return -this->radius * plane.GetNormal( );
		}
	private:
		FieldT radius;
	};

	template< int dimension, class FieldT >
	class BoxOffsetCalc {
	public:
		BoxOffsetCalc( const Matrix< dimension, 1, FieldT > & _box ) : box( _box ) { }
		template< class PlaneT >
		Matrix< dimension, 1, FieldT > operator()( const PlaneT & plane ) const {
			Matrix< dimension, 1, FieldT > offset;
			for( int i = 0; i < dimension; i++ ) {
				offset[ i ] = plane.GetNormal( )[ i ] < AdditiveIdentity< FieldT >::Identity( ) ? box[ i ] : -box[ i ];
			}
			return offset;
		}
	private:
		Matrix< dimension, 1, FieldT > box;
	};

	template< int dimension, class FieldT, class PlaneT >
	class PlanarPolygon {
	public:
		template< class InputIterator >
		PlanarPolygon( const PlaneT & _plane, InputIterator begin, InputIterator end ) : plane( _plane ), points( begin, end ) { }
		PlanarPolygon( const PlaneT & _plane ) : plane( _plane ) { }

		std::list< Matrix< dimension, 1, FieldT > > points;
		const PlaneT & plane;
	private:
	};

	template< int dimension, class FieldT, class PlaneT >
	class PlanarVolume {
	public:
		template< class InputIterator >
		PlanarVolume( InputIterator begin, InputIterator end ) : planes( begin, end ) { }

		bool Within( const Matrix< dimension, 1, FieldT > & point ) const {
			for( typename std::list< PlaneT >::const_iterator iter = this->planes.begin( ); iter != this->planes.end( ); ++iter ) {
				const PlaneT & plane = *iter;
				
				if( plane.Distance( point ) > AdditiveIdentity< FieldT >::Identity( ) ) {
					return false;
				}
			}
			return true;
		}

		template< template< int, class > class OffsetCalcT >
		PlanarVolumeIntersection< dimension, FieldT, PlaneT > IntersectRay( const Matrix< dimension, 1, FieldT > & start, const Matrix< dimension, 1, FieldT > & direction, const OffsetCalcT< dimension, FieldT > & offsetcalc ) const {
			//FieldT startFraction = AdditiveIdentity< FieldT >::Identity( );
			//FieldT startFraction = (FieldT)0;
			//FieldT startFraction = (FieldT)-1;
			FieldT startFraction = -std::numeric_limits< FieldT >::infinity( );
			FieldT endFraction = std::numeric_limits< FieldT >::infinity( );
			const PlaneT * startplane = 0, * endplane = 0;

			for( typename std::list< PlaneT >::const_iterator iter = this->planes.begin( ); iter != this->planes.end( ); ++iter ) {
				const PlaneT & plane = *iter;

				// find intersection point
				Matrix< dimension, 1, FieldT > offset = offsetcalc( plane );
				FieldT startDistance = plane.Distance( start + offset );
				FieldT intersection = plane.IntersectRay( start + offset, direction );
				
				if( intersection == std::numeric_limits< FieldT >::infinity( ) ) {
					if( startDistance > AdditiveIdentity< FieldT >::Identity( ) ) {
					//if( startDistance > (FieldT)0 ) {
						// ray is outside the volume and doesn't intersect any planes
						//return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( );
						//return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( std::numeric_limits< FieldT >::infinity( ), std::numeric_limits< FieldT >::infinity( ), 0, 0 );
						startFraction = -std::numeric_limits< FieldT >::infinity( );
						endFraction = std::numeric_limits< FieldT >::infinity( );
						startplane = 0;
						endplane = 0;
						break;
					} else {
						// ray is inside the volume, but doesn't intersect this plane
						continue;
					}
				} else {
					if( startDistance > AdditiveIdentity< FieldT >::Identity( ) ) {
					//if( startDistance > (FieldT)0 ) {
						// front intersection
						if( intersection > startFraction ) {
							// record furthest intersection
							startFraction = intersection;
							startplane = &plane;
						}
					} else {
						// back intersection
						if( intersection < endFraction ) {
							// record closest intersection
							endFraction = intersection;
							endplane = &plane;
						}
					}
				}
			}

			/*if( !startplane && endplane ) {
				//return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( endFraction, endplane );
				return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( -std::numeric_limits< float >::infinity( ), 0 );
			} else if( startplane ) {
				return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( startFraction, startplane );
			} else {
				return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( );
			}*/

			if( startFraction >= endFraction ) {
				startFraction = -std::numeric_limits< FieldT >::infinity( );
				endFraction = std::numeric_limits< FieldT >::infinity( );
				startplane = 0;
				endplane = 0;
			}

			return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( startFraction, endFraction, startplane, endplane );
			/*if( startFraction < (FieldT)0 ) {
				return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( -std::numeric_limits< float >::infinity( ), 0 );
			} else if( startFraction < endFraction && startFraction > (FieldT)0 ) {
				return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( startFraction, startplane );
			} else {
				return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( );
			}*/


			/*if( !startplane && !endplane ) {
				// no intersection, indicate that line lies inside brush
				return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( -std::numeric_limits< FieldT >::infinity( ), 0 );
			} else if( !startplane && endplane ) {
				// line started inside
				//return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( endFraction, endplane );
				return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( -std::numeric_limits< FieldT >::infinity( ), 0 );
			} else if( startplane && !endplane ) {
				// line started outside
				return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( startFraction, startplane );
			} else if( startplane && endplane ) {
				if( startFraction < endFraction ) {
					return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( endFraction, endplane );
				} else {
					return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( );
				}
			} else {
				return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( );
			}*/
		}

		template< template< int, class > class OffsetCalcT >
		PlanarVolumeIntersection< dimension, FieldT, PlaneT > IntersectLine( const Matrix< dimension, 1, FieldT > & start, const Matrix< dimension, 1, FieldT > & end, const OffsetCalcT< dimension, FieldT > & offsetcalc ) const {
			//FieldT startFraction = AdditiveIdentity< FieldT >::Identity( );
			//FieldT endFraction = MultiplicitiveIdentity< FieldT >::Identity( );
			//FieldT startFraction = (FieldT)-1, endFraction = (FieldT)1;
			FieldT startFraction = -std::numeric_limits< FieldT >::infinity( );
			FieldT endFraction = std::numeric_limits< FieldT >::infinity( );
			const PlaneT * startplane = 0, * endplane = 0;
			//bool startsout = false, endsout = false;

			for( typename std::list< PlaneT >::const_iterator iter = this->planes.begin( ); iter != this->planes.end( ); ++iter ) {
				const PlaneT & plane = *iter;

				// find distances between the endpoints and the plane
				Matrix< dimension, 1, FieldT > offset = offsetcalc( plane );
				FieldT startDistance = plane.Distance( start + offset );
				FieldT endDistance = plane.Distance( end + offset );

				//startsout = startDistance > (FieldT)0 ? true : startsout;
				//endsout = endDistance > (FieldT)0 ? true : endsout;

				if( startDistance > AdditiveIdentity< FieldT >::Identity( ) && endDistance > AdditiveIdentity< FieldT >::Identity( ) ) {
				//if( startDistance > (FieldT)0 && endDistance > (FieldT)0 ) {
					// both endpoints lie outside this plane; it can not intersect the volume
					//return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( );
					startFraction = -std::numeric_limits< FieldT >::infinity( );
					endFraction = std::numeric_limits< FieldT >::infinity( );
					startplane = 0;
					endplane = 0;
					break;
				} else if( startDistance <= AdditiveIdentity< FieldT >::Identity( ) && endDistance <= AdditiveIdentity< FieldT >::Identity( ) ) {
				//} else if( startDistance <= (FieldT)0 && endDistance <= (FieldT)0 ) {
					// both endpoints lie inside the plane; continue looping
					continue;
				} else if( startDistance > endDistance ) {
					// endpoints straddle the plane, this is a front intersection
					// find how far down this line the plane intersects
					FieldT fraction = startDistance / ( startDistance - endDistance );
					if( fraction > startFraction ) {
						// save the furthest intersection
						startFraction = fraction;
						startplane = &plane;
					}
				} else {
					// back intersection
					FieldT fraction = startDistance / ( startDistance - endDistance );
					if( fraction < endFraction ) {
						// save the closest intersection
						endFraction = fraction;
						endplane = &plane;
					}
				}
			}

			if( startFraction >= endFraction ) {
				startFraction = -std::numeric_limits< FieldT >::infinity( );
				endFraction = std::numeric_limits< FieldT >::infinity( );
				startplane = 0;
				endplane = 0;
			}

			return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( startFraction, endFraction, startplane, endplane );
			/*if( !startplane && !endplane ) {
				// no intersection, indicate that line lies inside brush
				return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( -std::numeric_limits< FieldT >::infinity( ), 0 );
			} else if( !startplane && endplane ) {
				// line started inside
				//return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( endFraction, endplane );
				return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( -std::numeric_limits< FieldT >::infinity( ), 0 );
			} else if( startplane && !endplane ) {
				// line started outside
				//std::cout << "startplane && !endplane" << std::endl;
				return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( startFraction, startplane );
			} else if( startplane && endplane ) {
				if( startFraction < endFraction ) {
					//std::cout << "startplane && endplane, startFraction < endFraction" << std::endl;
					return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( startFraction, startplane );
				} else {
					return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( );
				}
			} else {
				return PlanarVolumeIntersection< dimension, FieldT, PlaneT >( );
			}*/
		}

		const typename std::list< PlaneT > & getPlanes( ) const {
			return planes;
		}
	private:
		std::list< PlaneT > planes;
	};

	//template< template< class > class Container >
	//Container< Matrix< dimension, 1, FieldT > > GeneratePoints( ) const {
	template< class FieldT, class PlaneT >
	std::list< Matrix< 3, 1, FieldT > > GeneratePoints( const PlanarVolume< 3, FieldT, PlaneT > & pv ) {
		//Container< Matrix< dimension, 1, FieldT > > points;
		std::list< Matrix< 3, 1, FieldT > > points;

		typename std::list< PlaneT >::const_iterator iter0end = pv.getPlanes( ).end( ); iter0end--; iter0end--;
		for( typename std::list< PlaneT >::const_iterator iter0 = pv.getPlanes( ).begin( ); iter0 != iter0end; iter0++ ) {
			const PlaneT & plane0 = *iter0;

			typename std::list< PlaneT >::const_iterator iter1start = iter0; iter1start++;
			typename std::list< PlaneT >::const_iterator iter1end = pv.getPlanes( ).end( ); iter1end--;
			for( typename std::list< PlaneT >::const_iterator iter1 = iter1start; iter1 != iter1end; iter1++ ) {
				const PlaneT & plane1 = *iter1;

				//if( abs( DotProduct( plane0.GetNormal( ), plane1.GetNormal( ) ) ) < (FieldT)1 ) {
				if( Norm( CrossProduct( plane0.GetNormal( ), plane1.GetNormal( ) ) ) > (FieldT)0 ) {
					typename std::list< PlaneT >::const_iterator iter2start = iter1; iter2start++;
					for( typename std::list< PlaneT >::const_iterator iter2 = iter2start; iter2 != pv.getPlanes( ).end( ); iter2++ ) {
						const PlaneT & plane2 = *iter2;

						//if( abs( DotProduct( plane0.GetNormal( ), plane2.GetNormal( ) ) ) < (FieldT)1 &&
						//	abs( DotProduct( plane1.GetNormal( ), plane2.GetNormal( ) ) ) < (FieldT)1 ) {
						if( Norm( CrossProduct( plane0.GetNormal( ), plane2.GetNormal( ) ) ) > (FieldT)0 &&
							Norm( CrossProduct( plane1.GetNormal( ), plane2.GetNormal( ) ) ) > (FieldT)0 ) {

							points.push_back( IntersectPlanes< FieldT, PlaneT >( plane0, plane1, plane2 ) );
						}
					}
				}
			}
		}
		return points;
	}

	template< class FieldT, class PlaneT >
	std::list< PlanarPolygon< 3, FieldT, PlaneT > > GeneratePolygons( const PlanarVolume< 3, FieldT, PlaneT > & pv ) {
		std::list< PlanarPolygon< 3, FieldT, PlaneT > > polygons;

		for( typename std::list< PlaneT >::const_iterator iter0 = pv.getPlanes( ).begin( ); iter0 != pv.getPlanes( ).end( ); iter0++ ) {
			const PlaneT & plane0 = *iter0;

			polygons.push_back( PlanarPolygon< 3, FieldT, PlaneT >( plane0 ) );

			typename std::list< PlaneT >::const_iterator iter1end = pv.getPlanes( ).end( ); iter1end--;
			for( typename std::list< PlaneT >::const_iterator iter1 = pv.getPlanes( ).begin( ); iter1 != iter1end; iter1++ ) {
				const PlaneT & plane1 = *iter1;

				//if( abs( DotProduct( plane0.GetNormal( ), plane1.GetNormal( ) ) ) < 1 ) {
				if( Norm( CrossProduct( plane0.GetNormal( ), plane1.GetNormal( ) ) ) > (FieldT)0.01f ) {
					typename std::list< PlaneT >::const_iterator iter2start = iter1; iter2start++;
					for( typename std::list< PlaneT >::const_iterator iter2 = iter2start; iter2 != pv.getPlanes( ).end( ); iter2++ ) {
						const PlaneT & plane2 = *iter2;
						//if( abs( DotProduct( plane0.GetNormal( ), plane2.GetNormal( ) ) ) < 1 &&
						//	abs( DotProduct( plane1.GetNormal( ), plane2.GetNormal( ) ) ) < 1 ) {
						if( Norm( CrossProduct( plane0.GetNormal( ), plane2.GetNormal( ) ) ) > (FieldT)0.01f &&
							Norm( CrossProduct( plane1.GetNormal( ), plane2.GetNormal( ) ) ) > (FieldT)0.01f ) {

							polygons.back( ).points.push_back( IntersectPlanes< FieldT, PlaneT >( plane0, plane1, plane2 ) );
						}
					}
				}
			}
		}

		// order points counter clockwise
		for( typename std::list< PlanarPolygon< 3, FieldT, PlaneT > >::iterator iter0 = polygons.begin( ); iter0 != polygons.end( ); iter0++ ) {
			PlanarPolygon< 3, FieldT, PlaneT > & polygon = *iter0;

			Matrix< 3, 1, FieldT > & point0 = polygon.points.front( );

			typename std::list< Matrix< 3, 1, FieldT > >::iterator iter1start = polygon.points.begin( ); iter1start++;
			typename std::list< Matrix< 3, 1, FieldT > >::iterator iter1end = polygon.points.end( ); iter1end--;
			for( typename std::list< Matrix< 3, 1, FieldT > >::iterator iter1 = iter1start; iter1 != iter1end; iter1++ ) {
				Matrix< 3, 1, FieldT > & point1 = *iter1;

				typename std::list< Matrix< 3, 1, FieldT > >::iterator iter2start = iter1; iter2start++;
				for( typename std::list< Matrix< 3, 1, FieldT > >::iterator iter2 = iter2start; iter2 != polygon.points.end( ); iter2++ ) {
					Matrix< 3, 1, FieldT > & point2 = *iter2;

					if( DotProduct( CrossProduct( point2 - point0, point1 - point0 ), polygon.plane.GetNormal( ) ) < (FieldT)0 ) {
						Matrix< 3, 1, FieldT > temp = point1;
						point1 = Matrix< 3, 1, FieldT >( point2 );
						point2 = temp;
					}
				}
			}
		}

		return polygons;
	}

}

#endif
