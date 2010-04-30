/****************************************************
* Author: Steve Lombardi
* Date: January, 2010
* Description: Extension helper file
* How to use:
*    call
*
*       initExtensions( );
*
*    to set up the extension functions
****************************************************/
#ifndef SLIB_EXTENSIONS_H
#define SLIB_EXTENSIONS_H

#include <gl/freeglut.h>
#include <slib/glext.h>

namespace slib {

	extern PFNGLCREATESHADERPROC glCreateShader;
	extern PFNGLSHADERSOURCEPROC glShaderSource;
	extern PFNGLCOMPILESHADERPROC glCompileShader;
	extern PFNGLDETACHSHADERPROC glDetachShader;
	extern PFNGLDELETESHADERPROC glDeleteShader;
	extern PFNGLCREATEPROGRAMPROC glCreateProgram;
	extern PFNGLATTACHSHADERPROC glAttachShader;
	extern PFNGLLINKPROGRAMPROC glLinkProgram;
	extern PFNGLUSEPROGRAMPROC glUseProgram;
	extern PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
	extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
	extern PFNGLUNIFORM1IPROC glUniform1i;
	extern PFNGLUNIFORM1FPROC glUniform1f;
	extern PFNGLUNIFORM2FPROC glUniform2f;
	extern PFNGLUNIFORM3FPROC glUniform3f;
	extern PFNGLUNIFORM4FPROC glUniform4f;
	extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
	extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
	extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
	extern PFNGLACTIVETEXTUREPROC glActiveTexture;

	extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
	extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
	extern PFNGLGENBUFFERSPROC glGenBuffers;
	extern PFNGLBINDBUFFERPROC glBindBuffer;
	extern PFNGLBUFFERDATAPROC glBufferData;
	extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
	extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;

	extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
	extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
	extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
	extern PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
	extern PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;
	extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
	extern PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
	extern PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
	extern PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
	extern PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
	extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
	extern PFNGLDRAWBUFFERSPROC glDrawBuffers;

	void initExtensions( );

}

#endif