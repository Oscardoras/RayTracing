#ifndef ALGEBRA_POINT_H_
#define ALGEBRA_POINT_H_

#include "Vector.hpp"


class Point {

public:

	float x;
	float y;
	float z;

	Point(): x(0), y(0), z(0) {}
	Point(float x, float y, float z): x(x), y(y), z(z) {}

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

inline float operator*(Point const& a, Point const& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float operator*(Vector const& u, Point const& b) {
    return u.x * b.x + u.y * b.y + u.z * b.z;
}

inline float operator*(Point const& a, Vector const& v) {
    return a.x * v.x + a.y * v.y + a.z * v.z;
}


#endif
