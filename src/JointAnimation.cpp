#include <slib/JointAnimation.h>

namespace slib {

	JointAnimation::JointPose::JointPose( ) {
	}

	JointAnimation::JointPose::JointPose( const float * _rotation, const float * _position ) {
		for( int i = 0; i < 3; i++ ) {
			rotation[ i ] = _rotation[ i ];
			position[ i ] = _position[ i ];
		}
	}

	JointAnimation::JointPose JointAnimation::GetInterpolatedKeyframe( float time, bool loop ) const {
		float rotation[ 3 ], position[ 3 ];
		GetMatrixPerKeyframe( rotationKeyframes, time, loop, rotation );
		GetMatrixPerKeyframe( positionKeyframes, time, loop, position );
		return JointPose( rotation, position );
	}

	JointAnimation JointAnimation::Extract( float starttime, float endtime ) {
		JointAnimation result;
		ExtractPerKeyframe( result.rotationKeyframes, this->rotationKeyframes, starttime, endtime );
		ExtractPerKeyframe( result.positionKeyframes, this->positionKeyframes, starttime, endtime );
		return result;
	}

	float JointAnimation::TotalTime( ) const {
		return rotationKeyframes.back( ).time;
	}

	JointAnimation::Keyframe::Keyframe( ) {
	}

	JointAnimation::Keyframe::Keyframe( float _time, const float * _data ) : time( _time ) {
		for( int i = 0; i < 3; i++ ) {
			data[ i ] = _data[ i ];
		}
	}

	void JointAnimation::GetMatrixPerKeyframe( const std::vector< Keyframe > & keyframes, float time, bool loop, float * data ) {
		if( loop ) {
			time = keyframes.back( ).time * ( ( time / keyframes.back( ).time ) - static_cast< int >( time / keyframes.back( ).time ) );
		}

		if( keyframes.size( ) == 0 ) {
			data[ 0 ] = 0.0f; data[ 1 ] = 0.0f; data[ 2 ] = 0.0f;
		} else if( keyframes.size( ) == 1 ) {
			data[ 0 ] = keyframes[ 0 ].data[ 0 ];
			data[ 1 ] = keyframes[ 0 ].data[ 1 ];
			data[ 2 ] = keyframes[ 0 ].data[ 2 ];
		} else {
			/*size_t frame = 0;
			for( ; frame < keyframes.size( ) && keyframes[ frame ].time < time; frame++ );
			if( frame == keyframes.size( ) ) {
				frame = keyframes.size( ) - 1;
			}
			int prevframe = frame > 0 ? frame - 1 : keyframes.size( ) - 1;

			float previoust = frame > 0 ? keyframes[ frame - 1 ].time : 0.0f;
			float t = ( time - previoust ) / ( keyframes[ frame ].time - previoust );

			data[ 0 ] = keyframes[ prevframe ].data[ 0 ] * ( 1.0f - t ) + keyframes[ frame ].data[ 0 ] * t;
			data[ 1 ] = keyframes[ prevframe ].data[ 1 ] * ( 1.0f - t ) + keyframes[ frame ].data[ 1 ] * t;
			data[ 2 ] = keyframes[ prevframe ].data[ 2 ] * ( 1.0f - t ) + keyframes[ frame ].data[ 2 ] * t;*/

			/*if( time < keyframes.front( ).time ) {
				data[ 0 ] = keyframes.front( ).data[ 0 ];
				data[ 1 ] = keyframes.front( ).data[ 1 ];
				data[ 2 ] = keyframes.front( ).data[ 2 ];*/
			if( time >= keyframes.back( ).time ) {
				data[ 0 ] = keyframes.back( ).data[ 0 ];
				data[ 1 ] = keyframes.back( ).data[ 1 ];
				data[ 2 ] = keyframes.back( ).data[ 2 ];
			} else {
				size_t frame;
				for( frame = 0; frame < keyframes.size( ) - 1 && !( time >= keyframes[ frame ].time && time < keyframes[ frame + 1 ].time ); frame++ );
				/*	if( time >= keyframes[ frame ].time && time < keyframes[ frame + 1 ].time ) {
						break;
					}
				}*/
				int nextframe = frame + 1;

				if( time < keyframes.front( ).time ) {
					frame = keyframes.size( ) - 1;
					nextframe = 0;
				}

				float t = ( time - keyframes[ frame ].time ) / ( keyframes[ nextframe ].time - keyframes[ frame ].time );
				data[ 0 ] = keyframes[ frame ].data[ 0 ] * ( 1.0f - t ) + keyframes[ nextframe ].data[ 0 ] * t;
				data[ 1 ] = keyframes[ frame ].data[ 1 ] * ( 1.0f - t ) + keyframes[ nextframe ].data[ 1 ] * t;
				data[ 2 ] = keyframes[ frame ].data[ 2 ] * ( 1.0f - t ) + keyframes[ nextframe ].data[ 2 ] * t;
			}
		}
	}

	void JointAnimation::ExtractPerKeyframe( std::vector< Keyframe > & keyframeswrite, const std::vector< Keyframe > & keyframesread, float starttime, float endtime ) {
		size_t startkeyframe = 0, endkeyframe = 0;
		for( ; startkeyframe < keyframesread.size( ) && keyframesread[ startkeyframe ].time < starttime; startkeyframe++ );
		for( ; endkeyframe < keyframesread.size( ) && keyframesread[ endkeyframe ].time < endtime; endkeyframe++ );
		float prevframetime = startkeyframe > 0 ? keyframesread[ startkeyframe - 1 ].time : 0.0f;

		for( size_t j = startkeyframe; j <= endkeyframe; j++ ) {
			keyframeswrite.push_back( JointAnimation::Keyframe( keyframesread[ j ].time - prevframetime, keyframesread[ j ].data ) );
		}
	}

	JointAnimation::JointPose lerpPose( const JointAnimation::JointPose & x, const JointAnimation::JointPose & y, float alpha ) {
		float rotation[ 3 ], position[ 3 ];
		for( int i = 0; i < 3; i++ ) {
			rotation[ i ] = x.rotation[ i ] * ( 1.0f - alpha ) + y.rotation[ i ] * alpha;
			position[ i ] = x.position[ i ] * ( 1.0f - alpha ) + y.position[ i ] * alpha;
		}
		return JointAnimation::JointPose( rotation, position );
	}

	JointAnimation::JointPose blendPose( const JointAnimation::JointPose & x, float weight ) {
		float rotation[ 3 ], position[ 3 ];
		for( int i = 0; i < 3; i++ ) {
			rotation[ i ] = x.rotation[ i ] * weight;
			position[ i ] = x.position[ i ] * weight;
		}
		return JointAnimation::JointPose( rotation, position );
	}

}
