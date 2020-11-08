#ifndef MATERIALS_LAMBERTIAN_H_
#define MATERIALS_LAMBERTIAN_H_

#include "../algebra/Color.h"
#include "../algebra/Point.h"
#include "../algebra/Vector.h"
#include "Material.h"
#include "../World.h"


class Lambertian: public Material {

public:

	Lambertian() : Material() {}
	virtual Color hit(World const& world, Point point, Vector reflected, Vector faceDirection, int remaningRays, int maxDepth) const {
		Color color;
		Point p = point + 0.0000001*faceDirection;
		for (int i = 0; i < remaningRays; i++) {
			Ray r(p, Vector::random_in_unit_sphere().unit());
			Color c = world.trace(r, 1, maxDepth-1);
			color += c;
		}
		return 0.5*(color/remaningRays) + 0.5*Color();
	}

};


#endif
