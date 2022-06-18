#ifndef __ALGEBRA_LIGHT_H__
#define __ALGEBRA_LIGHT_H__

#include <map>

#include "Spectrum.hpp"


struct Smoothing {

	int radius;
	Spectrum smooth;
	Spectrum accurate;

	inline Smoothing(): radius(0), smooth(), accurate() {}

	inline Smoothing(int const radius, Spectrum const& smooth, Spectrum const& accurate): radius(radius), smooth(smooth), accurate(accurate) {}

};


struct Light {

	std::map<int, Smoothing> smoothings;

	inline Light() {}

	inline Light(int const id, int const radius, Spectrum const& smooth, Spectrum const& accurate) {
		smoothings[id] = Smoothing(radius, smooth, accurate);
	}

	inline Light(Smoothing const& smoothing) {
		smoothings[0] = smoothing;
	}

	inline Light(Spectrum const& spectrum) {
		smoothings[0] = Smoothing(0, Spectrum(1,1,1), spectrum);
	}

	inline Light& operator+=(Light const& l) {
		for (auto & pair : l.smoothings) {
			auto & smoothing = smoothings[pair.first];
			smoothing.accurate += pair.second.accurate;
			smoothing.smooth += pair.second.smooth;
		}
		return *this;
	}

	inline Light& operator*=(float const t) {
		for (auto & pair : smoothings)
			pair.second.accurate *= t;
		return *this;
	}

	inline Light& operator*=(Spectrum const& s) {
		for (auto & pair : smoothings)
			pair.second.accurate *= s;
		return *this;
	}

	inline Light& operator/=(float const t) {
		operator*=(1/t);
		return *this;
	}

	inline Light addId(int const id) const {
		Light light;
		for (auto & pair : smoothings)
			light.smoothings[pair.first+id] = pair.second;
		return light;
	}

	inline Spectrum compute() const {
		Spectrum value;
		for (auto & pair : smoothings)
			value += pair.second.accurate * pair.second.smooth;
		return value;
	}

};

inline Light operator+(Light const& l1, Light const& l2) {
	Light light;
	light += l1;
	light += l2;
	return light;
}

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
