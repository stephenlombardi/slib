/***********************************
Author: Steve Lombardi
Description: Matrix library

Matrix dimensions are template 
parameters and are therefore static
***********************************/

#ifndef SLIB_MATRIX_HPP
#define SLIB_MATRIX_HPP

#include <cmath>

namespace slib {

	// Field stuff

	template< class T >
	struct AdditiveIdentity {
	};

	template< >
	struct AdditiveIdentity< float > {
		static float Identity( ) { return 0.0f; }
	};

	template< >
	struct AdditiveIdentity< double > {
		static double Identity( ) { return 0.0; }
	};

	template< class T >
	struct MultiplicitiveIdentity {
	};

	template< >
	struct MultiplicitiveIdentity< float > {
		static float Identity( ) { return 1.0f; }
	};

	template< >
	struct MultiplicitiveIdentity< double > {
		static double Identity( ) { return 1.0; }
	};

	// Basic Matrix Container

	template< int _rows, int _cols, class _FieldT = float >
	class Matrix {
	public:
		// constructors
		Matrix( ) { }
		explicit Matrix( const _FieldT & init ) {
			for( int i = 0; i < _rows * _cols; i++ ) {
				this->data[ i ] = init;
			}
		}
		template< class InputIterator >
		Matrix( const InputIterator & begin, const InputIterator & end ) {
			int i = 0;
			for( InputIterator iter = begin; iter != end; ++iter, ++i ) {
				this->data[ i ] = *iter;
			}
		}
		Matrix( const Matrix< _rows, _cols, _FieldT > & rhs ) {
			for( int i = 0; i < _rows * _cols; i++ ) {
				this->data[ i ] = rhs.data[ i ];
			}
		}
		template< template< int, int, class > class RHS >
		explicit Matrix( const RHS< _rows, _cols, _FieldT > & rhs ) {
			for( int i = 0; i < _rows; i++ ) {
				for( int j = 0; j < _cols; j++ ) {
					(*this)( i, j ) = rhs( i, j );
				}
			}
		}
		template< template< int, int, class > class RHS >
		Matrix< _rows, _cols, _FieldT > & operator=( const RHS< _rows, _cols, _FieldT > & rhs ) {
			for( int i = 0; i < _rows; i++ ) {
				for( int j = 0; j < _cols; j++ ) {
					(*this)( i, j ) = rhs( i, j );
				}
			}
			return *this;
		}

		// accessors
		_FieldT & operator( )( int row, int col ) {
			return this->data[ row * _cols + col ];
		}
		const _FieldT & operator( )( int row, int col ) const {
			return this->data[ row * _cols + col ];
		}

		typedef _FieldT FieldT;
		const static int rows = _rows, cols = _cols;
	private:
		_FieldT data[ _rows * _cols ];
	};

	typedef Matrix< 2, 2, float > mat2f;
	typedef Matrix< 3, 3, float > mat3f;
	typedef Matrix< 4, 4, float > mat4f;

	// vector specialization
	template< int _rows, class _FieldT >
	class Matrix< _rows, 1, _FieldT > {
	public:
		Matrix( ) { }
		explicit Matrix( const _FieldT & init ) {
			for( int i = 0; i < _rows; i++ ) {
				this->data[ i ] = init;
			}
		}
		template< class InputIterator >
		Matrix( const InputIterator & begin, const InputIterator & end ) {
			int i = 0;
			for( InputIterator iter = begin; iter != end; ++iter, ++i ) {
				this->data[ i ] = *iter;
			}
		}
		Matrix( const Matrix< _rows, 1, _FieldT > & rhs ) {
			for( int i = 0; i < _rows; i++ ) {
				this->data[ i ] = rhs.data[ i ];
			}
		}
		template< template< int, int, class > class RHS >
		Matrix( const RHS< _rows, 1, _FieldT > & rhs ) {
			for( int i = 0; i < _rows; i++ ) {
				(*this)( i, 0 ) = rhs( i, 0 );
			}
		}
		template< template< int, int, class > class RHS >
		Matrix< _rows, 1, _FieldT > & operator=( const RHS< _rows, 1, _FieldT > & rhs ) {
			for( int i = 0; i < _rows; i++ ) {
				(*this)( i, 0 ) = rhs( i, 0 );
			}
			return *this;
		}

		// accessors
		_FieldT & operator( )( int row, int col ) {
			return this->data[ row ];
		}
		const _FieldT & operator( )( int row, int col ) const {
			return this->data[ row ];
		}

		_FieldT & operator[ ]( int row ) {
			return this->data[ row ];
		}
		const _FieldT & operator[ ]( int row ) const {
			return this->data[ row ];
		}

		_FieldT * c_vec( ) {
			return this->data;
		}
		const _FieldT * c_vec( ) const {
			return this->data;
		}

		typedef _FieldT FieldT;
		const static int rows = _rows, cols = 1, dimensions = _rows;
	private:
		_FieldT data[ _rows ];
	};

	typedef Matrix< 2, 1, float > vec2f;
	typedef Matrix< 3, 1, float > vec3f;
	typedef Matrix< 4, 1, float > vec4f;

	/// Submatrix

	template< template< int, int, class > class T, int _subrows, int _subcols >
	class Sub_t {
	public:
		template< int _rows, int _cols, class _FieldT >
		class Matrix {
		public:
			explicit Matrix( T< _subrows, _subcols, _FieldT > & _t, int _startrow = 0, int _startcol = 0 ) : t( _t ), startrow( _startrow ), startcol( _startcol ) { }
			template< template< int, int, class > class RHS >
			Matrix< _rows, _cols, _FieldT > & operator=( const RHS< _rows, _cols, _FieldT > & rhs ) {
				for( int i = 0; i < _rows; i++ ) {
					for( int j = 0; j < _cols; j++ ) {
						(*this)( i, j ) = rhs( i, j );
					}
				}
				return *this;
			}
			Matrix< _rows, _cols, _FieldT > & operator=( const typename Sub_t< T, _subrows, _subcols >::template Matrix< _rows, _cols, _FieldT > & rhs ) {
				for( int i = 0; i < _rows; i++ ) {
					for( int j = 0; j < _cols; j++ ) {
						(*this)( i, j ) = rhs( i, j );
					}
				}
				return *this;
			}
			_FieldT & operator( )( int row, int col ) {
				return t( row + startrow, col + startcol );
			}
			const _FieldT & operator( )( int row, int col ) const {
				return t( row + startrow, col + startcol );
			}
		private:
			T< _subrows, _subcols, _FieldT > & t;
			int startrow, startcol;
		};

		template< int _rows, class _FieldT >
		class Matrix< _rows, 1, _FieldT > {
		public:
			explicit Matrix( T< _subrows, _subcols, _FieldT > & _t, int _startrow = 0, int _startcol = 0 ) : t( _t ), startrow( _startrow ), startcol( _startcol ) { }
			template< template< int, int, class > class RHS >
			Matrix< _rows, 1, _FieldT > & operator=( const RHS< _rows, 1, _FieldT > & rhs ) {
				for( int i = 0; i < _rows; i++ ) {
					(*this)( i, 0 ) = rhs( i, 0 );
				}
				return *this;
			}
			Matrix< _rows, 1, _FieldT > & operator=( const typename Sub_t< T, _subrows, _subcols >::template Matrix< _rows, 1, _FieldT > & rhs ) {
				for( int i = 0; i < _rows; i++ ) {
					(*this)( i, 0 ) = rhs( i, 0 );
				}
				return *this;
			}
			_FieldT & operator( )( int row, int col ) {
				return t( row + startrow, startcol );
			}
			const _FieldT & operator( )( int row, int col ) const {
				return t( row + startrow, startcol );
			}
			_FieldT & operator[ ]( int row ) {
				return t( row + startrow, startcol );
			}
			const _FieldT & operator[ ]( int row ) const {
				return t( row + startrow, startcol );
			}
		private:
			T< _subrows, _subcols, _FieldT > & t;
			int startrow, startcol;
		};

		template< int _rows, int _cols, class _FieldT >
		class MatrixConst {
		public:
			MatrixConst( const T< _subrows, _subcols, _FieldT > & _t, int _startrow = 0, int _startcol = 0 ) : t( _t ), startrow( _startrow ), startcol( _startcol ) { }
			const _FieldT & operator( )( int row, int col ) const {
				return t( row + startrow, col + startcol );
			}
		private:
			const T< _subrows, _subcols, _FieldT > & t;
			int startrow, startcol;
		};

		template< int _rows, class _FieldT >
		class MatrixConst< _rows, 1, _FieldT > {
		public:
			MatrixConst( const T< _subrows, _subcols, _FieldT > & _t, int _startrow = 0, int _startcol = 0 ) : t( _t ), startrow( _startrow ), startcol( _startcol ) { }
			const _FieldT & operator( )( int row, int col ) const {
				return t( row + startrow, startcol );
			}
			const _FieldT & operator[ ]( int row ) const {
				return t( row + startrow, startcol );
			}
		private:
			const T< _subrows, _subcols, _FieldT > & t;
			int startrow, startcol;
		};

		template< int _rows, int _cols, class _FieldT >
		class MatrixVal {
		public:
			explicit MatrixVal( const T< _subrows, _subcols, _FieldT > & _t, int _startrow = 0, int _startcol = 0 ) : t( _t ), startrow( _startrow ), startcol( _startcol ) { }
			template< template< int, int, class > class RHS >
			MatrixVal< _rows, _cols, _FieldT > & operator=( const RHS< _rows, _cols, _FieldT > & rhs ) {
				for( int i = 0; i < _rows; i++ ) {
					for( int j = 0; j < _cols; j++ ) {
						(*this)( i, j ) = rhs( i, j );
					}
				}
				return *this;
			}
			MatrixVal< _rows, _cols, _FieldT > & operator=( const typename Sub_t< T, _subrows, _subcols >::template MatrixVal< _rows, _cols, _FieldT > & rhs ) {
				for( int i = 0; i < _rows; i++ ) {
					for( int j = 0; j < _cols; j++ ) {
						(*this)( i, j ) = rhs( i, j );
					}
				}
				return *this;
			}
			_FieldT & operator( )( int row, int col ) {
				return t( row + startrow, col + startcol );
			}
			const _FieldT & operator( )( int row, int col ) const {
				return t( row + startrow, col + startcol );
			}
		private:
			T< _subrows, _subcols, _FieldT > t;
			int startrow, startcol;
		};

		template< int _rows, class _FieldT >
		class MatrixVal< _rows, 1, _FieldT > {
		public:
			explicit MatrixVal( const T< _subrows, _subcols, _FieldT > & _t, int _startrow = 0, int _startcol = 0 ) : t( _t ), startrow( _startrow ), startcol( _startcol ) { }
			template< template< int, int, class > class RHS >
			MatrixVal< _rows, 1, _FieldT > & operator=( const RHS< _rows, 1, _FieldT > & rhs ) {
				for( int i = 0; i < _rows; i++ ) {
					(*this)( i, 0 ) = rhs( i, 0 );
				}
				return *this;
			}
			MatrixVal< _rows, 1, _FieldT > & operator=( const typename Sub_t< T, _subrows, _subcols >::template MatrixVal< _rows, 1, _FieldT > & rhs ) {
				for( int i = 0; i < _rows; i++ ) {
					(*this)( i, 0 ) = rhs( i, 0 );
				}
				return *this;
			}
			_FieldT & operator( )( int row, int col ) {
				return t( row + startrow, startcol );
			}
			const _FieldT & operator( )( int row, int col ) const {
				return t( row + startrow, startcol );
			}
			_FieldT & operator[ ]( int row ) {
				return t( row + startrow, startcol );
			}
			const _FieldT & operator[ ]( int row ) const {
				return t( row + startrow, startcol );
			}
		private:
			T< _subrows, _subcols, _FieldT > t;
			int startrow, startcol;
		};
	};

	template< int _rows, int _cols >
	class Sub {
	public:
		template< template< int, int, class > class T, int _subrows, int _subcols, class _FieldT >
		static typename Sub_t< T, _subrows, _subcols >::template Matrix< _rows, _cols, _FieldT > Matrix( T< _subrows, _subcols, _FieldT > & t, int startrow = 0, int startcol = 0 ) {
			return typename Sub_t< T, _subrows, _subcols >::template Matrix< _rows, _cols, _FieldT >( t, startrow, startcol );
		}

		template< template< int, int, class > class T, int _subrows, int _subcols, class _FieldT >
		static typename Sub_t< T, _subrows, _subcols >::template MatrixConst< _rows, _cols, _FieldT > Matrix( const T< _subrows, _subcols, _FieldT > & t, int startrow = 0, int startcol = 0 ) {
			return typename Sub_t< T, _subrows, _subcols >::template MatrixConst< _rows, _cols, _FieldT >( t, startrow, startcol );
		}

		template< template< int, int, class > class T, int _subrows, int _subcols, class _FieldT >
		static typename Sub_t< T, _subrows, _subcols >::template MatrixVal< _rows, _cols, _FieldT > MatrixVal( const T< _subrows, _subcols, _FieldT > & t, int startrow = 0, int startcol = 0 ) {
			return typename Sub_t< T, _subrows, _subcols >::template MatrixVal< _rows, _cols, _FieldT >( t, startrow, startcol );
		}
	};

	////// Transpose

	template< template< int, int, class > class T >
	class Trans {
	public:
		template< int _rows, int _cols, class _FieldT >
		class Pose {
		public:
			Pose( T< _cols, _rows, _FieldT > & _t ) : t( _t ) { }
			template< template< int, int, class > class RHS >
			Pose< _rows, _cols, _FieldT > & operator=( const RHS< _rows, _cols, _FieldT > & rhs ) {
				for( int i = 0; i < _rows; i++ ) {
					for( int j = 0; j < _cols; j++ ) {
						(*this)( i, j ) = rhs( i, j );
					}
				}
				return *this;
			}
			_FieldT & operator( )( int row, int col ) {
				return t( col, row );
			}
			const _FieldT & operator( )( int row, int col ) const {
				return t( col, row );
			}
		private:
			T< _cols, _rows, _FieldT > & t;
		};

		template< int _rows, class _FieldT >
		class Pose< _rows, 1, _FieldT > {
		public:
			Pose( T< 1, _rows, _FieldT > & _t ) : t( _t ) { }
			template< template< int, int, class > class RHS >
			Pose< _rows, 1, _FieldT > & operator=( const RHS< _rows, 1, _FieldT > & rhs ) {
				for( int i = 0; i < _rows; i++ ) {
					(*this)( i, 0 ) = rhs( i, 0 );
				}
				return *this;
			}
			_FieldT & operator( )( int row, int col ) {
				return t( col, row );
			}
			const _FieldT & operator( )( int row, int col ) const {
				return t( col, row );
			}
			_FieldT & operator[ ]( int row ) {
				return t( 0, row );
			}
			const _FieldT & operator[ ]( int row ) const {
				return t( 0, row );
			}
		private:
			T< 1, _rows, _FieldT > & t;
		};

		template< int _rows, int _cols, class _FieldT >
		class PoseConst {
		public:
			PoseConst( const T< _cols, _rows, _FieldT > & _t ) : t( _t ) { }
			const _FieldT & operator( )( int row, int col ) const {
				return t( col, row );
			}
		private:
			const T< _cols, _rows, _FieldT > & t;
		};

		template< int _rows, class _FieldT >
		class PoseConst< _rows, 1, _FieldT > {
		public:
			PoseConst( const T< 1, _rows, _FieldT > & _t ) : t( _t ) { }
			const _FieldT & operator( )( int row, int col ) const {
				return t( col, row );
			}
			const _FieldT & operator[ ]( int row ) const {
				return t( 0, row );
			}
		private:
			const T< 1, _rows, _FieldT > & t;
		};

		template< int _rows, int _cols, class _FieldT >
		class PoseVal {
		public:
			PoseVal( const T< _cols, _rows, _FieldT > & _t ) : t( _t ) { }
			template< template< int, int, class > class RHS >
			PoseVal< _rows, _cols, _FieldT > & operator=( const RHS< _rows, _cols, _FieldT > & rhs ) {
				for( int i = 0; i < _rows; i++ ) {
					for( int j = 0; j < _cols; j++ ) {
						(*this)( i, j ) = rhs( i, j );
					}
				}
				return *this;
			}
			_FieldT & operator( )( int row, int col ) {
				return t( col, row );
			}
			const _FieldT & operator( )( int row, int col ) const {
				return t( col, row );
			}
		private:
			T< _cols, _rows, _FieldT > t;
		};

		template< int _rows, class _FieldT >
		class PoseVal< _rows, 1, _FieldT > {
		public:
			PoseVal( const T< 1, _rows, _FieldT > & _t ) : t( _t ) { }
			template< template< int, int, class > class RHS >
			PoseVal< _rows, 1, _FieldT > & operator=( const RHS< _rows, 1, _FieldT > & rhs ) {
				for( int i = 0; i < _rows; i++ ) {
					(*this)( i, 0 ) = rhs( i, 0 );
				}
				return *this;
			}
			_FieldT & operator( )( int row, int col ) {
				return t( col, row );
			}
			const _FieldT & operator( )( int row, int col ) const {
				return t( col, row );
			}
			_FieldT & operator[ ]( int row ) {
				return t( 0, row );
			}
			const _FieldT & operator[ ]( int row ) const {
				return t( 0, row );
			}
		private:
			T< 1, _rows, _FieldT > t;
		};
	};

	template< template< int, int, class > class T, int _rows, int _cols, class _FieldT >
	typename Trans< T >::template Pose< _rows, _cols, _FieldT > Transpose( T< _cols, _rows, _FieldT > & t ) {
		return typename Trans< T >::template Pose< _rows, _cols, _FieldT >( t );
	}

	template< template< int, int, class > class T, int _rows, int _cols, class _FieldT >
	typename Trans< T >::template PoseConst< _rows, _cols, _FieldT > Transpose( const T< _cols, _rows, _FieldT > & t ) {
		return typename Trans< T >::template PoseConst< _rows, _cols, _FieldT >( t );
	}

	template< template< int, int, class > class T, int _rows, int _cols, class _FieldT >
	typename Trans< T >::template PoseVal< _rows, _cols, _FieldT > TransposeVal( const T< _cols, _rows, _FieldT > & t ) {
		return typename Trans< T >::template PoseVal< _rows, _cols, _FieldT >( t );
	}

	///////////// Row Major wrapper

	template< int _rows, int _cols, class _FieldT >
	class RowMajor_t {
	public:
		RowMajor_t( _FieldT * _data ) : data( _data ) { }
		template< template< int, int, class > class RHS >
		RowMajor_t< _rows, _cols, _FieldT > & operator=( const RHS< _rows, _cols, _FieldT > & rhs ) {
			for( int i = 0; i < _rows; i++ ) {
				for( int j = 0; j < _cols; j++ ) {
					(*this)( i, j ) = rhs( i, j );
				}
			}
			return *this;
		}
		_FieldT & operator( )( int row, int col ) {
			return data[ row * _cols + col ];
		}
		const _FieldT & operator( )( int row, int col ) const {
			return data[ row * _cols + col ];
		}
	private:
		_FieldT * data;
	};

	template< int _rows, class _FieldT >
	class RowMajor_t< _rows, 1, _FieldT > {
	public:
		RowMajor_t( _FieldT * _data ) : data( _data ) { }
		template< template< int, int, class > class RHS >
		RowMajor_t< _rows, 1, _FieldT > & operator=( const RHS< _rows, 1, _FieldT > & rhs ) {
			for( int i = 0; i < _rows; i++ ) {
				(*this)( i, 0 ) = rhs( i, 0 );
			}
			return *this;
		}
		_FieldT & operator( )( int row, int col ) {
			return data[ row ];
		}
		const _FieldT & operator( )( int row, int col ) const {
			return data[ row ];
		}
		_FieldT & operator[ ]( int row ) {
			return data[ row ];
		}
		const _FieldT & operator[ ]( int row ) const {
			return data[ row ];
		}
	private:
		_FieldT * data;
	};

	template< int _rows, int _cols, class _FieldT >
	class RowMajorConst_t {
	public:
		RowMajorConst_t( const _FieldT * _data ) : data( _data ) { }
		const _FieldT & operator( )( int row, int col ) const {
			return data[ row * _cols + col ];
		}
	private:
		const _FieldT * data;
	};

	template< int _rows, class _FieldT >
	class RowMajorConst_t< _rows, 1, _FieldT > {
	public:
		RowMajorConst_t( const _FieldT * _data ) : data( _data ) { }
		const _FieldT & operator( )( int row, int col ) const {
			return data[ row ];
		}
		const _FieldT & operator[ ]( int row ) const {
			return data[ row ];
		}
	private:
		const _FieldT * data;
	};

	template< int _rows, int _cols, class _FieldT >
	RowMajor_t< _rows, _cols, _FieldT > RowMajor( _FieldT * data ) {
		return RowMajor_t< _rows, _cols, _FieldT >( data );
	}

	template< int _rows, int _cols, class _FieldT >
	RowMajorConst_t< _rows, _cols, _FieldT > RowMajor( const _FieldT * data ) {
		return RowMajorConst_t< _rows, _cols, _FieldT >( data );
	}

	//////////// Column major wrapper

	template< int _rows, int _cols, class _FieldT >
	class ColMajor_t {
	public:
		ColMajor_t( _FieldT * _data ) : data( _data ) { }
		template< template< int, int, class > class RHS >
		ColMajor_t< _rows, _cols, _FieldT > & operator=( const RHS< _rows, _cols, _FieldT > & rhs ) {
			for( int i = 0; i < _rows; i++ ) {
				for( int j = 0; j < _cols; j++ ) {
					(*this)( i, j ) = rhs( i, j );
				}
			}
			return *this;
		}
		_FieldT & operator( )( int row, int col ) {
			return data[ col * _rows + row ];
		}
		const _FieldT & operator( )( int row, int col ) const {
			return data[ col * _rows + row ];
		}
	private:
		_FieldT * data;
	};

	template< int _rows, class _FieldT >
	class ColMajor_t< _rows, 1, _FieldT > {
	public:
		ColMajor_t( _FieldT * _data ) : data( _data ) { }
		template< template< int, int, class > class RHS >
		ColMajor_t< _rows, 1, _FieldT > & operator=( const RHS< _rows, 1, _FieldT > & rhs ) {
			for( int i = 0; i < _rows; i++ ) {
				(*this)( i, 0 ) = rhs( i, 0 );
			}
			return *this;
		}
		_FieldT & operator( )( int row, int col ) {
			return data[ row ];
		}
		const _FieldT & operator( )( int row, int col ) const {
			return data[ row ];
		}
		_FieldT & operator[ ]( int row ) {
			return data[ row ];
		}
		const _FieldT & operator[ ]( int row ) const {
			return data[ row ];
		}
	private:
		_FieldT * data;
	};

	template< int _rows, int _cols, class _FieldT >
	class ColMajorConst_t {
	public:
		ColMajorConst_t( const _FieldT * _data ) : data( _data ) { }
		const _FieldT & operator( )( int row, int col ) const {
			return data[ col * _rows + row ];
		}
	private:
		const _FieldT * data;
	};

	template< int _rows, class _FieldT >
	class ColMajorConst_t< _rows, 1, _FieldT > {
	public:
		ColMajorConst_t( const _FieldT * _data ) : data( _data ) { }
		const _FieldT & operator( )( int row, int col ) const {
			return data[ row ];
		}
		const _FieldT & operator[ ]( int row ) const {
			return data[ row ];
		}
	private:
		const _FieldT * data;
	};

	template< int _rows, int _cols, class _FieldT >
	ColMajor_t< _rows, _cols, _FieldT > ColMajor( _FieldT * data ) {
		return ColMajor_t< _rows, _cols, _FieldT >( data );
	}

	template< int _rows, int _cols, class _FieldT >
	ColMajorConst_t< _rows, _cols, _FieldT > ColMajor( const _FieldT * data ) {
		return ColMajorConst_t< _rows, _cols, _FieldT >( data );
	}

	/////////////// Vector helper function

	template< int _rows >
	class Vec {
	public:
		template< class _FieldT >
		static RowMajor_t< _rows, 1, _FieldT > Tor( _FieldT * data ) {
			return RowMajor_t< _rows, 1, _FieldT >( data );
		}

		template< class _FieldT >
		static RowMajorConst_t< _rows, 1, _FieldT > Tor( const _FieldT * data ) {
			return RowMajorConst_t< _rows, 1, _FieldT >( data );
		}
	};


	//////////////

	template< int n, class _FieldT >
	struct MultiplicitiveIdentity< Matrix< n, n, _FieldT > >  {
		static Matrix< n, n, _FieldT > Identity( ) {
			Matrix< n, n, _FieldT > result;
			for( int i = 0; i < n; i++ ) {
				for( int j = 0; j < n; j++ ) {
					result( i, j ) = ( i == j ? MultiplicitiveIdentity< _FieldT >::Identity( ) : AdditiveIdentity< _FieldT >::Identity( ) );
				}
			}
			return result;
		}
	};

	template< int _rows, int _cols, class _FieldT >
	struct AdditiveIdentity< Matrix< _rows, _cols, _FieldT > >  {
		static Matrix< _rows, _cols, _FieldT > Identity( ) {
			return Matrix< _rows, _cols, _FieldT >( AdditiveIdentity< _FieldT >::Identity( ) );
		}
	};

	// Arithmetic
	template< template< int, int, class > class LHS, template< int, int, class > class RHS, int _rows, int _cols, class _FieldT >
	Matrix< _rows, _cols, _FieldT > operator+( const LHS< _rows, _cols, _FieldT > & lhs, const RHS< _rows, _cols, _FieldT > & rhs ) {
		Matrix< _rows, _cols, _FieldT > result;
		for( int i = 0; i < _rows; i++ ) {
			for( int j = 0; j < _cols; j++ ) {
				result( i, j ) = lhs( i, j ) + rhs( i, j );
			}
		}
		return result;
	}

	template< template< int, int, class > class LHS, template< int, int, class > class RHS, int _rows, int _cols, class _FieldT >
	LHS< _rows, _cols, _FieldT > & operator+=( LHS< _rows, _cols, _FieldT > & lhs, const RHS< _rows, _cols, _FieldT > & rhs ) {
		for( int i = 0; i < _rows; i++ ) {
			for( int j = 0; j < _cols; j++ ) {
				lhs( i, j ) += rhs( i, j );
			}
		}
		return lhs;
	}

	template< template< int, int, class > class LHS, template< int, int, class > class RHS, int _rows, int _cols, class _FieldT >
	Matrix< _rows, _cols, _FieldT > operator-( const LHS< _rows, _cols, _FieldT > & lhs, const RHS< _rows, _cols, _FieldT > & rhs ) {
		Matrix< _rows, _cols, _FieldT > result;
		for( int i = 0; i < _rows; i++ ) {
			for( int j = 0; j < _cols; j++ ) {
				result( i, j ) = lhs( i, j ) - rhs( i, j );
			}
		}
		return result;
	}

	template< template< int, int, class > class LHS, template< int, int, class > class RHS, int _rows, int _cols, class _FieldT >
	LHS< _rows, _cols, _FieldT > & operator-=( LHS< _rows, _cols, _FieldT > & lhs, const RHS< _rows, _cols, _FieldT > & rhs ) {
		for( int i = 0; i < _rows; i++ ) {
			for( int j = 0; j < _cols; j++ ) {
				lhs( i, j ) -= rhs( i, j );
			}
		}
		return lhs;
	}

	template< template< int, int, class > class RHS, int _rows, int _cols, class _FieldT >
	Matrix< _rows, _cols, _FieldT > operator-( const RHS< _rows, _cols, _FieldT > & rhs ) {
		Matrix< _rows, _cols, _FieldT > result;
		for( int i = 0; i < _rows; i++ ) {
			for( int j = 0; j < _cols; j++ ) {
				result( i, j ) = -rhs( i, j );
			}
		}
		return result;
	}

	template< template< int, int, class > class LHS, int _rows, int _cols, class _FieldT >
	Matrix< _rows, _cols, _FieldT > operator*( const LHS< _rows, _cols, _FieldT > & lhs, const _FieldT & rhs ) {
		Matrix< _rows, _cols, _FieldT > result;
		for( int i = 0; i < _rows; i++ ) {
			for( int j = 0; j < _cols; j++ ) {
				result( i, j ) = lhs( i, j ) * rhs;
			}
		}
		return result;
	}

	template< template< int, int, class > class RHS, int _rows, int _cols, class _FieldT >
	Matrix< _rows, _cols, _FieldT > operator*( const _FieldT & lhs, const RHS< _rows, _cols, _FieldT > & rhs ) {
		Matrix< _rows, _cols, _FieldT > result;
		for( int i = 0; i < _rows; i++ ) {
			for( int j = 0; j < _cols; j++ ) {
				result( i, j ) = lhs * rhs( i, j );
			}
		}
		return result;
	}

	template< template< int, int, class > class LHS, int _rows, int _cols, class _FieldT >
	LHS< _rows, _cols, _FieldT > & operator*=( LHS< _rows, _cols, _FieldT > & lhs, const _FieldT & rhs ) {
		for( int i = 0; i < _rows; i++ ) {
			for( int j = 0; j < _cols; j++ ) {
				lhs( i, j ) *= rhs;
			}
		}
		return lhs;
	}

	template< template< int, int, class > class LHS, template< int, int, class > class RHS, int lhsrows, int lhscols, int rhscols, class _FieldT >
	Matrix< lhsrows, rhscols, _FieldT > operator*( const LHS< lhsrows, lhscols, _FieldT > & lhs, const RHS< lhscols, rhscols, _FieldT > & rhs ) {
		Matrix< lhsrows, rhscols, _FieldT > result;
		for( int i = 0; i < lhsrows; i++ ) {
			for( int j = 0; j < rhscols; j++ ) {
				_FieldT sum = lhs( i, 0 ) * rhs( 0, j );
				for( int k = 1; k < lhscols; k++ ) {
					sum += lhs( i, k ) * rhs( k, j );
				}
				result( i, j ) = sum;
			}
		}
		return result;
	}

	template< template< int, int, class > class LHS, template< int, int, class > class RHS, int _rows, int _cols, class _FieldT >
	LHS< _rows, _cols, _FieldT > & operator*=( LHS< _rows, _cols, _FieldT > & lhs, const RHS< _rows, _cols, _FieldT > & rhs ) {
		return lhs = lhs * rhs;
	}

	template< int n, class _FieldT >
	Matrix< n, 1, _FieldT > Solve( const Matrix< n, n, _FieldT > & lhs, const Matrix< n, 1, _FieldT > & rhs ) {
		Matrix< n, n, _FieldT > A( lhs );
		Matrix< n, 1, _FieldT > b( rhs );

		for( int i = 0; i < n; i++ ) {

			int max = i;
			for( int j = i + 1; j < n; j++ ) {
				if( std::abs( A( j, i ) ) > std::abs( A( max, i ) ) ) {
					max = j;
				}
			}

			for( int j = 0; j < n; j++ ) {
				_FieldT temp = A( i, j );
				A( i, j ) = A( max, j );
				A( max, j ) = temp;
			}
			for( int j = 0; j < 1; j++ ) {
				_FieldT temp = b( i, j );
				b( i, j ) = b( max, j );
				b( max, j ) = temp;
			}

			if( A( i, i ) == 0 ) {
				return Matrix< n, 1, _FieldT >( 0 );
			}

			for( int j = i + 1; j < n; j++ ) {
				b( j, 0 ) -= b( i, 0 ) * A( j, i ) / A( i, i );
			}

			for( int j = i + 1; j < n; j++ ) {
				_FieldT m = A( j, i ) / A( i, i );
				for( int k = i + 1; k < n; k++ ) {
					A( j, k ) -= A( i, k ) * m;
				}
				A( j, i ) = 0;
			}
		}

		// back substitution
		Matrix< n, 1, _FieldT > x;
		for( int j = n - 1; j >= 0; j-- ) {
			_FieldT t = 0;
			for( int k = j + 1; k < n; k++ ) {
				t += A( j, k ) * x( k, 0 );
			}
			x( j, 0 ) = ( b( j, 0 ) - t ) / A( j, j );
		}

		return x;
	}

	// Matrix 3x3 functions

	template< class _FieldT >
	Matrix< 3, 3, _FieldT > RotationMatrixX( const _FieldT & angle ) {
		_FieldT cosangle = std::cos( angle ), sinangle = std::sin( angle );
		Matrix< 3, 3, _FieldT > result;
		result( 0, 0 ) = MultiplicitiveIdentity< _FieldT >::Identity( ); result( 0, 1 ) = AdditiveIdentity< _FieldT >::Identity( ); result( 0, 2 ) = AdditiveIdentity< _FieldT >::Identity( );
		result( 1, 0 ) = AdditiveIdentity< _FieldT >::Identity( ); result( 1, 1 ) = cosangle; result( 1, 2 ) = -sinangle;
		result( 2, 0 ) = AdditiveIdentity< _FieldT >::Identity( ); result( 2, 1 ) = sinangle; result( 2, 2 ) = cosangle;
		return result;
	}

	template< class _FieldT >
	Matrix< 3, 3, _FieldT > RotationMatrixY( const _FieldT & angle ) {
		_FieldT cosangle = std::cos( angle ), sinangle = std::sin( angle );
		Matrix< 3, 3, _FieldT > result;
		result( 0, 0 ) = cosangle; result( 0, 1 ) = AdditiveIdentity< _FieldT >::Identity( ); result( 0, 2 ) = sinangle;
		result( 1, 0 ) = AdditiveIdentity< _FieldT >::Identity( ); result( 1, 1 ) = MultiplicitiveIdentity< _FieldT >::Identity( ); result( 1, 2 ) = AdditiveIdentity< _FieldT >::Identity( );
		result( 2, 0 ) = -sinangle; result( 2, 1 ) = AdditiveIdentity< _FieldT >::Identity( ); result( 2, 2 ) = cosangle;
		return result;
	}

	template< class _FieldT >
	Matrix< 3, 3, _FieldT > RotationMatrixZ( const _FieldT & angle ) {
		_FieldT cosangle = std::cos( angle ), sinangle = std::sin( angle );
		Matrix< 3, 3, _FieldT > result;
		result( 0, 0 ) = cosangle; result( 0, 1 ) = -sinangle; result( 0, 2 ) = AdditiveIdentity< _FieldT >::Identity( );
		result( 1, 0 ) = sinangle; result( 1, 1 ) = cosangle; result( 1, 2 ) = AdditiveIdentity< _FieldT >::Identity( );
		result( 2, 0 ) = AdditiveIdentity< _FieldT >::Identity( ); result( 2, 1 ) = AdditiveIdentity< _FieldT >::Identity( ); result( 2, 2 ) = MultiplicitiveIdentity< _FieldT >::Identity( );
		return result;
	}

	template< template< int, int, class > class RHS, class _FieldT >
	_FieldT Determinant( const RHS< 3, 3, _FieldT > & mat ) {
		return DotProduct( Sub< 3, 1 >::Matrix( mat ), CrossProduct( Sub< 3, 1 >::Matrix( mat, 1 ), Sub< 3, 1 >::Matrix( mat, 2 ) ) );
	}

	// Matrix 2x2

	template< template< int, int, class > class RHS, class _FieldT >
	_FieldT Determinant( const RHS< 2, 2, _FieldT > & mat ) {
		return mat( 0, 0 ) * mat( 1, 1 ) - mat( 0, 1 ) * mat( 1, 0 );
	}

	// Vector functions
	template< template< int, int, class > class LHS, template< int, int, class > class RHS, int _rows, class _FieldT >
	_FieldT DotProduct( const LHS< _rows, 1, _FieldT > & lhs, const RHS< _rows, 1, _FieldT > & rhs ) {
		_FieldT sum = lhs[ 0 ] * rhs[ 0 ];
		for( int i = 1; i < _rows; i++ ) {
			sum += lhs[ i ] * rhs[ i ];
		}
		return sum;
	}

	template< int N, class T >
	T DotProduct( const T * lhs, const T * rhs ) {
		T sum = lhs[ 0 ] * rhs[ 0 ];
		for( int i = 1; i < N; i++ ) {
			sum += lhs[ i ] * rhs[ i ];
		}
		return sum;
	}

	template< template< int, int, class > class RHS, int _rows, class _FieldT >
	_FieldT NormSquared( const RHS< _rows, 1, _FieldT > & v ) {
		_FieldT sum = v[ 0 ] * v[ 0 ];
		for( int i = 1; i < _rows; i++ ) {
			sum += v[ i ] * v[ i ];
		}
		return sum;
	}

	template< int N, class T >
	T NormSquared( const T * rhs ) {
		T sum = rhs[ 0 ] * rhs[ 0 ];
		for( int i = 1; i < N; i++ ) {
			sum += rhs[ i ] * rhs[ i ];
		}
		return sum;
	}

	template< template< int, int, class > class RHS, int _rows, class _FieldT >
	_FieldT Norm( const RHS< _rows, 1, _FieldT > & v ) {
		return std::sqrt( NormSquared( v ) );
	}

	template< int N, class T >
	T Norm( const T * rhs ) {
		return std::sqrt( NormSquared< N, T >( rhs ) );
	}

	template< template< int, int, class > class RHS, int _rows, class _FieldT >
	Matrix< _rows, 1, _FieldT > Normalize( const RHS< _rows, 1, _FieldT > & v ) {
		return v * ( MultiplicitiveIdentity< _FieldT >::Identity( ) / Norm( v ) );
	}

	template< int N, class _FieldT >
	Matrix< N, 1, _FieldT > Normalize( const _FieldT * rhs ) {
		return Matrix< N, 1, _FieldT >( Vec< N >::Tor( rhs ) ) * ( MultiplicitiveIdentity< _FieldT >::Identity( ) / Norm( rhs ) );
	}

	// Vector4 functions
	template< class _FieldT >
	Matrix< 4, 1, _FieldT > Vector4( const _FieldT & x0, const _FieldT & x1, const _FieldT & x2, const _FieldT & x3 ) {
		Matrix< 4, 1, _FieldT > result;
		result[ 0 ] = x0; result[ 1 ] = x1; result[ 2 ] = x2; result[ 3 ] = x3;
		return result;
	}

	// Vector3 functions
	template< class _FieldT >
	Matrix< 3, 1, _FieldT > Vector3( const _FieldT & x0, const _FieldT & x1, const _FieldT & x2 ) {
		Matrix< 3, 1, _FieldT > result;
		result[ 0 ] = x0; result[ 1 ] = x1; result[ 2 ] = x2;
		return result;
	}

	template< class _FieldT >
	Matrix< 3, 1, _FieldT > Vector3N( const _FieldT & x0, const _FieldT & x1, const _FieldT & x2 ) {
		Matrix< 3, 1, _FieldT > result;
		result[ 0 ] = x0; result[ 1 ] = x1; result[ 2 ] = x2;
		return Normalize( result );
	}

	template< template< int, int, class > class LHS, template< int, int, class > class RHS, class _FieldT >
	Matrix< 3, 1, _FieldT > CrossProduct( const LHS< 3, 1, _FieldT > & lhs, const RHS< 3, 1, _FieldT > & rhs ) {
		return Vector3( lhs[ 1 ] * rhs[ 2 ] - lhs[ 2 ] * rhs[ 1 ], lhs[ 2 ] * rhs[ 0 ] - lhs[ 0 ] * rhs[ 2 ], lhs[ 0 ] * rhs[ 1 ] - lhs[ 1 ] * rhs[ 0 ] );
	}

	template< class _FieldT >
	Matrix< 3, 1, _FieldT > CrossProduct( const _FieldT * lhs, const _FieldT * rhs ) {
		return Vector3( lhs[ 1 ] * rhs[ 2 ] - lhs[ 2 ] * rhs[ 1 ], lhs[ 2 ] * rhs[ 0 ] - lhs[ 0 ] * rhs[ 2 ], lhs[ 0 ] * rhs[ 1 ] - lhs[ 1 ] * rhs[ 0 ] );
	}

	template< template< int, int, class > class LHS, template< int, int, class > class RHS, class _FieldT >
	Matrix< 3, 1, _FieldT > Rotate( const LHS< 3, 1, _FieldT > & vec, const RHS< 3, 1, _FieldT > & axis, const _FieldT & angle ) {
		_FieldT c = std::cos( angle ), s = std::sin( angle ), t = 1 - c;
		return Vector3( ( t * axis[ 0 ] * axis[ 0 ] + c ) * vec[ 0 ] + ( t * axis[ 0 ] * axis[ 1 ] - s * axis[ 2 ] ) * vec[ 1 ] + ( t * axis[ 0 ] * axis[ 2 ] + s * axis[ 1 ] ) * vec[ 2 ],
					( t * axis[ 0 ] * axis[ 1 ] + s * axis[ 2 ] ) * vec[ 0 ] + ( t * axis[ 1 ] * axis[ 1 ] + c ) * vec[ 1 ] + ( t * axis[ 1 ] * axis[ 2 ] - s * axis[ 0 ] ) * vec[ 2 ],
					( t * axis[ 0 ] * axis[ 2 ] - s * axis[ 1 ] ) * vec[ 0 ] + ( t * axis[ 1 ] * axis[ 2 ] + s * axis[ 0 ] ) * vec[ 1 ] + ( t * axis[ 2 ] * axis[ 2 ] + c ) * vec[ 2 ] );
	}

	template< template< int, int, class > class LHS, template< int, int, class > class RHS, class _FieldT >
	Matrix< 3, 1, _FieldT > Rotate( const _FieldT * vec, const _FieldT * axis, const _FieldT & angle ) {
		_FieldT c = std::cos( angle ), s = std::sin( angle ), t = 1 - c;
		return Vector3( ( t * axis[ 0 ] * axis[ 0 ] + c ) * vec[ 0 ] + ( t * axis[ 0 ] * axis[ 1 ] - s * axis[ 2 ] ) * vec[ 1 ] + ( t * axis[ 0 ] * axis[ 2 ] + s * axis[ 1 ] ) * vec[ 2 ],
					( t * axis[ 0 ] * axis[ 1 ] + s * axis[ 2 ] ) * vec[ 0 ] + ( t * axis[ 1 ] * axis[ 1 ] + c ) * vec[ 1 ] + ( t * axis[ 1 ] * axis[ 2 ] - s * axis[ 0 ] ) * vec[ 2 ],
					( t * axis[ 0 ] * axis[ 2 ] - s * axis[ 1 ] ) * vec[ 0 ] + ( t * axis[ 1 ] * axis[ 2 ] + s * axis[ 0 ] ) * vec[ 1 ] + ( t * axis[ 2 ] * axis[ 2 ] + c ) * vec[ 2 ] );
	}

	// vector2

	template< class _FieldT >
	Matrix< 2, 1, _FieldT > Vector2( const _FieldT & x0, const _FieldT & x1 ) {
		Matrix< 2, 1, _FieldT > result;
		result[ 0 ] = x0; result[ 1 ] = x1;
		return result;
	}

	template< class _FieldT >
	Matrix< 2, 1, _FieldT > Vector2N( const _FieldT & x0, const _FieldT & x1 ) {
		Matrix< 2, 1, _FieldT > result;
		result[ 0 ] = x0; result[ 1 ] = x1;
		return Normalize( result );
	}

	template< template< int, int, class > class LHS, class _FieldT >
	Matrix< 2, 1, _FieldT > Rotate( const LHS< 2, 1, _FieldT > & vec, const _FieldT & angle ) {
		return Vector2( vec[ 0 ] * std::cos( angle ) - vec[ 1 ] * std::sin( angle ), vec[ 0 ] * std::sin( angle ) + vec[ 1 ] * std::cos( angle ) );
	}
	
}

#endif
