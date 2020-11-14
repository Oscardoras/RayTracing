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

	inline Color& operator/=(Color const& c) {
		r /= c.r;
		g /= c.g;
		b /= c.b;
		return *this;
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

inline Color operator*(Color const& c, Color const& d) {
	return Color(c.r * d.r, c.g * d.g, c.b * d.b);
}

inline Color operator/(Color const& c, float const& t) {
	return c * (1/t);
}

inline Color operator/(Color const& c, Color const& d) {
	return Color(c.r / d.r, c.g / d.g, c.b / d.b);
}

void writeColor(std::ostream &out, Color const& color) {
    out << static_cast<int>(255. * color.r) << ' '
        << static_cast<int>(255. * color.g) << ' '
        << static_cast<int>(255. * color.b) << '\n';
}


#endif
