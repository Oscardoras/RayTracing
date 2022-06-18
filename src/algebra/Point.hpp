#ifndef __ALGEBRA_POINT_H__
#define __ALGEBRA_POINT_H__

#include "Spectrum.hpp"


struct Point {

	float x;
	float y;
	float z;

	inline Point(): x(0), y(0), z(0) {}
	inline Point(float const x, float const y, float const z): x(x), y(y), z(z) {}

	inline Point& operator+=(Vector const& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	inline Point& operator-=(Vector const& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

};

inline Point operator+(Point const& a, Vector const& u) {
	return Point(a.x + u.x, a.y + u.y, a.z + u.z);
}

inline Point operator-(Point const& a, Vector const& u) {
	return Point(a.x - u.x, a.y - u.y, a.z - u.z);
}

inline Vector operator-(Point const& a, Point const& b) {
	return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}


#endif
