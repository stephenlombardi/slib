/*****************************
Author: Steve Lombardi
Description: kd tree class
*****************************/

#ifndef SLIB_KDTREE_HPP
#define SLIB_KDTREE_HPP

#include <list>
#include <limits>
#include <memory>

#include <slib/Matrix.hpp>

namespace slib {

	namespace kd {
	
		template< int dimensions, class FieldT >
		class Plane {
		public:
			Plane( ) { }
			Plane( int _axisnum, FieldT _dist ) : axisnum( _axisnum ), dist( _dist ) { }

			inline FieldT Distance( const Matrix< dimensions, 1, FieldT > & point ) const {
				return point[ axisnum ] - dist;
			}
			inline FieldT IntersectLine( const Matrix< dimensions, 1, FieldT > & pointa, const Matrix< dimensions, 1, FieldT > & pointb ) const {
				return ( dist - pointa[ axisnum ] ) / ( pointb[ axisnum ] - pointa[ axisnum ] );
			}
			inline int GetAxis( ) const {
				return this->axisnum;
			}
			inline Matrix< dimensions, 1, FieldT > GetNormal( ) const {
				//Matrix< dimensions, 1, FieldT > result( (FieldT)0 );
				//result[ axisnum ] = (FieldT)1;
				Matrix< dimensions, 1, FieldT> result( AdditiveIdentity< FieldT >::Identity( ) );
				result[ axisnum ] = MultiplicitiveIdentity< FieldT >::Identity( );
				return result;
			}
			inline FieldT GetDistance( ) const {
				return this->dist;
			}
		private:
			int axisnum;
			FieldT dist;
		};
	
		template< int dimensions, class IntersectionT, class PrimitiveT >
		class Tree {
		public:
			Tree( );
			Tree( const Tree & rhs );
			~Tree( );
			Tree& operator=( const Tree & rhs );

			void Add( const PrimitiveT & prim );
			void Build( int maxdepth );

			template< class OffsetCalcT >
			IntersectionT IntersectLine( const Matrix< dimensions, 1, float > & pointa, const Matrix< dimensions, 1, float > & pointb, const OffsetCalcT & offsetcalc ) const {
				return this->root->IntersectLine( pointa, pointb, 0.0f, 1.0f, offsetcalc );
			}
		private:
			class Node {
			public:
				Node( );
				Node( const Matrix< dimensions, 1, float > & max, const Matrix< dimensions, 1, float > & min );
				Node( const Node & rhs );
				~Node( );

				// Tree building functions
				void CalcDimensions( );
				Plane< dimensions, float > OptimalSplittingPlane( ) const;
				float SurfaceAreaHeuristic( const Plane< dimensions, float > & axis ) const;
				void Split( const Plane< dimensions, float > & splitplane );

				// Intersection functions
				template< class OffsetCalcT >
				IntersectionT IntersectLine( const Matrix< dimensions, 1, float > & pointa, const Matrix< dimensions, 1, float > & pointb, float startfraction, float endfraction, const OffsetCalcT & offsetcalc ) const {
					if( front && back ) {
						const Matrix< dimensions, 1, float > direction = pointb - pointa;

						Matrix< dimensions, 1, float > offset = offsetcalc( split );
						/*if( DotProduct( split.GetNormal( ), direction ) > 0.0f ) {
							offset = -offset;
						}
						float offsetdist = fabs( split.Distance( offset ) );*/
						float offsetdist = Norm( offset );

						//float startdist = this->split.Distance( pointa + startfraction * direction + offset );
						//float enddist = this->split.Distance( pointa + endfraction * direction + offset );
						float startdist = this->split.Distance( pointa + startfraction * direction );
						float enddist = this->split.Distance( pointa + endfraction * direction );

						//float adist = this->split.Distance( pointa + offset );
						//float bdist = this->split.Distance( pointb + offset );
						float adist = this->split.Distance( pointa );
						float bdist = this->split.Distance( pointb );
				
						if( startdist >= offsetdist && enddist >= offsetdist ) {
							//std::cout << "front" << std::endl;
							return this->front->IntersectLine( pointa, pointb, startfraction, endfraction, offsetcalc );
						} else if( startdist < -offsetdist && enddist < -offsetdist ) {
							//std::cout << "back" << std::endl;
							return this->back->IntersectLine( pointa, pointb, startfraction, endfraction, offsetcalc );
						} else {
							if( startdist > enddist ) {
								float denom = 1.0f / ( adist - bdist );
								//float midfraction1 = ( adist + 32.03125f ) * denom;
								//float midfraction2 = ( adist - 32.03125f ) * denom;
								//float midfraction1 = adist * denom;
								//float midfraction2 = adist * denom;
								float midfraction1 = ( adist + offsetdist ) * denom;
								float midfraction2 = ( adist - offsetdist ) * denom;

								/*IntersectionT intersection = this->front->IntersectLine( pointa, pointb, startfraction, midfraction1, offsetcalc );
								if( intersection.Intersection( ) ) {
									//std::cout << "crossing (front)-back " << intersection.RayDistance( ) << std::endl;

									IntersectionT intersection2 = this->back->IntersectLine( pointa, pointb, midfraction2, endfraction, offsetcalc );
									std::cout << "front-back " << intersection.RayDistance( ) << " " << intersection2.RayDistance( ) << std::endl;
									std::cout << "pointa: " << pointa[ 0 ] << " " << pointa[ 1 ] << " " << pointa[ 2 ] << std::endl;
									std::cout << "pointb: " << pointb[ 0 ] << " " << pointb[ 1 ] << " " << pointb[ 2 ] << std::endl;
									std::cout << "splitting plane: " << split.GetNormal( )[ 0 ] << " " << split.GetNormal( )[ 1 ] << " " << split.GetNormal( )[ 2 ] << " " << split.GetDistance( ) << std::endl;
									std::cout << "brush" << std::endl;
									for( std::list< slib::Plane< 3, float > >::const_iterator iter = intersection.brush->getPlanes( ).begin( ); iter != intersection.brush->getPlanes( ).end( ); ++iter ) {
										std::cout << iter->GetNormal( )[ 0 ] << " " << iter->GetNormal( )[ 1 ] << " " << iter->GetNormal( )[ 2 ] << " " << iter->GetDistance( ) << std::endl;
									}
									if( intersection2.brush ) {
										std::cout << "brush2" << std::endl;
										for( std::list< slib::Plane< 3, float > >::const_iterator iter = intersection2.brush->getPlanes( ).begin( ); iter != intersection2.brush->getPlanes( ).end( ); ++iter ) {
											std::cout << iter->GetNormal( )[ 0 ] << " " << iter->GetNormal( )[ 1 ] << " " << iter->GetNormal( )[ 2 ] << " " << iter->GetDistance( ) << std::endl;
										}
									}
									return intersection;
								} else {
									IntersectionT intersection2 = this->back->IntersectLine( pointa, pointb, midfraction2, endfraction, offsetcalc );
									if( intersection2.Intersection( ) ) {
										std::cout << "crossing front-(back) " << intersection2.RayDistance( ) << std::endl;
									}
									return intersection2;
								}*/
								if( offsetdist == 0.0f ) {
									IntersectionT intersection1 = this->front->IntersectLine( pointa, pointb, startfraction, midfraction1, offsetcalc );
									if( intersection1.Intersection( ) ) {
										return intersection1;
									} else {
										return this->back->IntersectLine( pointa, pointb, midfraction2, endfraction, offsetcalc );
									}
								} else {
									IntersectionT intersection1 = this->front->IntersectLine( pointa, pointb, startfraction, endfraction, offsetcalc );
									IntersectionT intersection2 = this->back->IntersectLine( pointa, pointb, startfraction, endfraction, offsetcalc );
									if( intersection1.RayDistance( ) < intersection2.RayDistance( ) ) {
										return intersection1;
									} else {
										return intersection2;
									}
								}
							} else {
								float denom = 1.0f / ( adist - bdist );
								//float midfraction1 = ( adist - 32.03125f ) * denom;
								//float midfraction2 = ( adist + 32.03125f ) * denom;
								//float midfraction1 = adist * denom;
								//float midfraction2 = adist * denom;
								float midfraction1 = ( adist - offsetdist ) * denom;
								float midfraction2 = ( adist + offsetdist ) * denom;

								/*IntersectionT intersection = this->back->IntersectLine( pointa, pointb, startfraction, midfraction1, offsetcalc );
								if( intersection.Intersection( ) ) {
									//std::cout << "crossing (back)-front " << intersection.RayDistance( ) << std::endl;
									IntersectionT intersection2 = this->front->IntersectLine( pointa, pointb, midfraction2, endfraction, offsetcalc );
									std::cout << "back-front " << intersection.RayDistance( ) << " " << intersection2.RayDistance( ) << std::endl;
									std::cout << "pointa: " << pointa[ 0 ] << " " << pointa[ 1 ] << " " << pointa[ 2 ] << std::endl;
									std::cout << "pointb: " << pointb[ 0 ] << " " << pointb[ 1 ] << " " << pointb[ 2 ] << std::endl;
									std::cout << "splitting plane: " << split.GetNormal( )[ 0 ] << " " << split.GetNormal( )[ 1 ] << " " << split.GetNormal( )[ 2 ] << " " << split.GetDistance( ) << std::endl;
									std::cout << "brush" << std::endl;
									for( std::list< slib::Plane< 3, float > >::const_iterator iter = intersection.brush->getPlanes( ).begin( ); iter != intersection.brush->getPlanes( ).end( ); ++iter ) {
										std::cout << iter->GetNormal( )[ 0 ] << " " << iter->GetNormal( )[ 1 ] << " " << iter->GetNormal( )[ 2 ] << " " << iter->GetDistance( ) << std::endl;
									}
									if( intersection2.brush ) {
										std::cout << "brush2" << std::endl;
										for( std::list< slib::Plane< 3, float > >::const_iterator iter = intersection2.brush->getPlanes( ).begin( ); iter != intersection2.brush->getPlanes( ).end( ); ++iter ) {
											std::cout << iter->GetNormal( )[ 0 ] << " " << iter->GetNormal( )[ 1 ] << " " << iter->GetNormal( )[ 2 ] << " " << iter->GetDistance( ) << std::endl;
										}
									}
									return intersection;
								} else {
									IntersectionT intersection2 = this->front->IntersectLine( pointa, pointb, midfraction2, endfraction, offsetcalc );
									if( intersection2.Intersection( ) ) {
										std::cout << "crossing back-(front) " << intersection2.RayDistance( ) << std::endl;
									}
									return intersection2;
								}*/
								if( offsetdist == 0.0f ) {
									IntersectionT intersection1 = this->back->IntersectLine( pointa, pointb, startfraction, midfraction1, offsetcalc );
									if( intersection1.Intersection( ) ) {
										return intersection1;
									} else {
										return this->front->IntersectLine( pointa, pointb, midfraction2, endfraction, offsetcalc );
									}
								} else {
									IntersectionT intersection1 = this->front->IntersectLine( pointa, pointb, startfraction, endfraction, offsetcalc );
									IntersectionT intersection2 = this->back->IntersectLine( pointa, pointb, startfraction, endfraction, offsetcalc );
									if( intersection1.RayDistance( ) < intersection2.RayDistance( ) ) {
										return intersection1;
									} else {
										return intersection2;
									}
								}
							}
						}
					} else {
						return this->IntersectPrimitives( pointa, pointb, startfraction, endfraction, offsetcalc );
					}
				}

				template< class OffsetCalcT >
				IntersectionT IntersectPrimitives( const Matrix< dimensions, 1, float > & pointa, const Matrix< dimensions, 1, float > & pointb, float startfraction, float endfraction, const OffsetCalcT & offsetcalc ) const {
					IntersectionT closest;

					for( typename std::list< PrimitiveT >::const_iterator iter = this->primlist.begin( ); iter != this->primlist.end( ); iter++ ) {
						const PrimitiveT & prim = *iter;

						IntersectionT intersection = prim.IntersectLine( pointa, pointb, offsetcalc );
						if( intersection.Intersection( ) && intersection.RayDistance( ) < closest.RayDistance( ) && intersection.RayDistance( ) >= startfraction && intersection.RayDistance( ) <= endfraction ) {
							closest = intersection;
						}
					}

					return closest;
				}

				// Misc
				void Add( const PrimitiveT & prim );
				size_t Primitives( ) const;
				Node * GetFront( );
				Node * GetBack( );
			private:
				//Plane split;
				Plane< dimensions, float > split;
				Matrix< dimensions, 1, float > maxdimensions, mindimensions;
				Node * front, * back;
				std::list< PrimitiveT > primlist;
			};

			void Build( Node * node, int depth );
			Node * root;
		};

	}

	///////////////////// Node /////////////////////

	template< int dimensions, class IntersectionT, class PrimitiveT >
	kd::Tree< dimensions, IntersectionT, PrimitiveT >::Node::Node( ) : front( 0 ), back( 0 ) {
	}

	template< int dimensions, class IntersectionT, class PrimitiveT >
	kd::Tree< dimensions, IntersectionT, PrimitiveT >::Node::Node( const Matrix< dimensions, 1, float > & max, const Matrix< dimensions, 1, float > & min ) : maxdimensions( max ), mindimensions( min ), front( 0 ), back( 0 ) {
	}

	template< int dimensions, class IntersectionT, class PrimitiveT >
	kd::Tree< dimensions, IntersectionT, PrimitiveT >::Node::Node( const typename kd::Tree< dimensions, IntersectionT, PrimitiveT >::Node & rhs ) : split( rhs.split ), maxdimensions( rhs.maxdimensions ), mindimensions( rhs.mindimensions ), front( 0 ), back( 0 ), primlist( rhs.primlist ) {
		if( rhs.front ) {
			front = new Node( *rhs.front );
		}
		if( rhs.back ) {
			back = new Node( *rhs.back );
		}
	}

	template< int dimensions, class IntersectionT, class PrimitiveT >
	kd::Tree< dimensions, IntersectionT, PrimitiveT >::Node::~Node( ) {
		delete this->front;
		delete this->back;
	}

	template< int dimensions, class IntersectionT, class PrimitiveT >
	void kd::Tree< dimensions, IntersectionT, PrimitiveT >::Node::CalcDimensions( ) {
		//Vector max( 0.0f, 0.0f, 0.0f ), min( 0.0f, 0.0f, 0.0f );
		//Matrix< dimensions, 1, float > max( 0.0f ), min( 0.0f );
		Matrix< dimensions, 1, float > max( -std::numeric_limits< float >::infinity( ) ), min( std::numeric_limits< float >::infinity( ) );
		for( typename std::list< PrimitiveT >::const_iterator iter = this->primlist.begin( ); iter != this->primlist.end( ); iter++ ) {
			const PrimitiveT & prim = *iter;
		
			for( int i = 0; i < dimensions; i++ ) {
				Plane< dimensions, float > axis( i, 0.0f );
		
				float least = prim.LeastDistance( axis ), greatest = prim.GreatestDistance( axis );
				if( least < min[ i ] ) { min[ i ] = least; }
				if( greatest > max[ i ] ) { max[ i ] = greatest; }
			}
		}
		this->maxdimensions = max;
		this->mindimensions = min;
	}

	template< int dimensions, class IntersectionT, class PrimitiveT >
	kd::Plane< dimensions, float > kd::Tree< dimensions, IntersectionT, PrimitiveT >::Node::OptimalSplittingPlane( ) const {
		float bestcost = std::numeric_limits< float >::infinity( );
		kd::Plane< dimensions, float > bestplane;
		for( typename std::list< PrimitiveT >::const_iterator iter = this->primlist.begin( ); iter != this->primlist.end( ); iter++ ) {
			const PrimitiveT & prim = *iter;
			for( int i = 0; i < dimensions; i++ ) {
				// find greatest and least points
				kd::Plane< dimensions, float > axis( i, 0.0f );
				float cost;
				float least = prim.LeastDistance( axis ), greatest = prim.GreatestDistance( axis );
				// find costs
				if( ( cost = this->SurfaceAreaHeuristic( Plane< dimensions, float >( i, least ) ) ) < bestcost ) {
					bestcost = cost;
					bestplane = kd::Plane< dimensions, float >( i, least );
				}
				if( ( cost = this->SurfaceAreaHeuristic( Plane< dimensions, float >( i, greatest ) ) ) < bestcost ) {
					bestcost = cost;
					bestplane = kd::Plane< dimensions, float >( i, greatest );
				}
			}
		}
		return bestplane;
	}

	template< int dimensions, class IntersectionT, class PrimitiveT >
	float kd::Tree< dimensions, IntersectionT, PrimitiveT >::Node::SurfaceAreaHeuristic( const Plane< dimensions, float > & axis ) const {
		Matrix< dimensions, 1, float > diagonal = this->maxdimensions - this->mindimensions;
		//float totalarea = 2.0f * ( diagonal[ 0 ] * diagonal[ 1 ] + diagonal[ 1 ] * diagonal[ 2 ] + diagonal[ 2 ] * diagonal[ 0 ] );
		float totalarea = 0.0f;
		for( int i = 0; i < dimensions; i++ ) {
			totalarea += 2.0f * diagonal[ i ] * diagonal[ ( i + 1 ) % dimensions ];
		}

		float frontdist = axis.Distance( this->maxdimensions );
		float backdist = -axis.Distance( this->mindimensions );

		float frontarea = frontdist * totalarea / ( frontdist + backdist );
		float backarea = backdist * totalarea / ( frontdist + backdist );

		int frontcount = 0;
		int backcount = 0;

		for( typename std::list< PrimitiveT >::const_iterator iter = this->primlist.begin( ); iter != this->primlist.end( ); iter++ ) {
			const PrimitiveT & prim = *iter;				

			float mindist = prim.LeastDistance( axis );
			float maxdist = prim.GreatestDistance( axis );

			if( mindist >= 0.0f ) {
				frontcount++;
			} else if( maxdist <= 0.0f ) {
				backcount++;
			} else {
				frontcount++;
				backcount++;
			}
		}

		return 1.0f + 1.0f * ( frontcount * frontarea + backcount * backarea );
	}

	template< int dimensions, class IntersectionT, class PrimitiveT >
	void kd::Tree< dimensions, IntersectionT, PrimitiveT >::Node::Split( const Plane< dimensions, float > & splitplane ) {
		this->split = splitplane;

		Matrix< dimensions, 1, float > newmin = this->mindimensions; newmin[ this->split.GetAxis( ) ] = this->split.GetDistance( );
		Matrix< dimensions, 1, float > newmax = this->maxdimensions; newmax[ this->split.GetAxis( ) ] = this->split.GetDistance( );

		this->front = new Node( this->maxdimensions, newmin ); this->back = new Node( newmax, this->mindimensions );

		for( typename std::list< PrimitiveT >::const_iterator iter = this->primlist.begin( ); iter != this->primlist.end( ); iter++ ) {
			const PrimitiveT & prim = *iter;

			float mindist = prim.LeastDistance( splitplane );
			float maxdist = prim.GreatestDistance( splitplane );

			if( mindist >= 0.0f ) {
				this->front->Add( prim );
			} else if( maxdist <= 0.0f ) {
				this->back->Add( prim );
			} else {
				this->front->Add( prim );
				this->back->Add( prim );
			}
		}
	}

	/*template< int dimensions, class T >
	T kd::Tree< dimensions, T >::Node::IntersectLine( const Matrix< dimensions, 1, float > & pointa, const Matrix< dimensions, 1, float > & pointb, float startfraction, float endfraction ) const {
		if( front && back ) {
			const Matrix< dimensions, 1, float > direction = pointb - pointa;

			float startdist = this->split.Distance( pointa + startfraction * direction );
			float enddist = this->split.Distance( pointa + endfraction * direction );

			float adist = this->split.Distance( pointa );
			float bdist = this->split.Distance( pointb );
	
			if( startdist >= 0.0f && enddist >= 0.0f ) {
				return this->front->IntersectLine( pointa, pointb, startfraction, endfraction );
			} else if( startdist < 0.0f && enddist < 0.0f ) {
				return this->back->IntersectLine( pointa, pointb, startfraction, endfraction );
			} else {
				if( startdist > enddist ) {
					float denom = 1.0f / ( adist - bdist );
					float midfraction1 = ( adist + 0.03125f ) * denom;
					float midfraction2 = ( adist - 0.03125f ) * denom;

					T intersection = this->front->IntersectLine( pointa, pointb, startfraction, midfraction1 );
					if( intersection.Intersection( ) ) {
						return intersection;
					} else {
						return this->back->IntersectLine( pointa, pointb, midfraction2, endfraction );
					}
				} else {
					float denom = 1.0f / ( adist - bdist );
					float midfraction1 = ( adist - 0.03125f ) * denom;
					float midfraction2 = ( adist + 0.03125f ) * denom;

					T intersection = this->back->IntersectLine( pointa, pointb, startfraction, midfraction1 );
					if( intersection.Intersection( ) ) {
						return intersection;
					} else {
						return this->front->IntersectLine( pointa, pointb, midfraction2, endfraction );
					}
				}
			}
		} else {
			return this->IntersectPrimitives( pointa, pointb, startfraction, endfraction );
		}
	}

	template< int dimensions, class T >
	T kd::Tree< dimensions, T >::Node::IntersectPrimitives( const Matrix< dimensions, 1, float > & pointa, const Matrix< dimensions, 1, float > & pointb, float startfraction, float endfraction ) const {
		T closest;
		for( typename std::list< const kd::Primitive< dimensions, T > * >::const_iterator iter = this->primlist.begin( ); iter != this->primlist.end( ); iter++ ) {
			const kd::Primitive< dimesions, T > * prim = *iter;

			T intersection = prim->Intersect( line );
			if( intersection.Intersection( ) && intersection.raydistance < closest.raydistance && intersection.raydistance > startfraction && intersection.raydistance < endfraction ) {
				closest = intersection;
			}
		}

		return closest;
	}*/

	template< int dimensions, class IntersectionT, class PrimitiveT >
	void kd::Tree< dimensions, IntersectionT, PrimitiveT >::Node::Add( const PrimitiveT & prim ) {
		this->primlist.push_back( prim );
	}

	template< int dimensions, class IntersectionT, class PrimitiveT >
	size_t kd::Tree< dimensions, IntersectionT, PrimitiveT >::Node::Primitives( ) const {
		return this->primlist.size( );
	}

	template< int dimensions, class IntersectionT, class PrimitiveT >
	typename kd::Tree< dimensions, IntersectionT, PrimitiveT >::Node * kd::Tree< dimensions, IntersectionT, PrimitiveT >::Node::GetFront( ) {
		return this->front;
	}

	template< int dimensions, class IntersectionT, class PrimitiveT >
	typename kd::Tree< dimensions, IntersectionT, PrimitiveT >::Node * kd::Tree< dimensions, IntersectionT, PrimitiveT >::Node::GetBack( ) {
		return this->back;
	}

	///////////////////// Tree /////////////////////

	template< int dimensions, class IntersectionT, class PrimitiveT >
	kd::Tree< dimensions, IntersectionT, PrimitiveT >::Tree( ) : root( new Node( ) ) {
	}

	template< int dimensions, class IntersectionT, class PrimitiveT >
	kd::Tree< dimensions, IntersectionT, PrimitiveT >::Tree( const Tree & rhs ) : root( new Node( *rhs.root ) ) {
	}

	template< int dimensions, class IntersectionT, class PrimitiveT >
	kd::Tree< dimensions, IntersectionT, PrimitiveT >::~Tree( ) {
		delete this->root;
	}

	template< int dimensions, class IntersectionT, class PrimitiveT >
	kd::Tree< dimensions, IntersectionT, PrimitiveT > & kd::Tree< dimensions, IntersectionT, PrimitiveT >::operator=( const Tree & rhs ) {
		if( this != &rhs ) {
			delete this->root;
			root = new Node( *rhs.root );
		}

		return *this;
	}

	template< int dimensions, class IntersectionT, class PrimitiveT >
	void kd::Tree< dimensions, IntersectionT, PrimitiveT >::Add( const PrimitiveT & prim ) {
		this->root->Add( prim );
	}

	/*template< int dimensions, class T >
	T kd::Tree< dimensions, T >::IntersectLine( const Matrix< dimensions, 1, float > & pointa, const Matrix< dimensions, 1, float > & pointb ) const {
		return this->root->IntersectLine( pointa, pointb, 0.0f, 1.0f );
	}*/

	template< int dimensions, class IntersectionT, class PrimitiveT >
	void kd::Tree< dimensions, IntersectionT, PrimitiveT >::Build( int maxdepth ) {
		this->root->CalcDimensions( );
		this->Build( this->root, maxdepth );
	}

	template< int dimensions, class IntersectionT, class PrimitiveT >
	void kd::Tree< dimensions, IntersectionT, PrimitiveT >::Build( Node * node, int depth ) {
		if( node->Primitives( ) > 1 && depth > 0 ) {
			kd::Plane< dimensions, float > split = node->OptimalSplittingPlane( );
			node->Split( split );

			this->Build( node->GetFront( ), depth - 1 );
			this->Build( node->GetBack( ), depth - 1 );
		}
	}

}

#endif
