/****************************************************
* Author: Steve Lombardi
* Date: January, 2010
* Description: Wrapper class for GLSL shaders
*
* How to use:
*    To initialize the shader, call
*
*       Shader::initShader( ... )
*
*    with the vertex shader filename, the fragment
*    shader filename, and a pair of iterators to
*    an ordered list of attribute names. This list
*    should match up the variable names in the shader
*    file with the DrawableObject buffers. At the
*    end of execution
*
*       Shader::destroyShader( )
*
*    should be called to free the resources. After
*    calling initShader, a call to
*
*       Shader::getUniform( "uniform name" )
*
*    should occur for every uniform variable in the
*    shader. This will cause the shader to store
*    a handle to the uniform variable internally.
*    The variable can then be set with:
*
*       Shader::setUniform( "uniform name", ... )
*
*    The shader does a lookup into an STL map to
*    find the OpenGL handle ID of the uniform and
*    sets the uniform according to the arguments
*    of the function. This will incur a small
*    performance penalty due to the map lookup,
*    and while it should not be a bottleneck, the
*    function
*
*       Shader::getUniformLocation( "uniform name" )
*
*    can be used if you wish to keep track of your
*    own OpenGL handles. Finally, a call to
*
*       Shader::useProgram( )
*
*    will activate the shader prior to drawing.
*
* Example:
*    [.cpp file]
*
*    // global scope
*    Shader shader;
*
*    // in initialization method
*    const std::string [] = { "in_Position",
*                             "in_Color" };
*    shader.initShader( "shader.vert",
*                       "shader.frag",
*                       attribNames,
*                       attribNames + 2 );
*    shader.getUniform( "model" );
*    shader.getUniform( "view" );
*    shader.getUniform( "projection" );
*    ...
*
*    // in drawing method
*    shader.useProgram( );
*    float model[ 16 ], view[ 16 ], projection[ 16 ];
*    shader.setUniform( "model", model );
*    shader.setUniform( "view", view );
*    shader.setUniform( "projection", projection );
*    ...
*
*    [.vert file]
*
*    uniform mat4 model;
*    uniform mat4 view;
*    uniform mat4 projection;
*    in vec3 in_Position;
*    in vec3 in_Color;
*    out vec3 ex_Color;
*
*    void main( void ) {
*        gl_Position = projection * view * model * vec4(in_Position, 1.0);
*        ex_Color = in_Color;
*    }
*
*    [.frag file]
*
*    in vec3 ex_Color;
*    out vec4 out_Color;
*
*    void main( void ) {
*        out_Color = vec4( ex_Color, 1.0 );
*    }
*
****************************************************/

#ifndef SLIB_SHADER_H
#define SLIB_SHADER_H

#include <GL/freeglut.h>
#include <GL/glext.h>

#include <slib/extensions.h>

#include <map>
#include <string>
#include <fstream>
#include <iostream>

namespace slib {

	std::string read( const std::string & filename );

	class Shader {
	public:
		template< class InputIterator >
		void initShader( const std::string & vertfilename, const std::string & fragfilename, InputIterator attribnamesbegin, InputIterator attribnamesend, bool nofile = false ) {
			vshader = glCreateShader( GL_VERTEX_SHADER );
			std::string vshadersource = nofile ? vertfilename : read( vertfilename );
			const char * vstr [] = { vshadersource.c_str( ) };
			glShaderSource( vshader, 1, vstr, 0 );
			glCompileShader( vshader );

			char vshaderlog[ 4096 ];
			int vshaderloglen;
			glGetShaderInfoLog( vshader, 4096, &vshaderloglen, vshaderlog );
			if( vshaderloglen > 1 ) {
				std::cout << "vshaderlog: " << std::endl << vshaderlog << std::endl;
			}

			fshader = glCreateShader( GL_FRAGMENT_SHADER );
			std::string fshadersource = nofile ? fragfilename : read( fragfilename );
			const char * fstr [] = { fshadersource.c_str( ) };
			glShaderSource( fshader, 1, fstr, 0 );
			glCompileShader( fshader );

			char fshaderlog[ 4096 ];
			int fshaderloglen;
			glGetShaderInfoLog( fshader, 4096, &fshaderloglen, fshaderlog );
			if( fshaderloglen > 1 ) {
				std::cout << "fshaderlog: " << std::endl << fshaderlog << std::endl;
			}

			program = glCreateProgram( );
			glAttachShader( program, vshader );
			glAttachShader( program, fshader );

			int i = 0;
			for( InputIterator iter = attribnamesbegin; iter != attribnamesend; ++iter, i++ ) {
				bindAttribLocation( i, *iter );
			}

			glLinkProgram( program );

			glUseProgram( 0 );
		}

		void destroyShader( );
		unsigned int getUniformLocation( const std::string & str ) const;
		void getUniform( const std::string & str );
		void setUniform( const std::string & str, int i0 ) const;
		void setUniform( const std::string & str, float x0 ) const;
		void setUniform( const std::string & str, float x0, float x1 ) const;
		void setUniform( const std::string & str, float x0, float x1, float x2 ) const;
		void setUniform( const std::string & str, float x0, float x1, float x2, float x3 ) const;
		void setUniform( const std::string & str, const float * matrix ) const;
		void setUniform( const std::string & str, const float * matrix, int matrixcount ) const;
		void useProgram( ) const;
	private:
		void bindAttribLocation( int i, const std::string & str );
		void bindAttribLocation( int i, const char * str );
		unsigned int vshader, fshader, program;
		std::map< std::string, unsigned int > uniforms;
	};

}

#endif
