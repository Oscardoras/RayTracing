#ifndef ALGEBRA_LIGHT_H_
#define ALGEBRA_LIGHT_H_

#include <unordered_map>
#include <tuple>

#include "Spectrum.hpp"


class Smooth {

public:

	int id;
	int radius;
	Spectrum albedo;
	Spectrum light;

	Smooth(int const& id, int const& radius, Spectrum const& albedo, Spectrum const& light): id(id), radius(radius), albedo(albedo), light(light) {}

};


class Light {

public:

	std::vector<Smooth> smoothing;

	Light() {}
	Light(int id, int radius, Spectrum albedo, Spectrum light) {
		smoothing.push_back(Smooth(id, radius, albedo, light));
	}
	Light(Smooth const& smooth) {
		smoothing.push_back(smooth);
	}
	Light(Spectrum const& spectrum) {
		smoothing.push_back(Smooth(0, 0, spectrum, Spectrum(1,1,1)));
	}

	inline Light& operator+=(Light const& l) {
		for (Smooth smooth : l.smoothing) smoothing.push_back(smooth);
		return *this;
	}

	inline Light& operator*=(float const& t) {
		for (Smooth &smooth : smoothing) smooth.albedo *= t;
		return *this;
	}

	inline Light& operator*=(Spectrum const& s) {
		for (Smooth &smooth : smoothing) smooth.albedo *= s;
		return *this;
	}

	inline Light& operator/=(float const& t) {
		operator*=(1/t);
		return *this;
	}

	inline Light& addId(int const& id) {
		for (Smooth &smooth : smoothing) {
			smooth.id += id;
			smooth.id /= 2;
		}
		return *this;
	}

	inline Spectrum compute() const {
		Spectrum value = Spectrum();
		for (Smooth smooth : smoothing) value += smooth.albedo * smooth.light;
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
