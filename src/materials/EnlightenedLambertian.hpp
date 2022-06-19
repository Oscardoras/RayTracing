#ifndef __MATERIALS_MODELS_ENLIGHTENEDLAMBERTIAN_H__
#define __MATERIALS_MODELS_ENLIGHTENEDLAMBERTIAN_H__

#include "Lambertian.hpp"


class EnlightenedLambertian: public Lambertian {

protected:

	std::vector<Primitive*> lights;

public:

	inline EnlightenedLambertian(Texture* const diffuse, std::vector<Primitive*> lights = std::vector<Primitive*>()):
		Lambertian(diffuse), lights(lights) {}
	
	virtual Light color(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const override;

protected:

	Spectrum scatterLights(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const;

};


#endif
