#include <slib/smf.h>

namespace slib {

	void readSMF( const std::string & filename, int & outnverts, float * & outverts, float * & outcolors, float * & outnormals, int & outnindices, unsigned int * & outindices ) {
		std::vector< float > vertices;
		std::vector< float > colors;
		std::vector< float > normals;
		std::vector< unsigned int > indices;
		bool bindcolortovertex, bindnormaltovertex, bindtexeltovertex;

		bool inscope = false;

		std::ifstream file( filename.c_str( ) );

		while( !file.eof( ) ) {
			std::string line;
			getline( file, line );

			if( !inscope ) {
				if( line.find_first_not_of( " \t\n" ) == std::string::npos ) {
					// ignore line
				} else if( line[ 0 ] == '#' ) {
					// ignore comment
				} else if( line[ 0 ] == 'v' ) {
					std::istringstream strstream( line );
					char temp;
					float x, y, z;
					strstream >> temp >> x >> y >> z;
					vertices.push_back( x );
					vertices.push_back( y );
					vertices.push_back( z );
				} else if( line[ 0 ] == 'f' ) {
					std::istringstream strstream( line );
					char temp;
					unsigned int v1, v2, v3;
					strstream >> temp >> v1 >> v2 >> v3;
					indices.push_back( v1 );
					indices.push_back( v2 );
					indices.push_back( v3 );
				} else if( line.substr( 0, 4 ) == "bind" ) {
					std::istringstream strstream( line );
					std::string bindstr;
					strstream >> bindstr;
					char tobind;
					strstream >> tobind;
					std::string vertexorface;
					strstream >> vertexorface;

					if( tobind == 'c' ) {
						if( vertexorface == "vertex" ) {
							bindcolortovertex = true;
						} else if( vertexorface == "face" ) {
							bindcolortovertex = false;
						} else {
							// error
							assert( false );
						}
					} else if( tobind == 'n' ) {
						if( vertexorface == "vertex" ) {
							bindnormaltovertex = true;
						} else if( vertexorface == "face" ) {
							bindnormaltovertex = false;
						} else {
							// error
							assert( false );
						}
					} else {
						// error
						assert( false );
					}
				} else if( line[ 0 ] == 'c' ) {
					std::istringstream strstream( line );
					char temp;
					float r, g, b;
					strstream >> temp >> r >> g >> b;
					colors.push_back( r );
					colors.push_back( g );
					colors.push_back( b );
				} else if( line[ 0 ] == 'n' ) {
					std::istringstream strstream( line );
					char temp;
					float a, b, c;
					strstream >> temp >> a >> b >> c;
					normals.push_back( a );
					normals.push_back( b );
					normals.push_back( c );
				} else if( line[ 0 ] == 'r' ) {
					// unsupported
				} else if( line.substr( 0, 3 ) == "tex" ) {
					// unsupported
				} else if( line.substr( 0, 5 ) == "begin" ) {
					inscope = true;
				} else if( line.substr( 0, 3 ) == "end" ) {
					inscope = false;
				} else if( line.substr( 0, 5 ) == "trans" ) {
					// unsupported
				} else if( line.substr( 0, 3 ) == "rot" ) {
					// unsupported
				} else if( line.substr( 0, 5 ) == "scale" ) {
					// unsupported
				} else if( line.substr( 0, 7 ) == "t_trans" ) {
					// unsupported
				} else if( line.substr( 0, 7 ) == "t_scale" ) {
					// unsupported
				} else if( line.substr( 0, 3 ) == "set" ) {
					// unsupported
				} else {
					// unknown
				}
			}
		}

		outnverts = vertices.size( ) / 3;
		outverts = new float[ vertices.size( ) ];

		for( int i = 0; i < vertices.size( ); i++ ) {
			outverts[ i ] = vertices[ i ];
		}

		outnindices = indices.size( );
		outindices = new unsigned int[ indices.size( ) ];

		for( int i = 0; i < indices.size( ); i++ ) {
			outindices[ i ] = indices[ i ] - 1;
		}

		if( colors.empty( ) ) {
			outcolors = new float[ vertices.size( ) ];
			//outncols = vertices.size( ) / 3;

			for( int i = 0; i < vertices.size( ); i++ ) {
				outcolors[ i ] = 1.0f;
			}
		} else {
			if( bindcolortovertex ) {
				outcolors = new float[ colors.size( ) ];
				//result.ncols = colors.size( ) / 3;

				for( int i = 0; i < colors.size( ); i++ ) {
					outcolors[ i ] = colors[ i ];
				}
			} else {
				outcolors = new float[ colors.size( ) * 3 ];
				//result.ncols = colors.size( );

				for( int i = 0; i < colors.size( ); i += 3 ) {
					outcolors[ i * 3 + 0 ] = colors[ i + 0 ];
					outcolors[ i * 3 + 1 ] = colors[ i + 1 ];
					outcolors[ i * 3 + 2 ] = colors[ i + 2 ];
					outcolors[ i * 3 + 3 ] = colors[ i + 0 ];
					outcolors[ i * 3 + 4 ] = colors[ i + 1 ];
					outcolors[ i * 3 + 5 ] = colors[ i + 2 ];
					outcolors[ i * 3 + 6 ] = colors[ i + 0 ];
					outcolors[ i * 3 + 7 ] = colors[ i + 1 ];
					outcolors[ i * 3 + 8 ] = colors[ i + 2 ];
				}
			}
		}

		if( normals.empty( ) ) {
			outnormals = new float[ vertices.size( ) ];
			//result.nnormals = vertices.size( ) / 3;

			for( int i = 0; i < vertices.size( ); i++ ) {
				outnormals[ i ] = 0.0f;
			}

			for( int i = 0; i < indices.size( ); i += 3 ) {
				Matrix< 3, 1, float > normal = ( CrossProduct( Vec< 3 >::Tor( &outverts[ outindices[ i + 1 ] * 3 ] ) - Vec< 3 >::Tor( &outverts[ outindices[ i + 0 ] * 3 ] ),
					Vec< 3 >::Tor( &outverts[ outindices[ i + 2 ] * 3 ] ) - Vec< 3 >::Tor( &outverts[ outindices[ i + 0 ] * 3 ] ) ) );
				Vec< 3 >::Tor( &outnormals[ outindices[ i + 0 ] * 3 ] ) += normal;
				Vec< 3 >::Tor( &outnormals[ outindices[ i + 1 ] * 3 ] ) += normal;
				Vec< 3 >::Tor( &outnormals[ outindices[ i + 2 ] * 3 ] ) += normal;
			}

			for( int i = 0; i < vertices.size( ); i += 3 ) {
				Vec< 3 >::Tor( &outnormals[ i ] ) = Normalize( Vec< 3 >::Tor( &outnormals[ i ] ) );
			}
		} else {
			if( bindnormaltovertex ) {
				outnormals = new float[ normals.size( ) ];
				//result.nnormals = normals.size( ) / 3;

				for( int i = 0; i < normals.size( ); i += 3 ) {
					outnormals[ i + 0 ] = normals[ i + 0 ];
					outnormals[ i + 1 ] = normals[ i + 1 ];
					outnormals[ i + 2 ] = normals[ i + 2 ];
					Vec< 3 >::Tor( &outnormals[ i ] ) = Normalize( Vec< 3 >::Tor( &outnormals[ i ] ) );
				}
			} else {
				outnormals = new float[ normals.size( ) ];
				//result.nnormals = normals.size( );

				for( int i = 0; i < normals.size( ); i += 3 ) {
					outnormals[ i * 3 + 0 ] = normals[ i + 0 ];
					outnormals[ i * 3 + 1 ] = normals[ i + 1 ];
					outnormals[ i * 3 + 2 ] = normals[ i + 2 ];
					outnormals[ i * 3 + 3 ] = normals[ i + 0 ];
					outnormals[ i * 3 + 4 ] = normals[ i + 1 ];
					outnormals[ i * 3 + 5 ] = normals[ i + 2 ];
					outnormals[ i * 3 + 6 ] = normals[ i + 0 ];
					outnormals[ i * 3 + 7 ] = normals[ i + 1 ];
					outnormals[ i * 3 + 8 ] = normals[ i + 2 ];
				}
			}
		}
	}

}
