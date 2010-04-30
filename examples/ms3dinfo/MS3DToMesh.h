#ifndef SLIB_MS3DTOMESH_H
#define SLIB_MS3DTOMESH_H

#include <string>
#include <list>
#include <vector>

#include <slib/Matrix.hpp>

#include <slib/BoneMatrixContainer.h>
#include <slib/JointAnimation.h>
#include <slib/MS3D.h>

using namespace slib;

struct MeshVertex {
	MeshVertex( const float * _position, const float * _normal, char _boneid );
	float position[ 3 ];
	float normal[ 3 ];
	char boneId;
};

struct MeshTriangle {
	MeshTriangle( );
	MeshTriangle( const unsigned short * _indices, const float * _texcoords );
	int indices[ 3 ];
	float texcoords[ 3 ][ 2 ];
};

void InverseTransform( std::vector< MeshVertex > & vertices, const std::vector< mat4f > & matrices );

class MS3DToMesh {
public:
	void Header( const MS3D::Header & header );
	void VerticesInfo( int vertexCount );
	void Vertex( const MS3D::Vertex & vertex );
	void TrianglesInfo( int triangleCount );
	void Triangle( const MS3D::Triangle & triangle );
	void GroupsInfo( int groupCount );
	void Group( const MS3D::Group & group );
	void MaterialsInfo( int materialCount );
	void Material( const MS3D::Material & material );
	void JointsInfo( int _jointCount, float _animationfps, float _currenttime, int _totalframes );
	void Joint( const MS3D::Joint & joint );

	std::vector< MeshVertex > GetVertices( ) const;
	std::list< std::pair< int, std::vector< MeshTriangle > > > GetGroups( ) const;
	std::vector< std::string > GetMaterials( ) const;
	BoneMatrixContainer GetBoneMatrixContainer( ) const;
	std::vector< JointAnimation > GetJointAnimations( ) const;
	float GetAnimationFPS( ) const;
private:
	std::vector< MeshVertex > vertices;
	std::vector< MeshTriangle > triangles;

	std::list< std::pair< int, std::vector< MeshTriangle > > > groups;
	std::vector< std::string > materials;

	std::list< mat4f > bonematrices;
	std::vector< int > parentindices;
	std::vector< std::string > jointnames;

	std::vector< JointAnimation > jointanimations;

	float animationfps;
};

#endif
