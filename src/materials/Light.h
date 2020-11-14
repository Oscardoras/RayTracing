#ifndef MATERIALS_LIGHT_H_
#define MATERIALS_LIGHT_H_

#include "../algebra/Color.h"
#include "../algebra/Point.h"
#include "../algebra/Vector.h"
#include "Material.h"
#include "../World.h"


class Light: public Material {

public:

	Color color;

	Light(Color color) : Material(), color(color) {}
	virtual Color hit(World const& world, Point point, Vector reflected, Vector faceDirection, int remaningRays, int maxDepth) const {
		return color;
	}

};


#endif
