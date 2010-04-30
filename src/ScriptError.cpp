#include <slib/ScriptError.h>

namespace slib {

	ScriptError::ScriptError( const std::string & error ) : error_( error ) {
	}

	std::string ScriptError::getError( ) const {
		return error_;
	}

}
