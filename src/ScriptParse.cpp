#include <slib/ScriptParse.h>

namespace slib {

	SyntaxError::SyntaxError( const std::string & error ) : error_( error ) {
	}

	std::string SyntaxError::GetError( ) {
		return error_;
	}

	////////////////////////////////////////
	// functions
	////////////////////////////////////////

	// throws an error if the current token (tokens.front()) does not match the string
	// otherwise, advances the token stream
	void Match( std::list< std::string > & tokens, const std::string & str ) {
		if( tokens.empty( ) ) {
			throw SyntaxError( "unexpected end of token stream" );
		} else if( tokens.front( ) == str ) {
			tokens.pop_front( );
		} else {
			throw SyntaxError( "expected \"" + str + "\", found \"" + tokens.front( ) + "\"" );
		}
	}

	// throws an error if the current token (tokens.front()) is not a number
	// otherwise, advances the token stream
	std::string MatchNumber( std::list< std::string > & tokens ) {
		if( tokens.empty( ) ) {
			throw SyntaxError( "unexpected end of token stream" );
		}
		
		if( tokens.front( )[ 0 ] >= '0' && tokens.front( )[ 0 ] <= '9' || tokens.front( )[ 0 ] == '-' || tokens.front( )[ 0 ] == '+' ) {
		} else {
			throw SyntaxError( "expected number, found \"" + tokens.front( ) + "\"" );
		}

		for( size_t i = 1; i < tokens.front( ).size( ); i++ ) {
			if( tokens.front( )[ i ] >= '0' && tokens.front( )[ i ] <= '9' || tokens.front( )[ i ] == '.' ) {
			} else {
				throw SyntaxError( "expected number, found \"" + tokens.front( ) + "\"" );
			}
		}
		
		std::string str = tokens.front( );

		tokens.pop_front( );
		
		return str;
	}

	// throws an error if the current token (tokens.front()) is not a number
	// otherwise, advances the token stream
	std::string MatchIdentifier( std::list< std::string > & tokens ) {
		if( tokens.empty( ) ) {
			throw SyntaxError( "unexpected end of token stream" );
		}

		if( tokens.front( )[ 0 ] >= 'A' && tokens.front( )[ 0 ] <= 'Z' || 
			tokens.front( )[ 0 ] >= 'a' && tokens.front( )[ 0 ] <= 'z' || 
			tokens.front( )[ 0 ] == '_' ) {
		} else {
			throw SyntaxError( "expected identifier, found \"" + tokens.front( ) + "\"" );
		}
		
		for( size_t i = 1; i < tokens.front( ).size( ); i++ ) {
			if( tokens.front( )[ i ] >= 'A' && tokens.front( )[ i ] <= 'Z' || 
				tokens.front( )[ i ] >= 'a' && tokens.front( )[ i ] <= 'z' ||
				tokens.front( )[ i ] >= '0' && tokens.front( )[ i ] <= '9' || 
				tokens.front( )[ i ] == '_' ) {
			} else {
				throw SyntaxError( "expected identifier, found \"" + tokens.front( ) + "\"" );
			}
		}
		
		std::string str = tokens.front( );

		tokens.pop_front( );
		
		return str;
	}

	std::string MatchStringConst( std::list< std::string > & tokens ) {
		if( tokens.empty( ) ) {
			throw SyntaxError( "unexpected end of token stream" );
		}

		if( tokens.front( )[ 0 ] == '"' ) {
		} else {
			throw SyntaxError( "expected string constant, found \"" + tokens.front( ) + "\"" );
		}
		
		for( size_t i = 1; i < tokens.front( ).size( ) - 1; i++ ) {
			if( tokens.front( )[ i ] >= ' ' && tokens.front( )[ i ] <= '~' ) {
				/*if( tokens.front( )[ i ] == '"' && tokens.front( )[ i - 1 ] != '\\' ) {
					throw SyntaxError( "expected string constant, found \"" + tokens.front( ) + "\"" );
				}*/
			} else {
				throw SyntaxError( "expected string constant, found \"" + tokens.front( ) + "\"" );
			}
		}
		
		//if( tokens.front( )[ tokens.front( ).size( ) - 1 ] == '"' && tokens.front( )[ tokens.front( ).size( ) - 2 ] != '\\' ) {
		if( tokens.front( )[ tokens.front( ).size( ) - 1 ] == '"' ) {
		} else {
			throw SyntaxError( "expected string constant, found \"" + tokens.front( ) + "\"" );
		}
		
		std::string str = tokens.front( );

		tokens.pop_front( );
		
		return str;
	}

	////////////////////////////////////////
	//
	////////////////////////////////////////

	Program ReadProgram( std::list< std::string > & tokens ) {
		return Program( ReadExprList( tokens ) );
	}

	ExprList ReadExprList( std::list< std::string > & tokens ) {
		if( tokens.empty( ) ) {
			return ExprList( );
		} else {
			if( tokens.front( ) == "}" ) {
				return ExprList( );
			} else {
				boost::shared_ptr< Expr > expr = ReadExpr( tokens );
				
				if( tokens.empty( ) ) {
					throw SyntaxError( "ReadExprList: unexpected end of token stream" );
					//return ExprList( expr );
				} else if( tokens.front( ) == ";" ) {
					Match( tokens, ";" );
					ExprList exprlist = ReadExprList( tokens );
					exprlist.push_front( expr );
					return exprlist;
				} else {
					throw SyntaxError( "ReadExprList: unexpected token " + tokens.front( ) );
				}
			}
		}
	}

	std::list< boost::shared_ptr< Expr > > ReadArgList( std::list< std::string > & tokens ) {
		boost::shared_ptr< Expr > expr = ReadExpr( tokens );
		
		if( tokens.empty( ) ) {
			throw SyntaxError( "ReadArgList: unexpected end of token stream" );
		} else if( tokens.front( ) == "," ) {
			Match( tokens, "," );
			std::list< boost::shared_ptr< Expr > > arglist = ReadArgList( tokens );
			arglist.push_front( expr );
			return arglist;
		} else if( tokens.front( ) == ")" ) {
			return std::list< boost::shared_ptr< Expr > >( 1, expr );
		} else {
			throw SyntaxError( "ReadArgList: unexpected token " + tokens.front( ) );
		}
	}

	std::list< std::string > ReadParamList( std::list< std::string > & tokens ) {
		std::string ident = MatchIdentifier( tokens );
		
		if( tokens.empty( ) ) {
			throw SyntaxError( "ReadParamList: unexpected end of token stream" );
		} else if( tokens.front( ) == "," ) {
			Match( tokens, "," );
			std::list< std::string > paramlist = ReadParamList( tokens );
			paramlist.push_front( ident );
			return paramlist;
		} else if( tokens.front( ) == ")" ) {
			return std::list< std::string >( 1, ident );
		} else {
			throw SyntaxError( "ReadParamList: unexpected token " + tokens.front( ) );
		}
	}

	boost::shared_ptr< Expr > ReadExpr( std::list< std::string > & tokens ) {
		if( tokens.empty( ) ) {
			throw SyntaxError( "ReadExpr: unexpected end of token stream" );
		} else {
			if( tokens.front( ) == "function" ) {
				return ReadFuncDecl( tokens );
			/*} else if( tokens.front( ) == "assign" ) {
				return ReadAssignExpr( tokens );*/
			} else if( tokens.front( ) == "if" ) {
				return ReadIfExpr( tokens );
			} else if( tokens.front( ) == "while" ) {
				return ReadWhileExpr( tokens );
			/*} else if( tokens.front( ) == "call" ) {
				return ReadFuncCall( tokens );*/
			} else if( tokens.front( ) == "false" || tokens.front( ) == "true" ) {
				return ReadBool( tokens );
			} else if( tokens.front( )[ 0 ] >= '0' && tokens.front( )[ 0 ] <= '9' || tokens.front( )[ 0 ] == '-' || tokens.front( )[ 0 ] == '+' ) {
				if( tokens.front( ).find( '.' ) == -1 ) {
					return ReadInteger( tokens );
				} else {
					return ReadFloat( tokens );
				}
			} else if( tokens.front( )[ 0 ] >= 'A' && tokens.front( )[ 0 ] <= 'Z' || tokens.front( )[ 0 ] >= 'a' && tokens.front( )[ 0 ] <= 'z' || tokens.front( )[ 0 ] == '_' ) {
				if( tokens.size( ) == 1 ) {
					throw SyntaxError( "ReadExpr: unexpected end of stream" );
				} else {
					std::list< std::string >::iterator nexttoken = tokens.begin( ); ++nexttoken;
					if( *nexttoken == "(" ) {
						return ReadFuncCall( tokens );
					} else if( *nexttoken == "=" ) {
						return ReadAssignExpr( tokens );
					} else {
						return ReadIdent( tokens );
					}
				}
			} else if( tokens.front( )[ 0 ] == '"' ) {
				return ReadStringConst( tokens );
			} else {
				throw SyntaxError( "ReadExpr: unexpected token " + tokens.front( ) );
			}
		}
	}

	boost::shared_ptr< FuncDecl > ReadFuncDecl( std::list< std::string > & tokens ) {
		Match( tokens, "function" );
		Match( tokens, "(" );
		
		std::list< std::string > paramlist;
		if( tokens.empty( ) ) {
			throw SyntaxError( "ReadFuncDecl: unexpected end of token stream" );
		} else if( tokens.front( ) != ")" ) {
			paramlist = ReadParamList( tokens );
		}
		
		Match( tokens, ")" );
		Match( tokens, "{" );
		
		ExprList exprlist;
		if( tokens.empty( ) ) {
			throw SyntaxError( "ReadFuncDecl: unexpected end of token stream" );
		} else if( tokens.front( ) != "}" ) {
			exprlist = ReadExprList( tokens );
		}
		
		Match( tokens, "}" );
		
		return boost::shared_ptr< FuncDecl >( new FuncDecl( paramlist, exprlist ) );
	}

	boost::shared_ptr< AssignExpr > ReadAssignExpr( std::list< std::string > & tokens ) {
		//Match( tokens, "assign" );
		
		std::string ident = MatchIdentifier( tokens );
		
		Match( tokens, "=" );
		
		boost::shared_ptr< Expr > expr = ReadExpr( tokens );
		
		return boost::shared_ptr< AssignExpr >( new AssignExpr( ident, expr ) );
	}

	boost::shared_ptr< IfExpr > ReadIfExpr( std::list< std::string > & tokens ) {
		Match( tokens, "if" );
		Match( tokens, "(" );
		
		boost::shared_ptr< Expr > expr = ReadExpr( tokens );
		
		Match( tokens, ")" );
		Match( tokens, "{" );

		ExprList trueexpr = ReadExprList( tokens );
		
		Match( tokens, "}" );
		Match( tokens, "else" );
		Match( tokens, "{" );

		ExprList falseexpr = ReadExprList( tokens );
		
		Match( tokens, "}" );
		
		return boost::shared_ptr< IfExpr >( new IfExpr( expr, trueexpr, falseexpr ) );
	}

	boost::shared_ptr< WhileExpr > ReadWhileExpr( std::list< std::string > & tokens ) {
		Match( tokens, "while" );
		Match( tokens, "(" );
		
		boost::shared_ptr< Expr > expr = ReadExpr( tokens );
		
		Match( tokens, ")" );
		Match( tokens, "{" );

		ExprList exprlist = ReadExprList( tokens );
		
		Match( tokens, "}" );
		
		return boost::shared_ptr< WhileExpr >( new WhileExpr( expr, exprlist ) );
	}

	boost::shared_ptr< FuncCall > ReadFuncCall( std::list< std::string > & tokens ) {
		//Match( tokens, "call" );
		
		std::string ident = MatchIdentifier( tokens );
		
		Match( tokens, "(" );
		
		std::list< boost::shared_ptr< Expr > > arglist;
		if( tokens.empty( ) ) {
			throw SyntaxError( "ReadFuncCall: unexpected end of token stream" );
		} else if( tokens.front( ) != ")" ) {
			arglist = ReadArgList( tokens );		
		}
		
		Match( tokens, ")" );
		
		return boost::shared_ptr< FuncCall >( new FuncCall( ident, arglist ) );
	}

	boost::shared_ptr< IntegerExpr > ReadInteger( std::list< std::string > & tokens ) {
		std::string number = MatchNumber( tokens );
		
		return boost::shared_ptr< IntegerExpr >( new IntegerExpr( number ) );
	}

	boost::shared_ptr< FloatExpr > ReadFloat( std::list< std::string > & tokens ) {
		std::string number = MatchNumber( tokens );
		
		return boost::shared_ptr< FloatExpr >( new FloatExpr( number ) );
	}

	boost::shared_ptr< BoolExpr > ReadBool( std::list< std::string > & tokens ) {
		std::string number = MatchIdentifier( tokens );
		
		return boost::shared_ptr< BoolExpr >( new BoolExpr( number ) );
	}

	boost::shared_ptr< IdentExpr > ReadIdent( std::list< std::string > & tokens ) {
		std::string ident = MatchIdentifier( tokens );
		
		return boost::shared_ptr< IdentExpr >( new IdentExpr( ident ) );
	}

	boost::shared_ptr< StringConstExpr > ReadStringConst( std::list< std::string > & tokens ) {
		std::string str = MatchStringConst( tokens );
		
		return boost::shared_ptr< StringConstExpr >( new StringConstExpr( str ) );
	}

}
