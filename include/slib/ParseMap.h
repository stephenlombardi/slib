/*****************************
Author: Steve Lombardi
Description: loads .map files
from the GtkRadiant map editor
*****************************/

#ifndef SLIB_PARSEMAP_H
#define SLIB_PARSEMAP_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <list>
#include <string>
#include <sstream>

namespace slib {

	namespace Map {

		class BrushPlane {
		public:
		//private:
			int planepoints[ 9 ];
			std::string texturepath;
			float ushift, vshift, texrotate, uscale, vscale;
			int content, flags, value;
		};

		class Brush {
		public:
			std::list< BrushPlane > planes;
		};

		class Entity {
		public:
		//private:
			std::map< std::string, std::string > values;
			std::list< Brush > brushes;
		};

		// output

		std::ostream & operator<<( std::ostream & out, const BrushPlane & plane );
		std::ostream & operator<<( std::ostream & out, const Brush & brush );
		std::ostream & operator<<( std::ostream & out, const Entity & entity );

		/// input

		char Consume( std::istream & in, const std::string & delims );
		char Consume( std::istream & in );
		std::istream & ParseQuotedString( std::istream & in, std::string & string );

		template< class Iterator >
		std::istream & ParseVector( std::istream & in, Iterator iter ) {
			Consume( in, "(" );
			std::string str;
			char c;
			do {
				c = in.get( );
				if( c != ')' ) {
					str += c;
				}
			} while( c != ')' );

			std::istringstream iss( str );
			while( !iss.eof( ) ) {
				iss >> *iter++;
			}

			return in;
		}

		std::istream & operator>>( std::istream & in, BrushPlane & plane );
		std::istream & operator>>( std::istream & in, Brush & brush );
		std::istream & operator>>( std::istream & in, Entity & entity );
	}

}

#endif
