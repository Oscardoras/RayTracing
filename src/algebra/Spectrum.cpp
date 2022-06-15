#include "Color.hpp"
#include "Spectrum.hpp"
#include "Vector.hpp"

#include "../MathUtils.hpp"


Spectrum::Spectrum(): r(0), g(0), b(0) {}

Spectrum::Spectrum(float const r, float const g, float const b): r(r), g(g), b(b) {}

Spectrum& Spectrum::operator+=(Spectrum const& c) {
	r += c.r;
	g += c.g;
	b += c.b;
	return *this;
}

Spectrum& Spectrum::operator-=(Spectrum const& c) {
	r -= c.r;
	g -= c.g;
	b -= c.b;
	return *this;
}

Spectrum& Spectrum::operator*=(float const t) {
	r *= t;
	g *= t;
	b *= t;
	return *this;
}

Spectrum& Spectrum::operator*=(Spectrum const& c) {
	r *= c.r;
	g *= c.g;
	b *= c.b;
	return *this;
}

Spectrum& Spectrum::operator/=(float const t) {
	operator*=(1/t);
	return *this;
}

Spectrum& Spectrum::operator/=(Spectrum const& c) {
	r /= c.r;
	g /= c.g;
	b /= c.b;
	return *this;
}

float Spectrum::getIntensity() const {
	return (r+g+b)/3;
}

bool Spectrum::isNull() const {
	return r <= 0. && g <= 0. && b <= 0.;
}

Color Spectrum::toColor() const {
	return Color(r, g, b);
}

Vector Spectrum::toVector() const {
	return toColor().toVector();
}

Spectrum Spectrum::white() {
	return Spectrum(1,1,1);
}

Spectrum operator+(Spectrum const& c, Spectrum const& d) {
	return Spectrum(c.r + d.r, c.g + d.g, c.b + d.b);
}

Spectrum operator-(Spectrum const& c, Spectrum const& d) {
	return Spectrum(c.r - d.r, c.g - d.g, c.b - d.b);
}

Spectrum operator*(float const t, Spectrum const& c) {
	return Spectrum(t * c.r, t * c.g, t * c.b);
}

Spectrum operator-(Spectrum const& s) {
	return -1 * s;
}

Spectrum operator*(Spectrum const& c, float const t) {
	return t * c;
}

Spectrum operator*(Spectrum const& c, Spectrum const& d) {
	return Spectrum(c.r * d.r, c.g * d.g, c.b * d.b);
}

Spectrum operator/(Spectrum const& c, float const t) {
	return c * (1/t);
}

Spectrum operator/(Spectrum const& c, Spectrum const& d) {
	return Spectrum(c.r / d.r, c.g / d.g, c.b / d.b);
}