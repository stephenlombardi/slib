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

void InverseTransform( std::vector< float > & vertices, std::vector< float > & normals, const std::vector< float > & boneids, const std::vector< Matrix< 4, 4, float > > & matrices );

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

	const std::vector< float > & GetVertices( ) const;
	const std::vector< float > & GetNormals( ) const;
	const std::vector< float > & GetTexcoords( ) const;
	const std::vector< float > & GetBoneIds( ) const;
	const std::list< std::pair< int, std::vector< unsigned int > > > & GetGroups( ) const;
	const std::vector< std::string > & GetMaterials( ) const;
	BoneMatrixContainer GetBoneMatrixContainer( ) const;
	const std::vector< JointAnimation > & GetJointAnimations( ) const;
	float GetAnimationFPS( ) const;
private:
	std::vector< float > vertices;
	std::vector< float > normals;
	std::vector< float > texcoords;
	std::vector< float > boneids;
	std::vector< unsigned int > indices;

	std::list< std::pair< int, std::vector< unsigned int > > > groups;
	std::vector< std::string > materials;

	std::list< Matrix< 4, 4, float > > bonematrices;
	std::vector< int > parentindices;
	std::vector< std::string > jointnames;

	std::vector< JointAnimation > jointanimations;

	float animationfps;
};

#endif