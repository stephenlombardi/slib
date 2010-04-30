#include <limits>

#include <slib/kdTree.hpp>
#include <slib/Matrix.hpp>

using namespace slib;

class kdTreePrimIntersection {
public:
	bool Intersection( ) const {
		return false;
	}

	float RayDistance( ) const {
		return std::numeric_limits< float >::infinity( );
	}
private:
};

class kdTreePrim {
public:
	float LeastDistance( const kd::Plane< 3, float > & plane ) const {
		return std::numeric_limits< float >::infinity( );
	}

	float GreatestDistance( const kd::Plane< 3, float > & plane ) const {
		return std::numeric_limits< float >::infinity( );
	}

	template< class OffsetCalcT >
	kdTreePrimIntersection IntersectionLine( const vec3f & start, const vec3f & end, const OffsetCalcT & offsetcalc ) const {
		return kdTreePrimIntersection( );
	}
private:
};

int main( ) {
	kd::Tree< 3, kdTreePrimIntersection, kdTreePrim > kdtree;
	return 0;
}
