#include <cmath>
#include <fstream>
#include <functional>

#include <slib/ScriptAST.h>
#include <slib/ScriptError.h>
#include <slib/ScriptFunctions.h>
#include <slib/ScriptParse.h>
#include <slib/Tokenizer.h>

#include <slib/Matrix.hpp>

namespace slib {

	template< int n, int m, class FieldT >
	std::ostream & operator<<( std::ostream & out, const Matrix< n, m, FieldT > & matrix ) {
		out << "[";
		for( int i = 0; i < n; i++ ) {
			if( i != 0 ) {
				out << std::endl;
			}
			for( int j = 0; j < m; j++ ) {
				out << ( j == 0 ? "" : ", " ) << matrix( i, j );
			}
		}
		return out << "]";
	}

	template< int n, class FieldT >
	std::ostream & operator<<( std::ostream & out, const Matrix< n, 1, FieldT > & matrix ) {
		out << "[";
		for( int i = 0; i < n; i++ ) {
			out << ( i == 0 ? "" : ", " ) << matrix[ i ];
		}
		return out << "]";
	}

}

using namespace slib;

std::ostream & printAnyExtended( std::ostream & out, const boost::any & any ) {
	try {
		return printAny( out, any );
	} catch( ScriptError & e ) {
		if( boost::any_cast< vec3f >( &any ) ) {
			return out << boost::any_cast< vec3f >( any );
		} else if( boost::any_cast< mat3f >( &any ) ) {
			return out << boost::any_cast< mat3f >( any );
		} else {
			throw ScriptError( "printAnyExtended: unknown type" );
		}
	}
}

template< int n, int m, class FieldT >
boost::any ScriptMatrix( EnvT & env, const std::list< boost::any > & paramlist ) {
	if( n * m != paramlist.size( ) ) {
		throw ScriptError( "ScriptVector: incorrect parameter count" );
	} else {
		Matrix< n, m, FieldT > mat;
		int i = 0, j = 0;
		try {
			for( std::list< boost::any >::const_iterator iter = paramlist.begin( ); iter != paramlist.end( ) && i < n * m; ++iter ) {
				mat( i, j ) = boost::any_cast< FieldT >( *iter );
				i = j == m - 1 ? i + 1 : i;
				j = j == m - 1 ? 0 : j + 1;
			}
		} catch( const boost::bad_any_cast & ) {
			throw ScriptError( "ScriptVector: invalid argument type" );
		}
		return boost::any( mat );
	}
}

template< class Arg1T, class Arg2T, class ResultT >
struct multiplies : public std::binary_function< Arg1T, Arg2T, ResultT > {
	ResultT operator( )( const Arg1T & arg1, const Arg2T & arg2 ) const { return arg1 * arg2; }
};

int main( int argc, char ** argv ) {
	if( argc == 2 ) {	
		std::ifstream stream( argv[ 1 ] );
		
		Program program;
		
		try {
			std::list< std::string > tokens;

			// build the token list from the stream
			for( std::string token; !( token = GetToken( stream ) ).empty( ); tokens.push_back( token ) );
			
			// remove comments
			tokens.remove_if( boost::bind( (size_t (std::string::*)( char, size_t ) const)&std::string::find, _1, '#', 0 ) == 0 );
			
			program = ReadProgram( tokens );
		} catch( TokenizerError & e ) {
			std::cout << "Tokenizer error: " << e.GetError( ) << std::endl;
		} catch( SyntaxError & e ) {
			std::cout << "Syntax error: " << e.GetError( ) << std::endl;
		}
		
		EnvT env;
		env[ "print" ] = ScriptFuncT( ScriptFuncSequence( ScriptForeach( boost::bind( printAnyExtended, boost::ref( std::cout ), _1 ) ),
														ScriptFunc0( boost::bind( printAny, boost::ref( std::cout ), boost::any( std::string( 1, '\n' ) ) ), false ) ) );
		env[ "equal" ] = ScriptFuncT( ScriptEqual );
		env[ "add" ] = ScriptFuncT( Overload( ScriptAccumulate( AnyFunc< float, float >::Binary( std::plus< float >( ) ) ),
									Overload( ScriptAccumulate( AnyFunc< int, int >::Binary( std::plus< int >( ) ) ),
									Overload( ScriptAccumulate( AnyFunc< bool, bool >::Binary( std::logical_or< bool >( ) ) ),
									Overload( ScriptAccumulate( AnyFunc< vec3f, vec3f >::Binary( std::plus< vec3f >( ) ) ),
											ScriptAccumulate( AnyFunc< std::string, std::string >::Binary( std::plus< std::string >( ) ) ) ) ) ) ) );
		env[ "mult" ] = ScriptFuncT( Overload( ScriptAccumulate( AnyFunc< float, float >::Binary( std::multiplies< float >( ) ) ),
									Overload( ScriptAccumulate( AnyFunc< bool, bool >::Binary( std::logical_and< bool >( ) ) ),
									Overload( ScriptAccumulate( AnyFunc< mat3f, mat3f >::Binary( std::multiplies< mat3f >( ) ) ),
									Overload( ScriptFunc2r< mat3f, vec3f >::Func( multiplies< mat3f, vec3f, vec3f >( ) ),
											ScriptAccumulate( AnyFunc< int, int >::Binary( std::multiplies< int >( ) ) ) ) ) ) ) );
		env[ "addinverse" ] = ScriptFuncT( Overload( ScriptFunc1r< float >::Func( std::negate< float >( ) ),
											Overload( ScriptFunc1r< bool >::Func( std::logical_not< bool >( ) ),
													ScriptFunc1r< int >::Func( std::negate< int >( ) ) ) ) );
		env[ "multinverse" ] = ScriptFuncT( ScriptFunc1r< float >::Func( boost::bind( std::divides< float >( ), 1.0f, _1 ) ) );
		env[ "overload" ] = ScriptFuncT( ScriptAccumulate( AnyFunc< ScriptFuncT, ScriptFuncT >::Binary( OverloadReturnScriptFuncT< ScriptFuncT, ScriptFuncT > ) ) );
		
		env[ "addbool" ] = ScriptFuncT( ScriptAccumulate( AnyFunc< bool, bool >::Binary( std::logical_or< bool >( ) ) ) );
		env[ "addint" ] = ScriptFuncT( ScriptAccumulate( AnyFunc< int, int >::Binary( std::plus< int >( ) ) ) );
		env[ "addfloat" ] = ScriptFuncT( ScriptAccumulate( AnyFunc< float, float >::Binary( std::plus< float >( ) ) ) );
		
		env[ "pow" ] = ScriptFuncT( ScriptFunc2r< float, float >::Func( (float (*)( float, float ))&std::pow ) );
		
		env[ "vec3f" ] = ScriptFuncT( ScriptMatrix< 3, 1, float > );
		env[ "mat3f" ] = ScriptFuncT( ScriptMatrix< 3, 3, float > );
		
		env[ "dot" ] = ScriptFuncT( ScriptFunc2r< vec3f, vec3f >::Func( DotProduct< Matrix, Matrix, 3, float > ) );
		
		std::cout << "---- begin of execution ----" << std::endl;
		
		try {
			program.eval( env );
		} catch( ScriptError & e ) {
			std::cout << "Script error: " << e.getError( ) << std::endl;
		}
		
		std::cout << std::endl << "---- end of execution ----" << std::endl;
		/*std::cout << "---- begin variable dump ----" << std::endl;
		for( EnvT::iterator iter = env.begin( ); iter != env.end( ); ++iter ) {
			std::cout << iter->first << ": ";
			printAny( std::cout, iter->second );
			std::cout << std::endl;
		}*/
		
		/*try {
			std::cout << boost::any_cast< int >( boost::any_cast< ScriptFuncT >( env[ "factorial" ] )( env, std::list< boost::any >( 1, boost::any( 4 ) ) ) ) << std::endl;
		} catch( ScriptError & e ) {
			std::cout << "Script error: " << e.getError( ) << std::endl;
		}*/
	} else {
		std::cout << "usage: " << argv[ 0 ] << " filename" << std::endl;
	}
		
	return 0;
}