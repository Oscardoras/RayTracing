#include <cmath>

#include "Color.hpp"
#include "Spectrum.hpp"
#include "Vector.hpp"

#include "../MathUtils.hpp"


Vector::Vector(): x(0), y(0), z(0) {}

Vector::Vector(float const x, float const y, float const z): x(x), y(y), z(z) {}

Vector& Vector::operator+=(Vector const& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector& Vector::operator-=(Vector const& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector& Vector::operator*=(float const t) {
	x *= t;
	y *= t;
	z *= t;
	return *this;
}

Vector& Vector::operator/=(float const t) {
	operator*=(1/t);
	return *this;
}

float Vector::lengthSquared() const {
	return x*x + y*y + z*z;
}

float Vector::length() const {
	return sqrt(lengthSquared());
}

Vector Vector::unit() const {
	float inv = 1/length();
	return Vector(x * inv, y * inv, z * inv);
}

Color Vector::toColor() const {
	return Color(x, y, z);
}

Spectrum Vector::toSpectrum() const {
	return Spectrum(x, y, z);
}

Vector Vector::random() {
	return Vector(random_double(-1, 1), random_double(-1, 1), random_double(-1, 1));
}

Vector Vector::randomUnit() {
	Vector v = Vector::random();
	float l = 0.5 * (1 + v.lengthSquared());
	return v /= l;
}

Vector operator+(Vector const& u, Vector const& v) {
	return Vector(u.x + v.x, u.y + v.y, u.z + v.z);
}

Vector operator-(Vector const& u, Vector const& v) {
	return Vector(u.x - v.x, u.y - v.y, u.z - v.z);
}

Vector operator*(float const t, Vector const& u) {
	return Vector(t * u.x, t * u.y, t * u.z);
}

Vector operator*(Vector const& u, float const t) {
	return t * u;
}

Vector operator-(Vector const& v) {
	return -1*v;
}

float operator*(Vector const& u, Vector const& v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

Vector cross(Vector const& u, Vector const& v) {
    return Vector(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

Vector operator/(Vector const& u, float t) {
	return (1/t) * u;
}