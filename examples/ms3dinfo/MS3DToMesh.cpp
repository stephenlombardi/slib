#include "MS3DToMesh.h"

MeshVertex::MeshVertex( const float * _position, const float * _normal, char _boneid ) : boneId( _boneid ) {
	for( int i = 0; i < 3; i++ ) {
		position[ i ] = _position[ i ];
	}
	for( int i = 0; i < 3; i++ ) {
		normal[ i ] = _normal[ i ];
	}
}

MeshTriangle::MeshTriangle( ) { }

MeshTriangle::MeshTriangle( const unsigned short * _indices, const float * _texcoords ) {
	for( int i = 0; i < 3; i++ ) {
		indices[ i ] = _indices[ i ];
	}
	for( int i = 0; i < 3; i++ ) {
		for( int j = 0; j < 2; j++ ) {
			texcoords[ i ][ j ] = _texcoords[ i * 2 + j ];
		}
	}
}


void InverseTransform( std::vector< MeshVertex > & vertices, const std::vector< Matrix< 4, 4, float > > & matrices ) {
	for( size_t i = 0; i < vertices.size( ); i++ ) {
		if( vertices[ i ].boneId > -1 ) {
			const Matrix< 4, 4, float > & trans = matrices[ vertices[ i ].boneId ];

			Vec< 3 >::Tor( vertices[ i ].position ) = Transpose( Sub< 3, 3 >::Matrix( trans ) ) * ( Vec< 3 >::Tor( vertices[ i ].position ) - Sub< 3, 1 >::Matrix( trans, 0, 3 ) );

			Vec< 3 >::Tor( vertices[ i ].normal ) = Transpose( Sub< 3, 3 >::Matrix( trans ) ) * Vec< 3 >::Tor( vertices[ i ].normal );
		}
	}
}

void MS3DToMesh::Header( const MS3D::Header & header ) {
}

void MS3DToMesh::VerticesInfo( int vertexCount ) {
}

void MS3DToMesh::Vertex( const MS3D::Vertex & vertex ) {
	float empty [] = { 0.0f, 0.0f, 0.0f };
	vertices.push_back( MeshVertex( vertex.vertex, empty, vertex.boneId ) );
}

void MS3DToMesh::TrianglesInfo( int triangleCount ) {
}

void MS3DToMesh::Triangle( const MS3D::Triangle & triangle ) {
	const float texcoords [] = { triangle.s[ 0 ], triangle.t[ 0 ], triangle.s[ 1 ], triangle.t[ 1 ], triangle.s[ 2 ], triangle.t[ 2 ] };
	triangles.push_back( MeshTriangle( triangle.vertexIndices, texcoords ) );
	for( int i = 0; i < 3; i++ ) {
		for( int j = 0; j < 3; j++ ) {
			vertices[ triangle.vertexIndices[ i ] ].normal[ j ] = triangle.vertexNormals[ i ][ j ];
		}
	}
}

void MS3DToMesh::GroupsInfo( int groupCount ) {
}

void MS3DToMesh::Group( const MS3D::Group & group ) {
	groups.push_back( std::make_pair( group.materialIndex, std::vector< MeshTriangle >( group.numtriangles ) ) );
	for( int i = 0; i < group.numtriangles; i++ ) {
		groups.back( ).second[ i ] = triangles[ i ];
	}
}

void MS3DToMesh::MaterialsInfo( int materialCount ) {
}

void MS3DToMesh::Material( const MS3D::Material & material ) {
	materials.push_back( material.texture );
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

std::vector< MeshVertex > MS3DToMesh::GetVertices( ) const {
	return vertices;
}

std::list< std::pair< int, std::vector< MeshTriangle > > > MS3DToMesh::GetGroups( ) const {
	return groups;
}

std::vector< std::string > MS3DToMesh::GetMaterials( ) const {
	return materials;
}

BoneMatrixContainer MS3DToMesh::GetBoneMatrixContainer( ) const {
	return BoneMatrixContainer( bonematrices.begin( ), bonematrices.end( ), parentindices.begin( ), parentindices.end( ) );
}

std::vector< JointAnimation > MS3DToMesh::GetJointAnimations( ) const {
	return jointanimations;
}

float MS3DToMesh::GetAnimationFPS( ) const {
	return animationfps;
}
