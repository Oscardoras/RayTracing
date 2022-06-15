#include "Light.hpp"


Smooth::Smooth(int const d, int const radius, Spectrum const& smoothing, Spectrum const& accurate): id(id), radius(radius), smoothing(smoothing), accurate(accurate) {}

Light::Light() {}

Light::Light(int const id, int const radius, Spectrum const& smoothing, Spectrum const& accurate) {
	smoothings.push_back(Smooth(id, radius, smoothing, accurate));
}

Light::Light(Smooth const& smooth) {
	smoothings.push_back(smooth);
}

Light::Light(Spectrum const& spectrum) {
	smoothings.push_back(Smooth(0, 0, Spectrum(1,1,1), spectrum));
}

Light& Light::operator+=(Light const& l) {
	for (Smooth const& smooth : l.smoothings) smoothings.push_back(smooth);
	return *this;
}

Light& Light::operator*=(float const t) {
	for (Smooth & smooth : smoothings) smooth.accurate *= t;
	return *this;
}

Light& Light::operator*=(Spectrum const& s) {
	for (Smooth & smooth : smoothings) smooth.accurate *= s;
	return *this;
}

Light& Light::operator/=(float const t) {
	operator*=(1/t);
	return *this;
}

Light& Light::addId(int const id) {
	for (Smooth & smooth : smoothings) {
		smooth.id += id;
		smooth.id /= 2;
	}
	return *this;
}

Spectrum Light::compute() const {
	Spectrum value = Spectrum();
	for (Smooth const& smooth : smoothings)
		value += smooth.accurate * smooth.smoothing;
	return value;
}

Light operator+(Light const& l1, Light const& l2) {
	Light light = Light();
	light += l1;
	light += l2;
	return light;
}

Light operator*(float const t, Light const& l) {
	Light light(l);
	light *= t;
	return light;
}

Light operator*(Light const& l, float const t) {
	return t * l;
}

Light operator*(Light const& l, Spectrum const& s) {
	Light light(l);
	light *= s;
	return light;
}

Light operator*(Spectrum const& s, Light const& l) {
	return l * s;
}

Light operator/(Light const& l, float const t) {
	return l * (1/t);
}