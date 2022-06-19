#ifndef __ALGEBRA_RAY_H__
#define __ALGEBRA_RAY_H__

#include "Point.hpp"

struct Primitive;


struct Hit {

	Primitive const * primitive;
	float t;
	
};


struct Ray {

	Point p;
	Vector v;

	inline Ray(Point const& p, Vector const& v): p(p), v(v) {}

	inline Point at(float const t) const {
		return p + t*v;
	}

};


#endif
