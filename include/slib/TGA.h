/***********************************
Author: Steve Lombardi
Description: loads Targa image files

Only supports 24 or 32 bit images
***********************************/

#ifndef SLIB_TGA_H
#define SLIB_TGA_H

#include <cassert>
#include <istream>

namespace slib {
	unsigned char * ReadTGA( std::istream & in, int & width, int & height, int & depth );
	void WriteTGA( std::ostream & out, int width, int height, int depth, unsigned char * data );
}

#endif
