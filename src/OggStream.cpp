#include <slib/OggStream.h>

namespace slib {

	void OggStream::uninit( ) {
		alSourceStop( source );

		int queued;
		alGetSourcei( source, AL_BUFFERS_QUEUED, &queued );

		while( queued-- ) {
			ALuint buffer;

			alSourceUnqueueBuffers( source, 1, &buffer );
		}

		alDeleteSources( 1, &source );
		alDeleteBuffers( 2, buffers );

		ov_clear( &ovFile );
	}

	void OggStream::open( const std::string & filename ) {
		char * cfilename = new char[ filename.size( ) + 1 ];
		for( int i = 0; cfilename[ i ] = filename.c_str( )[ i ]; i++ );
		ov_fopen( cfilename, &ovFile );
		delete [] cfilename;

		vorbisInfo = ov_info( &ovFile, -1 );
		vorbisComment = ov_comment( &ovFile, -1 );

		if( vorbisInfo->channels == 1 ) {
			format = AL_FORMAT_MONO16;
		} else {
			format = AL_FORMAT_STEREO16;
		}

		alGenBuffers( 2, buffers );
		alGenSources( 1, &source );

		alSource3f( source, AL_POSITION, 0.0f, 0.0f, 0.0f );
		alSource3f( source, AL_VELOCITY, 0.0f, 0.0f, 0.0f );
		alSource3f( source, AL_DIRECTION, 0.0f, 0.0f, 0.0f );
		alSourcef( source, AL_ROLLOFF_FACTOR, 0.0f );
		alSourcei( source, AL_SOURCE_RELATIVE, AL_TRUE );
	}

	void OggStream::play( ) {
		stream( buffers[ 0 ] );
		stream( buffers[ 1 ] );

		alSourceQueueBuffers( source, 2, buffers );
		alSourcePlay( source );
	}

	void OggStream::stream( ALuint buffer ) {
		char bufferdata[ bufferSize ];
		int size = 0;

		while( size < bufferSize ) {
			int section;
			int result = ov_read( &ovFile, bufferdata + size, bufferSize - size, 0, 2, 1, &section );

			if( result > 0 ) {
				size += result;
			} else if( result == 0 ) {
				ov_raw_seek( &ovFile, 0 );
			} else {			
				break;
			}
		}

		alBufferData( buffer, format, bufferdata, size, vorbisInfo->rate );
	}

	void OggStream::update( ) {
		int processed;
		alGetSourcei( source, AL_BUFFERS_PROCESSED, &processed );

		while( processed-- ) {
			ALuint buffer;

			alSourceUnqueueBuffers( source, 1, &buffer );
			stream( buffer );
			alSourceQueueBuffers( source, 1, &buffer );
		}

		ALenum state;
		alGetSourcei( source, AL_SOURCE_STATE, &state );
		if( state != AL_PLAYING ) {
			alSourcePlay( source );
		}
	}

}
