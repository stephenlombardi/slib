////////////////////////////////////////
// Author: Steve Lombardi
// Date: April 3, 2010
// Description: Functions to tokenize input
////////////////////////////////////////

#ifndef SLIB_TOKENIZER_H
#define SLIB_TOKENIZER_H

#include <iostream>
#include <list>
#include <string>

namespace slib {

	// thrown when there is a tokenizer error
	class TokenizerError {
	public:
		TokenizerError( const std::string & error );
		std::string GetError( );
	private:
		std::string error_;
	};

	std::string GetNumber( std::istream & stream );
	std::string GetIdentifier( std::istream & stream );
	std::string GetStringConst( std::istream & stream );
	std::string GetComment( std::istream & stream );

	// reads the stream for the next token
	std::string GetToken( std::istream & stream );
	
}

#endif
