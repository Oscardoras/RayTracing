#ifndef PRIORITY_H_
#define PRIORITY_H_

#include <memory>

#include "Material.hpp"


class Priority {

public:

	Point center;
	float radius;
	float portion;

	Priority(Point center, float radius, float portion) : center(center), radius(radius), portion(portion) {}

	bool hit(Ray const& r) const {
		Vector oc = r.origin - center;
		float a = r.direction.lengthSquared();
		float halfB = oc*r.direction;
		if (halfB < 0) {
			float c = oc.lengthSquared() - radius*radius;
			return halfB*halfB > a*c;
		}
		return false;
	}

};

class Area {

public:

	std::shared_ptr<Priority> priority;
	float probability;
	Spectrum spectrum;
	int rays;

	Area(std::shared_ptr<Priority> priority, Point origin): priority(priority), spectrum(), rays(0) {
		probability = ( (Pi*priority->radius*priority->radius) / (priority->center - origin).lengthSquared() ) / (4*Pi*1*1);
	}
};


class Priorisable: public Material {

public:

	std::vector<std::shared_ptr<Priority>> priorities;
	std::shared_ptr<Texture> lightMap;

	Priorisable(std::vector<std::shared_ptr<Priority>> priorities = std::vector<std::shared_ptr<Priority>>()):
		Material(), priorities(priorities) {}

};


#endif
