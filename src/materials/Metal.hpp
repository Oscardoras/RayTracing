#ifndef MATERIALS_METAL_H_
#define MATERIALS_METAL_H_

#include "Material.hpp"
#include "../World.hpp"


class Metal: public Material {

public:

	Color albedo;

	Metal(Color albedo) : Material(), albedo(albedo) {}
	virtual Color hit(World const& world, Point const& point, Vector const& reflected, Vector const& faceDirection, int const& remaningRays, int const& maxDepth) const {
		Point p = point + 0.001*faceDirection;
		Ray r(p, reflected - 2*dot(reflected, faceDirection)*faceDirection);
		Color color = world.trace(r, remaningRays, maxDepth-1);;
		return color*albedo;
	}

};


#endif
