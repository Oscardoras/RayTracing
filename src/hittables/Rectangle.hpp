#ifndef HITTABLES_RECTANGLE_H_
#define HITTABLES_RECTANGLE_H_

#include <memory>
#include <algorithm>

#include "Hittable.hpp"
#include "../materials/Material.hpp"


class Rectangle: public Hittable {

public:

	Point point1;
	Point point2;
	std::shared_ptr<Material> material;

	Rectangle(Point point1, Point point2, std::shared_ptr<Material> material): Hittable(), point1(point1), point2(point2), material(material) {}

	virtual float hit(Ray const& r, float const& tMax) const {
		Vector oc = r.origin - center;
		float a = r.direction.lengthSquared();
		float halfB = dot(oc, r.direction);
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
		return material->color(world, point, reflected, (point-center).unit(), remaningRays, maxDepth);
	}

};


#endif
