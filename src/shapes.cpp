#include <slib/shapes.h>

namespace slib {

	// input: xradius, yradius, zradius
	// output: vertex array, vertex count
	void genCube( float * & verts, int & nverts, float xradius, float yradius, float zradius ) {
		nverts = 6 * 6;
		verts = new float[ nverts * 3 ];

		// face 0
		verts[ 0 ] = xradius; verts[ 1 ] = -yradius; verts[ 2 ] = zradius;
		verts[ 3 ] = xradius; verts[ 4 ] = yradius; verts[ 5 ] = zradius;
		verts[ 6 ] = -xradius; verts[ 7 ] = yradius; verts[ 8 ] = zradius;

		verts[ 9 ] = -xradius; verts[ 10 ] = yradius; verts[ 11 ] = zradius;
		verts[ 12 ] = -xradius; verts[ 13 ] = -yradius; verts[ 14 ] = zradius;
		verts[ 15 ] = xradius; verts[ 16 ] = -yradius; verts[ 17 ] = zradius;

		// face 1
		verts[ 18 ] = -xradius; verts[ 19 ] = -yradius; verts[ 20 ] = zradius;
		verts[ 21 ] = -xradius; verts[ 22 ] = yradius; verts[ 23 ] = zradius;
		verts[ 24 ] = -xradius; verts[ 25 ] = yradius; verts[ 26 ] = -zradius;

		verts[ 27 ] = -xradius; verts[ 28 ] = yradius; verts[ 29 ] = -zradius;
		verts[ 30 ] = -xradius; verts[ 31 ] = -yradius; verts[ 32 ] = -zradius;
		verts[ 33 ] = -xradius; verts[ 34 ] = -yradius; verts[ 35 ] = zradius;

		// face 2
		verts[ 36 ] = -xradius; verts[ 37 ] = -yradius; verts[ 38 ] = -zradius;
		verts[ 39 ] = -xradius; verts[ 40 ] = yradius; verts[ 41 ] = -zradius;
		verts[ 42 ] = xradius; verts[ 43 ] = yradius; verts[ 44 ] = -zradius;

		verts[ 45 ] = xradius; verts[ 46 ] = yradius; verts[ 47 ] = -zradius;
		verts[ 48 ] = xradius; verts[ 49 ] = -yradius; verts[ 50 ] = -zradius;
		verts[ 51 ] = -xradius; verts[ 52 ] = -yradius; verts[ 53 ] = -zradius;

		// face 3
		verts[ 54 ] = xradius; verts[ 55 ] = -yradius; verts[ 56 ] = -zradius;
		verts[ 57 ] = xradius; verts[ 58 ] = yradius; verts[ 59 ] = -zradius;
		verts[ 60 ] = xradius; verts[ 61 ] = yradius; verts[ 62 ] = zradius;

		verts[ 63 ] = xradius; verts[ 64 ] = yradius; verts[ 65 ] = zradius;
		verts[ 66 ] = xradius; verts[ 67 ] = -yradius; verts[ 68 ] = zradius;
		verts[ 69 ] = xradius; verts[ 70 ] = -yradius; verts[ 71 ] = -zradius;

		// face 4
		verts[ 72 ] = xradius; verts[ 73 ] = yradius; verts[ 74 ] = zradius;
		verts[ 75 ] = xradius; verts[ 76 ] = yradius; verts[ 77 ] = -zradius;
		verts[ 78 ] = -xradius; verts[ 79 ] = yradius; verts[ 80 ] = -zradius;

		verts[ 81 ] = -xradius; verts[ 82 ] = yradius; verts[ 83 ] = -zradius;
		verts[ 84 ] = -xradius; verts[ 85 ] = yradius; verts[ 86 ] = zradius;
		verts[ 87 ] = xradius; verts[ 88 ] = yradius; verts[ 89 ] = zradius;

		// face 5
		verts[ 90 ] = xradius; verts[ 91 ] = -yradius; verts[ 92 ] = -zradius;
		verts[ 93 ] = xradius; verts[ 94 ] = -yradius; verts[ 95 ] = zradius;
		verts[ 96 ] = -xradius; verts[ 97 ] = -yradius; verts[ 98 ] = zradius;

		verts[ 99 ] = -xradius; verts[ 100 ] = -yradius; verts[ 101 ] = zradius;
		verts[ 102 ] = -xradius; verts[ 103 ] = -yradius; verts[ 104 ] = -zradius;
		verts[ 105 ] = xradius; verts[ 106 ] = -yradius; verts[ 107 ] = -zradius;
	}

	// input: radius, height, slices, stacks
	// output: vertex array, vertex count
	void genCylinder( float * & verts, int & nverts, float bottomradius, float topradius, float height, int slices, int stacks ) {
		verts = new float[ stacks * slices * 18 + ( slices - 2 ) * 9 * 2 ];
		nverts = stacks * slices * 6 + ( slices - 2 ) * 3 * 2;

		for( int i = 0; i < stacks; i++ ) {
			for( int j = 0; j < slices; j++ ) {
				float radius0 = ( 1.0f - (float)i / stacks ) * bottomradius + ( (float)i / stacks ) * topradius;
				float radius1 = ( 1.0f - (float)( i + 1 ) / stacks ) * bottomradius + ( (float)( i + 1 ) / stacks ) * topradius;

				float x0 = radius0 * cos( 2.0f * 3.1415926f * j / slices );
				float y0 = radius0 * sin( 2.0f * 3.1415926f * j / slices );
				float z0 = i * height / stacks;

				float x1 = radius0 * cos( 2.0f * 3.1415926f * ( j + 1 ) / slices );
				float y1 = radius0 * sin( 2.0f * 3.1415926f * ( j + 1 ) / slices );
				float z1 = i * height / stacks;

				float x2 = radius1 * cos( 2.0f * 3.1415926f * j / slices );
				float y2 = radius1 * sin( 2.0f * 3.1415926f * j / slices );
				float z2 = ( i + 1 ) * height / stacks;

				float x3 = radius1 * cos( 2.0f * 3.1415926f * ( j + 1 ) / slices );
				float y3 = radius1 * sin( 2.0f * 3.1415926f * ( j + 1 ) / slices );
				float z3 = ( i + 1 ) * height / stacks;

				verts[ i * slices * 18 + j * 18 + 0 ] = x2;
				verts[ i * slices * 18 + j * 18 + 1 ] = y2;
				verts[ i * slices * 18 + j * 18 + 2 ] = z2;

				verts[ i * slices * 18 + j * 18 + 3 ] = x0;
				verts[ i * slices * 18 + j * 18 + 4 ] = y0;
				verts[ i * slices * 18 + j * 18 + 5 ] = z0;

				verts[ i * slices * 18 + j * 18 + 6 ] = x1;
				verts[ i * slices * 18 + j * 18 + 7 ] = y1;
				verts[ i * slices * 18 + j * 18 + 8 ] = z1;

				verts[ i * slices * 18 + j * 18 + 9 ] = x1;
				verts[ i * slices * 18 + j * 18 + 10 ] = y1;
				verts[ i * slices * 18 + j * 18 + 11 ] = z1;

				verts[ i * slices * 18 + j * 18 + 12 ] = x3;
				verts[ i * slices * 18 + j * 18 + 13 ] = y3;
				verts[ i * slices * 18 + j * 18 + 14 ] = z3;

				verts[ i * slices * 18 + j * 18 + 15 ] = x2;
				verts[ i * slices * 18 + j * 18 + 16 ] = y2;
				verts[ i * slices * 18 + j * 18 + 17 ] = z2;
			}
		}

		// caps
		for( int j = 1; j < slices - 1; j++ ) {
			float x0 = bottomradius * cos( 2.0f * 3.1415926f * j / slices );
			float y0 = bottomradius * sin( 2.0f * 3.1415926f * j / slices );
			float x1 = bottomradius * cos( 2.0f * 3.1415926f * ( j + 1 ) / slices );
			float y1 = bottomradius * sin( 2.0f * 3.1415926f * ( j + 1 ) / slices );

			verts[ stacks * slices * 18 + ( j - 1 ) * 9 + 0 ] = bottomradius;
			verts[ stacks * slices * 18 + ( j - 1 ) * 9 + 1 ] = 0.0f;
			verts[ stacks * slices * 18 + ( j - 1 ) * 9 + 2 ] = 0.0f;

			verts[ stacks * slices * 18 + ( j - 1 ) * 9 + 3 ] = x1;
			verts[ stacks * slices * 18 + ( j - 1 ) * 9 + 4 ] = y1;
			verts[ stacks * slices * 18 + ( j - 1 ) * 9 + 5 ] = 0.0f;

			verts[ stacks * slices * 18 + ( j - 1 ) * 9 + 6 ] = x0;
			verts[ stacks * slices * 18 + ( j - 1 ) * 9 + 7 ] = y0;
			verts[ stacks * slices * 18 + ( j - 1 ) * 9 + 8 ] = 0.0f;
		}

		for( int j = 1; j < slices - 1; j++ ) {
			float x0 = topradius * cos( 2.0f * 3.1415926f * j / slices );
			float y0 = topradius * sin( 2.0f * 3.1415926f * j / slices );
			float x1 = topradius * cos( 2.0f * 3.1415926f * ( j + 1 ) / slices );
			float y1 = topradius * sin( 2.0f * 3.1415926f * ( j + 1 ) / slices );

			verts[ stacks * slices * 18 + ( slices - 2 ) * 9 + ( j - 1 ) * 9 + 0 ] = topradius;
			verts[ stacks * slices * 18 + ( slices - 2 ) * 9 + ( j - 1 ) * 9 + 1 ] = 0.0f;
			verts[ stacks * slices * 18 + ( slices - 2 ) * 9 + ( j - 1 ) * 9 + 2 ] = height;

			verts[ stacks * slices * 18 + ( slices - 2 ) * 9 + ( j - 1 ) * 9 + 3 ] = x0;
			verts[ stacks * slices * 18 + ( slices - 2 ) * 9 + ( j - 1 ) * 9 + 4 ] = y0;
			verts[ stacks * slices * 18 + ( slices - 2 ) * 9 + ( j - 1 ) * 9 + 5 ] = height;

			verts[ stacks * slices * 18 + ( slices - 2 ) * 9 + ( j - 1 ) * 9 + 6 ] = x1;
			verts[ stacks * slices * 18 + ( slices - 2 ) * 9 + ( j - 1 ) * 9 + 7 ] = y1;
			verts[ stacks * slices * 18 + ( slices - 2 ) * 9 + ( j - 1 ) * 9 + 8 ] = height;
		}
	}

	// input: radius, slices, stacks
	// output: vertex array, vertex count
	void genSphere( float * & verts, int & nverts, float radius, int slices, int stacks ) {
		verts = new float[ stacks * slices * 18 ];
		nverts = stacks * slices * 6;

		for( int i = 0; i < stacks; i++ ) {
			for( int j = 0; j < slices; j++ ) {
				float x0 = radius * cos( 2.0f * 3.1415926f * j / slices ) * sin( 3.1415926f * i / stacks );
				float y0 = radius * sin( 2.0f * 3.1415926f * j / slices ) * sin( 3.1415926f * i / stacks );
				float z0 = radius * cos( 3.1415926f * i / stacks );
				float x1 = radius * cos( 2.0f * 3.1415926f * ( j + 1 ) / slices ) * sin( 3.1415926f * i / stacks );
				float y1 = radius * sin( 2.0f * 3.1415926f * ( j + 1 ) / slices ) * sin( 3.1415926f * i / stacks );
				float z1 = radius * cos( 3.1415926f * i / stacks );
				float x2 = radius * cos( 2.0f * 3.1415926f * j / slices ) * sin( 3.1415926f * ( i + 1 ) / stacks );
				float y2 = radius * sin( 2.0f * 3.1415926f * j / slices ) * sin( 3.1415926f * ( i + 1 ) / stacks );
				float z2 = radius * cos( 3.1415926f * ( i + 1 ) / stacks );
				float x3 = radius * cos( 2.0f * 3.1415926f * ( j + 1 ) / slices ) * sin( 3.1415926f * ( i + 1 ) / stacks );
				float y3 = radius * sin( 2.0f * 3.1415926f * ( j + 1 ) / slices ) * sin( 3.1415926f * ( i + 1 ) / stacks );
				float z3 = radius * cos( 3.1415926f * ( i + 1 ) / stacks );

				verts[ i * slices * 18 + j * 18 + 0 ] = x0;
				verts[ i * slices * 18 + j * 18 + 1 ] = y0;
				verts[ i * slices * 18 + j * 18 + 2 ] = z0;

				verts[ i * slices * 18 + j * 18 + 3 ] = x2;
				verts[ i * slices * 18 + j * 18 + 4 ] = y2;
				verts[ i * slices * 18 + j * 18 + 5 ] = z2;

				verts[ i * slices * 18 + j * 18 + 6 ] = x3;
				verts[ i * slices * 18 + j * 18 + 7 ] = y3;
				verts[ i * slices * 18 + j * 18 + 8 ] = z3;

				verts[ i * slices * 18 + j * 18 + 9 ] = x3;
				verts[ i * slices * 18 + j * 18 + 10 ] = y3;
				verts[ i * slices * 18 + j * 18 + 11 ] = z3;

				verts[ i * slices * 18 + j * 18 + 12 ] = x1;
				verts[ i * slices * 18 + j * 18 + 13 ] = y1;
				verts[ i * slices * 18 + j * 18 + 14 ] = z1;

				verts[ i * slices * 18 + j * 18 + 15 ] = x0;
				verts[ i * slices * 18 + j * 18 + 16 ] = y0;
				verts[ i * slices * 18 + j * 18 + 17 ] = z0;
			}
		}
	}

	// input: inner radius, outer radius, slices, rings
	// output: vertex array, vertex count
	void genDisk( float * & verts, int & nverts, float innerradius, float outerradius, int slices, int rings ) {
		nverts = rings * slices * 6;
		verts = new float[ nverts * 3 ];

		for( int i = 0; i < rings; i++ ) {
			for( int j = 0; j < slices; j++ ) {
				float t0 = 2.0f * 3.1415926f * j / slices;
				float t1 = 2.0f * 3.1415926f * ( j + 1 ) / slices;

				float r = (float)i / rings, s = (float)( i + 1 ) / rings;
				float radius0 = ( 1.0f - r ) * innerradius + r * outerradius;
				float radius1 = ( 1.0f - s ) * innerradius + s * outerradius;

				float x0 = cos( t0 ) * radius0, y0 = sin( t0 ) * radius0;
				float x1 = cos( t1 ) * radius0, y1 = sin( t1 ) * radius0;
				float x2 = cos( t0 ) * radius1, y2 = sin( t0 ) * radius1;
				float x3 = cos( t1 ) * radius1, y3 = sin( t1 ) * radius1;

				verts[ i * slices * 18 + j * 18 + 0 ] = x0;
				verts[ i * slices * 18 + j * 18 + 1 ] = y0;
				verts[ i * slices * 18 + j * 18 + 2 ] = 0.0f;

				verts[ i * slices * 18 + j * 18 + 3 ] = x2;
				verts[ i * slices * 18 + j * 18 + 4 ] = y2;
				verts[ i * slices * 18 + j * 18 + 5 ] = 0.0f;

				verts[ i * slices * 18 + j * 18 + 6 ] = x3;
				verts[ i * slices * 18 + j * 18 + 7 ] = y3;
				verts[ i * slices * 18 + j * 18 + 8 ] = 0.0f;

				verts[ i * slices * 18 + j * 18 + 9 ] = x3;
				verts[ i * slices * 18 + j * 18 + 10 ] = y3;
				verts[ i * slices * 18 + j * 18 + 11 ] = 0.0f;

				verts[ i * slices * 18 + j * 18 + 12 ] = x1;
				verts[ i * slices * 18 + j * 18 + 13 ] = y1;
				verts[ i * slices * 18 + j * 18 + 14 ] = 0.0f;

				verts[ i * slices * 18 + j * 18 + 15 ] = x0;
				verts[ i * slices * 18 + j * 18 + 16 ] = y0;
				verts[ i * slices * 18 + j * 18 + 17 ] = 0.0f;
			}
		}
	}

}
