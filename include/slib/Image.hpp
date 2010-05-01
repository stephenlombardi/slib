#ifndef SLIB_IMAGE_HPP
#define SLIB_IMAGE_HPP

// todo: figure out issues that could be caused by overflowing small data type, like char
// todo: do something to normalize multiplication of data types not in the range of [0,1] (e.g. char types: 127 * 127 != 0.5 * 0.5
// todo: memory access patterns with performing operations only on single channels...

// todo : more helper functions

// arithmetic views
// more iterators..

// pixel copy, pixel iteration

#include <numeric>

namespace slib {

	struct Kernel {
		Kernel( float * _kernel, int _width, int _height ) : kernel( _kernel ), width( _width ), height( _height ) { }
		float * kernel;
		int width, height;
	};

	template< class T >
	class InterleavedView {
	public:
		InterleavedView( T * _data, int _width, int _height, int _channels ) : data( _data ), width( _width ), height( _height ), channels( _channels ) { }

		T& operator( )( int x, int y, int c ) {
			return data[ y * width * channels + x * channels + c ];
		}

		T operator( )( int x, int y, int c ) const {
			return data[ y * width * channels + x * channels + c ];
		}

		int getWidth( ) const { return width; }
		int getHeight( ) const { return height; }
		int getChannels( ) const { return channels; }

		class PixelIter {
		private:
			PixelIter( InterleavedView< T > _view, int _index ) : view( _view ), index( _index ) { }

		public:
			void incX( ) { index += channels; }
			void decX( ) { index -= channels; }
			void incY( ) { index += width * channels; }
			void decY( ) { index -= width * channels; }
			void incC( ) { index++; }
			void decC( ) { index--; }
			T& operator*( ) { return view.data[ index ]; }
			T operator*( ) const { return view.data[ index ]; }

		private:
			InterleavedView< T > view;
			int index;
		};

		PixelIter getIter( int x, int y, int c ) {
			return PixelIter( *this, y * width * channels + x * channels + c );
		}
	//private:
		int width, height, channels;
		T * data;
	};

	template< class T, class Iter >
	class ChannelIter {
	public:
		ChannelIter( Iter _iter ) : iter( _iter ) { }

		ChannelIter< T, Iter > operator++( int ) { iter.incC( ); return iter; }
		ChannelIter< T, Iter > operator--( int ) { iter.decC( ); return iter; }
		T& operator*( ) { return *iter; }
		T operator*( ) const { return *iter; }
	private:
		Iter iter;
	};

	template< template< class > class ViewT1, template< class > class ViewT2 >
	class BinaryComposer {
	public:
		template< class T >
		class AddView {
		public:
			AddView( const ViewT1< T > & _view1, const ViewT2< T > & _view2 )  : view1( _view1 ), view2( _view2 ) { }

			T operator( )( int x, int y, int c ) const {
				return view1( x, y, c % view1.getChannels( ) ) + view2( x, y, c % view2.getChannels( ) );
			}

			int getWidth( ) const { return view1.getWidth( ); }
			int getHeight( ) const { return view1.getHeight( ); }
			int getChannels( ) const { return std::max( view1.getChannels( ), view2.getChannels( ) ); }
		private:
			ViewT1< T > view1;
			ViewT2< T > view2;
		};

		template< class T >
		class SubtractView {
		public:
			SubtractView( const ViewT1< T > & _view1, const ViewT2< T > & _view2 )  : view1( _view1 ), view2( _view2 ) { }

			T operator( )( int x, int y, int c ) const {
				return view1( x, y, c % view1.getChannels( ) ) - view2( x, y, c % view2.getChannels( ) );
			}

			int getWidth( ) const { return view1.getWidth( ); }
			int getHeight( ) const { return view1.getHeight( ); }
			int getChannels( ) const { return std::max( view1.getChannels( ), view2.getChannels( ) ); }
		private:
			ViewT1< T > view1;
			ViewT2< T > view2;
		};

		template< class T >
		class MultiplyView {
		public:
			MultiplyView( const ViewT1< T > & _view1, const ViewT2< T > & _view2 )  : view1( _view1 ), view2( _view2 ) { }

			T operator( )( int x, int y, int c ) const {
				return view1( x, y, c % view1.getChannels( ) ) * view2( x, y, c % view2.getChannels( ) );
			}

			int getWidth( ) const { return view1.getWidth( ); }
			int getHeight( ) const { return view1.getHeight( ); }
			int getChannels( ) const { return std::max( view1.getChannels( ), view2.getChannels( ) ); }
		private:
			ViewT1< T > view1;
			ViewT2< T > view2;
		};
	};

	template< template< class > class ViewT >
	class Composer {
	public:
		template< class T >
		class AddView {
		public:
			AddView( const ViewT< T > & _view, T _addby ) : view( _view ), addby( _addby ) { }

			T operator( )( int x, int y, int c ) const {
				return view( x, y, c ) + addby;
			}

			int getWidth( ) const { return view.getWidth( ); }
			int getHeight( ) const { return view.getHeight( ); }
			int getChannels( ) const { return view.getChannels( ); }
		private:
			ViewT< T > view;
			T addby;
		};

		template< class T >
		class MultiplyView {
		public:
			MultiplyView( const ViewT< T > & _view, float _multiplyby ) : view( _view ), multiplyby( _multiplyby ) { }

			T operator( )( int x, int y, int c ) const {
				return view( x, y, c ) * multiplyby;
			}

			int getWidth( ) const { return view.getWidth( ); }
			int getHeight( ) const { return view.getHeight( ); }
			int getChannels( ) const { return view.getChannels( ); }
		private:
			ViewT< T > view;
			float multiplyby;
		};

		template< class T >
		class DivideView {
		public:
			DivideView( const ViewT< T > & _view, T _divideby ) : view( _view ), divideby( _divideby ) { }

			T operator( )( int x, int y, int c ) const {
				return view( x, y, c ) / divideby;
			}

			int getWidth( ) const { return view.getWidth( ); }
			int getHeight( ) const { return view.getHeight( ); }
			int getChannels( ) const { return view.getChannels( ); }
		private:
			ViewT< T > view;
			T divideby;
		};

		template< class T >
		class SingleChannelView {
		public:
			SingleChannelView( const ViewT< T > & _view, int _channel ) : view( _view ), channel( _channel ) { }

			T& operator( )( int x, int y, int c ) {
				return view( x, y, channel );
			}

			T operator( )( int x, int y, int c ) const {
				return view( x, y, channel );
			}

			int getWidth( ) const { return view.getWidth( ); }
			int getHeight( ) const { return view.getHeight( ); }
			int getChannels( ) const { return 1; }
		private:
			ViewT< T > view;
			int channel;
		};

		template< class T >
		class LuminosityView {
		public:
			LuminosityView( const ViewT< T > & _view ) : view( _view ) { }

			T operator( )( int x, int y, int c ) const {
				//return std::accumulate( ChannelIter( view.getIter( x, y, 0 ) ), ChannelIter( view.getIter( x, y, view.getChannels( ) ) ) ) / view.getChannels( );
				float sum = 0.0f;
				for( int i = 0; i < view.getChannels( ); i++ ) {
					sum += view( x, y, i );
				}
				return sum / view.getChannels( );
			}

			int getWidth( ) const { return view.getWidth( ); }
			int getHeight( ) const { return view.getHeight( ); }
			int getChannels( ) const { return view.getChannels( ); }
		private:
			ViewT< T > view;
		};

		template< class T >
		class MirrorXView {
		public:
			MirrorXView( const ViewT< T > & _view ) : view( _view ) { }

			T& operator( )( int x, int y, int c ) {
				return view( view.getWidth( ) - 1 - x, y, c );
			}

			T operator( )( int x, int y, int c ) const {
				return view( view.getWidth( ) - 1 - x, y, c );
			}

			int getWidth( ) const { return view.getWidth( ); }
			int getHeight( ) const { return view.getHeight( ); }
			int getChannels( ) const { return view.getChannels( ); }
		private:
			ViewT< T  > view;
		};

		template< class T >
		class SubimageView {
		public:
			SubimageView( const ViewT< T > & _view, int _startx, int _starty, int _width, int _height ) : view( _view ), startx( _startx ), starty( _starty ), width( _width ), height( _height ) { }

			T& operator( )( int x, int y, int c ) {
				return view( startx + x, starty + y, c );
			}

			T operator( )( int x, int y, int c ) const {
				return view( startx + x, starty + y, c );
			}

			int getWidth( ) const { return width; }
			int getHeight( ) const { return height; }
			int getChannels( ) const { return view.getChannels( ); }
		private:
			ViewT< T > view;
			int startx, starty, width, height;
		};

		template< class T >
		class SubsampleView {
		public:
			SubsampleView( const ViewT< T > & _view, int _factor ) : view( _view ), factor( _factor ) { }

			T operator( )( int x, int y, int c ) const {
				return view( x * factor, y * factor, c );
			}

			int getWidth( ) const { return view.getWidth( ) / factor; }
			int getHeight( ) const { return view.getHeight( ) / factor; }
			int getChannels( ) const { return view.getChannels( ); }
		private:
			ViewT< T > view;
			int factor;
		};

		template< class T >
		class GaussianSubsampleView {
		public:
			GaussianSubsampleView( const ViewT< T > & _view, int _factor ) : view( _view ), factor( _factor ) { }

			T operator( )( int x, int y, int c ) const {
				//return view( x / factor, y / factor, c );
				return view( factor * x - 1, factor * y - 1, c ) / 16 + view( factor * x, factor * y - 1, c ) / 8 + view( factor * x + 1, factor * y - 1, c ) / 16 +
					view( factor * x - 1, factor * y, c ) / 8 + view( factor * x, factor * y, c ) / 4 + view( factor * x + 1, factor * y, c ) / 8 +
					view( factor * x - 1, factor * y + 1, c ) / 16 + view( factor * x, factor * y + 1, c ) / 8 + view( factor * x + 1, factor * y + 1, c ) / 16;
			}

			int getWidth( ) const { return view.getWidth( ) / factor; }
			int getHeight( ) const { return view.getHeight( ) / factor; }
			int getChannels( ) const { return view.getChannels( ); }
		private:
			ViewT< T > view;
			int factor;
		};

		template< class T >
		class TransformView {
		public:
			// matrix is 3x3, row major
			TransformView( const ViewT< T > & _view, float * _matrix ) : view( _view ), matrix( _matrix ) { }

			T operator( )( int x, int y, int c ) const {
				float denom = matrix[ 6 ] * x + matrix[ 7 ] * y + matrix[ 8 ];
				return view( ( matrix[ 0 ] * x + matrix[ 1 ] * y + matrix[ 2 ] ) / denom, ( matrix[ 3 ] * x + matrix[ 4 ] * y + matrix[ 5 ] ) / denom, c );
			}

			int getWidth( ) const { return view.getWidth( ); }
			int getHeight( ) const { return view.getHeight( ); }
			int getChannels( ) const { return view.getChannels( ); }
		private:
			ViewT< T > view;
			float * matrix;
		};

		template< class T >
		class ClampedView {
		public:
			ClampedView( const ViewT< T > & _view ) : view( _view ) { }

			T operator( )( int x, int y, int c ) const {
				if( x < 0 ) {
					return 0;
				} else if( x >= view.getWidth( ) ) {
					return 0;
				} else if( y < 0 ) {
					return 0;
				} else if( y >= view.getHeight( ) ) {
					return 0;
				} else {
					return view( x, y, c );
				}
			}

			int getWidth( ) const { return view.getWidth( ); }
			int getHeight( ) const { return view.getHeight( ); }
			int getChannels( ) const { return view.getChannels( ); }
		private:
			ViewT< T > view;
		};

		template< class T >
		class MirrorEdgeView {
		public:
			MirrorEdgeView( const ViewT< T > & _view ) : view( _view ) { }

			T operator()( int x, int y, int c ) const {
				if( x < 0 ) {
					return 2 * (*this)( 0, y, c ) - (*this)( -x, y, c );
				} else if( x >= view.getWidth( ) ) {
					return 2 * (*this)( view.getWidth( ) - 1, y, c ) - (*this)( 2 * view.getWidth( ) - x - 2, y, c );
					//return 2 * (*this)( view.width( ) - 1, y, c ) - (*this)( view.width( ) - 1 - ( x - ( view.width( ) - 1 ) ), y, c );
				} else if( y < 0 ) {
					return 2 * (*this)( x, 0, c ) - (*this)( x, -y, c );
				} else if( y >= view.getHeight( ) ) {
					return 2 * (*this)( x, view.getHeight( ) - 1, c ) - (*this)( x, 2 * view.getHeight( ) - y - 2, c );
					//return 2 * (*this)( x, view.height( ) - 1, c ) - (*this)( x, view.height( ) - 1 - ( y - ( view.height( ) - 1 ) ), c );
				} else {
					return view( x, y, c );
				}
			}

			int getWidth( ) const { return view.getWidth( ); }
			int getHeight( ) const { return view.getHeight( ); }
			int getChannels( ) const { return view.getChannels( ); }
		private:
			ViewT< T > view;
		};

		template< class T >
		class ConvolveView {
		public:
			ConvolveView( const ViewT< T > & _view, float * _kernel, int _kwidth, int _kheight ) : view( _view ), kernel( _kernel ), kwidth( _kwidth ), kheight( _kheight ) { }

			T operator()( int x, int y, int c ) {
				// rewrite using itertors
				float sum = 0.0f;
				for( int i = 0, py = y - kheight; py <= y + kheight; py++ ) {
					for( int px = x - kwidth; px <= x + kwidth; px++, i++ ) {
						sum += view( px, py, c ) * kernel[ i ];
					}
				}
				return static_cast< T >( sum );
			}

			int getWidth( ) const { return view.getWidth( ); }
			int getHeight( ) const { return view.getHeight( ); }
			int getChannels( ) const { return view.getChannels( ); }
		private:
			ViewT< T > view;
			float * kernel;
			int kwidth, kheight;
		};

		template< class T >
		class CylindricalReprojectionView {
		public:
			CylindricalReprojectionView( const ViewT< T > & _view, int _focallength ) : view( _view ), focallength( _focallength ) { }

			T operator()( int x, int y, int c ) {
				float cylinderx = ( x - view.getWidth( ) / 2.0f ) / focallength, cylindery = ( y - view.getHeight( ) / 2.0f ) / focallength;
				float threedx = sin( cylinderx ), threedy = cylindery;
				float normalizedx = threedx / sqrt( 1.0f - threedx * threedx ), normalizedy = threedy / sqrt( 1.0f - threedx * threedx );
				float finalx = focallength * normalizedx + view.getWidth( ) / 2, finaly = focallength * normalizedy + view.getHeight( ) / 2;
				return view( (int)finalx, (int)finaly, c );
			}

			int getWidth( ) const { return view.getWidth( ); }
			int getHeight( ) const { return view.getHeight( ); }
			int getChannels( ) const { return view.getChannels( ); }
		private:
			ViewT< T > view;
			int focallength;
		}; 

		template< class T >
		class UndistortedView {
		public:
			UndistortedView( const ViewT< T > & _view, int _focallength, float _k1, float _k2 ) : view( _view ), focallength( _focallength ), k1( _k1 ), k2( _k2 ) { }

			T operator()( int x, int y, int c ) {
				float normalizedx = ( x - view.getWidth( ) / 2.0f ) / focallength, normalizedy = ( y - view.getHeight( ) / 2.0f ) / focallength;
				float r = sqrt( normalizedx * normalizedx + normalizedy * normalizedy );
				float radialdist = 1.0f + k1 * r * r + k2 * r * r * r * r;
				float undistortedx = normalizedx * radialdist, undistortedy = normalizedy * radialdist;
				float finalx = focallength * undistortedx + view.getWidth( ) / 2, finaly = focallength * undistortedy + view.getHeight( ) / 2;
				return view( (int)finalx, (int)finaly, c );
			}

			int getWidth( ) const { return view.getWidth( ); }
			int getHeight( ) const { return view.getHeight( ); }
			int getChannels( ) const { return view.getChannels( ); }
		private:
			ViewT< T > view;
			int focallength;
			float k1, k2;
		}; 

		template< class T >
		class DerivativeXView {
		public:
			DerivativeXView( const ViewT< T > & _view ) : view( _view ) { }

			T operator()( int x, int y, int c ) {
				// central differences operator
				return ( view( x + 1, y, c ) - view( x - 1, y, c ) ) / 2;
				//return view( x, y, c );
			}

			int getWidth( ) const { return view.getWidth( ); }
			int getHeight( ) const { return view.getHeight( ); }
			int getChannels( ) const { return view.getChannels( ); }
		private:
			ViewT< T > view;
		}; 

		template< class T >
		class DerivativeYView {
		public:
			DerivativeYView( const ViewT< T > & _view ) : view( _view ) { }

			T operator()( int x, int y, int c ) {
				// central differences operator
				return ( view( x, y + 1, c ) - view( x, y - 1, c ) ) / 2;
				//return view( x, y, c );
			}

			int getWidth( ) const { return view.getWidth( ); }
			int getHeight( ) const { return view.getHeight( ); }
			int getChannels( ) const { return view.getChannels( ); }
		private:
			ViewT< T > view;
		}; 
	};

	// helper functions
	template< class T >
	InterleavedView< T > Interleaved( T * data, int width, int height, int channels ) {
		return InterleavedView< T >( data, width, height, channels );
	}

	/*template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template ConvolveView< T > Convolve( ViewT< T > & view, float * kernel, int kwidth, int kheight ) {
		return typename Composer< ViewT >::template ConvolveView< T >( view, kernel, kwidth, kheight );
	}*/

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template ClampedView< T > Clamp( const ViewT< T > & view ) {
		return typename Composer< ViewT >::template ClampedView< T >( view );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template MirrorEdgeView< T > MirrorEdge( const ViewT< T > & view ) {
		return typename Composer< ViewT >::template MirrorEdgeView< T >( view );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template MirrorXView< T > MirrorX( const ViewT< T > & view ) {
		return typename Composer< ViewT >::template MirrorXView< T >( view );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template CylindricalReprojectionView< T > CylindricalReprojection( const ViewT< T > & view, int focallength ) {
		return typename Composer< ViewT >::template CylindricalReprojectionView< T >( view, focallength );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template UndistortedView< T > Undistort( const ViewT< T > & view, int focallength, float k1, float k2 ) {
		return typename Composer< ViewT >::template UndistortedView< T >( view, focallength, k1, k2 );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template GaussianSubsampleView< T > GaussianSubsample( const ViewT< T > & view, int factor ) {
		return typename Composer< ViewT >::template GaussianSubsampleView< T >( view, factor );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template SubsampleView< T > Subsample( const ViewT< T > & view, int factor ) {
		return typename Composer< ViewT >::template SubsampleView< T >( view, factor );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template SingleChannelView< T > SingleChannel( const ViewT< T > & view, int channel ) {
		return typename Composer< ViewT >::template SingleChannelView< T >( view, channel );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template LuminosityView< T > Luminosity( const ViewT< T > & view ) {
		return typename Composer< ViewT >::template LuminosityView< T >( view );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template SubimageView< T > Subimage( const ViewT< T > & view, int startx, int starty, int width, int height ) {
		return typename Composer< ViewT >::template SubimageView< T >( view, startx, starty, width, height );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template DerivativeXView< T > DerivativeX( const ViewT< T > & view ) {
		return typename Composer< ViewT >::template DerivativeXView< T >( view );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template DerivativeYView< T > DerivativeY( const ViewT< T > & view ) {
		return typename Composer< ViewT >::template DerivativeYView< T >( view );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template AddView< T > operator+( const ViewT< T > & view, T addby ) {
		return typename Composer< ViewT >::template AddView< T >( view, addby );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template AddView< T > operator+( T addby, const ViewT< T > & view ) {
		return typename Composer< ViewT >::template AddView< T >( view, addby );
	}

	template< template< class > class ViewT1, template< class > class ViewT2, class T >
	typename BinaryComposer< ViewT1, ViewT2 >::template AddView< T > operator+( const ViewT1< T > & view1, const ViewT2< T > & view2 ) {
		return typename BinaryComposer< ViewT1, ViewT2 >::template AddView< T >( view1, view2 );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template AddView< T > operator-( const ViewT< T > & view, T subby ) {
		return typename Composer< ViewT >::template AddView< T >( view, -subby );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template AddView< T > operator-( T subby, const ViewT< T > & view ) {
		return typename Composer< ViewT >::template AddView< T >( view, -subby );
	}

	template< template< class > class ViewT1, template< class > class ViewT2, class T >
	typename BinaryComposer< ViewT1, ViewT2 >::template SubtractView< T > operator-( const ViewT1< T > & view1, const ViewT2< T > & view2 ) {
		return typename BinaryComposer< ViewT1, ViewT2 >::template SubtractView< T >( view1, view2 );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template ConvolveView< T > operator*( const ViewT< T > & view, const Kernel & kernel ) {
		return typename Composer< ViewT >::template ConvolveView< T >( view, kernel.kernel, kernel.width, kernel.height );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template ConvolveView< T > operator*( const Kernel & kernel, const ViewT< T > & view ) {
		return typename Composer< ViewT >::template ConvolveView< T >( view, kernel.kernel, kernel.width, kernel.height );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template MultiplyView< T > operator*( const ViewT< T > & view, float multiplyby ) {
		return typename Composer< ViewT >::template MultiplyView< T >( view, multiplyby );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template MultiplyView< T > operator*( float multiplyby, const ViewT< T > & view ) {
		return typename Composer< ViewT >::template MultiplyView< T >( view, multiplyby );
	}

	template< template< class > class ViewT, class T >
	typename Composer< ViewT >::template DivideView< T > operator/( const ViewT< T > & view, T divideby ) {
		return typename Composer< ViewT >::template DivideView< T >( view, divideby );
	}

	template< template< class > class ViewT1, template< class > class ViewT2, class T >
	typename BinaryComposer< ViewT1, ViewT2 >::template MultiplyView< T > operator*( const ViewT1< T > & view1, const ViewT2< T > & view2 ) {
		return typename BinaryComposer< ViewT1, ViewT2 >::template MultiplyView< T >( view1, view2 );
	}

	// 

	template< class OutT, class InT >
	class Convert {
	public:
		static OutT convert( InT val ) { }
	};

	template<>
	class Convert< float, unsigned char > {
	public:
		static float convert( unsigned char val ) { return val / 255.0f; }
	};

	template<>
	class Convert< unsigned char, float > {
	public:
		static unsigned char convert( float val ) { return static_cast< unsigned char >( val * 255 ); }
	};

	template< template< class > class ViewT, class InT >
	class Cast {
	public:
		template< class OutT >
		class CastView {
		public:
			CastView( const ViewT< InT > & _view ) : view( _view ) { }

			OutT operator()( int x, int y, int c ) {
				return Convert< OutT, InT >::convert( view( x, y, c ) );
			}
		private:
			ViewT< InT > view;
		};
	};

}

#endif
