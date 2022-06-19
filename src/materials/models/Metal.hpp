#ifndef __MATERIALS_METAL_H__
#define __MATERIALS_METAL_H__

#include "../Material.hpp"

#include "../../textures/Texture.hpp"


class Metal: public Material {

protected:

	Texture* reflection;
	Texture* roughness;

public:

	Metal(Texture* const reflection, Texture* const roughness);

	virtual Light color(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const override;

};


#endif
