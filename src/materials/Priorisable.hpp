#ifndef MATERIALS_PRIORISABLE_H_
#define MATERIALS_PRIORISABLE_H_

#include <memory>

#include "Material.hpp"


class Priority {

public:

	Point center;
	float radius;
	float importance;

	Priority(Point center, float radius, float importance): center(center), radius(radius), importance(importance) {}

	bool hit(Ray const& r) const {
		Vector oc = r.p - center;
		float a = r.v.lengthSquared();
		float halfB = oc*r.v;
		if (halfB < 0) {
			float c = oc.lengthSquared() - radius*radius;
			return halfB*halfB > a*c;
		}
		return false;
	}

	Vector random(Point const& from) const {
		return (center - from) + radius*Vector::randomUnit();
	}

};


class Priorisable: public Material {

public:

	std::vector<std::shared_ptr<Priority>> priorities;

	Priorisable(std::vector<std::shared_ptr<Priority>> const& priorities = std::vector<std::shared_ptr<Priority>>()):
		Material(), priorities(priorities) {}

};


#endif
