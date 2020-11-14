#ifndef MATERIALS_LIGHT_H_
#define MATERIALS_LIGHT_H_

#include "Material.hpp"
#include "../World.hpp"


class Light: public Material {

public:

	Color color;

	Light(Color color) : Material(), color(color) {}
	virtual Color hit(World const& world, Point const& point, Vector const& reflected, Vector const& faceDirection, int const& remaningRays, int const& maxDepth) const {
		return color;
	}

};


#endif
