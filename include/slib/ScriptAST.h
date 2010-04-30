#ifndef SLIB_SCRIPTAST_H
#define SLIB_SCRIPTAST_H

#include <algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <sstream>
#include <string>

#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <slib/ScriptError.h>

namespace slib {

	typedef std::map< std::string, boost::any > EnvT;
	typedef boost::function< boost::any ( EnvT &, const std::list< boost::any > & ) > ScriptFuncT;

	class Expr {
	public:
		virtual ~Expr( );
		virtual boost::any eval( EnvT & env ) const = 0;
	private:
	};

	class ExprList {
	public:
		ExprList( );
		ExprList( boost::shared_ptr< Expr > expr );
		ExprList( const std::list< boost::shared_ptr< Expr > > & exprlist );
		void push_front( boost::shared_ptr< Expr > expr );
		boost::any eval( EnvT & env ) const;
	private:
		std::list< boost::shared_ptr< Expr > > exprlist_;
	};

	class Program {
	public:
		Program( );
		Program( const ExprList & exprlist );
		void eval( EnvT & env ) const;
	private:
		ExprList exprlist_;
	};

	class FuncAdapter {
	public:
		//FuncAdapter( const std::list< std::string > & paramlist, const ExprList & exprlist );
		FuncAdapter( const std::list< std::string > & paramlist, const ExprList & exprlist, const EnvT & env );
		boost::any operator( )( EnvT & env, const std::list< boost::any > & params ) const;
	private:
		std::list< std::string > paramlist_;
		ExprList exprlist_;
		EnvT env_;
	};

	class FuncDecl : public Expr {
	public:
		FuncDecl( const std::list< std::string > & paramlist, const ExprList & exprlist );
		boost::any eval( EnvT & env ) const;
	private:
		std::list< std::string > paramlist_;
		ExprList exprlist_;
	};

	class AssignExpr : public Expr {
	public:
		AssignExpr( const std::string & ident, boost::shared_ptr< Expr > expr );
		boost::any eval( EnvT & env ) const;
	private:
		std::string ident_;
		boost::shared_ptr< Expr > expr_;
	};

	class IfExpr : public Expr {
	public:
		IfExpr( boost::shared_ptr< Expr > expr, const ExprList & trueexpr, const ExprList & falseexpr );
		boost::any eval( EnvT & env ) const;
	private:
		boost::shared_ptr< Expr > expr_;
		ExprList trueexpr_, falseexpr_;
	};

	class WhileExpr : public Expr {
	public:
		WhileExpr( boost::shared_ptr< Expr > expr, const ExprList & exprlist );
		boost::any eval( EnvT & env ) const;
	private:
		boost::shared_ptr< Expr > expr_;
		ExprList exprlist_;
	};

	class FuncCall : public Expr {
	public:
		FuncCall( const std::string & fname, const std::list< boost::shared_ptr< Expr > > & arglist );
		boost::any eval( EnvT & env ) const;
	private:
		std::string fname_;
		std::list< boost::shared_ptr< Expr > > arglist_;
	};

	class IntegerExpr : public Expr {
	public:
		IntegerExpr( const std::string & number );
		boost::any eval( EnvT & env ) const;
	private:
		int number_;
	};

	class FloatExpr : public Expr {
	public:
		FloatExpr( const std::string & number );
		boost::any eval( EnvT & env ) const;
	private:
		float number_;
	};

	class BoolExpr : public Expr {
	public:
		BoolExpr( const std::string & b );
		boost::any eval( EnvT & env ) const;
	private:
		bool b_;
	};

	class IdentExpr : public Expr {
	public:
		IdentExpr( const std::string & ident );
		boost::any eval( EnvT & env ) const;
	private:
		std::string ident_;
	};

	class StringConstExpr : public Expr {
	public:
		StringConstExpr( const std::string & str );
		boost::any eval( EnvT & env ) const;
	private:
		std::string str_;
	};

}

#endif