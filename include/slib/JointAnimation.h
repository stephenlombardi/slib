#ifndef SLIB_JOINTANIMATION_H
#define SLIB_JOINTANIMATION_H

#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

namespace slib {

	class JointAnimation {
	public:
		struct Keyframe {
			Keyframe( );
			Keyframe( float _time, const float * _data );
			float time;
			float data[ 3 ];
		};
		
		struct JointPose {
			JointPose( );
			JointPose( const float * _rotation, const float * _position );
			float rotation[ 3 ], position[ 3 ];
		};

		JointAnimation( ) { }
		template< class InputIterator >
		JointAnimation( InputIterator rotbegin, InputIterator rotend, InputIterator posbegin, InputIterator posend ) {
			std::copy( rotbegin, rotend, std::back_inserter( rotationKeyframes ) );
			std::copy( posbegin, posend, std::back_inserter( positionKeyframes ) );
		}

		JointPose GetInterpolatedKeyframe( float time, bool loop ) const;

		JointAnimation Extract( float starttime, float endtime );

		float TotalTime( ) const;
	private:
		static void GetMatrixPerKeyframe( const std::vector< Keyframe > & keyframes, float time, bool loop, float * data );
		static void ExtractPerKeyframe( std::vector< Keyframe > & keyframeswrite, const std::vector< Keyframe > & keyframesread, float starttime, float endtime );

		std::vector< Keyframe > rotationKeyframes;
		std::vector< Keyframe > positionKeyframes;
	};

	JointAnimation::JointPose lerpPose( const JointAnimation::JointPose & x, const JointAnimation::JointPose & y, float alpha );
	JointAnimation::JointPose weightPose( const JointAnimation::JointPose & x, float weight );

}

#endif
