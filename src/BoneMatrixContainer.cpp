#include <slib/BoneMatrixContainer.h>

namespace slib {

	BoneMatrixContainer::BoneMatrixContainer( ) {
	}

	void BoneMatrixContainer::Calculate( ) {
		for( size_t i = 0; i < bonematrices.size( ); i++ ) {
			finalmatrices[ i ] = bonematrices[ i ];
			if( parentindices[ i ] > -1 ) {
				finalmatrices[ i ] = finalmatrices[ parentindices[ i ] ] * finalmatrices[ i ];
			}
		}
	}

	const std::vector< mat4f > & BoneMatrixContainer::GetMatrices( ) const {
		return finalmatrices;
	}

}
