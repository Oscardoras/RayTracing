#ifndef __MATERIALS_DIELECTRIC_H__
#define __MATERIALS_DIELECTRIC_H__

#include <algorithm>
#include <tuple>

#include "../Material.hpp"

#include "../../textures/Texture.hpp"


class Dielectric: public Material {

protected:

	float ior;
	Texture* roughness;

public:

	Dielectric(float const ior, Texture* const roughness);

	virtual Light color(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const override;

};


#endif
