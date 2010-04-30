#ifndef SLIB_MATRIXIO_HPP
#define SLIB_MATRIXIO_HPP

#include <iostream>

#include <slib/Matrix.hpp>

namespace slib {

	template< int n, int m, class FieldT >
	std::ostream & operator<<( std::ostream & out, const Matrix< n, m, FieldT > & matrix ) {
		out << "[";
		for( int i = 0; i < n; i++ ) {
			if( i != 0 ) {
				out << std::endl;
			}
			for( int j = 0; j < m; j++ ) {
				out << ( j == 0 ? "" : ", " ) << matrix( i, j );
			}
		}
		return out << "]";
	}

	template< int n, class FieldT >
	std::ostream & operator<<( std::ostream & out, const Matrix< n, 1, FieldT > & matrix ) {
		out << "[";
		for( int i = 0; i < n; i++ ) {
			out << ( i == 0 ? "" : ", " ) << matrix[ i ];
		}
		return out << "]";
	}
	
}

#endif