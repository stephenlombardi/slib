#include <slib/ScriptAST.h>

namespace slib {

	Expr::~Expr( ) {
	}

	ExprList::ExprList( ) {
	}

	ExprList::ExprList( boost::shared_ptr< Expr > expr ) : exprlist_( 1, expr ) {
	}

	ExprList::ExprList( const std::list< boost::shared_ptr< Expr > > & exprlist ) : exprlist_( exprlist ) {
	}

	void ExprList::push_front( boost::shared_ptr< Expr > expr ) {
		exprlist_.push_front( expr );
	}

	boost::any ExprList::eval( EnvT & env ) const {
		std::for_each( exprlist_.begin( ), exprlist_.end( ), boost::bind( &Expr::eval, _1, boost::ref( env ) ) );
		return boost::any( );
	}

	Program::Program( ) {
	}

	Program::Program( const ExprList & exprlist ) : exprlist_( exprlist ) {
	}

	void Program::eval( EnvT & env ) const {
		exprlist_.eval( env );
	}

	//FuncAdapter::FuncAdapter( const std::list< std::string > & paramlist, const ExprList & exprlist ) : paramlist_( paramlist ), exprlist_( exprlist ) {
	FuncAdapter::FuncAdapter( const std::list< std::string > & paramlist, const ExprList & exprlist, const EnvT & env ) : paramlist_( paramlist ), exprlist_( exprlist ), env_( env ) {
	}

	boost::any FuncAdapter::operator( )( EnvT & env, const std::list< boost::any > & params ) const {
		// copy environment so there are no side effects from function call
		//EnvT envcopy = env;
		EnvT envcopy = env_;
		
		for( EnvT::iterator iter = envcopy.begin( ); iter != envcopy.end( ); ++iter ) {
			if( env.find( iter->first ) != env.end( ) ) {
				iter->second = env[ iter->first ];
			}
		}
			
		// update environment with function parameter values
		/*std::list< std::string >::const_iterator paramlistiter = paramlist_.begin( );
		std::list< boost::any >::const_iterator paramsiter = params.begin( );
		for( ; paramsiter != params.end( ); ++paramlistiter, ++paramsiter ) {
			envcopy[ *paramlistiter ] = *paramsiter;
		}*/
		std::for_each( paramlist_.begin( ), paramlist_.end( ), boost::bind( (size_t (EnvT::*)( const std::string & ))&EnvT::erase, boost::ref( envcopy ), _1 ) );
		std::transform( paramlist_.begin( ), paramlist_.end( ), params.begin( ), std::inserter( envcopy, envcopy.begin( ) ), boost::bind( ( std::pair< std::string, boost::any > (*)( std::string, boost::any ) )&std::make_pair, _1, _2 ) );
		//std::transform( paramlist_.begin( ), paramlist_.end( ), params.begin( ), std::inserter( envcopy, envcopy.begin( ) ), std::pointer_to_binary_function< std::string, boost::any, std::pair< std::string, boost::any > >( std::make_pair ) );
		
		exprlist_.eval( envcopy );
		return envcopy[ "return" ];
	}

	FuncDecl::FuncDecl( const std::list< std::string > & paramlist, const ExprList & exprlist ) : paramlist_( paramlist ), exprlist_( exprlist ) {
	}

	boost::any FuncDecl::eval( EnvT & env ) const {
		return ScriptFuncT( FuncAdapter( paramlist_, exprlist_, env ) );
	}

	AssignExpr::AssignExpr( const std::string & ident, boost::shared_ptr< Expr > expr ) : ident_( ident ), expr_( expr ) {
	}

	boost::any AssignExpr::eval( EnvT & env ) const {
		// strong typing
		/*boost::any val = expr_->eval( env );
		if( env.find( ident_ ) != env.end( ) && env[ ident_ ].type( ) != val.type( ) ) {
			throw ScriptError( "cannot assign type " + std::string( val.type( ).name( ) ) + " to " + std::string( env[ ident_ ].type( ).name( ) ) );
		} else {
			return env[ ident_ ] = val;
		}*/
		return env[ ident_ ] = expr_->eval( env );
	}

	IfExpr::IfExpr( boost::shared_ptr< Expr > expr, const ExprList & trueexpr, const ExprList & falseexpr ) : expr_( expr ), trueexpr_( trueexpr ), falseexpr_( falseexpr ) {
	}

	boost::any IfExpr::eval( EnvT & env ) const {
		try {
			if( boost::any_cast< bool >( expr_->eval( env ) ) ) {
				return trueexpr_.eval( env );
			} else {
				return falseexpr_.eval( env );
			}
		} catch( const boost::bad_any_cast & ) {
			throw ScriptError( "expression in if statement does not evaluate to bool" );
		}
	}

	WhileExpr::WhileExpr( boost::shared_ptr< Expr > expr, const ExprList & exprlist ) : expr_( expr ), exprlist_( exprlist ) {
	}

	boost::any WhileExpr::eval( EnvT & env ) const {
		try {
			while( boost::any_cast< bool >( expr_->eval( env ) ) ) {
				exprlist_.eval( env );
			}
			
			return boost::any( );
		} catch( const boost::bad_any_cast & ) {
			throw ScriptError( "expression in if statement does not evaluate to bool" );
		}
	}

	FuncCall::FuncCall( const std::string & fname, const std::list< boost::shared_ptr< Expr > > & arglist ) : fname_( fname ), arglist_( arglist ) {
	}

	boost::any FuncCall::eval( EnvT & env ) const {
		std::list< boost::any > fvals;
		std::transform( arglist_.begin( ), arglist_.end( ), std::back_inserter( fvals ), boost::bind( &Expr::eval, _1, boost::ref( env ) ) );
			
		if( env.find( fname_ ) != env.end( ) ) {
			try {
				return boost::any_cast< ScriptFuncT >( env[ fname_ ] )( env, fvals );
			} catch( const boost::bad_any_cast & ) {
				std::ostringstream stream;
				stream << "cannot make function call to type " << env[ fname_ ].type( ).name( );
				throw ScriptError( stream.str( ) );
			}
		} else {
			std::ostringstream stream;
			stream << "call to undefined function " << fname_;
			throw ScriptError( stream.str( ) );
		}
	}

	IntegerExpr::IntegerExpr( const std::string & number ) {
		std::istringstream stream( number );
		stream >> number_;
	}

	boost::any IntegerExpr::eval( EnvT & env ) const {
		return boost::any( number_ );
	}

	FloatExpr::FloatExpr( const std::string & number ) {
		std::istringstream stream( number );
		stream >> number_;
	}

	boost::any FloatExpr::eval( EnvT & env ) const {
		return boost::any( number_ );
	}

	BoolExpr::BoolExpr( const std::string & number ) {
		b_ = number == "true" ? true : false;
	}

	boost::any BoolExpr::eval( EnvT & env ) const {
		return boost::any( b_ );
	}

	IdentExpr::IdentExpr( const std::string & ident ) : ident_( ident ) {
	}

	boost::any IdentExpr::eval( EnvT & env ) const {
		if( env.find( ident_ ) != env.end( ) ) {
			return env[ ident_ ];
		} else {
			std::ostringstream stream;
			stream << "undefined variable " << ident_;
			throw ScriptError( stream.str( ) );
		}
	}

	StringConstExpr::StringConstExpr( const std::string & str ) : str_( str.substr( 1, str.size( ) - 2 ) ) {
	}

	boost::any StringConstExpr::eval( EnvT & env ) const {
		return boost::any( str_ );
	}

}
