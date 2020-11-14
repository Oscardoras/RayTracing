#ifndef MATERIALS_METAL_H_
#define MATERIALS_METAL_H_

#include "../algebra/Color.h"
#include "../algebra/Point.h"
#include "../algebra/Vector.h"
#include "Material.h"
#include "../World.h"


class Metal: public Material {

public:

	Color albedo;

	Metal(Color albedo) : Material(), albedo(albedo) {}
	virtual Color hit(World const& world, Point point, Vector reflected, Vector faceDirection, int remaningRays, int maxDepth) const {
		Point p = point + 0.001*faceDirection;
		Ray r(p, reflected - 2*dot(reflected, faceDirection)*faceDirection);
		Color color = world.trace(r, remaningRays, maxDepth-1);;
		return color*albedo;
	}

};


#endif
