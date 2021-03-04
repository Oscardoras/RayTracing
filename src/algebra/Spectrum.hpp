#ifndef ALGEBRA_SPECTRUM_H_
#define ALGEBRA_SPECTRUM_H_

#include "Color.hpp"


class Spectrum {

public:

	float r;
	float g;
	float b;

	Spectrum(): r(0), g(0), b(0) {}
	Spectrum(float r, float g, float b): r(r), g(g), b(b) {}

	inline Spectrum& operator+=(Spectrum const& c) {
		r += c.r;
		g += c.g;
		b += c.b;
		return *this;
	}

	inline Spectrum& operator-=(Spectrum const& c) {
		r -= c.r;
		g -= c.g;
		b -= c.b;
		return *this;
	}

	inline Spectrum& operator*=(float const& t) {
		r *= t;
		g *= t;
		b *= t;
		return *this;
	}

	inline Spectrum& operator*=(Spectrum const& c) {
		r *= c.r;
		g *= c.g;
		b *= c.b;
		return *this;
	}

	inline Spectrum& operator/=(float const& t) {
		r /= t;
		g /= t;
		b /= t;
		return *this;
	}

	inline float getIntensity() const {
		return r+g+b;
	}

	inline Color toColor() const {
		return Color(r, g, b);
	}

};

inline Spectrum operator+(Spectrum const& c, Spectrum const& d) {
	return Spectrum(c.r + d.r, c.g + d.g, c.b + d.b);
}

inline Spectrum operator-(Spectrum const& c, Spectrum const& d) {
	return Spectrum(c.r - d.r, c.g - d.g, c.b - d.b);
}

inline Spectrum operator*(float const& t, Spectrum const& c) {
	return Spectrum(t * c.r, t * c.g, t * c.b);
}

inline Spectrum operator*(Spectrum const& c, float const& t) {
	return t * c;
}

inline Spectrum operator*(Spectrum const& c, Spectrum const& d) {
	return Spectrum(c.r * d.r, c.g * d.g, c.b * d.b);
}

inline Spectrum operator/(Spectrum const& c, float const& t) {
	return c * (1/t);
}


inline Spectrum Color::toSpectrum() const {
	return Spectrum(r, g, b);
}


#endif
