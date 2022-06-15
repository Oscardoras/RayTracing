#ifndef __ALGEBRA_VECTOR_H__
#define __ALGEBRA_VECTOR_H__

struct Color;
struct Spectrum;


struct Vector {

	float x;
	float y;
	float z;

	Vector();
	Vector(float const x, float const y, float const z);

	Vector& operator+=(Vector const& v);

	Vector& operator-=(Vector const& v);

	Vector& operator*=(float const t);

	Vector& operator/=(float const t);

	float lengthSquared() const;

	float length() const;

	Vector unit() const;

	Color toColor() const;

	Spectrum toSpectrum() const;

	static Vector random();

	static Vector randomUnit();

};

Vector operator+(Vector const& u, Vector const& v);

Vector operator-(Vector const& u, Vector const& v);

Vector operator*(float const t, Vector const& u);

Vector operator*(Vector const& u, float const t);

Vector operator-(Vector const& v);

float operator*(Vector const& u, Vector const& v);

Vector cross(Vector const& u, Vector const& v);

Vector operator/(Vector const& u, float const t);


#endif
