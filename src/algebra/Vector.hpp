#ifndef ALGEBRA_VECTOR_H_
#define ALGEBRA_VECTOR_H_

#include <cmath>

#include "../MathUtils.cpp"
class Color;


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

	inline Color toColor() const;

	inline static Vector random() {
		return Vector(random_double(-1, 1), random_double(-1, 1), random_double(-1, 1));
	}

	inline static Vector randomUnit() {
		Vector v = Vector::random();
		float l = 0.5 * (1 + v.lengthSquared());
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

inline Vector cross(Vector const& u, Vector const& v) {
    return Vector(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

inline Vector operator/(Vector const& u, float const& t) {
	return (1/t) * u;
}


class Transformation {

public:

	std::shared_ptr<Transformation> transformation;

	Transformation(std::shared_ptr<Transformation> transformation = nullptr): transformation(transformation) {}

	virtual Vector transform(Vector const& vector) const {
		if (transformation == nullptr) return vector;
		else return transformation->transform(vector);
	}

};


#endif
