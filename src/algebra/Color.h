#ifndef COLOR_H_
#define COLOR_H_

#include <iostream>


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

	inline Color& operator/=(float const& t) {
		r /= t;
		g /= t;
		b /= t;
		return *this;
	}

};

inline Color operator+(Color const& u, Color const& v) {
	return Color(u.r + v.r, u.g + v.g, u.b + v.b);
}

inline Color operator-(Color const& u, Color const& v) {
	return Color(u.r - v.r, u.g - v.g, u.b - v.b);
}

inline Color operator*(float const& t, Color const& u) {
	return Color(t * u.r, t * u.g, t * u.b);
}

inline Color operator*(Color const& u, float const& t) {
	return t * u;
}

inline Color operator/(Color const& u, float const& t) {
	return u * (1/t);
}

void writeColor(std::ostream &out, Color const& color) {
    out << static_cast<int>(255.999 * color.r) << ' '
        << static_cast<int>(255.999 * color.g) << ' '
        << static_cast<int>(255.999 * color.b) << '\n';
}


#endif
