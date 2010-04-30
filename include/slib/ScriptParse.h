#ifndef SLIB_SCRIPTPARSE_H
#define SLIB_SCRIPTPARSE_H

#include <list>
#include <string>

#include <slib/ScriptAST.h>

namespace slib {

	// thrown when there is a syntax error
	class SyntaxError {
	public:
		SyntaxError( const std::string & error );
		std::string GetError( );
	private:
		std::string error_;
	};

	// throws an error if the current token (tokens.front()) does not match the string
	// otherwise, advances the token stream
	void Match( std::list< std::string > & tokens, const std::string & str );

	// throws an error if the current token (tokens.front()) is not a number
	// otherwise, advances the token stream
	std::string MatchNumber( std::list< std::string > & tokens );
	std::string MatchIdentifier( std::list< std::string > & tokens );
	std::string MatchStringConst( std::list< std::string > & tokens );

	Program ReadProgram( std::list< std::string > & tokens );
	ExprList ReadExprList( std::list< std::string > & tokens );
	std::list< boost::shared_ptr< Expr > > ReadArgList( std::list< std::string > & tokens );
	std::list< std::string > ReadParamList( std::list< std::string > & tokens );
	boost::shared_ptr< Expr > ReadExpr( std::list< std::string > & tokens );
	boost::shared_ptr< FuncDecl > ReadFuncDecl( std::list< std::string > & tokens );
	boost::shared_ptr< AssignExpr > ReadAssignExpr( std::list< std::string > & tokens );
	boost::shared_ptr< IfExpr > ReadIfExpr( std::list< std::string > & tokens );
	boost::shared_ptr< WhileExpr > ReadWhileExpr( std::list< std::string > & tokens );
	boost::shared_ptr< FuncCall > ReadFuncCall( std::list< std::string > & tokens );
	boost::shared_ptr< IntegerExpr > ReadInteger( std::list< std::string > & tokens );
	boost::shared_ptr< FloatExpr > ReadFloat( std::list< std::string > & tokens );
	boost::shared_ptr< BoolExpr > ReadBool( std::list< std::string > & tokens );
	boost::shared_ptr< IdentExpr > ReadIdent( std::list< std::string > & tokens );
	boost::shared_ptr< StringConstExpr > ReadStringConst( std::list< std::string > & tokens );

}

#endif