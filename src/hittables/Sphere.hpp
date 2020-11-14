#ifndef HITTABLES_SPHERE_H_
#define HITTABLES_SPHERE_H_

#include <memory>
#include <algorithm>

#include "Hittable.hpp"
#include "../materials/Material.hpp"


class Sphere: public Hittable {

public:

	Point center;
	float radius;
	std::shared_ptr<Material> material;

	Sphere(Point center, float radius, std::shared_ptr<Material> material): Hittable(), center(center), radius(radius), material(material) {}

	virtual Hit hit(World const& world, Ray const& r, float const& tMin, float const& tMax, int const& remaningRays, int const& maxDepth) const {
		Vector oc = r.origin - center;
		float a = r.direction.lengthSquared();
		float halfB = dot(oc, r.direction);
		float c = oc.lengthSquared() - radius*radius;
		float discriminant = halfB*halfB - a*c;

		if (discriminant > 0) {
			float root = sqrt(discriminant);
			float temp1 = (-halfB - root)/a;
			float temp2 = (-halfB + root)/a;
			float temp = std::min(temp1, temp2);
			if (temp > tMin && temp < tMax) {
				Point point = r.at(temp);
				return Hit(material->hit(world, point, r.direction, (point-center).unit(), remaningRays, maxDepth), temp);
			}
		}

		return Hit();
	}

};


#endif
