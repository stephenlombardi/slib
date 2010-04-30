#ifndef SLIB_BONEMATRIXCONTAINER_H
#define SLIB_BONEMATRIXCONTAINER_H

#include <vector>

#include <slib/Matrix.hpp>

namespace slib {

	class BoneMatrixContainer {
	public:
		BoneMatrixContainer( );

		template< class BoneMatInputIterator, class ParentIndInputIterator >
		BoneMatrixContainer( BoneMatInputIterator bonematbegin, BoneMatInputIterator bonematend, ParentIndInputIterator parentindbegin, ParentIndInputIterator parentindend ) : bonematrices( bonematbegin, bonematend ), parentindices( parentindbegin, parentindend ) {
			finalmatrices.resize( bonematrices.size( ) );
		}
		template< class InputIterator >
		void Calculate( InputIterator begin, InputIterator end ) {
			InputIterator iter = begin;
			for( size_t i = 0; i < bonematrices.size( ); i++, ++iter ) {
				finalmatrices[ i ] = bonematrices[ i ] * (*iter);
				if( parentindices[ i ] > -1 ) {
					finalmatrices[ i ] = finalmatrices[ parentindices[ i ] ] * finalmatrices[ i ];
				}
			}
		}
		void Calculate( );
		const std::vector< mat4f > & GetMatrices( ) const;
	private:
		std::vector< mat4f > finalmatrices;
		std::vector< mat4f > bonematrices;
		std::vector< int > parentindices;
	};

}

#endif
