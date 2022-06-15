#ifndef __ALGEBRA_COLOR_H__
#define __ALGEBRA_COLOR_H__

#include <vector>

struct Spectrum;
struct Vector;


struct Color {

	float r;
	float g;
	float b;

	Color();
	Color(float const r, float const g, float const b);

	Color& operator+=(Color const& c);

	Color& operator-=(Color const& c);

	Color& operator*=(float const t);

	Color& operator*=(Color const& c);

	Color& operator/=(float const t);

	float toBlackAndWhite() const;

	Spectrum toSpectrum() const;

	Vector toVector() const;

	static Color white();

	static Color median(std::vector<Color> & colors);

};

Color operator+(Color const& c, Color const& d);

Color operator-(Color const& c, Color const& d);

Color operator*(float const t, Color const& c);

Color operator*(Color const& c, float const t);

Color operator-(Color const& c);

Color operator*(Color const& c, Color const& d);

Color operator/(Color const& c, float const t);


#endif
