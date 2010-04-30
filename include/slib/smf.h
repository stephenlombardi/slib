/****************************************************
* Author: Steve Lombardi
* Date: January, 2010
* Description: function to load SMF files
****************************************************/
#ifndef SLIB_SMF_H
#define SLIB_SMF_H

#include <cassert>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include <slib/Matrix.hpp>

namespace slib {

	void readSMF( const std::string & filename, int & outnverts, float * & outverts, float * & outcolors, float * & outnormals, int & outnindices, unsigned int * & outindices );

}

#endif