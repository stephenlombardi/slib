#ifndef SLIB_MENURENDEREROPENGL_H
#define SLIB_MENURENDEREROPENGL_H

#include <gl/freeglut.h>
#include <slib/glext.h>
#include <slib/extensions.h>

#include <slib/drawableobject.h>
#include <slib/shader.h>
#include <slib/OpenGLText.h>
#include <slib/TGA.h>

#include <slib/Menu.h>
#include <slib/Matrix.hpp>

namespace slib {

	class MenuRendererOpenGL : public Menu::Renderer {
	public:
		void init( const std::string & letterstexpath );
		//void init( const Shader & threedShader, const Shader & threedtexShader, GLuint lettersTex )
		void uninit( );
		void drawBox( const vec2f & position, const vec2f & dimensions, const vec4f & color );
		void drawString( const std::string & text, const vec2f & position, const vec2f & dimensions );
	private:
		DrawableObject screenquad_;
		DrawableObject textquad_;

		Shader threedShader_, threedtexShader_;

		GLuint lettersTex_;
	};

}

#endif