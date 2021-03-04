#ifndef OBJECTS_SPHERE_H_
#define OBJECTS_SPHERE_H_

#include <memory>
#include <algorithm>

#include "Object.hpp"
#include "../materials/Material.hpp"


class Sphere: public Object {

public:

	Point center;
	float radius;
	std::shared_ptr<Material> material;

	Sphere(Point center, float radius, std::shared_ptr<Material> material): Object(), center(center), radius(radius), material(material) {}

	virtual float hit(Ray const& r, float const& tMin, float const& tMax, bool const& in) const {
		Vector oc = r.origin - center;

		float l2 = oc.lengthSquared();
		if (in && l2 + 0.01 < radius*radius) return 0;

		float a = r.direction.lengthSquared();
		float halfB = oc*r.direction;
		if (halfB < 0) {
			float c = l2 - radius*radius;
			float discriminant = halfB*halfB - a*c;

			if (discriminant > 0) {
				float root = sqrt(discriminant);
				float t = (-halfB - root)/a;
				if (t < tMin) t = (-halfB + root)/a;
				if (tMin < t && t < tMax) return t;
			}
		}

		return NaN;
	}

	virtual Light color(Ray const& ray, World const& world, int const& remainingRays, int const& maxDepth) const override {
		Vector oc = ray.origin - center;
		if (oc.lengthSquared() + 0.01 < radius*radius) return material->color(RelativePosition(oc, 0., 0.), Vector(), ray, world, remainingRays, maxDepth);

		float xz_radius = Vector(oc.x, 0, oc.z).length();
		float theta = std::acos(oc.x / xz_radius);
		if (oc.z < 0) theta = -theta + 2*pi;

		float phi = std::asin(oc.y / radius) + pi/2;

		return material->color(RelativePosition(oc, theta*radius, phi*radius), oc.unit(), ray, world, remainingRays, maxDepth);
	}

	virtual Box getBox() const {
		Box box = Box(center + radius*Vector(1,1,1), center - radius*Vector(1,1,1));
		box.objects.push_back(std::make_shared<Sphere>(*this));
		return box;
	}

};


#endif
