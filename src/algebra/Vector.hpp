#ifndef __ALGEBRA_VECTOR_H__
#define __ALGEBRA_VECTOR_H__

#include "../MathUtils.hpp"

struct Color;
struct Spectrum;


struct Vector {

	float x;
	float y;
	float z;

	inline Vector(): x(0), y(0), z(0) {}
	inline Vector(float const x, float const y, float const z): x(x), y(y), z(z) {}

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

	inline Vector& operator*=(float const t) {
		x *= t;
		y *= t;
		z *= t;
		return *this;
	}

	inline Vector& operator/=(float const t) {
		operator*=(1/t);
		return *this;
	}

	inline float lengthSquared() const {
		return x*x + y*y + z*z;
	}

	inline float length() const {
		return sqrt(lengthSquared());
	}

	inline Vector unit() const {
		float inv = 1/length();
		return Vector(x * inv, y * inv, z * inv);
	}

	inline Color toColor() const;

	inline Spectrum toSpectrum() const;

	inline static Vector randomUnit() {
		Vector v = Vector(random_double(-1, 1), random_double(-1, 1), random_double(-1, 1));
		float l = 0.5 * (1 + v.lengthSquared());
		return v /= l;
	}

};

inline Vector operator+(Vector const& u, Vector const& v) {
	return Vector(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline Vector operator-(Vector const& u, Vector const& v) {
	return Vector(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline Vector operator*(float const t, Vector const& u) {
	return Vector(t * u.x, t * u.y, t * u.z);
}

inline Vector operator*(Vector const& u, float const t) {
	return t * u;
}

inline Vector operator-(Vector const& v) {
	return -1*v;
}

inline float operator*(Vector const& u, Vector const& v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline Vector cross(Vector const& u, Vector const& v) {
    return Vector(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

inline Vector operator/(Vector const& u, float const t) {
	return (1/t) * u;
}


#endif
