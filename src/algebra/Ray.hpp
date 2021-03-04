#ifndef ALGEBRA_RAY_H_
#define ALGEBRA_RAY_H_

#include <cmath>
#include <limits>

#include "Color.hpp"
#include "Point.hpp"
#include "Vector.hpp"

class Object;


struct Hit {
	std::shared_ptr<Object> object;
	float t;
};


class Ray {

public:

	const Point origin;
	const Vector direction;
	float ior;

	Ray() : ior(1) {}
	Ray(Point const origin, Vector const direction, float const ior = 1) : origin(origin), direction(direction), ior(ior) {}

	inline Point at(float const t) const {
		return origin + t*direction;
	}

};


#endif
