#ifndef ALGEBRA_COLOR_H_
#define ALGEBRA_COLOR_H_

#include "../MathUtils.cpp"
#include "Vector.hpp"
class Spectrum;


class Color {

public:

	float r;
	float g;
	float b;

	Color(): r(0), g(0), b(0) {}
	Color(float r, float g, float b): r(r), g(g), b(b) {}

	inline Color& operator+=(Color const& c) {
		r += c.r;
		g += c.g;
		b += c.b;
		return *this;
	}

	inline Color& operator-=(Color const& c) {
		r -= c.r;
		g -= c.g;
		b -= c.b;
		return *this;
	}

	inline Color& operator*=(float const& t) {
		r *= t;
		g *= t;
		b *= t;
		return *this;
	}

	inline Color& operator*=(Color const& c) {
		r *= c.r;
		g *= c.g;
		b *= c.b;
		return *this;
	}

	inline Color& operator/=(float const& t) {
		r /= t;
		g /= t;
		b /= t;
		return *this;
	}

	inline float toBlackAndWhite() const {
		return (r+g+b)/3;
	}

	inline Spectrum toSpectrum() const;

	inline Vector toVector() const {
		return Vector(r, g, b);
	}

};

inline Color operator+(Color const& c, Color const& d) {
	return Color(c.r + d.r, c.g + d.g, c.b + d.b);
}

inline Color operator-(Color const& c, Color const& d) {
	return Color(c.r - d.r, c.g - d.g, c.b - d.b);
}

inline Color operator*(float const& t, Color const& c) {
	return Color(t * c.r, t * c.g, t * c.b);
}

inline Color operator*(Color const& c, float const& t) {
	return t * c;
}

inline Color operator-(Color const& c) {
	return -1 * c;
}

inline Color operator*(Color const& c, Color const& d) {
	return Color(c.r * d.r, c.g * d.g, c.b * d.b);
}

inline Color operator/(Color const& c, float const& t) {
	return c * (1/t);
}

Color med(std::vector<Color> &array) {
	std::vector<float> vr;
	std::vector<float> vg;
	std::vector<float> vb;
	for (Color color : array) {
		vr.push_back(color.r);
		vg.push_back(color.g);
		vb.push_back(color.b);
	}
	return Color(med(vr), med(vg), med(vb));
}


inline Color Vector::toColor() const {
	return Color(x, y, z);
}


#endif
