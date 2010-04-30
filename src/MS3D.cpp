#include <slib/MS3D.h>

namespace slib {

	namespace MS3D {

		Header GetHeader( std::istream & in ) {
			Header header;
			in.read( header.id, 10 );
			in.read( reinterpret_cast< char * >( &header.version ), 4 );
			return header;
		}

		Vertex GetVertex( std::istream & in ) {
			Vertex vertex;
			vertex.flags = in.get( );
			in.read( reinterpret_cast< char * >( &vertex.vertex[ 0 ] ), 12 );
			vertex.boneId = in.get( );
			vertex.referenceCount = in.get( );
			return vertex;
		}

		Triangle GetTriangle( std::istream & in ) {
			Triangle triangle;
			in.read( reinterpret_cast< char * >( &triangle.flags ), 2 );
			in.read( reinterpret_cast< char * >( &triangle.vertexIndices[ 0 ] ), 6 );
			in.read( reinterpret_cast< char * >( &triangle.vertexNormals[ 0 ][ 0 ] ), 36 );
			in.read( reinterpret_cast< char * >( &triangle.s[ 0 ] ), 12 );
			in.read( reinterpret_cast< char * >( &triangle.t[ 0 ] ), 12 );
			triangle.smoothingGroup = in.get( );
			triangle.groupIndex = in.get( );
			return triangle;
		}

		Group GetGroup( std::istream & in ) {
			Group group;
			group.flags = in.get( );
			in.read( group.name, 32 );
			in.read( reinterpret_cast< char * >( &group.numtriangles ), 2 );
			group.triangleIndices = new unsigned short[ group.numtriangles ];
			for( int i = 0; i < group.numtriangles; i++ ) {
				in.read( reinterpret_cast< char * >( &group.triangleIndices[ i ] ), 2 );
			}
			group.materialIndex = in.get( );
			return group;
		}

		Material GetMaterial( std::istream & in ) {
			Material material;
			in.read( material.name, 32 );
			in.read( reinterpret_cast< char * >( material.ambient ), 16 );
			in.read( reinterpret_cast< char * >( material.diffuse ), 16 );
			in.read( reinterpret_cast< char * >( material.specular ), 16 );
			in.read( reinterpret_cast< char * >( material.emissive ), 16 );
			in.read( reinterpret_cast< char * >( &material.shininess ), 4 );
			in.read( reinterpret_cast< char * >( &material.transparency ), 4 );
			material.mode = in.get( );
			in.read( material.texture, 128 );
			in.read( material.alphamap, 128 );
			return material;
		}

		KeyframeRotation GetKeyframeRotation( std::istream & in ) {
			KeyframeRotation keyframe;
			in.read( reinterpret_cast< char * >( &keyframe.time ), 4 );
			in.read( reinterpret_cast< char * >( keyframe.rotation ), 12 );
			return keyframe;
		}

		KeyframePosition GetKeyframePosition( std::istream & in ) {
			KeyframePosition keyframe;
			in.read( reinterpret_cast< char * >( &keyframe.time ), 4 );
			in.read( reinterpret_cast< char * >( keyframe.position ), 12 );
			return keyframe;
		}

		Joint GetJoint( std::istream & in ) {
			Joint joint;
			joint.flags = in.get( );
			in.read( joint.name, 32 );
			in.read( joint.parentName, 32 );
			in.read( reinterpret_cast< char * >( joint.rotation ), 12 );
			in.read( reinterpret_cast< char * >( joint.position ), 12 );
			in.read( reinterpret_cast< char * >( &joint.numKeyFramesRot ), 2 );
			in.read( reinterpret_cast< char * >( &joint.numKeyFramesPos ), 2 );
			joint.keyFramesRot = new KeyframeRotation[ joint.numKeyFramesRot ];
			for( int i = 0; i < joint.numKeyFramesRot; i++ ) {
				joint.keyFramesRot[ i ] = GetKeyframeRotation( in );
			}
			joint.keyFramesPos = new KeyframePosition[ joint.numKeyFramesPos ];
			for( int i = 0; i < joint.numKeyFramesPos; i++ ) {
				joint.keyFramesPos[ i ] = GetKeyframePosition( in );
			}
			return joint;
		}

	}

}
