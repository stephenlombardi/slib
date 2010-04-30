#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>

#include <slib/Matrix.hpp>
#include <slib/ParseMap.h>
#include <slib/PlanarVolume.hpp>
#include <slib/Plane.hpp>
#include <slib/TGA.h>

#include "Brush.h"

using namespace slib;

std::list< PlanarVolume< 3, float, Plane< 3, float > > > MapEntityToPlanarVolumes( const Map::Entity & entity ) {
	std::list< PlanarVolume< 3, float, Plane< 3, float > > > pvs;
	
	for( std::list< Map::Brush >::const_iterator biter = entity.brushes.begin( ); biter != entity.brushes.end( ); ++biter ) {
		const Map::Brush & mapbrush = *biter;

		std::list< Plane< 3, float > > planes;

		for( std::list< Map::BrushPlane >::const_iterator piter = mapbrush.planes.begin( ); piter != mapbrush.planes.end( ); ++piter ) {
			const Map::BrushPlane & mapbrushplane = *piter;

			vec3f point1 = Vector3( (float)mapbrushplane.planepoints[ 0 ], (float)mapbrushplane.planepoints[ 2 ], (float)-mapbrushplane.planepoints[ 1 ] );
			vec3f point2 = Vector3( (float)mapbrushplane.planepoints[ 3 ], (float)mapbrushplane.planepoints[ 5 ], (float)-mapbrushplane.planepoints[ 4 ] );
			vec3f point3 = Vector3( (float)mapbrushplane.planepoints[ 6 ], (float)mapbrushplane.planepoints[ 8 ], (float)-mapbrushplane.planepoints[ 7 ] );
			vec3f vec1 = point2 - point1;
			vec3f vec2 = point3 - point1;
			vec3f normal = Normalize( CrossProduct( vec2, vec1 ) );
			float distance = DotProduct( normal, point1 );

			planes.push_back( Plane< 3, float >( normal, distance ) );
		}

		pvs.push_back( PlanarVolume< 3, float, Plane< 3, float > >( planes.begin( ), planes.end( ) ) );
	}
	
	return pvs;
}

template< class OffsetCalcT >
PlanarVolumeIntersection< 3, float, Plane< 3, float > > IntersectRay( const std::list< PlanarVolume< 3, float, Plane< 3, float > > > & pvs, const vec3f & start, const vec3f & dir, const OffsetCalcT & offsetcalc ) {
	PlanarVolumeIntersection< 3, float, Plane< 3, float > > closestpvi( -std::numeric_limits< float >::infinity( ), std::numeric_limits< float >::infinity( ), 0, 0 );
	
	for( std::list< PlanarVolume< 3, float, Plane< 3, float > > >::const_iterator iter = pvs.begin( ); iter != pvs.end( ); ++iter ) {
		const PlanarVolume< 3, float, Plane< 3, float > > & pv = *iter;
		
		PlanarVolumeIntersection< 3, float, Plane< 3, float > > pvi = pv.IntersectRay( start, dir, offsetcalc );
		
		if( pvi.enterplane && ( !closestpvi.enterplane || pvi.enterfraction < closestpvi.enterfraction ) ) {
			closestpvi = pvi;
		}
	}
	
	return closestpvi;
}

template< class OffsetCalcT >
PlanarVolumeIntersection< 3, float, Plane< 3, float > > IntersectRay( const kd::Tree< 3, BrushIntersection, Brush > & kdtree, const vec3f & start, const vec3f & dir, const OffsetCalcT & offsetcalc ) {
	BrushIntersection bi = kdtree.IntersectLine( start, start + dir * 4096.0f, offsetcalc );
	bi.intersection.enterfraction *= 4096.0f;
	return bi.intersection;
}

int main( int argc, char ** argv ) {
	if( argc != 15 ) {
		std::cout << "usage: " << argv[ 0 ] << " filename.map camerax cameray cameraz lookatx lookaty lookatz upx upy upz outfilename kddepth width height" << std::endl;
	} else {
		std::ifstream mapstream( argv[ 1 ] );

		if( !mapstream ) {
			std::cout << "file not found" << std::endl;
		} else {
			// read in map data
			std::list< Map::Entity > entities;
			std::list< PlanarVolume< 3, float, Plane< 3, float > > > pvs;

			while( !mapstream.eof( ) ) {
				Map::Consume( mapstream, "{" );

				if( !mapstream.eof( ) ) {
					mapstream.putback( '{' );
					entities.push_back( Map::Entity( ) );
					mapstream >> entities.back( );

					if( entities.back( ).values[ "classname" ] == "worldspawn" ) {
						pvs = MapEntityToPlanarVolumes( entities.back( ) );
					}
				}
			}
			
			mapstream.close( );

			kd::Tree< 3, BrushIntersection, Brush > kdtree;
			for( std::list< PlanarVolume< 3, float, Plane< 3, float > > >::const_iterator iter = pvs.begin( ); iter != pvs.end( ); ++iter ) {
				kdtree.Add( Brush( *iter ) );
			}

			// parse camera data
			std::istringstream paramstream( std::string( argv[ 2 ] ) + " " + argv[ 3 ] + " " + argv[ 4 ] + " " + 
				argv[ 5 ] + " " + argv[ 6 ] + " " + argv[ 7 ] + " " + 
				argv[ 8 ] + " " + argv[ 9 ] + " " + argv[ 10 ] + " " + 
				argv[ 12 ] + " " +
				argv[ 13 ] + " " + argv[ 14 ] );
			
			float camerax, cameray, cameraz;
			float lookatx, lookaty, lookatz;
			float upx, upy, upz;
			int kddepth;
			int width, height;
			
			paramstream >> camerax >> cameray >> cameraz;
			paramstream >> lookatx >> lookaty >> lookatz;
			paramstream >> upx >> upy >> upz;
			paramstream >> kddepth;
			paramstream >> width >> height;
			
			vec3f camerapos = Vector3( camerax, cameray, cameraz );
			vec3f cameradir = Normalize( Vector3( lookatx, lookaty, lookatz ) - camerapos );
			vec3f cameraup = Vector3N( upx, upy, upz );
			vec3f cameraright = Normalize( CrossProduct( cameraup, cameradir ) );
			cameraup = Normalize( CrossProduct( cameradir, cameraright ) );

			// build kd tree
			if( kddepth > 0 ) {
				kdtree.Build( kddepth );
			}

			std::clock_t starttime = std::clock( );
			
			// do rendering
			unsigned char * tgadata = new unsigned char[ width * height * 3 ];
			
			for( int y = 0; y < height; y++ ) {
				for( int x = 0; x < width; x++ ) {
					vec3f gridpos = ( x - width / 2.0f ) / ( width / 2.0f ) * cameraright + ( y - ( height / 2.0f ) ) / ( height / 2.0f ) * cameraup;
					vec3f raydir = Normalize( cameradir + gridpos );
					
					//PlanarVolumeIntersection< 3, float, Plane< 3, float > > closest = IntersectRay( pvs, camerapos, raydir, ZeroOffsetCalc< 3, float >( ) );
					//PlanarVolumeIntersection< 3, float, Plane< 3, float > > closest = IntersectRay( kdtree, camerapos, raydir, ZeroOffsetCalc< 3, float >( ) );
					PlanarVolumeIntersection< 3, float, Plane< 3, float > > closest = kddepth > 0 ? IntersectRay( kdtree, camerapos, raydir, ZeroOffsetCalc< 3, float >( ) ) : IntersectRay( pvs, camerapos, raydir, ZeroOffsetCalc< 3, float >( ) );
					float cameradepth = DotProduct( cameradir, raydir * closest.enterfraction );
					unsigned char rval = (unsigned char)( ( cos( cameradepth * 2.0f * 3.1415926f / 256.0f ) * 0.5f + 0.5f ) * 255.0f );
					unsigned char gval = (unsigned char)( ( cos( ( cameradepth + 384.0f ) * 2.0f * 3.1415926f / 512.0f ) * 0.5f + 0.5f ) * 255.0f );
					unsigned char bval = (unsigned char)( ( cos( ( cameradepth + 256.0f ) * 2.0f * 3.1415926f / 384.0f ) * 0.5f + 0.5f ) * 255.0f );
					//unsigned char val = closest.enterplane ? (unsigned char)( closest.enterfraction * 256.0f / 256.0f ) : 0;

					tgadata[ y * width * 3 + x * 3 + 0 ] = rval;
					tgadata[ y * width * 3 + x * 3 + 1 ] = gval;
					tgadata[ y * width * 3 + x * 3 + 2 ] = bval;
				}
			}
			
			std::cout << ( std::clock( ) - starttime ) << std::endl;

			std::ofstream tgastream( argv[ 11 ], std::ios::binary );
			WriteTGA( tgastream, width, height, 24, tgadata );
			tgastream.close( );
			
			delete [] tgadata;
		}
	}

	return 0;
}
