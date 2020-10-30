#ifndef HITTABLES_SPHERE_H_
#define HITTABLES_SPHERE_H_

#include "../algebra/Point.h"
#include "../algebra/Ray.h"
#include "Hittable.h"
#include "../materials/Material.h"


class Sphere: public Hittable {

public:

	Point center;
	float radius;
	std::shared_ptr<Material> material;

	Sphere(Point center, float radius): Hittable(), center(center), radius(radius) {}

	virtual Hit hit(World* world, Ray const& r, float const& tMin, float const& tMax, int remaningRays, int maxDepth) const {
		Vector oc = r.origin - center;
		float a = r.direction.lengthSquared();
		float halfB = dot(oc, r.direction);
		float c = oc.lengthSquared() - radius*radius;
		float discriminant = halfB*halfB - a*c;

		if (discriminant > 0) {
			float root = sqrt(discriminant);
			float temp = (-halfB - root)/a;
			if (temp > tMin && temp < tMax) {
				Point point = r.at(temp);
				return Hit(Color());
			}
		}

		return Hit();
	}

};


#endif
