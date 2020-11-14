#ifndef HITTABLE_H_
#define HITTABLE_H_

#include "../algebra/Ray.hpp"

class World;

class Hittable {

public:

	virtual Hit hit(World const& world, Ray const& r, float const& tMin, float const& tMax, int const& remaningRays, int const& maxDepth) const = 0;

};


#endif
