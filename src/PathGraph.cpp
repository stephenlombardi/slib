#include <slib/PathGraph.h>

namespace slib {

	bool PathGraphCmp::operator()( std::pair< int, float > a, std::pair< int, float > b ) const {
		return a.second > b.second;
	}

	void PathGraph::addVertex( const vec3f & position ) {
		adjacency.push_back( std::list< int >( ) );
		vertexPositions.push_back( position );
	}

	void PathGraph::addEdge( int u, int v ) {
		adjacency[ u ].push_back( v );
		adjacency[ v ].push_back( u );
	}

	int PathGraph::numVertices( ) const {
		return adjacency.size( );
	}

	const vec3f & PathGraph::getVertexPosition( int v ) const {
		return vertexPositions[ v ];
	}

	const std::list< int > & PathGraph::getNeighbors( int v ) const {
		return adjacency[ v ];
	}

	/*void PathGraph::shortestPath( int source, std::vector< float > & distances, std::vector< int > & predecessors ) const {
		// bellman-ford cause i'm lazy
		for( int i = 0; i < numVertices( ); i++ ) {
			distances[ i ] = std::numeric_limits< float >::infinity( );
			predecessors[ i ] = -1;
		}
		distances[ source ] = 0.0f;

		for( int i = 1; i < numVertices( ); i++ ) {
			for( int j = 0; j < numVertices( ); j++ ) {
				for( std::list< int >::const_iterator iter = getNeighbors( j ).begin( ); iter != getNeighbors( j ).end( ); iter++ ) {
					int k = *iter;

					float weight = Norm( getVertexPosition( j ) - getVertexPosition( k ) );
					if( distances[ j ] + weight < distances[ k ] ) {
						distances[ k ] = distances[ j ] + weight;
						predecessors[ k ] = j;
					}
				}
			}
		}
	}*/

	void PathGraph::shortestPath( int source, std::vector< float > & distances, std::vector< int > & predecessors ) const {
		// dijkstra's

		// init
		for( int i = 0; i < numVertices( ); i++ ) {
			distances[ i ] = std::numeric_limits< float >::infinity( );
			predecessors[ i ] = -1;
		}
		distances[ source ] = 0.0f;

		// create priority queue
		std::vector< bool > used( numVertices( ), false );
		std::priority_queue< std::pair< int, float >, std::vector< std::pair< int, float > >, PathGraphCmp > queue( (PathGraphCmp( )) );
		queue.push( std::make_pair( source, 0.0f ) );

		// main loop
		while( !queue.empty( ) ) {
			// remove lowest
			int u = queue.top( ).first;
			float dist = queue.top( ).second;
			queue.pop( );

			//if( !used[ u ] ) {
				used[ u ] = true;

				for( std::list< int >::const_iterator iter = getNeighbors( u ).begin( ); iter != getNeighbors( u ).end( ); iter++ ) {
					int v = *iter;

					float weight = Norm( getVertexPosition( v ) - getVertexPosition( u ) );
					if( dist + weight < distances[ v ] ) {
						distances[ v ] = dist + weight;
						predecessors[ v ] = u;
						queue.push( std::make_pair( v, distances[ v ] ) );
					}
				}
			//}
		}
	}

	/*void PathGraph::shortestPath( int source, std::vector< float > & distances, std::vector< int > & predecessors ) const {
		for( int i = 0; i < numVertices( ); i++ ) {
			distances[ i ] = allpairdistances[ source * numVertices( ) + i ];

			int next = source;
			while( allpairpredecessors[ next * numVertices( ) + i ] != -1 ) {
				next = allpairpredecessors[ next * numVertices( ) + i ];
			}

			predecessors[ i ] = next;
		}
	}

	void PathGraph::initShortestPath( ) {
		// floyd-warshall
		allpairdistances = std::vector< float >( numVertices( ) * numVertices( ), std::numeric_limits< float >::infinity( ) );
		for( int i = 0; i < numVertices( ); i++ ) {
			for( std::list< int >::const_iterator iter = getNeighbors( i ).begin( ); iter != getNeighbors( i ).end( ); iter++ ) {
				allpairdistances[ i * numVertices( ) + *iter ] = Norm( getVertexPosition( i ) - getVertexPosition( *iter ) );
			}
		}
		allpairpredecessors = std::vector< int >( numVertices( ) * numVertices( ), -1 );

		for( int k = 0; k < numVertices( ); k++ ) {
			for( int i = 0; i < numVertices( ); i++ ) {
				for( int j = 0; j < numVertices( ); j++ ) {
					if( allpairdistances[ i * numVertices( ) + k ] + allpairdistances[ k * numVertices( ) + j ] < allpairdistances[ i * numVertices( ) + j ] ) {
						allpairdistances[ i * numVertices( ) + j ] = allpairdistances[ i * numVertices( ) + k ] + allpairdistances[ k * numVertices( ) + j ];
						allpairpredecessors[ i * numVertices( ) + j ] = k;
					}
				}
			}
		}
	}*/

}
