#ifndef RAY_H_
#define RAY_H_

#include "Color.h"
#include "Point.h"
#include "Vector.h"


class Ray {

public:

	const Point origin;
	const Vector direction;

	Ray() {}
	Ray(Point const& origin, Vector const& direction): origin(origin), direction(direction) {}

	inline Point at(float const& t) const {
		return origin + t*direction;
	}

};

class Hit: public Color {

public:

	bool hitten;

	Hit(): Color(), hitten(false) {}
	Hit(float r, float g, float b): Color(r, g, b), hitten(true) {}
	inline friend bool operator==(bool const& b, Hit const& h);

};


#endif
