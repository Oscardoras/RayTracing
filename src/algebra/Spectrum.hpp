#ifndef __ALGEBRA_SPECTRUM_H__
#define __ALGEBRA_SPECTRUM_H__

#include "Color.hpp"


struct Spectrum {

	float r;
	float g;
	float b;

	inline Spectrum(): r(0), g(0), b(0) {}
	inline Spectrum(float const r, float const g, float const b): r(r), g(g), b(b) {}

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

	inline Spectrum& operator*=(float const t) {
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

	inline Spectrum& operator/=(float const t) {
		operator*=(1/t);
		return *this;
	}

	inline Spectrum& operator/=(Spectrum const& c) {
		r /= c.r;
		g /= c.g;
		b /= c.b;
		return *this;
	}

	inline float getIntensity() const {
		return (r+g+b)/3;
	}

	inline bool isNull() const {
		return r <= 0. && g <= 0. && b <= 0.;
	}

	inline Color toColor() const {
		return Color(r, g, b);
	}

	inline Vector toVector() const {
		return toColor().toVector();
	}

	inline static Spectrum white() {
		return Spectrum(1,1,1);
	}

};

inline Spectrum operator+(Spectrum const& c, Spectrum const& d) {
	return Spectrum(c.r + d.r, c.g + d.g, c.b + d.b);
}

inline Spectrum operator-(Spectrum const& c, Spectrum const& d) {
	return Spectrum(c.r - d.r, c.g - d.g, c.b - d.b);
}

inline Spectrum operator*(float const t, Spectrum const& c) {
	return Spectrum(t * c.r, t * c.g, t * c.b);
}

inline Spectrum operator-(Spectrum const& s) {
	return -1 * s;
}

inline Spectrum operator*(Spectrum const& c, float const t) {
	return t * c;
}

inline Spectrum operator*(Spectrum const& c, Spectrum const& d) {
	return Spectrum(c.r * d.r, c.g * d.g, c.b * d.b);
}

inline Spectrum operator/(Spectrum const& c, float const t) {
	return c * (1/t);
}

inline Spectrum operator/(Spectrum const& c, Spectrum const& d) {
	return Spectrum(c.r / d.r, c.g / d.g, c.b / d.b);
}


inline Spectrum Vector::toSpectrum() const {
	return Spectrum(x, y, z);
}

inline Spectrum Color::toSpectrum() const {
	return Spectrum(r, g, b);
}


#endif
