#include <cassert>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

#include <slib/Matrix.hpp>

#include <slib/BoneMatrixContainer.h>
#include <slib/JointAnimation.h>
#include <slib/MS3D.h>

#include "MS3DToMesh.h"

using namespace slib;

int main( int argc, char ** argv ) {
	if( argc != 2 ) {
		std::cout << "usage: " << argv[ 0 ] << " filename" << std::endl;
	} else {
		MS3DToMesh meshloader;
		std::ifstream filestream( argv[ 1 ], std::ios::binary );
		MS3D::Load( filestream, meshloader );
		filestream.close( );

		std::vector< MeshVertex > vertices = meshloader.GetVertices( );
		std::cout << vertices.size( ) << " vertices" << std::endl;

		std::list< std::pair< int, std::vector< MeshTriangle > > > groups = meshloader.GetGroups( );
		std::cout << groups.size( ) << " groups" << std::endl;

		std::vector< std::string > materials = meshloader.GetMaterials( );
		for( size_t i = 0; i < materials.size( ); i++ ) {
			std::cout << "material: " << materials[ i ] << std::endl;
		}

		std::vector< JointAnimation > jointanimations = meshloader.GetJointAnimations( );
		std::cout << jointanimations.size( ) << " joints" << std::endl;
	}

	return 0;
}
