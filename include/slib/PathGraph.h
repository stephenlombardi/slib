/*****************************
Author: Steve Lombardi
Description: graph for ai navigation
*****************************/

#ifndef SLIB_PATHGRAPH_H
#define SLIB_PATHGRAPH_H

#include <list>
#include <vector>
#include <limits>
#include <queue>

#include <slib/Matrix.hpp>

namespace slib {

	struct PathGraphCmp {
		bool operator()( std::pair< int, float > a, std::pair< int, float > b ) const;
	};

	class PathGraph {
	public:
		void addVertex( const vec3f & position );
		void addEdge( int u, int v );
		int numVertices( ) const;
		const vec3f & getVertexPosition( int v ) const;
		const std::list< int > & getNeighbors( int v ) const;
		void shortestPath( int source, std::vector< float > & distances, std::vector< int > & predecessors ) const;

		//void initShortestPath( );
	private:
		std::vector< std::list< int > > adjacency;
		std::vector< vec3f > vertexPositions;

		//std::vector< float > allpairdistances;
		//std::vector< int > allpairpredecessors;
	};

}

#endif