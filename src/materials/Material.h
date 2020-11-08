#ifndef MATERIALS_MATERIAL_H_
#define MATERIALS_MATERIAL_H_

#include "../algebra/Color.h"
#include "../algebra/Point.h"


class Material {

public:

	virtual Color hit(World const& world, Point point, Vector reflected, Vector faceDirection, int remaningRays, int maxDepth) const = 0;

};


#endif
