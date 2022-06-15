#ifndef __ALGEBRA_RAY_H__
#define __ALGEBRA_RAY_H__

#include "Point.hpp"

struct Primitive;


struct Hit {

	Primitive* primitive;
	float t;
	
};


struct Ray {

	Point p;
	Vector v;

	Ray(Point const& p, Vector const& v);

	Point at(float const t) const;

};


#endif
