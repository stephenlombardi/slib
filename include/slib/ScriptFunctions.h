#ifndef SLIB_SCRIPTFUNCTIONS_H
#define SLIB_SCRIPTFUNCTIONS_H

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <sstream>
#include <string>

#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <slib/ScriptError.h>

namespace slib {

	typedef std::map< std::string, boost::any > EnvT;
	typedef boost::function< boost::any ( EnvT &, const std::list< boost::any > & ) > ScriptFuncT;

	std::ostream & printAny( std::ostream & out, const boost::any & any );
	boost::any addAny( const boost::any & x, const boost::any & y );
	boost::any multAny( const boost::any & x, const boost::any & y );
	boost::any equalAny( const boost::any & x, const boost::any & y );
	boost::any addInverseAny( const boost::any & any );
	boost::any multInverseAny( const boost::any & any );

	boost::any ScriptPrint( EnvT & env, const std::list< boost::any > & paramlist );
	boost::any ScriptAdd( EnvT & env, const std::list< boost::any > & paramlist );
	boost::any ScriptMult( EnvT & env, const std::list< boost::any > & paramlist );
	boost::any ScriptEqual( EnvT & env, const std::list< boost::any > & paramlist );
	boost::any ScriptAddInverse( EnvT & env, const std::list< boost::any > & paramlist );
	boost::any ScriptMultInverse( EnvT & env, const std::list< boost::any > & paramlist );

	// templated functions

	template< class Func1T, class Func2T >
	class ScriptFuncSequence_t {
	public:
		ScriptFuncSequence_t( Func1T func1, Func2T func2 ) : func1_( func1 ), func2_( func2 ) { }
		boost::any operator( )( EnvT & env, const std::list< boost::any > & paramlist ) const {
			func1_( env, paramlist );
			func2_( env, paramlist );
			return boost::any( );
		}
	private:
		Func1T func1_;
		Func2T func2_;
	};

	template< class Func1T, class Func2T >
	ScriptFuncSequence_t< Func1T, Func2T > ScriptFuncSequence( Func1T func1, Func2T func2 ) {
		return ScriptFuncSequence_t< Func1T, Func2T >( func1, func2 );
	}

	template< class FuncT, class T1, class T2 >
	class AnyFuncBinary_t {
	public:
		AnyFuncBinary_t( FuncT func ) : func_( func ) { }
		boost::any operator( )( const boost::any & x, const boost::any & y ) const {
			return boost::any( func_( boost::any_cast< T1 >( x ), boost::any_cast< T2 >( y ) ) );
		}
	private:
		FuncT func_;
	};

	template< class T1, class T2 >
	class AnyFunc {
	public:
		template< class FuncT >
		static AnyFuncBinary_t< FuncT, T1, T2 > Binary( FuncT func ) {
			return AnyFuncBinary_t< FuncT, T1, T2 >( func );
		}
	private:
	};

	template< class FuncT >
	class ScriptForeach_t {
	public:
		ScriptForeach_t( FuncT func ) : func_( func ) { }
		boost::any operator( )( EnvT & env, const std::list< boost::any > & paramlist ) const {
			try {
				std::for_each( paramlist.begin( ), paramlist.end( ), func_ );
				return boost::any( );
			} catch( const boost::bad_any_cast & ) {
				throw ScriptError( "ScriptForeach_t: inconsistent argument types" );
			}
		}
	private:
		FuncT func_;
	};

	template< class FuncT >
	ScriptForeach_t< FuncT > ScriptForeach( FuncT func ) {
		return ScriptForeach_t< FuncT >( func );
	}

	template< class FuncT >
	class ScriptAccumulate_t {
	public:
		ScriptAccumulate_t( FuncT func ) : func_( func ) { }
		boost::any operator( )( EnvT & env, const std::list< boost::any > & paramlist ) const {
			if( paramlist.empty( ) ) {
				throw ScriptError( "ScriptAccumulate_t: incorrect parameter count" );
			} else {
				try {
					std::list< boost::any >::const_iterator second = paramlist.begin( ); ++second;
					//return std::accumulate( second, paramlist.end( ), paramlist.front( ), boost::bind( func_, _1, _2 ) );
					return std::accumulate( second, paramlist.end( ), paramlist.front( ), func_ );
				} catch( const boost::bad_any_cast & ) {
					throw ScriptError( "ScriptAccumulate_t: inconsistent argument types" );
				}
			}
		}
	private:
		FuncT func_;
	};

	template< class FuncT >
	ScriptAccumulate_t< FuncT > ScriptAccumulate( FuncT func ) {
		return ScriptAccumulate_t< FuncT >( func );
	}

	template< class Func1T, class Func2T >
	class Overload_t {
	public:
		Overload_t( Func1T func1, Func2T func2 ) : func1_( func1 ), func2_( func2 ) { }
		boost::any operator( )( EnvT & env, const std::list< boost::any > & paramlist ) const {
			try {
				return func1_( env, paramlist );
			} catch( ScriptError & e ) {
				return func2_( env, paramlist );
			}
		}
	private:
		Func1T func1_;
		Func2T func2_;
	};

	template< class Func1T, class Func2T >
	Overload_t< Func1T, Func2T > Overload( Func1T func1, Func2T func2 ) {
		return Overload_t< Func1T, Func2T >( func1, func2 );
	}

	template< class Func1T, class Func2T >
	ScriptFuncT OverloadReturnScriptFuncT( Func1T func1, Func2T func2 ) {
		return ScriptFuncT( Overload_t< Func1T, Func2T >( func1, func2 ) );
	}

	template< class FuncT >
	class ScriptFunc0_t {
	public:
		ScriptFunc0_t( FuncT func, bool check = true ) : func_( func ), check_( check ) { }
		boost::any operator( )( EnvT & env, const std::list< boost::any > & paramlist ) const {
			if( check_ && paramlist.size( ) != 0 ) {
				throw ScriptError( "incorrect parameter count" );
			}
			
			func_( );
			return boost::any( );
		}
	private:
		FuncT func_;
		bool check_;
	};

	template< class FuncT >
	ScriptFunc0_t< FuncT > ScriptFunc0( FuncT func, bool check = true ) {
		return ScriptFunc0_t< FuncT >( func, check );
	}

	template< class FuncT >
	class ScriptFunc0r_t {
	public:
		ScriptFunc0r_t( FuncT func ) : func_( func ) { }
		boost::any operator( )( EnvT & env, const std::list< boost::any > & paramlist ) const {
			if( paramlist.size( ) == 0 ) {
				return func_( );
			} else {
				throw ScriptError( "incorrect parameter count" );
			}
		}
	private:
		FuncT func_;
	};

	template< class FuncT >
	ScriptFunc0r_t< FuncT > ScriptFunc0r( FuncT func ) {
		return ScriptFunc0r_t< FuncT >( func );
	}

	template< class FuncT, class Arg1T >
	class ScriptFunc1_t {
	public:
		ScriptFunc1_t( FuncT func ) : func_( func ) { }
		boost::any operator( )( EnvT & env, const std::list< boost::any > & paramlist ) const {
			if( paramlist.size( ) == 1 ) {
				try {
					std::list< boost::any >::const_iterator iter0 = paramlist.begin( );
					func_( boost::any_cast< Arg1T >( *iter0 ) );
					return boost::any( );
				} catch( const boost::bad_any_cast & ) {
					throw ScriptError( "invalid argument type" );
				}
			} else {
				throw ScriptError( "incorrect parameter count" );
			}
		}
	private:
		FuncT func_;
	};

	template< class Arg1T >
	class ScriptFunc1 {
	public:
		template< class FuncT >
		static ScriptFunc1_t< FuncT, Arg1T > Func( FuncT func ) {
			return ScriptFunc1_t< FuncT, Arg1T >( func );
		}
	private:
	};

	template< class FuncT, class Arg1T >
	class ScriptFunc1r_t {
	public:
		ScriptFunc1r_t( FuncT func ) : func_( func ) { }
		boost::any operator( )( EnvT & env, const std::list< boost::any > & paramlist ) const {
			if( paramlist.size( ) == 1 ) {
				try {
					std::list< boost::any >::const_iterator iter0 = paramlist.begin( );
					return func_( boost::any_cast< Arg1T >( *iter0 ) );
				} catch( const boost::bad_any_cast & ) {
					throw ScriptError( "invalid argument type" );
				}
			} else {
				throw ScriptError( "incorrect parameter count" );
			}
		}
	private:
		FuncT func_;
	};

	template< class Arg1T >
	class ScriptFunc1r {
	public:
		template< class FuncT >
		static ScriptFunc1r_t< FuncT, Arg1T > Func( FuncT func ) {
			return ScriptFunc1r_t< FuncT, Arg1T >( func );
		}
	private:
	};

	template< class FuncT, class Arg1T, class Arg2T >
	class ScriptFunc2_t {
	public:
		ScriptFunc2_t( FuncT func ) : func_( func ) { }
		boost::any operator( )( EnvT & env, const std::list< boost::any > & paramlist ) const {
			if( paramlist.size( ) == 2 ) {
				try {
					std::list< boost::any >::const_iterator iter0 = paramlist.begin( );
					std::list< boost::any >::const_iterator iter1 = iter0; ++iter1;
					func_( boost::any_cast< Arg1T >( *iter0 ), boost::any_cast< Arg2T >( *iter1 ) );
					return boost::any( );
				} catch( const boost::bad_any_cast & ) {
					throw ScriptError( "invalid argument type" );
				}
			} else {
				throw ScriptError( "incorrect parameter count" );
			}		
		}
	private:
		FuncT func_;
	};

	template< class Arg1T, class Arg2T >
	class ScriptFunc2 {
	public:
		template< class FuncT >
		static ScriptFunc2_t< FuncT, Arg1T, Arg2T > Func( FuncT func ) {
			return ScriptFunc2_t< FuncT, Arg1T, Arg2T >( func );
		}
	private:
	};

	template< class FuncT, class Arg1T, class Arg2T >
	class ScriptFunc2r_t {
	public:
		ScriptFunc2r_t( FuncT func ) : func_( func ) { }
		boost::any operator( )( EnvT & env, const std::list< boost::any > & paramlist ) const {
			if( paramlist.size( ) == 2 ) {
				try {
					std::list< boost::any >::const_iterator iter0 = paramlist.begin( );
					std::list< boost::any >::const_iterator iter1 = iter0; ++iter1;
					return func_( boost::any_cast< Arg1T >( *iter0 ), boost::any_cast< Arg2T >( *iter1 ) );
				} catch( const boost::bad_any_cast & ) {
					throw ScriptError( "invalid argument type" );
				}
			} else {
				throw ScriptError( "incorrect parameter count" );
			}		
		}
	private:
		FuncT func_;
	};

	template< class Arg1T, class Arg2T >
	class ScriptFunc2r {
	public:
		template< class FuncT >
		static ScriptFunc2r_t< FuncT, Arg1T, Arg2T > Func( FuncT func ) {
			return ScriptFunc2r_t< FuncT, Arg1T, Arg2T >( func );
		}
	private:
	};
	
}

#endif