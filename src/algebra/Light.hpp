#ifndef ALGEBRA_LIGHT_H_
#define ALGEBRA_LIGHT_H_

#include <unordered_map>
#include <tuple>

#include "Spectrum.hpp"


class Scatter {

public:

	int id;
	Spectrum albedo;
	Spectrum scattered;

	Scatter(int id, Spectrum albedo, Spectrum scattered): id(id), albedo(albedo), scattered(scattered) {}

};


class Light {

public:

	Spectrum accurate;
	std::vector<Scatter> smooth;

	Light() {}
	Light(int const& id, Spectrum const& albedo, Spectrum const& scattered) {
		smooth.push_back(Scatter(id, albedo, scattered));
	}
	Light(Scatter const& scatter) {
		smooth.push_back(scatter);
	}
	Light(Spectrum const& spectrum) {
		accurate = spectrum;
	}

	inline Light& operator+=(Light const& l) {
		accurate += l.accurate;
		for (Scatter scatter : l.smooth) smooth.push_back(scatter);
		return *this;
	}

	inline Light& operator*=(float const& t) {
		accurate *= t;
		for (Scatter& scatter : smooth) scatter.albedo *= t;
		return *this;
	}

	inline Light& operator*=(Spectrum const& s) {
		accurate *= s;
		for (Scatter& scatter : smooth) scatter.albedo *= s;
		return *this;
	}

	inline Light& operator/=(float const& t) {
		operator*=(1/t);
		return *this;
	}

	inline void addId(int const& id) {
		for (Scatter& scatter : smooth) {
			scatter.id += id;
			scatter.id /= 2;
		}
	}

	inline Spectrum compute() const {
		Spectrum value = accurate;
		for (Scatter scatter : smooth) value += scatter.albedo * scatter.scattered;
		return value;
	}

};

inline Light operator+(Light const& l1, Light const& l2) {
	Light light = Light();
	light += l1;
	light += l2;
	return light;
}

inline Light operator*(float const& t, Light const& l) {
	Light light(l);
	light *= t;
	return light;
}

inline Light operator*(Light const& l, float const& t) {
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

inline Light operator/(Light const& l, float const& t) {
	return l * (1/t);
}


#endif
