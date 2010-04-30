#ifndef SLIB_NAVMESH_H
#define SLIB_NAVMESH_H

#include <iterator>
#include <limits>
#include <list>
#include <queue>
#include <vector>

#include <slib/Matrix.hpp>

namespace slib {

	class MeshGraph {
	public:
		struct MeshEdge {
			MeshEdge( );
			MeshEdge( int _mindex, int _pindex0, int _pindex1 );
			int mindex, pindex0, pindex1;
		};

		void addPoint( const vec2f & point );
		void addMesh( const std::vector< int > & mesh );
		size_t numMeshes( ) const;
		void calcConnectivity( );
		int getMeshIndex( const vec2f & point ) const;
		const std::vector< MeshEdge > & getNeighbors( int index ) const;
		vec2f getMeshCentroid( int index ) const;
		
		void shortestPath( int source, std::vector< float > & distances, std::vector< MeshEdge > & predecessors ) const;
		
		std::list< std::pair< MeshEdge, float > > getPath( const vec2f & start, const vec2f & end, const std::vector< MeshEdge > & predecessors, float & distance ) const;
		void refinePath( const vec2f & start, const vec2f & end, std::list< std::pair< MeshEdge, float > > & path, float & distance );
	private:
		struct MeshGraphCmp {
			bool operator()( std::pair< int, float > a, std::pair< int, float > b ) const;
		};

		std::vector< vec2f > points_;
		std::vector< std::vector< MeshEdge > > meshes_;
	};

}

#endif