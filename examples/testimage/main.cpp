#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>

#include <slib/Image.hpp>
#include <slib/TGA.h>

using namespace slib;

template< template< class > class ViewInT, template< class > class ViewOutT, class T >
void copypixels( ViewInT< T > & in, ViewOutT< T > & out ) {
	int width = in.getWidth( ), height = in.getHeight( ), channels = in.getChannels( );
	for( int y = 0; y < height; y++ ) {
		for( int x = 0; x < width; x++ ) {
			for( int c = 0; c < channels; c++ ) {
				out( x, y, c ) = in( x, y, c );
			}
		}
	}
}

void gaussianKernel( float * kernel, int width, float sigma ) {
	float invdenom = 1.0f / ( sigma * sqrt( 2.0f * 3.1415926f ) );
	for( int x = 0; x < width; x++ ) {
		int offset = x - width / 2;
		kernel[ x ] = invdenom * exp( -offset * offset / ( 2.0f * sigma * sigma ) );
	}
}

// mosaicing todo:
// optical flow
// pyramid blending

template< class T >
std::list< InterleavedView< T > > MakeGaussianPyramid( InterleavedView< T > original, int levels ) {
	std::list< InterleavedView< T > > p;
	p.push_back( original );
	for( int i = 1; i < levels; i++ ) {
		T * newlevel = new T[ ( p.back( ).getWidth( ) / 2 ) * ( p.back( ).getHeight( ) / 2 ) * p.back( ).getChannels( ) ];
		copypixels( GaussianSubsample( Clamp( p.back( ) ), 2 ), 
					Interleaved( newlevel, p.back( ).getWidth( ) / 2, p.back( ).getHeight( ) / 2, p.back( ).getChannels( ) ) );
		p.push_back( Interleaved( newlevel, p.back( ).getWidth( ) / 2, p.back( ).getHeight( ) / 2, p.back( ).getChannels( ) ) );
	}
	return p;
}

////////////////

template< template< class > class ViewInT, class T >
float getImage( ViewInT< T > & in, float x, float y, int c ) {
	int ix = (int)x, iy = (int)y;
	float ax = x - floor( x ), ay = y - floor( y );
	return in( ix, iy, c ) * ( 1.0f - ax ) * ( 1.0f - ay ) +
			in( ix + 1, iy, c ) * ax * ( 1.0f - ay ) +
			in( ix, iy + 1, c ) * ay * ( 1.0f - ax ) +
			in( ix + 1, iy + 1, c ) * ax * ay;
}

// optical flow

template< template< class > class PT, class T >
void gradientMatrix( PT< T > & image, float px, float py, int wx, int wy, float ox, float oy, float & gxx, float & gxy, float & gyy ) {
	gxx = gxy = gyy = 0.0f;

	//for( int y = py - wy; y <= py + wy; y++ ) {
	for( float y = py - wy; y <= py + wy; y += 1.0f ) {
	//for( int y = ( py - wy < 0 ? 0 : py - wy ); y <= ( py + wy >= image.getHeight( ) ? image.getHeight( ) - 1 : py + wy ); y++ ) {
		//for( int x = px - wx; x <= px + wx; x++ ) {
		for( float x = px - wx; x <= px + wx; x += 1.0f ) {
		//for( int x = ( px - wx < 0 ? 0 : px - wx ); x <= ( px + wx >= image.getWidth( ) ? image.getWidth( ) - 1 : px + wx ); x++ ) {
			for( int c = 0; c < image.getChannels( ); c++ ) {
				if( x >= 0 && x < image.getWidth( ) && y >= 0 && y < image.getHeight( ) && 
					( x + ox ) >= 0 && ( x + ox ) < image.getWidth( ) && ( y + oy ) >= 0 && ( y + oy ) <= image.getHeight( ) ) {
					//float ix = ( (float)image( x + 1, y, c ) - (float)image( x - 1, y, c ) ) * 0.5f;
					float ix = ( (float)getImage( image, x + 1, y, c ) - (float)getImage( image, x - 1, y, c ) ) * 0.5f;
					//float ix = ( (float)image( x + 1, y - 1, c ) - (float)image( x - 1, y - 1, c ) ) * 0.25f +
					//			( (float)image( x + 1, y, c ) - (float)image( x - 1, y, c ) ) * 0.5f +
					//			( (float)image( x + 1, y + 1, c ) - (float)image( x - 1, y + 1, c ) ) * 0.25f;
					//float iy = ( (float)image( x, y + 1, c ) - (float)image( x, y - 1, c ) ) * 0.5f;
					float iy = ( (float)getImage( image, x, y + 1, c ) - (float)getImage( image, x, y - 1, c ) ) * 0.5f;
					//float iy = ( (float)image( x - 1, y + 1, c ) - (float)image( x - 1, y - 1, c ) ) * 0.25f +
					//			( (float)image( x, y + 1, c ) - (float)image( x, y - 1, c ) ) * 0.5f +
					//			( (float)image( x + 1, y + 1, c ) - (float)image( x + 1, y - 1, c ) ) * 0.25f;
					
					/// weight sum by distance to center
					float xx = x - px, yy = y - py;
					float sigma = 6.0f;
					//float g = exp( -0.5f * ( ( xx * xx ) / ( sigma * sigma ) + ( yy * yy ) / ( sigma * sigma ) ) );
					//float g = ( 1.0f / ( 2.0f * 3.1415926f * sigma * sigma ) ) * exp( -0.5f * ( ( xx * xx ) / ( sigma * sigma ) + ( yy * yy ) / ( sigma * sigma ) ) );
					float g = 1.0f;
					
					gxx += ix * ix * g * g;
					gxy += ix * iy * g * g;
					gyy += iy * iy * g * g;
				}
			}
		}
	}
}

template< template< class > class PT1, template< class > class PT2, class T >
void mismatchVector( PT1< T > & image1, PT2< T > & image2, float px, float py, int wx, int wy, float ox, float oy, float & bx, float & by ) {
	bx = by = 0.0f;

	//for( int y = py - wy; y <= py + wy; y++ ) {
	for( float y = py - wy; y <= py + wy; y += 1.0f ) {
	//for( int y = ( py - wy < 0 ? 0 : py - wy ); y <= ( py + wy >= image1.getHeight( ) ? image1.getHeight( ) - 1 : py + wy ); y++ ) {
		//for( int x = px - wx; x <= px + wx; x++ ) {
		for( float x = px - wx; x <= px + wx; x += 1.0f ) {
		//for( int x = ( px - wx < 0 ? 0 : px - wx ); x <= ( px + wx >= image1.getWidth( ) ? image1.getWidth( ) - 1 : px + wx ); x++ ) {
			for( int c = 0; c < image1.getChannels( ); c++ ) {
				if( x >= 0 && x < image1.getWidth( ) && y >= 0 && y < image1.getHeight( ) && 
					( x + ox ) >= 0 && ( x + ox ) < image1.getWidth( ) && ( y + oy ) >= 0 && ( y + oy ) < image1.getHeight( ) ) {
					//float it = ( (float)image1( x, y, c ) - (float)image2( x + ox, y + oy, c ) );
					float it = ( (float)getImage( image1, x, y, c ) - (float)getImage( image2, x + ox, y + oy, c ) );
					//float it = ( (float)image1( x, y, c ) - (float)image2( x + ox, y + oy, c ) ) / 5.0f +
					//			( (float)image1( x + 1, y, c ) - (float)image2( x + ox + 1, y + oy, c ) ) / 5.0f +
					//			( (float)image1( x - 1, y, c ) - (float)image2( x + ox - 1, y + oy, c ) ) / 5.0f +
					//			( (float)image1( x, y + 1, c ) - (float)image2( x + ox, y + oy + 1, c ) ) / 5.0f +
					//			( (float)image1( x, y - 1, c ) - (float)image2( x + ox, y + oy - 1, c ) ) / 5.0f;
					//float ix = ( (float)image1( x + 1, y, c ) - (float)image1( x - 1, y ,c ) ) * 0.5f;
					float ix = ( (float)getImage( image1, x + 1, y, c ) - (float)getImage( image1, x - 1, y ,c ) ) * 0.5f;
					//float ix = ( (float)image1( x + 1, y - 1, c ) - (float)image1( x - 1, y - 1, c ) ) * 0.25f +
					//			( (float)image1( x + 1, y, c ) - (float)image1( x - 1, y, c ) ) * 0.5f +
					//			( (float)image1( x + 1, y + 1, c ) - (float)image1( x - 1, y + 1, c ) ) * 0.25f;
					//float iy = ( (float)image1( x, y + 1, c ) - (float)image1( x, y - 1 ,c ) ) * 0.5f;
					float iy = ( (float)getImage( image1, x, y + 1, c ) - (float)getImage( image1, x, y - 1 ,c ) ) * 0.5f;
					//float iy = ( (float)image1( x - 1, y + 1, c ) - (float)image1( x - 1, y - 1, c ) ) * 0.25f +
					//			( (float)image1( x, y + 1, c ) - (float)image1( x, y - 1, c ) ) * 0.5f +
					//			( (float)image1( x + 1, y + 1, c ) - (float)image1( x + 1, y - 1, c ) ) * 0.25f;
					
					/// weight sum by distance to center
					float xx = x - px, yy = y - py;
					float sigma = 6.0f;
					//float g = exp( -0.5f * ( ( xx * xx ) / ( sigma * sigma ) + ( yy * yy ) / ( sigma * sigma ) ) );
					//float g = ( 1.0f / ( 2.0f * 3.1415926f * sigma * sigma ) ) * exp( -0.5f * ( ( xx * xx ) / ( sigma * sigma ) + ( yy * yy ) / ( sigma * sigma ) ) );
					float g = 1.0f;

					bx += it * ix * g * g;
					by += it * iy * g * g;
				}
			}
		}
	}
}

template< template< class > class PT1, class T >
void opticalFlow( std::list< PT1< T > > & pyramid1, std::list< PT1< T > > & pyramid2, float ux, float uy, int wx, int wy, int iterations, float & dx, float & dy ) {
	float guessx = 0.0f, guessy = 0.0f;

	int l = pyramid1.size( ) - 1;
	std::list< PT1< T > >::reverse_iterator piter1 = pyramid1.rbegin( );
	std::list< PT1< T > >::reverse_iterator piter2 = pyramid2.rbegin( );
	for( ; piter1 != pyramid1.rend( ); piter1++, piter2++, l-- ) {
		float px = ux / pow( 2.0, l ), py = uy / pow( 2.0, l );

		float gxx = 0.0f, gxy = 0.0f, gyy = 0.0f;
		//gradientMatrix( *piter1, px, py, wx, wy, gxx, gxy, gyy );

		float vx = 0.0f, vy = 0.0f;

		for( int k = 0; k < iterations; k++ ) {
			float bx = 0.0f, by = 0.0f;

			gradientMatrix( *piter1, px, py, wx, wy, guessx + vx, guessy + vy, gxx, gxy, gyy );

			mismatchVector( *piter1, *piter2, px, py, wx, wy, guessx + vx, guessy + vy, bx, by );

			float det = gxx * gyy - gxy * gxy;
			float etax = ( gyy * bx - gxy * by ) / det, etay = ( -gxy * bx + gxx * by ) / det;

			vx += etax; vy += etay;

		}

		guessx = 2.0f * ( guessx + vx );
		guessy = 2.0f * ( guessy + vy );

		std::cout << "guess: " << guessx << " " << guessy << std::endl;
	}

	// temp
	float error = 0.0f, error2 = 0.0f, pixels = 0.0f, close = 0.0f;
	for( float y = uy - wy; y <= uy + wy; y += 1.0f ) {
		for( float x = ux - wx; x <= ux + wx; x += 1.0f ) {
			for( int c = 0; c < pyramid1.front( ).getChannels( ); c++ ) {
				if( x >= 0 && x < pyramid1.front( ).getWidth( ) && y >= 0 && y < pyramid1.front( ).getHeight( ) && 
					( x + guessx / 2.0f ) >= 0 && ( x + guessx / 2.0f ) < pyramid1.front( ).getWidth( ) && 
					( y + guessy / 2.0f ) >= 0 && ( y + guessy / 2.0f ) < pyramid1.front( ).getHeight( ) ) {

					error += abs( getImage( pyramid1.front( ), x, y, c ) - getImage( pyramid2.front( ), x + guessx / 2.0f, y + guessy / 2.0f, c ) );
					error2 += pow( getImage( pyramid1.front( ), x, y, c ) - getImage( pyramid2.front( ), x + guessx / 2.0f, y + guessy / 2.0f, c ), 2.0f );
					close += abs( getImage( pyramid1.front( ), x, y, c ) - getImage( pyramid2.front( ), x + guessx / 2.0f, y + guessy / 2.0f, c ) ) < 16.0f ? 1.0f : 0.0f;
					pixels += 1.0f;
				}
			}
		}
	}
	std::cout << "error/pixel: " << ( error / pixels ) << " pixels: " << pixels << " close: " << ( close / pixels ) << " error^2/pixel: " << ( error2 / pixels ) << std::endl;

	dx = guessx / 2.0f;
	dy = guessy / 2.0f;
}

template< template< class > class ViewT1, template< class > class ViewT2, class T2 > 
void GradientMatrix( ViewT1< unsigned char > & view1, ViewT2< T2 > & view2 ) {
	for( int y = 0; y < view1.getHeight( ); y++ ) {
		for( int x = 0; x < view1.getWidth( ); x++ ) {
			char ix = view1( x + 1, y, 0 ) - view1( x - 1, y, 0 );
			char iy = view1( x, y + 1, 0 ) - view1( x, y - 1, 0 );
			view2( x, y, 0 ) = ix * ix / 128;
			view2( x, y, 1 ) = ix * iy / 128;
			view2( x, y, 2 ) = iy * iy / 128;
		}
	}
}

int main( ) {
	srand( time( 0 ) );

	// load image 1
	std::ifstream tgafilein( "DSC00097.tga", std::ios::binary );
	int width1, height1, depth1;
	unsigned char * data1 = ReadTGA( tgafilein, width1, height1, depth1 );
	tgafilein.close( );

	// load image 2
	tgafilein.open( "DSC00098.tga", std::ios::binary );
	int width2, height2, depth2;
	unsigned char * data2 = ReadTGA( tgafilein, width2, height2, depth2 );
	tgafilein.close( );

	//int startTime = timeGetTime( );

	//unsigned char * gradient = new unsigned char[ width1 * height1 * 2 ];

	//unsigned char * dataout1 = new unsigned char[ width1 * height1 * depth1 / 8 ];
	//copypixels( CylindricalReprojection( Undistort( Clamp( Interleaved( data1, width1, height1, depth1 / 8 ) ), 717, -0.06462f, -0.38987f ), 717 ), Interleaved( dataout1, width1, height1, depth1 / 8 ) );
	unsigned char * dataout1 = new unsigned char[ width1 * height1 * depth1 / 8 ];
	GradientMatrix( Luminosity( Clamp( Interleaved( data1, width1, height1, depth1 / 8 ) ) ), Interleaved( dataout1, width1, height1, 3 ) );
	copypixels( (unsigned char)127 + Interleaved( dataout1, width1, height1, 3 ), Interleaved( dataout1, width1, height1, 3 ) );
	//copypixels( (unsigned char)127 + DerivativeX( Luminosity( Clamp( Interleaved( data1, width1, height1, depth1 / 8 ) ) ) ), SingleChannel( Interleaved( dataout1, width1, height1, 3 ), 0 ) );
	//copypixels( (unsigned char)127 + DerivativeY( Luminosity( Clamp( Interleaved( data1, width1, height1, depth1 / 8 ) ) ) ), SingleChannel( Interleaved( dataout1, width1, height1, 3 ), 1 ) );
	//copypixels( Luminosity( Clamp( Interleaved( data1, width1, height1, depth1 / 8 ) ) ), SingleChannel( Interleaved( dataout1, width1, height1, 3 ), 2 ) );

	//unsigned char * dataout2 = new unsigned char[ width2 * height2 * depth2 / 8 ];
	//copypixels( CylindricalReprojection( Undistort( Clamp( Interleaved( data2, width2, height2, depth2 / 8 ) ), 717, -0.06462f, -0.38987f ), 717 ), Interleaved( dataout2, width2, height2, depth2 / 8 ) );

	//std::list< InterleavedView< unsigned char > > pyramid1 = MakeGaussianPyramid( Interleaved( dataout1, width1, height1, depth1 / 8 ), 7 );
	//std::list< InterleavedView< unsigned char > > pyramid2 = MakeGaussianPyramid( Interleaved( dataout2, width2, height2, depth2 / 8 ), 7 );

	std::ofstream tgafileout( "out1.tga", std::ios::binary );
	TGA::SetData( tgafileout, width1, height1, depth1, dataout1 );
	tgafileout.close( );

	//tgafileout.open( "out2.tga", std::ios::binary );
	//TGA::SetData( tgafileout, width2, height2, depth2, dataout2 );
	//tgafileout.close( );

	//std::list< Composer< InterleavedView >::ClampedView< unsigned char > > pyramid1c;
	//std::list< Composer< InterleavedView >::ClampedView< unsigned char > > pyramid2c;

	//for( std::list< InterleavedView< unsigned char > >::iterator iter1 = pyramid1.begin( ), iter2 = pyramid2.begin( ); iter1 != pyramid1.end( ); iter1++, iter2++ ) {
	//	pyramid1c.push_back( Clamp( *iter1 ) );
	//	pyramid2c.push_back( Clamp( *iter2 ) );
	//}

	//float dx = 0.0f, dy = 0.0f;
	//opticalFlow( pyramid1c, pyramid2c, width1 / 2, height1 / 2, width1 / 2, height1 / 2, 10, dx, dy );

	//std::cout << dx << " " << dy << std::endl;

	//int endTime = timeGetTime( );

	//std::cout << ( endTime - startTime ) << std::endl;

	//std::cin.get( );

	return 0;
}