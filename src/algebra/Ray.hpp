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

	Ray() {}
	Ray(Point const origin, Vector const direction): origin(origin), direction(direction) {}

	inline Point at(float const t) const {
		return origin + t*direction;
	}

};


#endif
