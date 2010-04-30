#include <slib/NavMesh.h>

namespace slib {

	bool MeshGraph::MeshGraphCmp::operator()( std::pair< int, float > a, std::pair< int, float > b ) const {
		return a.second > b.second;
	}

	MeshGraph::MeshEdge::MeshEdge( ) {
	}

	MeshGraph::MeshEdge::MeshEdge( int _mindex, int _pindex0, int _pindex1 ) : mindex( _mindex ), pindex0( _pindex0 ), pindex1( _pindex1 ) {
	}

	void MeshGraph::addPoint( const vec2f & point ) {
		points_.push_back( point );
	}

	void MeshGraph::addMesh( const std::vector< int > & mesh ) {
		meshes_.push_back( std::vector< MeshGraph::MeshEdge >( mesh.size( ) ) );
		for( size_t i = 0; i < mesh.size( ); i++ ) {
			meshes_.back( )[ i ] = MeshGraph::MeshEdge( -1, mesh[ i ], mesh[ ( i + 1 ) % mesh.size( ) ] );
		}
	}

	size_t MeshGraph::numMeshes( ) const {
		return meshes_.size( );
	}

	void MeshGraph::calcConnectivity( ) {
		for( size_t i = 0; i < meshes_.size( ); i++ ) {
			for( size_t j = 0; j < meshes_[ i ].size( ); j++ ) {
				int aindex0 = meshes_[ i ][ j ].pindex0;
				int aindex1 = meshes_[ i ][ j ].pindex1;
				
				for( size_t k = 0; k < meshes_.size( ); k++ ) {
					for( size_t l = 0; l < meshes_[ k ].size( ); l++ ) {
						int bindex0 = meshes_[ k ][ l ].pindex0;
						int bindex1 = meshes_[ k ][ l ].pindex1;
						
						if( aindex0 == bindex1 && aindex1 == bindex0 ) {
							meshes_[ i ][ j ].mindex = k;
						}
					}
				}
			}
		}
	}

	int MeshGraph::getMeshIndex( const vec2f & point ) const {
		for( size_t i = 0; i < meshes_.size( ); i++ ) {
			bool within = true;
			for( size_t j = 0; j < meshes_[ i ].size( ); j++ ) {
				int aindex0 = meshes_[ i ][ j ].pindex0;
				int aindex1 = meshes_[ i ][ j ].pindex1;
				
				vec2f vec = points_[ aindex1 ] - points_[ aindex0 ];
				vec2f normal = Vector2N( vec[ 1 ], -vec[ 0 ] );
				
				float planedist = DotProduct( normal, points_[ aindex0 ] );
				float pointdist = DotProduct( normal, point ) - planedist;
				
				if( pointdist > 0.0f ) {
					within = false;
					break;
				}
			}
			
			if( within ) {
				return i;
			}
		}
		
		return -1;
	}

	const std::vector< MeshGraph::MeshEdge > & MeshGraph::getNeighbors( int index ) const {
		return meshes_[ index ];
	}

	vec2f MeshGraph::getMeshCentroid( int index ) const {
		vec2f centroid( 0.0f );
		
		const std::vector< MeshGraph::MeshEdge > & neighbors = getNeighbors( index );
		for( size_t i = 0; i < neighbors.size( ); i++ ) {
			centroid += points_[ neighbors[ i ].pindex0 ];
		}
		
		return centroid * ( 1.0f / neighbors.size( ) );
	}

	void MeshGraph::shortestPath( int source, std::vector< float > & distances, std::vector< MeshGraph::MeshEdge > & predecessors ) const {
		// dijkstra's

		// init
		if( distances.size( ) != numMeshes( ) ) {
			distances.resize( numMeshes( ) );
		}
		if( predecessors.size( ) != numMeshes( ) ) {
			predecessors.resize( numMeshes( ) );
		}
		for( size_t i = 0; i < numMeshes( ); i++ ) {
			distances[ i ] = std::numeric_limits< float >::infinity( );
			predecessors[ i ] = MeshGraph::MeshEdge( -1, -1, -1 );
		}
		distances[ source ] = 0.0f;

		// create priority queue
		//std::vector< bool > used( numVertices( ), false );
		std::priority_queue< std::pair< int, float >, std::vector< std::pair< int, float > >, MeshGraphCmp > queue( (MeshGraphCmp( )) );
		queue.push( std::make_pair( source, 0.0f ) );

		// main loop
		while( !queue.empty( ) ) {
			// remove lowest
			int u = queue.top( ).first;
			float dist = queue.top( ).second;
			queue.pop( );

			//if( !used[ u ] ) {
				//used[ u ] = true;

				const std::vector< MeshGraph::MeshEdge > & neighbors = getNeighbors( u );
				for( size_t i = 0; i < neighbors.size( ); i++ ) {
					const MeshGraph::MeshEdge & edge = neighbors[ i ];

					vec2f edgemidpoint = ( points_[ edge.pindex0 ] + points_[ edge.pindex1 ] ) * 0.5f;
					float weight = Norm( getMeshCentroid( u ) - edgemidpoint ) + Norm( getMeshCentroid( edge.mindex ) - edgemidpoint );
					if( dist + weight < distances[ edge.mindex ] ) {
						distances[ edge.mindex ] = dist + weight;
						predecessors[ edge.mindex ] = MeshGraph::MeshEdge( u, edge.pindex0, edge.pindex1 );
						queue.push( std::make_pair( edge.mindex, distances[ edge.mindex ] ) );
					}
				}
			//}
		}
	}

	std::list< std::pair< MeshGraph::MeshEdge, float > > MeshGraph::getPath( const vec2f & start, const vec2f & end, const std::vector< MeshGraph::MeshEdge > & predecessors, float & distance ) const {
		std::list< std::pair< MeshGraph::MeshEdge, float > > result;
		distance = 0;
		
		int endindex = getMeshIndex( end );
		if( endindex != -1 ) {
			int index = endindex;
			vec2f lastpoint = end;
			
			result.push_front( std::make_pair( MeshGraph::MeshEdge( index, -1, -1 ), 0.5f ) );
			while( predecessors[ index ].mindex != -1 ) {
				vec2f edgept = points_[ predecessors[ index ].pindex0 ] * 0.5f + points_[ predecessors[ index ].pindex1 ] * 0.5f;
				distance += Norm( edgept - lastpoint );
				lastpoint = edgept;
				
				result.push_front( std::make_pair( predecessors[ index ], 0.5f ) );
				index = predecessors[ index ].mindex;
			}
			
			distance += Norm( lastpoint - start );
		}
		
		return result;
	}

	void MeshGraph::refinePath( const vec2f & start, const vec2f & end, std::list< std::pair< MeshGraph::MeshEdge, float > > & path, float & distance ) {
		distance = 0;
		
		if( path.size( ) <= 1 ) {
			return;
		}
		
		vec2f thisnewpt;
		
		std::list< std::pair< MeshGraph::MeshEdge, float > >::iterator enditer = path.end( ); --enditer;
		std::list< std::pair< MeshGraph::MeshEdge, float > >::iterator lastiter = enditer; --lastiter;
		for( std::list< std::pair< MeshGraph::MeshEdge, float > >::iterator iter = path.begin( ); iter != enditer; ++iter ) {

			vec2f thisedgept0 = points_[ iter->first.pindex0 ];
			vec2f thisedgept1 = points_[ iter->first.pindex1 ];
			
			vec2f prevedgept0 = start;
			vec2f prevedgept1 = start;
			float prevedget = 0.0f;

			if( iter != path.begin( ) ) {
				std::list< std::pair< MeshGraph::MeshEdge, float > >::iterator previter = iter; --previter;
				prevedgept0 = points_[ previter->first.pindex0 ];
				prevedgept1 = points_[ previter->first.pindex1 ];
				prevedget = previter->second;
			}

			vec2f nextedgept0 = end;
			vec2f nextedgept1 = end;
			float nextedget = 0.0f;
			
			if( iter != lastiter ) {
				std::list< std::pair< MeshGraph::MeshEdge, float > >::iterator nextiter = iter; nextiter++;
				nextedgept0 = points_[ nextiter->first.pindex0 ];
				nextedgept1 = points_[ nextiter->first.pindex1 ];
				nextedget = nextiter->second;
			}
			
			vec2f prevedgept = prevedgept0 * ( 1.0f - prevedget ) + prevedgept1 * prevedget;
			vec2f nextedgept = nextedgept0 * ( 1.0f - nextedget ) + nextedgept1 * nextedget;
			
			// calculate intersection of line segments
			vec2f vec0 = thisedgept1 - thisedgept0;
			vec2f vec1 = nextedgept - prevedgept;
			vec2f normal = Vector2N( vec1[ 1 ], -vec1[ 0 ] );
			float planedist = DotProduct( normal, prevedgept );
			float newt = ( planedist - DotProduct( normal, thisedgept0 ) ) / DotProduct( normal, vec0 );
			
			iter->second = newt < 0.0f ? 0.0f : ( newt > 1.0f ? 1.0f : newt );
			thisnewpt = thisedgept0 * ( 1.0f - iter->second ) + thisedgept1 * iter->second;
			
			distance += Norm( thisnewpt - prevedgept );
		}
		
		distance += Norm( end - thisnewpt );
	}

}
