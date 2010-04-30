#include <slib/TGA.h>

unsigned char * slib::ReadTGA( std::istream & in, int & width, int & height, int & depth ) {
	in.seekg( 0, std::ios::beg );
	unsigned char idlength = in.get( );

	in.seekg( 2, std::ios::beg );
	unsigned char type = in.get( );

	//throw Exception, unsupported type
	assert( type == 2 );

	unsigned short _width, _height;
	in.seekg( 12, std::ios::beg );
	in.read( reinterpret_cast< char * >( &_width ), 2 );
	in.read( reinterpret_cast< char * >( &_height ), 2 );
	unsigned char _depth = in.get( );

	width = _width;
	height = _height;
	depth = _depth;

	unsigned char * data = new unsigned char[ width * height * depth / 8 ];
	in.seekg( 18 + idlength, std::ios::beg );
	//in.read( reinterpret_cast< char * >( data ), width * height * depth / 8 );
	for( int y = height - 1; y >= 0; y-- ) {
		for( int x = 0; x < width; x++ ) {
			//for( int j = depth / 8 - 1; j >= 0; j-- ) {
			//for( int j = 0; j < depth / 8; j++ ) {
			for( int j = 2; j >= 0; j-- ) {
				data[ y * width * depth / 8 + x * depth / 8 + j ] = in.get( );
			}
			if( depth / 8 == 4 ) {
				data[ y * width * depth / 8 + x * depth / 8 + 3 ] = in.get( );
			}
		}
	}

	return data;
}

void slib::WriteTGA( std::ostream & out, int width, int height, int depth, unsigned char * data ) {
	out.put( 0 ); // id length
	out.put( 0 ); // color map type
	out.put( 2 ); // image type

	unsigned short zero = 0;
	out.write( reinterpret_cast< char * >( &zero ), 2 ); // colormap info
	out.write( reinterpret_cast< char * >( &zero ), 2 ); // colormap info
	out.put( 0 );

	out.write( reinterpret_cast< char * >( &zero ), 2 ); // x-origin
	out.write( reinterpret_cast< char * >( &zero ), 2 ); // y-origin

	unsigned short _width = width, _height = height;
	out.write( reinterpret_cast< char * >( &_width ), 2 ); // width
	out.write( reinterpret_cast< char * >( &_height ), 2 ); // height
	out.put( (char)depth ); // depth

	out.put( 0 ); // image descriptor

	out.write( reinterpret_cast< char * >( data ), width * height * depth / 8 );
}
