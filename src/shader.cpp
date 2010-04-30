#include <slib/shader.h>

namespace slib {

	std::string read( const std::string & filename ) {
		std::ifstream stream( filename.c_str( ) );
		return std::string( (std::istreambuf_iterator< char >( stream )), std::istreambuf_iterator< char >( ) );
	}

	void Shader::destroyShader( ) {
		glDetachShader( program, vshader );
		glDetachShader( program, fshader );

		glDeleteShader( vshader );
		glDeleteShader( fshader );

		glDeleteProgram( program );
	}

	unsigned int Shader::getUniformLocation( const std::string & str ) const {
		return glGetUniformLocation( program, str.c_str( ) );
	}

	void Shader::getUniform( const std::string & str ) {
		uniforms[ str ] = glGetUniformLocation( program, str.c_str( ) );
	}

	void Shader::setUniform( const std::string & str, int i0 ) const {
		std::map< std::string, unsigned int >::const_iterator iter = uniforms.find( str );
		glUniform1i( iter->second, i0 );
	}

	void Shader::setUniform( const std::string & str, float x0 ) const {
		std::map< std::string, unsigned int >::const_iterator iter = uniforms.find( str );
		glUniform1f( iter->second, x0 );
	}

	void Shader::setUniform( const std::string & str, float x0, float x1 ) const {
		std::map< std::string, unsigned int >::const_iterator iter = uniforms.find( str );
		glUniform2f( iter->second, x0, x1 );
	}

	void Shader::setUniform( const std::string & str, float x0, float x1, float x2 ) const {
		std::map< std::string, unsigned int >::const_iterator iter = uniforms.find( str );
		glUniform3f( iter->second, x0, x1, x2 );
	}

	void Shader::setUniform( const std::string & str, float x0, float x1, float x2, float x3 ) const {
		std::map< std::string, unsigned int >::const_iterator iter = uniforms.find( str );
		glUniform4f( iter->second, x0, x1, x2, x3 );
	}

	void Shader::setUniform( const std::string & str, const float * matrix ) const {
		std::map< std::string, unsigned int >::const_iterator iter = uniforms.find( str );
		glUniformMatrix4fv( iter->second, 1, GL_TRUE, matrix );
	}

	void Shader::setUniform( const std::string & str, const float * matrix, int matrixcount ) const {
		std::map< std::string, unsigned int >::const_iterator iter = uniforms.find( str );
		glUniformMatrix4fv( iter->second, matrixcount, GL_TRUE, matrix );
	}

	void Shader::useProgram( ) const {
		glUseProgram( program );
	}

	void Shader::bindAttribLocation( int i, const std::string & str ) {
		glBindAttribLocation( program, i, str.c_str( ) );
	}

	void Shader::bindAttribLocation( int i, const char * str ) {
		glBindAttribLocation( program, i, str );
	}

}
