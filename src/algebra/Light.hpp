#ifndef ALGEBRA_LIGHT_H_
#define ALGEBRA_LIGHT_H_

#include <cstdlib>

#include "Spectrum.hpp"


class Light {

public:

	Spectrum scatterAlbedo;
	Spectrum scatterIllumination;
	Spectrum addition;
	long id;

	Light(int id = 0): id(id) {}
	Light(Light const& light): scatterAlbedo(light.scatterAlbedo), scatterIllumination(light.scatterIllumination), addition(light.addition), id(light.id) {}
	Light(Spectrum emitted, Light transmitted, Light specular, Spectrum scatteredAlbedo, Spectrum scatteredIllumination, int id = 0): id(id) {
		long t = transmitted.scatterIllumination.getIntensity();
		long s = specular.scatterIllumination.getIntensity();
		long si = scatterIllumination.getIntensity();
		if (si >= t && si >= s) {
			scatterAlbedo = scatteredAlbedo;
			scatterIllumination = scatteredIllumination;
			addition = emitted + transmitted.compute() + specular.compute();
		} else if (s >= t && s >= si) {
			scatterAlbedo = specular.scatterAlbedo;
			scatterIllumination = specular.scatterIllumination;
			addition = emitted + transmitted.compute() + scatteredAlbedo*scatteredIllumination;
		} else {
			scatterAlbedo = transmitted.scatterAlbedo;
			scatterIllumination = transmitted.scatterIllumination;
			addition = emitted + specular.compute() + scatteredAlbedo*scatteredIllumination;
		}
	}

	inline Light& operator+=(Light const& c) {
		scatterAlbedo += c.scatterAlbedo;
		scatterIllumination += c.scatterIllumination;
		addition += c.addition;
		id += c.id;
		id /= 2;
		return *this;
	}

	inline Light& operator-=(Light const& c) {
		scatterAlbedo -= c.scatterAlbedo;
		scatterIllumination -= c.scatterIllumination;
		addition -= c.addition;
		id -= c.id;
		id *= 2;
		return *this;
	}

	inline Light& operator*=(float const& t) {
		scatterAlbedo *= t;
		scatterIllumination *= t;
		addition *= t;
		return *this;
	}

	inline Light& operator*=(Spectrum const& c) {
		scatterAlbedo *= c.r;
		scatterIllumination *= c.g;
		addition *= c.b;
		return *this;
	}

	inline Light& operator/=(float const& t) {
		scatterAlbedo /= t;
		scatterIllumination /= t;
		addition /= t;
		return *this;
	}

	inline Spectrum compute() const {
		return scatterAlbedo*scatterIllumination + addition;
	}

};

inline Light operator+(Light const& c, Light const& d) {
	Light light = Light();
	light.scatterAlbedo = c.scatterAlbedo + d.scatterAlbedo;
	light.scatterIllumination = c.scatterIllumination + d.scatterIllumination;
	light.addition = c.addition + d.addition;
	light.id = (c.id + d.id)/2;
	return light;
}

inline Light operator-(Light const& c, Light const& d) {
	Light light = Light();
	light.scatterAlbedo = c.scatterAlbedo - d.scatterAlbedo;
	light.scatterIllumination = c.scatterIllumination - d.scatterIllumination;
	light.addition = c.addition - d.addition;
	light.id = (c.id - d.id)*2;
	return light;
}

inline Light operator*(float const& t, Light const& c) {
	Light light = Light();
	light.scatterAlbedo = t * c.scatterAlbedo;
	light.scatterIllumination = t * c.scatterIllumination;
	light.addition = t * c.addition;
	return light;
}

inline Light operator*(Light const& c, float const& t) {
	return t * c;
}

inline Light operator*(Light const& c, Spectrum const& d) {
	Light light = Light();
	light.scatterAlbedo = d * c.scatterAlbedo;
	light.scatterIllumination = d * c.scatterIllumination;
	light.addition = d * c.addition;
	return light;
}

inline Light operator*(Spectrum const& c, Light const& d) {
	return d * c;
}

inline Light operator/(Light const& c, float const& t) {
	return c * (1/t);
}


#endif
