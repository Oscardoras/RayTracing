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

	Point p;
	Vector v;

	Ray() {}
	Ray(Point p, Vector v): p(p), v(v) {}

	inline Point at(float const& t) const {
		return p + t*v;
	}

};


#endif
