#include "MS3DToMesh.h"

void InverseTransform( std::vector< float > & vertices, std::vector< float > & normals, const std::vector< float > & boneids, const std::vector< Matrix< 4, 4, float > > & matrices ) {
	for( size_t i = 0; i < boneids.size( ); i++ ) {
		if( boneids[ i ] > -1 ) {
			const Matrix< 4, 4, float > & trans = matrices[ (int)boneids[ i ] ];

			Vec< 3 >::Tor( &vertices[ i * 3 ] ) = Transpose( Sub< 3, 3 >::Matrix( trans ) ) * ( Vec< 3 >::Tor( &vertices[ i * 3 ] ) - Sub< 3, 1 >::Matrix( trans, 0, 3 ) );
			Vec< 3 >::Tor( &normals[ i * 3 ] ) = Transpose( Sub< 3, 3 >::Matrix( trans ) ) * Vec< 3 >::Tor( &normals[ i * 3 ] );
		}
	}
}

void MS3DToMesh::Header( const MS3D::Header & header ) {
}

void MS3DToMesh::VerticesInfo( int vertexCount ) {
}

void MS3DToMesh::Vertex( const MS3D::Vertex & vertex ) {
	for( int i = 0; i < 3; i++ ) {
		vertices.push_back( vertex.vertex[ i ] );
		normals.push_back( 0.0f );
	}

	for( int i = 0; i < 2; i++ ) {
		texcoords.push_back( -1.0f );
	}

	boneids.push_back( vertex.boneId );
}

void MS3DToMesh::TrianglesInfo( int triangleCount ) {
}

void MS3DToMesh::Triangle( const MS3D::Triangle & triangle ) {
	for( int i = 0; i < 3; i++ ) {

		bool conflict = false;
		for( size_t j = 0; j < indices.size( ); j++ ) {
			if( indices[ j ] == triangle.vertexIndices[ i ] ) {
				if( texcoords[ indices[ j ] * 2 + 0 ] != -1.0f ||
					texcoords[ indices[ j ] * 2 + 1 ] != -1.0f ) {
					if( texcoords[ indices[ j ] * 2 + 0 ] != triangle.s[ i ] ||
						texcoords[ indices[ j ] * 2 + 1 ] != triangle.t[ i ] ) {
						conflict = true;
						break;
					}
				}
			}
		}

		if( conflict ) {
			indices.push_back( vertices.size( ) / 3 );

			for( int j = 0; j < 3; j++ ) {
				vertices.push_back( vertices[ triangle.vertexIndices[ i ] * 3 + j ] );
			}

			texcoords.push_back( triangle.s[ i ] );
			texcoords.push_back( triangle.t[ i ] );

			for( int j = 0; j < 3; j++ ) {
				normals.push_back( triangle.vertexNormals[ i ][ j ] );
			}

			boneids.push_back( boneids[ triangle.vertexIndices[ i ] ] );
		} else {
			indices.push_back( triangle.vertexIndices[ i ] );

			texcoords[ triangle.vertexIndices[ i ] * 2 + 0 ] = triangle.s[ i ];
			texcoords[ triangle.vertexIndices[ i ] * 2 + 1 ] = triangle.t[ i ];

			for( int j = 0; j < 3; j++ ) {
				normals[ triangle.vertexIndices[ i ] * 3 + j ] = triangle.vertexNormals[ i ][ j ];
			}
		}
	}
}

void MS3DToMesh::GroupsInfo( int groupCount ) {
}

void MS3DToMesh::Group( const MS3D::Group & group ) {
	groups.push_back( std::make_pair( group.materialIndex, std::vector< unsigned int >( group.numtriangles * 3 ) ) );
	for( unsigned short i = 0; i < group.numtriangles; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			groups.back( ).second[ i * 3 + j ] = indices[ group.triangleIndices[ i ] * 3 + j ];
		}
	}
}

void MS3DToMesh::MaterialsInfo( int materialCount ) {
}

void MS3DToMesh::Material( const MS3D::Material & material ) {
	materials.push_back( std::string( material.texture ) );
}

void MS3DToMesh::JointsInfo( int _jointCount, float _animationfps, float _currenttime, int _totalframes ) {
	animationfps = _animationfps;
}

void MS3DToMesh::Joint( const MS3D::Joint & joint ) {
	Matrix< 4, 4, float > transform = MultiplicitiveIdentity< Matrix< 4, 4, float > >::Identity( );
	Sub< 3, 3 >::Matrix( transform ) = RotationMatrixX( joint.rotation[ 0 ] ) * RotationMatrixY( joint.rotation[ 1 ] ) * RotationMatrixZ( joint.rotation[ 2 ] );
	Sub< 3, 1 >::Matrix( transform, 0, 3 ) = Vec< 3 >::Tor( joint.position );
	bonematrices.push_back( transform );

	int parentindex = -1;
	for( size_t i = 0; i < jointnames.size( ); i++ ) {
		if( std::string( joint.parentName ) == jointnames[ i ] ) {
			parentindex = i;
		}
	}
	jointnames.push_back( std::string( joint.name ) );
	parentindices.push_back( parentindex );

	std::vector< JointAnimation::Keyframe > rotationKeyframes( joint.numKeyFramesRot );
	for( int i = 0; i < joint.numKeyFramesRot; i++ ) {
		rotationKeyframes[ i ] = JointAnimation::Keyframe( joint.keyFramesRot[ i ].time, joint.keyFramesRot[ i ].rotation );
	}

	std::vector< JointAnimation::Keyframe > positionKeyframes( joint.numKeyFramesPos );
	for( int i = 0; i < joint.numKeyFramesPos; i++ ) {
		positionKeyframes[ i ] = JointAnimation::Keyframe( joint.keyFramesPos[ i ].time, joint.keyFramesPos[ i ].position );
	}
	jointanimations.push_back( JointAnimation( rotationKeyframes.begin( ), rotationKeyframes.end( ), positionKeyframes.begin( ), positionKeyframes.end( ) ) );
}

const std::vector< float > & MS3DToMesh::GetVertices( ) const {
	return vertices;
}

const std::vector< float > & MS3DToMesh::GetNormals( ) const {
	return normals;
}

const std::vector< float > & MS3DToMesh::GetTexcoords( ) const {
	return texcoords;
}

const std::vector< float > & MS3DToMesh::GetBoneIds( ) const {
	return boneids;
}

const std::list< std::pair< int, std::vector< unsigned int > > > & MS3DToMesh::GetGroups( ) const {
	return groups;
}

const std::vector< std::string > & MS3DToMesh::GetMaterials( ) const {
	return materials;
}

BoneMatrixContainer MS3DToMesh::GetBoneMatrixContainer( ) const {
	return BoneMatrixContainer( bonematrices.begin( ), bonematrices.end( ), parentindices.begin( ), parentindices.end( ) );
}

const std::vector< JointAnimation > & MS3DToMesh::GetJointAnimations( ) const {
	return jointanimations;
}

float MS3DToMesh::GetAnimationFPS( ) const {
	return animationfps;
}
