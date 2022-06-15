#ifndef __ALGEBRA_SPECTRUM_H__
#define __ALGEBRA_SPECTRUM_H__

struct Color;
struct Vector;


struct Spectrum {

	float r;
	float g;
	float b;

	Spectrum();
	Spectrum(float const r, float const g, float const b);

	Spectrum& operator+=(Spectrum const& c);

	Spectrum& operator-=(Spectrum const& c);

	Spectrum& operator*=(float const t);

	Spectrum& operator*=(Spectrum const& c);

	Spectrum& operator/=(float const t);

	Spectrum& operator/=(Spectrum const& c);

	float getIntensity() const;

	bool isNull() const;

	Color toColor() const;

	Vector toVector() const;

	static Spectrum white();

};

Spectrum operator+(Spectrum const& c, Spectrum const& d);

Spectrum operator-(Spectrum const& c, Spectrum const& d);

Spectrum operator*(float const t, Spectrum const& c);

Spectrum operator-(Spectrum const& s);

Spectrum operator*(Spectrum const& c, float const t);

Spectrum operator*(Spectrum const& c, Spectrum const& d);

Spectrum operator/(Spectrum const& c, float const t);

Spectrum operator/(Spectrum const& c, Spectrum const& d);


#endif
