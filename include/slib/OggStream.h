/*****************************
Author: Steve Lombardi
Description: class for streaming
ogg vorbis files from the harddrive

Requires: OpenAL
          libogg
		  libvorbis
*****************************/

#ifndef SLIB_OGGSTREAM_H
#define SLIB_OGGSTREAM_H

#include <string>

#include <al.h>
#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>

namespace slib {

	class OggStream {
	public:
		void uninit( );
		void open( const std::string & filename );
		void play( );
		void update( );
	private:
		void stream( ALuint buffer );

		OggVorbis_File ovFile;
		vorbis_info * vorbisInfo;
		vorbis_comment * vorbisComment;

		const static int bufferCount = 2;
		const static int bufferSize = 1024 * 64;

		ALuint buffers[ bufferCount ];
		ALuint source;
		ALenum format;
	};

}

#endif