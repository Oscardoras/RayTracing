#ifndef HITTABLE_H_
#define HITTABLE_H_

#include "../algebra/Ray.h"

class World;

class Hittable {

public:

	virtual Hit hit(World const& world, Ray const& r, float const& tMin, float const& tMax, int remaningRays, int maxDepth) const = 0;

};


#endif
