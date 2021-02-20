#ifndef HITTABLES_HITTABLE_H_
#define HITTABLES_HITTABLE_H_

#include "../algebra/Ray.hpp"

class World;

class Hittable {

public:

	virtual float hit(Ray const& r, float const& tMax) const = 0;

	virtual LightData color(World const& world, Point const& point, Vector const& reflected, int const& remaningRays, int const& maxDepth) const = 0;

};


#endif
