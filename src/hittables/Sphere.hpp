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

	virtual float hit(Ray const& r, float const& tMax) const {
		Vector oc = r.origin - center;
		float l2 = oc.lengthSquared();
		if (l2 < radius*radius) return 0;
		float a = r.direction.lengthSquared();
		float halfB = oc*r.direction;
		if (halfB < 0) {
			float c = l2 - radius*radius;
			float discriminant = halfB*halfB - a*c;

			if (discriminant > 0) {
				float root = sqrt(discriminant, 0.001);
				//float temp1 = (-halfB - root)/a;
				//float temp2 = (-halfB + root)/a;
				float t = (-halfB - root)/a;//std::min(temp1, temp2);
				if (0 < t && t < tMax) return t;
			}
		}

		return NaN;
	}

	virtual LightData color(World const& world, Point const& point, Vector const& reflected, int const& remaningRays, int const& maxDepth) const override {
		Vector oc = point - center;
		if (oc.lengthSquared() < radius*radius) return material->color(world, point, reflected, Vector(), *this, 0., 0., remaningRays, maxDepth);

		float xz_radius = Vector(oc.x, 0, oc.z).length();
		float theta = std::acos(oc.x / xz_radius);
		if (oc.z < 0) theta = -theta + 2*pi;

		float phi = std::asin(oc.y / radius) + pi/2;

		return material->color(world, point, reflected, oc.unit(), *this, theta*radius, phi*radius, remaningRays, maxDepth);
	}

};


#endif
