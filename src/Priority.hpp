#ifndef PRIORITY_H_
#define PRIORITY_H_

#include <memory>

#include "algebra/Ray.hpp"


class Priority {

public:

	Point center;
	float radius;

	Priority(Point center, float radius) : center(center), radius(radius) {}

	bool hit(Ray const& r) const {
		Vector oc = r.origin - center;
		float a = r.direction.lengthSquared();
		float halfB = oc*r.direction;
		if (halfB < 0) {
			float c = oc.lengthSquared() - radius*radius;
			return halfB*halfB > a*c;
		}
		return false;
	}

};


#endif
