#include "Ray.hpp"


Ray::Ray(Point const& p, Vector const& v): p(p), v(v) {}

Point Ray::at(float const t) const {
	return p + t*v;
}