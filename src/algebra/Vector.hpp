#ifndef ALGEBRA_VECTOR_H_
#define ALGEBRA_VECTOR_H_

#include <cmath>

#include "../MathUtils.cpp"
class Point;


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

	inline Point toPoint() const;

	inline static Vector random(double min, double max) {
		return Vector(random_double(min, max), random_double(min, max), random_double(min, max));
	}

	inline static Vector random() {
		return random(-1., 1.);
	}

	inline static Vector randomUnit() {
		Vector v = random(-1., 1.);
		float l = 0.5 + v.lengthSquared()/2;
		return Vector(v.x / l, v.y / l, v.z / l);
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
inline Vector operator-(Vector const& v) {
	return -1*v;
}

inline float operator*(Vector const& u, Vector const& v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline Vector operator/(Vector const& u, float const& t) {
	return (1/t) * u;
}

inline Vector cross(Vector const& u, Vector const& v) {
    return Vector(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}


#endif
