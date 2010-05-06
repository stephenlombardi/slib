#include <cmath>
#include <fstream>
#include <list>
#include <sstream>
#include <map>

#include <GL/freeglut.h>

#include <boost/shared_ptr.hpp>

#include <slib/Matrix.hpp>
#include <slib/MS3D.h>
#include <slib/TGA.h>

#include <slib/drawableobject.h>
#include <slib/extensions.h>
#include <slib/matrixutils.h>
#include <slib/OpenGLText.h>
#include <slib/shader.h>

#include "MS3DToMesh.h"

using namespace slib;

mat4f KeyframeMatrix( const JointAnimation::JointPose & pose ) {
	mat4f result = MultiplicitiveIdentity< mat4f >::Identity( );
	Sub< 3, 3 >::Matrix( result ) = RotationMatrixX( pose.rotation[ 0 ] ) * RotationMatrixY( pose.rotation[ 1 ] ) * RotationMatrixZ( pose.rotation[ 2 ] );
	Sub< 3, 1 >::Matrix( result, 0, 3 ) = Vec< 3 >::Tor( pose.position );
	return result;
}

class AnimBase;
typedef boost::shared_ptr< AnimBase > animptr_t;

class AnimBase {
public:
	virtual JointAnimation::JointPose operator( )( float time ) const = 0;
	virtual animptr_t simplify( float time ) {
		return clone( );
	}
	virtual animptr_t clone( ) const = 0;
	virtual int nodes( ) = 0;
private:
};

class AnimAnim : public AnimBase {
public:
	AnimAnim( const JointAnimation & jointanim, float animstart, bool loop ) : jointanim_( jointanim ), animstart_( animstart ), loop_( loop ) { }
	JointAnimation::JointPose operator( )( float time ) const {
		return jointanim_.GetInterpolatedKeyframe( time - animstart_, loop_ );
	}
	animptr_t clone( ) const {
		return animptr_t( new AnimAnim( jointanim_, animstart_, loop_ ) );
	}
	int nodes( ) {
		return 1;
	}
private:
	JointAnimation jointanim_;
	float animstart_;
	bool loop_;
};

class AnimTransition : public AnimBase {
public:
	AnimTransition( boost::shared_ptr< AnimBase > a0, boost::shared_ptr< AnimBase > a1, float transstart, float transtime ) : a0_( a0 ), a1_( a1 ), transstart_( transstart ), transtime_( transtime ) { }
	JointAnimation::JointPose operator( )( float time ) const {
		float alpha = ( time - transstart_ ) / transtime_;
		alpha = alpha < 0.0f ? 0.0f : ( alpha > 1.0f ? 1.0f : alpha );
		return lerpPose( (*a0_)( time ), (*a1_)( time ), alpha );
	}
	animptr_t simplify( float time ) {
		return time - transstart_ > transtime_ ? a1_->simplify( time ) : clone( );
	}
	animptr_t clone( ) const {
		return animptr_t( new AnimTransition( a0_, a1_, transstart_, transtime_ ) );
	}
	int nodes( ) {
		return a0_->nodes( ) + a1_->nodes( ) + 1;
	}
private:
	boost::shared_ptr< AnimBase > a0_, a1_;
	float transstart_, transtime_;
};

class AnimBlend : public AnimBase {
public:
	AnimBlend( boost::shared_ptr< AnimBase > a0, boost::shared_ptr< AnimBase > a1, float weight ) : a0_( a0 ), a1_( a1 ), weight_( weight ) { }
	JointAnimation::JointPose operator( )( float time ) const {
		return lerpPose( (*a0_)( time ), (*a1_)( time ), weight_ );
	}
	animptr_t simplify( float time ) {
		return weight_ == 0.0f ? a0_ : ( weight_ == 1.0f ? a1_->simplify( time ) : clone( ) );
	}
	animptr_t clone( ) const {
		return animptr_t( new AnimBlend( a0_, a1_, weight_ ) );
	}
	int nodes( ) {
		return a0_->nodes( ) + a1_->nodes( ) + 1;
	}
private:
	boost::shared_ptr< AnimBase > a0_, a1_;
	float weight_;
};

class AnimController {
public:
	void setSize( size_t s ) {
		curranim_.resize( s );
	}

	void setAnimation( const std::vector< JointAnimation > & anim, float time, bool loop ) {
		for( size_t i = 0; i < curranim_.size( ); i++ ) {
			curranim_[ i ] = animptr_t( new AnimAnim( anim[ i ], time, loop ) );
		}
	}

	void setAnimation( const std::vector< JointAnimation > & anim, float time, bool loop, float transtime ) {
		for( size_t i = 0; i < curranim_.size( ); i++ ) {
			curranim_[ i ] = animptr_t( new AnimTransition( curranim_[ i ], animptr_t( new AnimAnim( anim[ i ], time, loop ) ), time, transtime ) );
		}
	}

	template< class OutputIter >
	void toMatrix( float time, OutputIter out ) {
		for( size_t i = 0; i < curranim_.size( ); i++, ++out ) {
			if( curranim_[ i ].get( ) ) {
				*out = KeyframeMatrix( (*curranim_[ i ])( time ) );
			}
		}
	}

	void simplify( float time ) {
		for( size_t i = 0; i < curranim_.size( ); i++ ) {
			curranim_[ i ] = curranim_[ i ]->simplify( time );
		}
	}

	float nodes( ) {
		float n = 0.0f;
		for( size_t i = 0; i < curranim_.size( ); i++ ) {
			if( curranim_[ i ].get( ) ) {
				n += curranim_[ i ]->nodes( );
			}
		}
		return n / curranim_.size( );
	}
private:
	std::vector< animptr_t > curranim_;
};

int width = 512, height = 512;

// animation
BoneMatrixContainer matrixContainer;
std::map< std::string, std::vector< JointAnimation > > animations;
std::string currentanimation, nextanimation;
std::vector< float > weights;

AnimController animctrl;

// mesh data
Shader animshader;
std::vector< GLuint > textures;
std::list< std::pair< int, DrawableObject > > groups;

// command interface
std::string commandstring;
float view[ 16 ];
GLuint lettertex;
DrawableObject cmdtext;
Shader textshader;

// timers
bool transition = false;
float currtime = 0.0f;
float animstart = 0.0f;
float transitionstart = 0.0f;
float transitiontime = 1.0f;
bool loop = true;

void executeCommand( const std::string & cmd ) {
	std::istringstream cmdstream( cmd );

	std::string cmdname;
	cmdstream >> cmdname;

	if( cmdname == "lookat" ) {
		float camerax, cameray, cameraz, lookatx, lookaty, lookatz, upx, upy, upz;
		cmdstream >> camerax >> cameray >> cameraz >> lookatx >> lookaty >> lookatz >> upx >> upy >> upz;
		lookat( view, camerax, cameray, cameraz, lookatx, lookaty, lookatz, upx, upy, upz );
	} else if( cmdname == "setanim" ) {
		std::string animname;
		cmdstream >> animname >> loop;
		if( animations.find( animname ) != animations.end( ) ) {
			currentanimation = animname;
			transition = false;
			animstart = currtime;

			animctrl.setAnimation( animations[ animname ], currtime, loop );
		}
	} else if( cmdname == "weight" ) {
		int jointindexstart, jointindexend;
		float jointweight;
		cmdstream >> jointindexstart >> jointindexend >> jointweight;
		for( int i = jointindexstart; i <= jointindexend; i++ ) {
			if( i >= 0 && i < weights.size( ) ) {
				weights[ i ] = jointweight;
			}
		}
	} else if( cmdname == "transto" ) {
		std::string animname;
		cmdstream >> animname >> loop >> transitiontime;
		if( animations.find( animname ) != animations.end( ) ) {
			nextanimation = animname;
			transition = true;
			transitionstart = currtime;

			animctrl.setAnimation( animations[ animname ], currtime, loop, transitiontime );
		}
	} else if( cmdname == "loop" ) {
		cmdstream >> loop;
	} else if( cmdname == "simplify" ) {
		animctrl.simplify( currtime );
	}
}

GLuint LoadTexture( const std::string path ) {
	GLuint texid;
	// create new texture
	glGenTextures( 1, &texid );
	glBindTexture( GL_TEXTURE_2D, texid );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	int texwidth, texheight, texdepth;
	std::ifstream filestream( path.c_str( ), std::ios::binary );
	unsigned char * texdata = ReadTGA( filestream, texwidth, texheight, texdepth );
	filestream.close( );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texwidth, texheight, 0, texdepth == 24 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, texdata );
	delete [] texdata;

	return texid;
}

void initShaders( ) {
	const std::string animshaderattribs[] = { "in_Position", "in_Normal", "in_Texcoord", "in_Boneid" };
	animshader.initShader( "./ambientanim.vert", "./ambient.frag", animshaderattribs, animshaderattribs + 4 );
	animshader.getUniform( "model" );
	animshader.getUniform( "view" );
	animshader.getUniform( "projection" );
	animshader.getUniform( "bonematrices" );
	animshader.getUniform( "texture" );
	animshader.getUniform( "intensity" );

	const std::string textshaderAttribs[] = { "in_Position", "in_Texcoord" };
	textshader.initShader( "./textshader.vert", "./textshader.frag", textshaderAttribs, textshaderAttribs + 2 );
	textshader.getUniform( "model" );
	textshader.getUniform( "view" );
	textshader.getUniform( "projection" );
	textshader.getUniform( "texture" );
}

void initDrawableObjects( ) {
	cmdtext.initVAO( );
	cmdtext.addBuffers( 3 );

	float * verts = createTextVerts( commandstring ), * texcoords = createTextTexcoords( commandstring );
	unsigned int * indices = createTextIndices( commandstring );
	cmdtext.setBuffer( 0, verts, commandstring.size( ) * 4, 3, GL_DYNAMIC_DRAW );
	cmdtext.setBuffer( 1, texcoords, commandstring.size( ) * 4, 2, GL_DYNAMIC_DRAW );
	cmdtext.setBuffer( 2, indices, commandstring.size( ) * 6, GL_DYNAMIC_DRAW );
	delete [] verts;
	delete [] texcoords;
	delete [] indices;
}

void initTextures( ) {
	lettertex = LoadTexture( "./letters.tga" );
}

void initMesh( ) {
	std::ifstream infostream( "zombie02.txt", std::ios::binary );
	std::map< std::string, std::pair< int, int > > animationinfo;
	while( infostream ) {
		std::string animationname;
		int startframe, endframe;
		infostream >> animationname >> startframe >> endframe;
		if( infostream ) {
			animationinfo[ animationname ] = std::make_pair( startframe, endframe );
			std::cout << "animation: " << animationname << std::endl;
		}
	}
	infostream.close( );

	MS3DToMesh meshloader;
	std::ifstream filestream( "zombie02.ms3d", std::ios::binary );
	MS3D::Load( filestream, meshloader );
	filestream.close( );

	std::vector< std::string > materials = meshloader.GetMaterials( );
	for( size_t i = 0; i < materials.size( ); i++ ) {
		textures.push_back( LoadTexture( materials[ i ] ) );
		std::cout << "material: " << materials[ i ] << std::endl;
	}

	matrixContainer = meshloader.GetBoneMatrixContainer( );
	std::vector< JointAnimation > jointanimations = meshloader.GetJointAnimations( );
	matrixContainer.Calculate( );

	weights = std::vector< float >( jointanimations.size( ), 0.0f );

	std::vector< float > vertices = meshloader.GetVertices( );
	std::vector< float > normals = meshloader.GetNormals( );

	std::cout << "vertices: " << ( vertices.size( ) / 3 ) << std::endl;

	InverseTransform( vertices, normals, meshloader.GetBoneIds( ), matrixContainer.GetMatrices( ) );

	animctrl.setSize( jointanimations.size( ) );
	for( std::map< std::string, std::pair< int, int > >::iterator iter = animationinfo.begin( ); iter != animationinfo.end( ); ++iter ) {
		animations[ iter->first ].resize( jointanimations.size( ) );
		for( size_t i = 0; i < jointanimations.size( ); i++ ) {
			animations[ iter->first ][ i ] = jointanimations[ i ].Extract( iter->second.first / meshloader.GetAnimationFPS( ), iter->second.second / meshloader.GetAnimationFPS( ) );
		}
	}

	for( std::list< std::pair< int, std::vector< unsigned int > > >::const_iterator iter = meshloader.GetGroups( ).begin( ); iter != meshloader.GetGroups( ).end( ); ++iter ) {
		DrawableObject dobject;

		dobject.initVAO( );
		dobject.addBuffers( 5 );
		dobject.setBuffer( 0, &vertices[ 0 ], meshloader.GetVertices( ).size( ) / 3, 3, GL_STATIC_DRAW );
		dobject.setBuffer( 1, &normals[ 0 ], meshloader.GetVertices( ).size( ) / 3, 3, GL_STATIC_DRAW );
		dobject.setBuffer( 2, &meshloader.GetTexcoords( )[ 0 ], meshloader.GetVertices( ).size( ) / 3, 2, GL_STATIC_DRAW );
		dobject.setBuffer( 3, &meshloader.GetBoneIds( )[ 0 ], meshloader.GetVertices( ).size( ) / 3, 1, GL_STATIC_DRAW );
		dobject.setBuffer( 4, &iter->second[ 0 ], iter->second.size( ), GL_STATIC_DRAW );

		groups.push_back( std::make_pair( iter->first, dobject ) );
	}
}

void init( ) {
	initShaders( );
	initDrawableObjects( );
	initTextures( );

	initMesh( );

	lookat( view, 0.0f, 0.0f, -16.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
}

void reshape( int _width, int _height ) {
	glViewport( 0, 0, _width, _height );
	width = _width;
	height = _height;
}

void display( ) {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	static const float identitymatrix [] = { 1.0f, 0.0f, 0.0f, 0.0f,
											0.0f, 1.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 1.0f, 0.0f,
											0.0f, 0.0f, 0.0f, 1.0f };
	float projection[ 16 ];
	perspective( projection, 3.1415926f / 3.0f, 1.0f, 1.0f, 4096.0f );

	animshader.useProgram( );
	animshader.setUniform( "projection", projection );
	animshader.setUniform( "view", view );
	animshader.setUniform( "model", identitymatrix );
	animshader.setUniform( "bonematrices", &matrixContainer.GetMatrices( )[ 0 ]( 0, 0 ), matrixContainer.GetMatrices( ).size( ) );
	animshader.setUniform( "texture", 0 );
	glActiveTexture( GL_TEXTURE0 );

	for( std::list< std::pair< int, DrawableObject > >::iterator iter = groups.begin( ); iter != groups.end( ); ++iter ) {
		glBindTexture( GL_TEXTURE_2D, textures[ iter->first ] );
		iter->second.bindBuffers( );
		iter->second.drawIndexed( GL_TRIANGLES );
	}

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	textshader.useProgram( );
	textshader.setUniform( "projection", identitymatrix );
	textshader.setUniform( "view", identitymatrix );

	const float model [] = { 32.0f / width, 0.0f, 0.0f, -1.0f,
							0.0f, 32.0f / height, 0.0f, -1.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f };
	textshader.setUniform( "model", model );
	textshader.setUniform( "texture", 0 );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, lettertex );
	cmdtext.bindBuffers( );
	cmdtext.drawIndexed( GL_TRIANGLES );

	glutSwapBuffers( );
}

void timerfunc( int value ) {
	/*if( transition ) {
		if( animations.find( currentanimation ) != animations.end( ) ) {
			if( animations.find( nextanimation ) != animations.end( ) ) {
				float alpha = ( currtime - transitionstart ) / transitiontime;
				alpha = alpha > 1.0f ? 1.0f : alpha;

				std::vector< mat4f > transformations( animations[ currentanimation ].size( ) );
				std::vector< mat4f >::iterator titer = transformations.begin( );
				std::vector< float >::iterator witer = weights.begin( );
				std::vector< JointAnimation >::iterator a1iter = animations[ currentanimation ].begin( );
				std::vector< JointAnimation >::iterator a2iter = animations[ nextanimation ].begin( );
				for( ; a1iter != animations[ currentanimation ].end( ); ++a1iter, ++a2iter, ++titer, ++witer ) {
					JointAnimation::JointPose a1pose = a1iter->GetInterpolatedKeyframe( currtime - animstart, loop );
					JointAnimation::JointPose a2pose = a2iter->GetInterpolatedKeyframe( currtime - animstart, loop );
					*titer = KeyframeMatrix( lerpPose( lerpPose( a1pose, a2pose, 0.0f ), lerpPose( a1pose, a2pose, 1.0f - *witer ), alpha ) );
				}
				matrixContainer.Calculate( transformations.begin( ), transformations.end( ) );

				if( count( weights.begin( ), weights.end( ), 0.0f ) == weights.size( ) && alpha == 1.0f ) {
					currentanimation = nextanimation;
					transition = false;
				}
			}
		}
	} else {
		if( animations.find( currentanimation ) != animations.end( ) ) {
			std::vector< mat4f > transformations( animations[ currentanimation ].size( ) );
			std::vector< mat4f >::iterator titer = transformations.begin( );
			std::vector< float >::iterator witer = weights.begin( );
			std::vector< JointAnimation >::iterator iter = animations[ currentanimation ].begin( );
			for( ; iter != animations[ currentanimation ].end( ); ++iter, ++titer, ++witer ) {
				*titer = KeyframeMatrix( iter->GetInterpolatedKeyframe( currtime - animstart, loop ) );
			}
			matrixContainer.Calculate( transformations.begin( ), transformations.end( ) );
		}
	}*/

	std::vector< mat4f > transformations( animations.begin( )->second.size( ) );
	animctrl.toMatrix( currtime, transformations.begin( ) );
	matrixContainer.Calculate( transformations.begin( ), transformations.end( ) );

	std::cout << animctrl.nodes( ) << std::endl;

	currtime += 0.030f;

	glutPostRedisplay( );

	glutTimerFunc( 30, timerfunc, 0 );
}

void key( unsigned char key, int x, int y ) {
	if( key == 13 ) {
		executeCommand( commandstring );
		commandstring = "";
	} else if( key == 8 ) {
		if( commandstring.size( ) > 0 ) {
			commandstring.erase( commandstring.size( ) - 1 );
		}
	} else {
		commandstring += (char)key;
	}

	float * verts = createTextVerts( commandstring ), * texcoords = createTextTexcoords( commandstring );
	unsigned int * indices = createTextIndices( commandstring );
	cmdtext.setBuffer( 0, verts, commandstring.size( ) * 4, 3, GL_DYNAMIC_DRAW );
	cmdtext.setBuffer( 1, texcoords, commandstring.size( ) * 4, 2, GL_DYNAMIC_DRAW );
	cmdtext.setBuffer( 2, indices, commandstring.size( ) * 6, GL_DYNAMIC_DRAW );
	delete [] verts;
	delete [] texcoords;
	delete [] indices;
}

void keyup( unsigned char key, int x, int y ) {
}

int main( int argc, char ** argv ) {
	glutInit( &argc, argv );
	//glutInitContextVersion( 3, 2 );
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( width, height );
	glutInitWindowPosition( 50, 50 );

	glutCreateWindow( "MS3D Viewer" );

	glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );

	glEnable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );
	
	glutReshapeFunc( reshape );
	glutDisplayFunc( display );
	glutKeyboardFunc( key );
	glutKeyboardUpFunc( keyup );

	initExtensions( );
	init( );

	glutTimerFunc( 100, timerfunc, 0 );
	glutMainLoop( );

	return 0;
}
