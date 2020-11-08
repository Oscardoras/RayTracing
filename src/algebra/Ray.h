#ifndef RAY_H_
#define RAY_H_

#include <cmath>
#include <limits>

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

	float t;

	Hit(): Color(), t(std::numeric_limits<double>::signaling_NaN()) {}
	Hit(Color color, float t): Color(color), t(t) {}
	Hit(float r, float g, float b, float t): Color(r, g, b), t(t) {}

	inline bool hitten() const {
		return !std::isnan(t);
	}

};


#endif
