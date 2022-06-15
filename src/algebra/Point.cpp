#include "Point.hpp"
#include "Vector.hpp"


Point::Point(): x(0), y(0), z(0) {}
Point::Point(float const x, float const y, float const z): x(x), y(y), z(z) {}

Point& Point::operator+=(Vector const& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Point& Point::operator-=(Vector const& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Point operator+(Point const& a, Vector const& u) {
	return Point(a.x + u.x, a.y + u.y, a.z + u.z);
}

Point operator-(Point const& a, Vector const& u) {
	return Point(a.x - u.x, a.y - u.y, a.z - u.z);
}

Vector operator-(Point const& a, Point const& b) {
	return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}