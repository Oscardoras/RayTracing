#include "Color.hpp"
#include "Spectrum.hpp"
#include "Vector.hpp"

#include "../MathUtils.hpp"


Color::Color(): r(0), g(0), b(0) {}

Color::Color(float const r, float const g, float const b): r(r), g(g), b(b) {}

Color& Color::operator+=(Color const& c) {
	r += c.r;
	g += c.g;
	b += c.b;
	return *this;
}

Color& Color::operator-=(Color const& c) {
	r -= c.r;
	g -= c.g;
	b -= c.b;
	return *this;
}

Color& Color::operator*=(float const t) {
	r *= t;
	g *= t;
	b *= t;
	return *this;
}

Color& Color::operator*=(Color const& c) {
	r *= c.r;
	g *= c.g;
	b *= c.b;
	return *this;
}

Color& Color::operator/=(float const t) {
	operator*=(1/t);
	return *this;
}

float Color::toBlackAndWhite() const {
	return (r+g+b)/3;
}

Spectrum Color::toSpectrum() const {
	return Spectrum(r, g, b);
}

Vector Color::toVector() const {
	return Vector(r, g, b);
}

Color Color::white() {
	return Color(1,1,1);
}

Color Color::median(std::vector<Color> & colors) {
	std::vector<float> vr;
	std::vector<float> vg;
	std::vector<float> vb;
	for (Color & color : colors) {
		vr.push_back(color.r);
		vg.push_back(color.g);
		vb.push_back(color.b);
	}
	return Color(fmedian(vr), fmedian(vg), fmedian(vb));
}

Color operator+(Color const& c, Color const& d) {
	return Color(c.r + d.r, c.g + d.g, c.b + d.b);
}

Color operator-(Color const& c, Color const& d) {
	return Color(c.r - d.r, c.g - d.g, c.b - d.b);
}

Color operator*(float const t, Color const& c) {
	return Color(t * c.r, t * c.g, t * c.b);
}

Color operator*(Color const& c, float const t) {
	return t * c;
}

Color operator-(Color const& c) {
	return -1 * c;
}

Color operator*(Color const& c, Color const& d) {
	return Color(c.r * d.r, c.g * d.g, c.b * d.b);
}

Color operator/(Color const& c, float const t) {
	return c * (1/t);
}