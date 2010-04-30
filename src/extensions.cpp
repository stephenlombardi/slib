#include <slib/extensions.h>

namespace slib {

	void initExtensions( ) {
		// shaders
		glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress( "glCreateShader" );
		glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress( "glShaderSource" );
		glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress( "glCompileShader" );
		glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress( "glDetachShader" );
		glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress( "glDeleteShader" );
		glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress( "glCreateProgram" );
		glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress( "glAttachShader" );
		glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress( "glLinkProgram" );
		glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress( "glUseProgram" );
		glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress( "glBindAttribLocation" );
		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress( "glGetUniformLocation" );
		glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress( "glUniform1i" );
		glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress( "glUniform1f" );
		glUniform2f = (PFNGLUNIFORM2FPROC)wglGetProcAddress( "glUniform2f" );
		glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress( "glUniform3f" );
		glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress( "glUniform4f" );
		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress( "glUniformMatrix4fv" );
		glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress( "glDeleteProgram" );
		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress( "glGetShaderInfoLog" );
		glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress( "glGetProgramInfoLog" );
		glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress( "glActiveTexture" );

		// vao
		glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress( "glGenVertexArrays" );
		glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress( "glBindVertexArray" );
		glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress( "glGenBuffers" );
		glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress( "glBindBuffer" );
		glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress( "glBufferData" );
		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress( "glVertexAttribPointer" );
		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress( "glEnableVertexAttribArray" );
		glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress( "glDeleteBuffers" );
		glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress( "glDeleteVertexArrays" );

		// fbo
		glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress( "glGenFramebuffers" );
		glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress( "glBindFramebuffer" );
		glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress( "glFramebufferTexture2D" );
		glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress( "glFramebufferRenderbuffer" );
		glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)wglGetProcAddress( "glBlitFramebuffer" );
		glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress( "glDeleteFramebuffers" );
		glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress( "glGenRenderbuffers" );
		glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress( "glBindRenderbuffer" );
		glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress( "glRenderbufferStorage" );
		glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress( "glDeleteRenderbuffers" );
		glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress( "glCheckFramebufferStatus" );
		glDrawBuffers = (PFNGLDRAWBUFFERSPROC)wglGetProcAddress( "glDrawBuffers" );
	}

	PFNGLCREATESHADERPROC glCreateShader;
	PFNGLSHADERSOURCEPROC glShaderSource;
	PFNGLCOMPILESHADERPROC glCompileShader;
	PFNGLDETACHSHADERPROC glDetachShader;
	PFNGLDELETESHADERPROC glDeleteShader;
	PFNGLCREATEPROGRAMPROC glCreateProgram;
	PFNGLATTACHSHADERPROC glAttachShader;
	PFNGLLINKPROGRAMPROC glLinkProgram;
	PFNGLUSEPROGRAMPROC glUseProgram;
	PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
	PFNGLUNIFORM1IPROC glUniform1i;
	PFNGLUNIFORM1FPROC glUniform1f;
	PFNGLUNIFORM2FPROC glUniform2f;
	PFNGLUNIFORM3FPROC glUniform3f;
	PFNGLUNIFORM4FPROC glUniform4f;
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
	PFNGLDELETEPROGRAMPROC glDeleteProgram;
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
	PFNGLACTIVETEXTUREPROC glActiveTexture;

	PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
	PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
	PFNGLGENBUFFERSPROC glGenBuffers;
	PFNGLBINDBUFFERPROC glBindBuffer;
	PFNGLBUFFERDATAPROC glBufferData;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	PFNGLDELETEBUFFERSPROC glDeleteBuffers;
	PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;

	PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
	PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
	PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
	PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
	PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;
	PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
	PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
	PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
	PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
	PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
	PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
	PFNGLDRAWBUFFERSPROC glDrawBuffers;

}
