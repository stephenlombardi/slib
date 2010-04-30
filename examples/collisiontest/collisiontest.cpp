#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>

#include <GL/freeglut.h>

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
PlanarVolumeIntersection< 3, float, Plane< 3, float > > IntersectLine( const std::list< PlanarVolume< 3, float, Plane< 3, float > > > & pvs, const vec3f & start, const vec3f & end, const OffsetCalcT & offsetcalc ) {
	PlanarVolumeIntersection< 3, float, Plane< 3, float > > closestpvi( -std::numeric_limits< float >::infinity( ), std::numeric_limits< float >::infinity( ), 0, 0 );
	
	for( std::list< PlanarVolume< 3, float, Plane< 3, float > > >::const_iterator iter = pvs.begin( ); iter != pvs.end( ); ++iter ) {
		const PlanarVolume< 3, float, Plane< 3, float > > & pv = *iter;
		
		PlanarVolumeIntersection< 3, float, Plane< 3, float > > pvi = pv.IntersectLine( start, end, offsetcalc );
		
		if( pvi.enterplane && ( !closestpvi.enterplane || pvi.enterfraction < closestpvi.enterfraction ) ) {
			closestpvi = pvi;
		}
	}
	
	return closestpvi;
}

template< class OffsetCalcT >
PlanarVolumeIntersection< 3, float, Plane< 3, float > > IntersectLine( const kd::Tree< 3, BrushIntersection, Brush > & kdtree, const vec3f & start, const vec3f & end, const OffsetCalcT & offsetcalc ) {
	return kdtree.IntersectLine( start, end, offsetcalc ).intersection;
}

std::list< PlanarVolume< 3, float, Plane< 3, float > > > pvs;
std::list< PlanarPolygon< 3, float, Plane< 3, float > > > ppolys;
kd::Tree< 3, BrushIntersection, Brush > kdtree;
int width = 512, height = 512;
bool keys[ 256 ];
vec3f objpos = Vector3( 0.0f, 32.0f, 0.0f ), objvel( 0.0f ), objforce( 0.0f );
float objspeed = 8.0f, objmass = 16.0f;

void init( ) {
	std::ifstream mapstream( "test4.map" );

	if( !mapstream ) {
		std::cout << "file not found" << std::endl;
	} else {
		// read in map data
		while( !mapstream.eof( ) ) {
			Map::Consume( mapstream, "{" );

			if( !mapstream.eof( ) ) {
				mapstream.putback( '{' );
				Map::Entity entity;
				mapstream >> entity;

				if( entity.values[ "classname" ] == "worldspawn" ) {
					pvs = MapEntityToPlanarVolumes( entity );
				}
			}
		}
		
		mapstream.close( );

		for( std::list< PlanarVolume< 3, float, Plane< 3, float > > >::const_iterator iter = pvs.begin( ); iter != pvs.end( ); ++iter ) {
			std::list< PlanarPolygon< 3, float, Plane< 3, float > > > polygons = GeneratePolygons( *iter );
			ppolys.splice( ppolys.end( ), polygons );
			kdtree.Add( Brush( *iter ) );
		}

		int kddepth = 2;
		
		// build kd tree
		if( kddepth > 0 ) {
			kdtree.Build( kddepth );
		}
	}

	for( int i = 0; i < 256; i++ ) {
		keys[ i ] = false;
	}
}

void reshape( int _width, int _height ) {
	glViewport( 0, 0, _width, _height );
	width = _width;
	height = _height;
}

void display( ) {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluPerspective( 90.0, 1.0, 1.0, 4096.0 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	//gluLookAt( objpos[ 0 ], objpos[ 1 ] + 384.0f, objpos[ 2 ], objpos[ 0 ], objpos[ 1 ], objpos[ 2 ], 0.0f, 0.0f, -1.0f );
	gluLookAt( objpos[ 0 ], objpos[ 1 ] + 384.0f, objpos[ 2 ] + 64.0f, objpos[ 0 ], objpos[ 1 ], objpos[ 2 ], 0.0f, 0.0f, -1.0f );

	glPolygonMode( GL_FRONT, GL_FILL );
	glColor3f( 0.0f, 0.0f, 0.0f );
	glEnable( GL_POLYGON_OFFSET_FILL );
	glPolygonOffset( 1.0f, 1.0f );
	
	for( std::list< PlanarPolygon< 3, float, Plane< 3, float > > >::const_iterator iter = ppolys.begin( ); iter != ppolys.end( ); ++iter ) {
		const PlanarPolygon< 3, float, Plane< 3, float > > & polygon = *iter;

		glBegin( GL_POLYGON );

			for( std::list< vec3f >::const_reverse_iterator iter2 = polygon.points.rbegin( ); iter2 != polygon.points.rend( ); ++iter2 ) {
				const vec3f & point = *iter2;

				glVertex3f( point[ 0 ], point[ 1 ], point[ 2 ] );
			}

		glEnd( );
	}

	glDisable( GL_POLYGON_OFFSET_FILL );

	glPolygonMode( GL_FRONT, GL_LINE );
	glColor3f( 1.0f, 1.0f, 1.0f );
	
	for( std::list< PlanarPolygon< 3, float, Plane< 3, float > > >::const_iterator iter = ppolys.begin( ); iter != ppolys.end( ); ++iter ) {
		const PlanarPolygon< 3, float, Plane< 3, float > > & polygon = *iter;

		glBegin( GL_POLYGON );

			for( std::list< vec3f >::const_reverse_iterator iter2 = polygon.points.rbegin( ); iter2 != polygon.points.rend( ); ++iter2 ) {
				const vec3f & point = *iter2;

				glVertex3f( point[ 0 ], point[ 1 ], point[ 2 ] );
			}

		glEnd( );
	}

	glTranslatef( objpos[ 0 ], objpos[ 1 ], objpos[ 2 ] );
	//glutWireSphere( 32.0f, 8, 8 );
	glutWireCube( 32.0f );

	glutSwapBuffers( );
}

void timerfunc( int value ) {
	if( keys[ 'w' ] ) {
		objforce += Vector3( 0.0f, 0.0f, -1.0f ) * objspeed;
	}

	if( keys[ 's' ] ) {
		objforce += Vector3( 0.0f, 0.0f, 1.0f ) * objspeed;
	}

	if( keys[ 'a' ] ) {
		objforce += Vector3( -1.0f, 0.0f, 0.0f ) * objspeed;
	}

	if( keys[ 'd' ] ) {
		objforce += Vector3( 1.0f, 0.0f, 0.0f ) * objspeed;
	}

	objforce += -objvel; // friction

	objvel += objforce * ( 1.0f / objmass );
	objforce = vec3f( 0.0f );

	// collision detection
	do {
		//PlanarVolumeIntersection< 3, float, Plane< 3, float > > pvi = IntersectLine( pvs, objpos, objpos + objvel, ZeroOffsetCalc< 3, float >( ) );
		//PlanarVolumeIntersection< 3, float, Plane< 3, float > > pvi = IntersectLine( pvs, objpos, objpos + objvel, BoxOffsetCalc< 3, float >( vec3f( 16.0f ) ) );
		//PlanarVolumeIntersection< 3, float, Plane< 3, float > > pvi = IntersectLine( kdtree, objpos, objpos + objvel, ZeroOffsetCalc< 3, float >( ) );
		PlanarVolumeIntersection< 3, float, Plane< 3, float > > pvi = IntersectLine( kdtree, objpos, objpos + objvel, BoxOffsetCalc< 3, float >( vec3f( 16.0f ) ) );

		if( pvi.enterplane != 0 ) {
			objpos = objpos + objvel * ( pvi.enterfraction - ( 1.0f / 1024.0f ) );
			objvel = objvel - pvi.enterplane->GetNormal( ) * DotProduct( objvel, pvi.enterplane->GetNormal( ) );
		} else {
			break;
		}
	} while( true );

	objpos += objvel;

	glutPostRedisplay( );

	glutTimerFunc( 30, timerfunc, 0 );
}

void key( unsigned char key, int x, int y ) {
	keys[ key ] = true;
}

void keyup( unsigned char key, int x, int y ) {
	keys[ key ] = false;
}

int main( int argc, char ** argv ) {
	glutInit( &argc, argv );
	//glutInitContextVersion( 3, 2 );
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( width, height );
	glutInitWindowPosition( 50, 50 );

	glutCreateWindow( "Collision Test" );

	glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );

	glEnable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );
	
	glutReshapeFunc( reshape );
	glutDisplayFunc( display );
	glutKeyboardFunc( key );
	glutKeyboardUpFunc( keyup );
	//glutMouseFunc( mouse );
	//glutMotionFunc( mousemotion );
	//glutPassiveMotionFunc( mousemotion );

	init( );

	glutTimerFunc( 100, timerfunc, 0 );
	glutMainLoop( );

	return 0;
}
