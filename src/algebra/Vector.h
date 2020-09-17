#ifndef VECTOR_H_
#define VECTOR_H_

#include <cmath>


class Vector {

public:

	float x;
	float y;
	float z;

	Vector(): x(0), y(0), z(0) {}
	Vector(float x, float y, float z): x(x), y(y), z(z) {}

	inline Vector& operator+=(Vector const& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	inline Vector& operator-=(Vector const& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	inline Vector& operator*=(float const& t) {
		x *= t;
		y *= t;
		z *= t;
		return *this;
	}

	inline Vector& operator/=(float const& t) {
		x /= t;
		y /= t;
		z /= t;
		return *this;
	}

	inline float lengthSquared() const {
		return x*x + y*y + z*z;
	}

	inline float length() const {
		return sqrt(lengthSquared());
	}

	inline Vector unit() const {
		float l = length();
		return Vector(x / l, y / l, z / l);
	}

	inline static Vector random() {

	}

};

inline Vector operator+(Vector const& u, Vector const& v) {
	return Vector(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline Vector operator-(Vector const& u, Vector const& v) {
	return Vector(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline Vector operator*(float const& t, Vector const& u) {
	return Vector(t * u.x, t * u.y, t * u.z);
}

inline Vector operator*(Vector const& u, float const& t) {
	return t * u;
}

inline Vector operator/(Vector const& u, float const& t) {
	return (1/t) * u;
}

float dot(Vector const& u, Vector const& v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

Vector cross(Vector const& u, Vector const& v) {
    return Vector(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}


#endif
