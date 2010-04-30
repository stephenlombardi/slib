#include <slib/MenuRendererOpenGL.h>

namespace slib {

	void MenuRendererOpenGL::init( const std::string & letterstexpath );
		const std::string projAttribs[] = { "in_Position" };
		threedShader.initShader( 
			"#version 140\n\n"
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n\n"
			"in vec3 in_Position;\n\n"
			"void main( void ) {\n"
			"	gl_Position = projection * view * model * vec4(in_Position, 1.0);\n"
			"}\n", 
			"#version 140\n\n"
			"uniform vec4 color;\n"
			"out vec4 out_Color;\n\n"
			"void main( void ) {\n"
			"	out_Color = color;\n"
			"}\n", 
			projAttribs, projAttribs + 1, true );
		threedShader.getUniform( "model" );
		threedShader.getUniform( "view" );
		threedShader.getUniform( "projection" );
		threedShader.getUniform( "color" );

		const std::string threedtexAttribs[] = { "in_Position", "in_Texcoord" };
		threedtexShader.initShader( 
			"#version 140\n\n"
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n\n"
			"in vec3 in_Position;\n"
			"in vec2 in_Texcoord;\n\n"
			"out vec2 ex_Texcoord;\n\n"
			"void main( void ) {\n"
			"	gl_Position = projection * view * model * vec4(in_Position, 1.0);\n"
			"	ex_Texcoord = in_Texcoord;\n"
			"}\n",
			"#version 140\n\n"
			"uniform sampler2D texture;\n\n"
			"in vec2 ex_Texcoord;\n\n"
			"out vec4 out_Color;\n\n"
			"void main( void ) {\n"
			"	out_Color = texture2D( texture, ex_Texcoord );\n"
			"}\n",
			threedtexAttribs, threedtexAttribs + 2, true );
		threedtexShader.getUniform( "model" );
		threedtexShader.getUniform( "view" );
		threedtexShader.getUniform( "projection" );
		threedtexShader.getUniform( "texture" );

		{
			float verts [] = { 
				1.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f
			};

			float texcoords [] = {
				1.0f, 1.0f,
				1.0f, 0.0f,
				0.0f, 0.0f,
				0.0f, 0.0f,
				0.0f, 1.0f,
				1.0f, 1.0f
			};

			screenquad_.initVAO( );
			screenquad_.addBuffers( 2 );
			screenquad_.setBuffer( 0, verts, 6, 3, GL_STATIC_DRAW );
			screenquad_.setBuffer( 1, texcoords, 6, 2, GL_STATIC_DRAW );
		}

		{
			textquad_.initVAO( );
			textquad_.addBuffers( 3 );
		}

		{
			std::ifstream stream( letterstexpath.c_str( ), std::ios::binary );
			glGenTextures( 1, &lettersTex_ );
			glBindTexture( GL_TEXTURE_2D, lettersTex_ );

			int texwidth, texheight, texdepth;
			unsigned char * texdata = ReadTGA( stream, texwidth, texheight, texdepth );

			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texwidth, texheight, 0, texdepth == 24 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, texdata );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

			delete [] texdata;
		}
	}

	void MenuRendererOpenGL::uninit( ) {
		glDeleteTextures( 1, &lettersTex_ );

		textquad_.destroyVAO( );
		screenquad_.destroyVAO( );

		threedtexShader.destroyShader( );
		threedShader.destroyShader( );
	}

	void MenuRendererOpenGL::drawBox( const vec2f & position, const vec2f & dimensions, const vec4f & color ) {
		static const float identitymatrix [] = { 1.0f, 0.0f, 0.0f, 0.0f,
												0.0f, 1.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 0.0f, 1.0f };

		threedShader_.useProgram( );
		threedShader_.setUniform( "projection", identitymatrix );
		threedShader_.setUniform( "view", identitymatrix );

		const float model [] = { dimensions[ 0 ], 0.0f, 0.0f, position[ 0 ],
								0.0f, dimensions[ 1 ], 0.0f, position[ 1 ],
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f };
		threedShader_.setUniform( "model", model );
		threedShader_.setUniform( "color", color[ 0 ], color[ 1 ], color[ 2 ], color[ 3 ] );

		screenquad_.bindBuffers( );
		screenquad_.draw( GL_TRIANGLES );
	}

	void MenuRendererOpenGL::drawString( const std::string & text, const vec2f & position, const vec2f & dimensions ) {
		static const float identitymatrix [] = { 1.0f, 0.0f, 0.0f, 0.0f,
												0.0f, 1.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 0.0f, 1.0f };

		threedtexShader_.useProgram( );
		threedtexShader_.setUniform( "projection", identitymatrix );
		threedtexShader_.setUniform( "view", identitymatrix );

		const float model [] = { dimensions[ 1 ], 0.0f, 0.0f, position[ 0 ],
								0.0f, dimensions[ 1 ], 0.0f, position[ 1 ],
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f };
		threedtexShader_.setUniform( "model", model );

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, lettersTex_ );
		threedtexShader_.setUniform( "texture", 0 );

		float * verts = createTextVerts( text ), * texcoords = createTextTexcoords( text );
		unsigned int * indices = createTextIndices( text );
		textquad_.setBuffer( 0, verts, text.size( ) * 4, 3, GL_DYNAMIC_DRAW );
		textquad_.setBuffer( 1, texcoords, text.size( ) * 4, 2, GL_DYNAMIC_DRAW );
		textquad_.setBuffer( 2, indices, text.size( ) * 6, GL_DYNAMIC_DRAW );
		delete [] verts;
		delete [] texcoords;
		delete [] indices;

		textquad_.bindBuffers( );
		textquad_.drawIndexed( GL_TRIANGLES );
	}

}
