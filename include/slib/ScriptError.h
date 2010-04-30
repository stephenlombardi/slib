#ifndef SLIB_SCRIPTERROR_H
#define SLIB_SCRIPTERROR_H

#include <string>

namespace slib {

	class ScriptError {
	public:
		ScriptError( const std::string & error );
		std::string getError( ) const;
	private:
		std::string error_;
	};

}

#endif