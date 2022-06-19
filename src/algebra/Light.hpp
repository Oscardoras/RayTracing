#ifndef __ALGEBRA_LIGHT_H__
#define __ALGEBRA_LIGHT_H__

#include <map>

#include "Spectrum.hpp"


struct Light {

	int id;
	int radius;
	Spectrum smooth;
	Spectrum accurate;

	inline Light(): id(0), radius(0) {}

	inline Light(int const id, int const radius, Spectrum const& smooth, Spectrum const& accurate):
		id(id), radius(radius), smooth(smooth), accurate(accurate) {}
	
	inline Light(void const * const ptr, int const radius, Spectrum const& smooth, Spectrum const& accurate):
		id((int) long(ptr)), radius(radius), smooth(smooth), accurate(accurate) {}

	inline Light(Spectrum const& spectrum):
		id(0), radius(0), smooth(Spectrum::white()), accurate(spectrum) {}

	inline Light& operator*=(float const t) {
		accurate *= t;
		return *this;
	}

	inline Light& operator*=(Spectrum const& spectrum) {
		accurate *= spectrum;
		return *this;
	}

	inline Light& operator/=(float const t) {
		operator*=(1/t);
		return *this;
	}

	inline Light& addId(int const id) {
		this->id += id;
		return *this;
	}

	inline Light& addId(void const * const ptr) {
		id += (int) long(ptr);
		return *this;
	}

	inline Spectrum compute() const {
		return smooth * accurate;
	}

};

inline Light operator*(float const t, Light const& l) {
	Light light(l);
	light *= t;
	return light;
}

inline Light operator*(Light const& l, float const t) {
	return t * l;
}

inline Light operator*(Light const& l, Spectrum const& s) {
	Light light(l);
	light *= s;
	return light;
}

inline Light operator*(Spectrum const& s, Light const& l) {
	return l * s;
}

inline Light operator/(Light const& l, float const t) {
	return l * (1/t);
}


#endif
