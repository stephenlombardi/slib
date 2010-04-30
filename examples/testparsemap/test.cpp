#include <cassert>
#include <sstream>
#include <string>

#include <slib/ParseMap.h>

using namespace slib;

void test0( ) {
	std::string inputdata = "{\n"
		"\"classname\" \"worldspawn\"\n"
		"}\n";

	std::istringstream inputstream( inputdata );

	Map::Entity entity;
	inputstream >> entity;

	assert( entity.values[ "classname" ] == "worldspawn" );
}

void test1( ) {
	std::string inputdata = "// entity 0\n"
		"{\n"
		"\"classname\" \"worldspawn\"\n"
		"// brush 0\n"
		"{\n"
		"( 56 96 -56 ) ( 56 -64 -56 ) ( -56 96 -56 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 56 128 64 ) ( -56 128 64 ) ( 56 128 -64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 64 96 64 ) ( 64 96 -64 ) ( 64 -64 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -56 -64 -64 ) ( 56 -64 -64 ) ( -56 96 -64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -56 -128 -64 ) ( -56 -128 64 ) ( 56 -128 -64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -64 -64 -64 ) ( -64 96 -64 ) ( -64 -64 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"}\n"
		"}\n";

	std::istringstream inputstream( inputdata );

	Map::Entity entity;
	inputstream >> entity;

	assert( entity.values[ "classname" ] == "worldspawn" );
	assert( entity.brushes.size( ) == 1 );
	assert( entity.brushes.front( ).planes.size( ) == 6 );
	assert( entity.brushes.front( ).planes.front( ).texturepath == "gradient.tga" );
}

void test2( ) {
	std::string inputdata = "// entity 0\n"
		"{\n"
		"\"classname\" \"worldspawn\"\n"
		"// brush 0\n"
		"{\n"
		"( 56 96 0 ) ( 56 -64 0 ) ( -56 96 0 ) floor.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 40 264 64 ) ( -72 264 64 ) ( 40 264 -64 ) floor.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 264 32 64 ) ( 264 32 -64 ) ( 264 -128 64 ) floor.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -64 -64 -8 ) ( 48 -64 -8 ) ( -64 96 -8 ) floor.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -72 -264 -64 ) ( -72 -264 64 ) ( 40 -264 -64 ) floor.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -264 -72 -64 ) ( -264 88 -64 ) ( -264 -72 64 ) floor.tga 0 0 0 0.5 0.5 0 0 0\n"
		"}\n"
		"// brush 1\n"
		"{\n"
		"( 64 -120 64 ) ( 64 -128 64 ) ( -64 -120 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 56 -248 64 ) ( -72 -248 64 ) ( 56 -248 -56 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 264 -112 64 ) ( 264 -112 -56 ) ( 264 -120 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -64 -128 0 ) ( 64 -128 0 ) ( -64 -120 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -64 -264 -56 ) ( -64 -264 64 ) ( 64 -264 -56 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -264 -136 -56 ) ( -264 -128 -56 ) ( -264 -136 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"}\n"
		"// brush 2\n"
		"{\n"
		"( 64 128 64 ) ( 64 120 64 ) ( -64 128 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 72 264 64 ) ( -56 264 64 ) ( 72 264 -56 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 264 128 64 ) ( 264 128 -56 ) ( 264 120 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -64 120 0 ) ( 64 120 0 ) ( -64 128 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -56 248 -56 ) ( -56 248 64 ) ( 72 248 -56 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -264 104 -56 ) ( -264 112 -56 ) ( -264 104 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"}\n"
		"// brush 3\n"
		"{\n"
		"( 64 120 64 ) ( 64 -120 64 ) ( -64 120 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 32 248 64 ) ( -96 248 64 ) ( 32 248 -56 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 264 112 64 ) ( 264 112 -56 ) ( 264 -128 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -64 -120 0 ) ( 64 -120 0 ) ( -64 120 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -64 -248 -56 ) ( -64 -248 64 ) ( 64 -248 -56 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 248 -104 0 ) ( 248 136 0 ) ( 248 -104 120 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"}\n"
		"// brush 4\n"
		"{\n"
		"( -56 120 64 ) ( -56 -120 64 ) ( -64 120 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -56 248 64 ) ( -64 248 64 ) ( -56 248 -56 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -248 120 120 ) ( -248 120 0 ) ( -248 -120 120 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -64 -120 0 ) ( -56 -120 0 ) ( -64 120 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -64 -248 -56 ) ( -64 -248 64 ) ( -56 -248 -56 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -264 -72 -56 ) ( -264 168 -56 ) ( -264 -72 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"}\n"
		"// brush 5\n"
		"{\n"
		"( 72 64 64 ) ( 72 -64 64 ) ( -56 64 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 64 72 32 ) ( -64 72 32 ) ( 64 72 24 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 136 72 32 ) ( 136 72 24 ) ( 136 -56 32 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -64 -64 0 ) ( 64 -64 0 ) ( -64 64 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -64 56 24 ) ( -64 56 32 ) ( 64 56 24 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -136 -80 24 ) ( -136 48 24 ) ( -136 -80 32 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"}\n"
		"// brush 6\n"
		"{\n"
		"( 136 -56 64 ) ( 136 -72 64 ) ( -136 -56 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 136 -56 64 ) ( -136 -56 64 ) ( 136 -56 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 136 -56 64 ) ( 136 -56 0 ) ( 136 -72 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -136 -72 0 ) ( 136 -72 0 ) ( -136 -56 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -136 -72 0 ) ( -136 -72 64 ) ( 136 -72 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -136 -72 0 ) ( -136 -56 0 ) ( -136 -72 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"}\n"
		"// brush 7\n"
		"{\n"
		"( 136 56 64 ) ( 136 -8 64 ) ( 120 56 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 136 56 64 ) ( 120 56 64 ) ( 136 56 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 136 56 64 ) ( 136 56 0 ) ( 136 -8 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 120 -8 0 ) ( 136 -8 0 ) ( 120 56 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 120 0 0 ) ( 120 0 64 ) ( 136 0 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 120 -8 0 ) ( 120 56 0 ) ( 120 -8 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"}\n"
		"// brush 8\n"
		"{\n"
		"( -120 8 64 ) ( -120 -56 64 ) ( -136 8 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -120 0 64 ) ( -136 0 64 ) ( -120 0 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -120 8 64 ) ( -120 8 0 ) ( -120 -56 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -136 -56 0 ) ( -120 -56 0 ) ( -136 8 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -136 -56 0 ) ( -136 -56 64 ) ( -120 -56 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -136 -56 0 ) ( -136 8 0 ) ( -136 -56 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"}\n"
		"// brush 9\n"
		"{\n"
		"( 8 136 64 ) ( 8 72 64 ) ( -8 136 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 16 168 0 ) ( 0 168 0 ) ( 16 168 -8 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 8 136 64 ) ( 8 136 56 ) ( 8 72 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -8 72 0 ) ( 8 72 0 ) ( -8 136 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -8 72 -8 ) ( -8 72 0 ) ( 8 72 -8 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -8 72 -8 ) ( -8 136 -8 ) ( -8 72 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"}\n"
		"// brush 10\n"
		"{\n"
		"( 8 -72 64 ) ( 8 -136 64 ) ( -8 -72 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 8 -72 64 ) ( -8 -72 64 ) ( 8 -72 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( 8 -72 64 ) ( 8 -72 0 ) ( 8 -136 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -8 -136 0 ) ( 8 -136 0 ) ( -8 -72 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -8 -168 0 ) ( -8 -168 64 ) ( 8 -168 0 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"( -8 -136 0 ) ( -8 -72 0 ) ( -8 -136 64 ) gradient.tga 0 0 0 0.5 0.5 0 0 0\n"
		"}\n"
		"}\n"
		"// entity 1\n"
		"{\n"
		"\"classname\" \"info_player_start\"\n"
		"\"origin\" \"-64 128 24\"\n"
		"}\n";

	std::istringstream inputstream( inputdata );

	{
		Map::Entity entity;
		inputstream >> entity;

		assert( entity.values[ "classname" ] == "worldspawn" );
		assert( entity.brushes.size( ) == 11 );
		assert( entity.brushes.front( ).planes.size( ) == 6 );
		assert( entity.brushes.front( ).planes.front( ).texturepath == "floor.tga" );
		assert( entity.brushes.back( ).planes.size( ) == 6 );
		assert( entity.brushes.back( ).planes.front( ).texturepath == "gradient.tga" );
	}
	
	{
		Map::Entity entity;
		inputstream >> entity;
		
		assert( entity.values[ "classname" ] == "info_player_start" );
		assert( entity.values[ "origin" ] == "-64 128 24" );
		assert( entity.brushes.empty( ) );
	}
}

int main( ) {
	test0( );
	test1( );
	test2( );

	return 0;
}
