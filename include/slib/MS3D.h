#ifndef SLIB_MS3D_H
#define SLIB_MS3D_H

#include <istream>

namespace slib {

	namespace MS3D {

		struct Header {
			char id[ 10 ];
			int version;
		};

		Header GetHeader( std::istream & in );

		struct Vertex {
			unsigned char flags;
			float vertex[ 3 ];
			char boneId;
			unsigned char referenceCount;
		};

		Vertex GetVertex( std::istream & in );

		struct Triangle {
			unsigned short flags;
			unsigned short vertexIndices[ 3 ];
			float vertexNormals[ 3 ][ 3 ];
			float s[ 3 ];
			float t[ 3 ];
			unsigned char smoothingGroup;
			unsigned char groupIndex;
		};

		Triangle GetTriangle( std::istream & in );

		struct Group {
			unsigned char flags;
			char name[ 32 ];
			unsigned short numtriangles;
			unsigned short * triangleIndices;
			char materialIndex;
		};

		Group GetGroup( std::istream & in );

		struct Material {
			char name[ 32 ];
			float ambient[ 4 ];
			float diffuse[ 4 ];
			float specular[ 4 ];
			float emissive[ 4 ];
			float shininess;
			float transparency;
			char mode;
			char texture[ 128 ];
			char alphamap[ 128 ];
		};

		Material GetMaterial( std::istream & in );

		struct KeyframeRotation {
			float time;
			float rotation[ 3 ];
		};

		KeyframeRotation GetKeyframeRotation( std::istream & in );

		struct KeyframePosition {
			float time;
			float position[ 3 ];
		};

		KeyframePosition GetKeyframePosition( std::istream & in );

		struct Joint {
			unsigned char flags;
			char name[ 32 ];
			char parentName[ 32 ];
			float rotation[ 3 ];
			float position[ 3 ];

			unsigned short numKeyFramesRot;
			unsigned short numKeyFramesPos;

			KeyframeRotation * keyFramesRot;
			KeyframePosition * keyFramesPos;
		};

		Joint GetJoint( std::istream & in );

		template< class CallbackT >
		void Load( std::istream & in, CallbackT & callback ) {
			callback.Header( MS3D::GetHeader( in ) );

			unsigned short vertexCount;
			in.read( reinterpret_cast< char * >( &vertexCount ), 2 );

			callback.VerticesInfo( vertexCount );
			for( int i = 0; i < vertexCount; i++ ) {
				callback.Vertex( MS3D::GetVertex( in ) );
			}

			unsigned short triangleCount;
			in.read( reinterpret_cast< char * >( &triangleCount ), 2 );

			callback.TrianglesInfo( vertexCount );
			for( int i = 0; i < triangleCount; i++ ) {
				callback.Triangle( MS3D::GetTriangle( in ) );
			}

			unsigned short groupCount;
			in.read( reinterpret_cast< char * >( &groupCount ), 2 );

			callback.GroupsInfo( groupCount );
			for( int i = 0; i < groupCount; i++ ) {
				callback.Group( MS3D::GetGroup( in ) );
			}

			unsigned short materialCount;
			in.read( reinterpret_cast< char * >( &materialCount ), 2 );

			callback.MaterialsInfo( materialCount );
			for( int i = 0; i < materialCount; i++ ) {
				callback.Material( MS3D::GetMaterial( in ) );
			}

			float animationfps;
			in.read( reinterpret_cast< char * >( &animationfps ), 4 );

			float currenttime;
			in.read( reinterpret_cast< char * >( &currenttime ), 4 );

			int totalframes;
			in.read( reinterpret_cast< char * >( &totalframes ), 4 );

			unsigned short jointCount;
			in.read( reinterpret_cast< char * >( &jointCount ), 2 );

			callback.JointsInfo( jointCount, animationfps, currenttime, totalframes );
			for( int i = 0; i < jointCount; i++ ) {
				callback.Joint( MS3D::GetJoint( in ) );
			}
		}
	}

}

#endif