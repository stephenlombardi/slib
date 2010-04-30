#include <slib/ScriptFunctions.h>

namespace slib {

	std::ostream & printAny( std::ostream & out, const boost::any & any ) {
		if( boost::any_cast< bool >( &any ) ) {
			return out << boost::any_cast< bool >( any );
		} else if( boost::any_cast< int >( &any ) ) {
			return out << boost::any_cast< int >( any );
		} else if( boost::any_cast< float >( &any ) ) {
			return out << boost::any_cast< float >( any );
		} else if( boost::any_cast< std::string >( &any ) ) {
			return out << boost::any_cast< std::string >( any );
		} else {
			//return out << any.type( ).name( );
			throw ScriptError( "printAny: unknown type" );
		}
	}

	boost::any addAny( const boost::any & x, const boost::any & y ) {
		if( boost::any_cast< bool >( &x ) ) {
			return boost::any_cast< bool >( x ) || boost::any_cast< bool >( y ) ;
		} else if( boost::any_cast< int >( &x ) ) {
			return boost::any_cast< int >( x ) + boost::any_cast< int >( y );
		} else if( boost::any_cast< float >( &x ) ) {
			return boost::any_cast< float >( x ) + boost::any_cast< float >( y );
		} else if( boost::any_cast< std::string >( &x ) ) {
			return boost::any_cast< std::string >( x ) + boost::any_cast< std::string >( y );
		} else {
			throw boost::bad_any_cast( );
		}
	}

	boost::any multAny( const boost::any & x, const boost::any & y ) {
		if( boost::any_cast< bool >( &x ) ) {
			return boost::any_cast< bool >( x ) && boost::any_cast< bool >( y ) ;
		} else if( boost::any_cast< int >( &x ) ) {
			return boost::any_cast< int >( x ) * boost::any_cast< int >( y );
		} else if( boost::any_cast< float >( &x ) ) {
			return boost::any_cast< float >( x ) * boost::any_cast< float >( y );
		} else {
			throw boost::bad_any_cast( );
		}
	}

	boost::any equalAny( const boost::any & x, const boost::any & y ) {
		if( boost::any_cast< bool >( &x ) ) {
			return boost::any_cast< bool >( x ) == boost::any_cast< bool >( y ) ;
		} else if( boost::any_cast< int >( &x ) ) {
			return boost::any_cast< int >( x ) == boost::any_cast< int >( y );
		} else if( boost::any_cast< float >( &x ) ) {
			return boost::any_cast< float >( x ) == boost::any_cast< float >( y );
		} else if( boost::any_cast< std::string >( &x ) ) {
			return boost::any_cast< std::string >( x ) == boost::any_cast< std::string >( y );
		} else {
			throw boost::bad_any_cast( );
		}
	}

	boost::any addInverseAny( const boost::any & any ) {
		if( boost::any_cast< bool >( &any ) ) {
			return !boost::any_cast< bool >( any );
		} else if( boost::any_cast< int >( &any ) ) {
			return -boost::any_cast< int >( any );
		} else if( boost::any_cast< float >( &any ) ) {
			return -boost::any_cast< float >( any );
		} else {
			throw boost::bad_any_cast( );
		}
	}

	boost::any multInverseAny( const boost::any & any ) {
		if( boost::any_cast< float >( &any ) ) {
			return 1.0f / boost::any_cast< float >( any );
		} else {
			throw boost::bad_any_cast( );
		}
	}

	boost::any ScriptPrint( EnvT & env, const std::list< boost::any > & paramlist ) {
		std::for_each( paramlist.begin( ), paramlist.end( ), boost::bind( printAny, boost::ref( std::cout ), _1 ) );
		std::cout << std::endl;
		return boost::any( );
	}

	boost::any ScriptAdd( EnvT & env, const std::list< boost::any > & paramlist ) {
		if( paramlist.empty( ) ) {
			throw ScriptError( "ScriptAdd: incorrect parameter count" );
		} else {
			try {
				std::list< boost::any >::const_iterator second = paramlist.begin( ); ++second;
				return std::accumulate( second, paramlist.end( ), paramlist.front( ), boost::bind( addAny, _1, _2 ) );
			} catch( const boost::bad_any_cast & ) {
				throw ScriptError( "ScriptAdd: inconsistent argument types" );
			}
		}
	}

	boost::any ScriptMult( EnvT & env, const std::list< boost::any > & paramlist ) {
		if( paramlist.empty( ) ) {
			throw ScriptError( "ScriptMult: incorrect parameter count" );
		} else {
			try {
				std::list< boost::any >::const_iterator second = paramlist.begin( ); ++second;
				return std::accumulate( second, paramlist.end( ), paramlist.front( ), boost::bind( multAny, _1, _2 ) );
			} catch( const boost::bad_any_cast & ) {
				throw ScriptError( "ScriptMult: inconsistent argument types" );
			}
		}
	}

	boost::any ScriptEqual( EnvT & env, const std::list< boost::any > & paramlist ) {
		if( paramlist.empty( ) ) {
			throw ScriptError( "ScriptEqual: incorrect parameter count" );
		} else {
			try {
				std::list< boost::any >::const_iterator second = paramlist.begin( ); ++second;
				return std::inner_product( second, paramlist.end( ), paramlist.begin( ), boost::any( true ), multAny, equalAny );
			} catch( const boost::bad_any_cast & ) {
				throw ScriptError( "ScriptEqual: inconsistent argument types" );
			}
		}
	}

	boost::any ScriptAddInverse( EnvT & env, const std::list< boost::any > & paramlist ) {
		if( paramlist.size( ) != 1 ) {
			throw ScriptError( "ScriptAddInverse: incorrect argument count (should be 1)" );
		} else {
			try {
				return addInverseAny( paramlist.front( ) );
			} catch( const boost::bad_any_cast & ) {
				throw ScriptError( "ScriptAddInverse: invalid argument type" );
			}
		}
	}

	boost::any ScriptMultInverse( EnvT & env, const std::list< boost::any > & paramlist ) {
		if( paramlist.size( ) != 1 ) {
			throw ScriptError( "ScriptMultInverse: incorrect argument count (should be 1)" );
		} else {
			try {
				return multInverseAny( paramlist.front( ) );
			} catch( const boost::bad_any_cast & ) {
				throw ScriptError( "ScriptMultInverse: invalid argument type" );
			}
		}
	}

}
