#include <slib/ParseMap.h>

namespace slib {

	namespace Map {

		std::ostream & operator<<( std::ostream & out, const Map::BrushPlane & plane ) {
			out << "( " << plane.planepoints[ 0 ] << " " << plane.planepoints[ 1 ] << " " << plane.planepoints[ 2 ] << " ) ";
			out << "( " << plane.planepoints[ 3 ] << " " << plane.planepoints[ 4 ] << " " << plane.planepoints[ 5 ] << " ) ";
			out << "( " << plane.planepoints[ 6 ] << " " << plane.planepoints[ 7 ] << " " << plane.planepoints[ 8 ] << " ) ";

			out << plane.texturepath << " ";

			out << plane.ushift << " " << plane.vshift << " " << plane.texrotate << " ";
			out << plane.uscale << " " << plane.vscale << " " << plane.content << " " << plane.flags << " " << plane.value;
			return out;
		}

		std::ostream & operator<<( std::ostream & out, const Map::Brush & brush ) {
			out << "{" << std::endl;
			std::copy( brush.planes.begin( ), brush.planes.end( ), std::ostream_iterator< Map::BrushPlane >( out, "\n" ) );
			out << "}" << std::endl;
			return out;
		}

		std::ostream & operator<<( std::ostream & out, const Map::Entity & entity ) {
			out << "{" << std::endl;
			for( std::map< std::string, std::string >::const_iterator iter = entity.values.begin( ); iter != entity.values.end( ); ++iter ) {
				const std::string & key = iter->first;
				const std::string & val = iter->second;
				out << "\"" << key << "\"=\"" << val << "\"" << std::endl;
			}

			std::copy( entity.brushes.begin( ), entity.brushes.end( ), std::ostream_iterator< Map::Brush >( out, "\n" ) );
			out << "}" << std::endl;
			return out;
		}

		/// input

		char Consume( std::istream & in, const std::string & delims ) {
			char c;
			do {
				c = in.get( );
			} while( !in.eof( ) && delims.find( c ) == std::string::npos );
			return c;
			//for( ; in.peek( ) > -1 && delims.find( (char)in.peek( ) ) == std::string::npos; in.get( ) );
			//return (char)in.peek( );
		}

		char Consume( std::istream & in ) {
			char c;
			do {
			c = in.get( );
			} while( !in.eof( ) && ( c == ' ' || c == '\t' || c == '\n' ) );
			return c;
			//for( ; in.peek( ) > -1 && ( in.peek( ) == 32 || in.peek( ) == 13 || in.peek( ) == 10 || in.peek( ) == 9 ); in.get( ) );
			//return (char)in.peek( );
		}

		std::istream & ParseQuotedString( std::istream & in, std::string & string ) {
			Consume( in, "\"" );
			while( !in.eof( ) ) {
				char c = in.get( );
				if( c == '\\' ) {
					char next = in.get( );
					if( next == '"' ) {
						string += next;
					} else {
						string += c;
						string += next;
					}
				} else if( c == '"' ) {
					break;
				} else {
					string += c;
				}
			}
			return in;
		}

		std::istream & operator>>( std::istream & in, Map::BrushPlane & plane ) {
			Map::ParseVector( in, &plane.planepoints[ 0 ] );
			Map::ParseVector( in, &plane.planepoints[ 3 ] );
			Map::ParseVector( in, &plane.planepoints[ 6 ] );

			in >> plane.texturepath;

			in >> plane.ushift >> plane.vshift >> plane.texrotate >> plane.uscale >> plane.vscale;
			in >> plane.content >> plane.flags >> plane.value;

			return in;
		}

		std::istream & operator>>( std::istream & in, Map::Brush & brush ) {
			Map::Consume( in, "{" );
			while( !in.eof( ) ) {
				char c = Map::Consume( in, "/(}" );
				if( c == '(' ) {
					in.putback( c );
					brush.planes.push_back( Map::BrushPlane( ) );
					in >> brush.planes.back( );
				} else if( c == '/' ) {
					Map::Consume( in, "\n" );
				} else if( c == '}' ) {
					break;
				}
			}
			return in;
		}

		std::istream & operator>>( std::istream & in, Map::Entity & entity ) {
			Map::Consume( in, "{" );
			while( !in.eof( ) ) {
				char c = Map::Consume( in, "/\"{}" );
				if( c == '{' ) {
					in.putback( c );
					entity.brushes.push_back( Map::Brush( ) );
					in >> entity.brushes.back( );
				} else if( c == '"' ) {
					in.putback( c );
					std::string key, value;
					Map::ParseQuotedString( in, key );
					Map::ParseQuotedString( in, value );
					entity.values[ key ] = value;
				} else if( c == '/' ) {
					Map::Consume( in, "\n" );
				} else if( c == '}' ) {
					break;
				}
			}
			return in;
		}

	}

}

