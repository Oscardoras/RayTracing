#ifndef __MATERIALS_MODELS_LAMBERTIAN_H__
#define __MATERIALS_MODELS_LAMBERTIAN_H__

#include "../Material.hpp"

#include "../../textures/Texture.hpp"


struct Lambertian: public Material {

	Texture* diffuse;

	Lambertian(Texture* const diffuse);
	
	virtual Light color(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const override;

	Spectrum scatter(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const;

};


#endif
