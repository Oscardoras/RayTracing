#ifndef __MATERIALS_METAL_H__
#define __MATERIALS_METAL_H__

#include <algorithm>

#include "../Material.hpp"

#include "../../textures/Texture.hpp"


struct Metal: public Material {

protected:

	Texture* reflection;
	Texture* roughness;

public:

	Metal(Texture* reflection, Texture* roughness);

	virtual Light color(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const override;

};


#endif
