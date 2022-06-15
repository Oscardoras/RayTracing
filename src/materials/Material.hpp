#ifndef __MATERIALS_MATERIAL_H__
#define __MATERIALS_MATERIAL_H__

#include "../objects/primitives/Primitive.hpp"
#include "../world/World.hpp"


struct Material {

	virtual Light color(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const = 0;

};


#endif
