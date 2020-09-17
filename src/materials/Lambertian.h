#ifndef MATERIALS_LAMBERTIAN_H_
#define MATERIALS_LAMBERTIAN_H_

#include "../algebra/Color.h"
#include "../algebra/Point.h"
#include "../algebra/Vector.h"
#include "Material.h"


class Lambertian {

public:

	Lambertian() : Material() {}
	Color hit(Point point, Vector reflected, Vector faceDirection, int remaningRays, std::shared_ptr<World> world) {
		int rr = remaningRays / 2;
		Color color();
		for (int i = 0; i < rr; i++) {
			color += world->trace(Ray(point, Vector.random()), rr);
		}
	}

};


#endif
