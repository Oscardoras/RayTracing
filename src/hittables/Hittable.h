#ifndef HITTABLE_H_
#define HITTABLE_H_

#include "../algebra/Ray.h"


class Hittable {

public:

	virtual ~Hittable();
	virtual Hit hit(Ray const& r, float const& tMin, float const& tMax, int remaningRays) const = 0;

};

Hittable::~Hittable() {}


#endif
