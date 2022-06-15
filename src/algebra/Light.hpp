#ifndef __ALGEBRA_LIGHT_H__
#define __ALGEBRA_LIGHT_H__

#include <vector>

#include "Spectrum.hpp"


struct Smooth {

	int id;
	int radius;
	Spectrum smoothing;
	Spectrum accurate;

	Smooth(int const id, int const radius, Spectrum const& smoothing, Spectrum const& accurate);

};


struct Light {

	std::vector<Smooth> smoothings;

	Light();
	Light(int const id, int const radius, Spectrum const& albedo, Spectrum const& light);
	Light(Smooth const& smooth);
	Light(Spectrum const& spectrum);

	Light& operator+=(Light const& l);

	Light& operator*=(float const t);

	Light& operator*=(Spectrum const& s);

	Light& operator/=(float const t);

	Light& addId(int const id);

	Spectrum compute() const;

};

Light operator+(Light const& l1, Light const& l2);

Light operator*(float const t, Light const& l);

Light operator*(Light const& l, float const t);

Light operator*(Light const& l, Spectrum const& s);

Light operator*(Spectrum const& s, Light const& l);

Light operator/(Light const& l, float const t);


#endif
