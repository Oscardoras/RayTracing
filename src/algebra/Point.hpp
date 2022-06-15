#ifndef __ALGEBRA_POINT_H__
#define __ALGEBRA_POINT_H__

#include "Vector.hpp"


struct Point {

	float x;
	float y;
	float z;

	Point();
	Point(float const x, float const y, float const z);

	Point& operator+=(Vector const& v);

	Point& operator-=(Vector const& v);

};

Point operator+(Point const& a, Vector const& u);

Point operator-(Point const& a, Vector const& u);

Vector operator-(Point const& a, Point const& b);


#endif
