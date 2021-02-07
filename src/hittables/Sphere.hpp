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
		float a = r.direction.lengthSquared();
		float halfB = oc*r.direction;
		if (halfB < 0) {
			float c = oc.lengthSquared() - radius*radius;
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
		Vector o = point - center;

		float xz_radius = Vector(o.x, 0, o.z).length();
		float theta = std::acos(o.x / xz_radius);
		if (o.z < 0) theta += pi;

		float phi = std::asin(o.y / radius) + pi;

		return material->color(world, point, reflected, o.unit(), theta*radius, phi*radius, remaningRays, maxDepth);
	}

};


#endif
