#ifndef MATERIALS_LAMBERTIAN_H_
#define MATERIALS_LAMBERTIAN_H_

#include "../algebra/Color.h"
#include "../algebra/Point.h"
#include "../algebra/Vector.h"
#include "Material.h"
#include "../World.h"


class Lambertian: public Material {

public:

	Color albedo;

	Lambertian(Color albedo) : Material(), albedo(albedo) {}
	virtual Color hit(World const& world, Point point, Vector reflected, Vector faceDirection, int remaningRays, int maxDepth) const {
		Color color;
		Point p = point + 0.001*faceDirection;
		for (int i = 0; i < remaningRays; i++) {
			Ray r(p, Vector::random().unit());
			Color c = world.trace(r, 1, maxDepth-1);
			color += c;
		}
		return (color/remaningRays)*albedo;
	}

};


#endif
