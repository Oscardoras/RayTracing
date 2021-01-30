#ifndef HITTABLES_RECTANGLE_H_
#define HITTABLES_RECTANGLE_H_

#include <memory>
#include <algorithm>

#include "Hittable.hpp"
#include "../materials/Material.hpp"


class Rectangle: public Hittable {

public:

	float x;
	float y1;
	float y2;
	float z1;
	float z2;
	std::shared_ptr<Material> material;

	Rectangle(float x, float y1, float y2, float z1, float z2, std::shared_ptr<Material> material): Hittable(), x(x), y1(y1), y2(y2), z1(z1), z2(z2), material(material) {}

	virtual float hit(Ray const& r, float const& tMax) const {
		if (r.direction.x != 0.) {
			float t = (x - r.origin.x) / r.direction.x;
			Point p = r.at(t);
			if (y1 <= p.y && p.y <= y2 && z1 <= p.z && p.z <= z2)
				if (0 < t && t < tMax)
					return t;
		}

		return NaN;
	}

	virtual LightData color(World const& world, Point const& point, Vector const& reflected, int const& remaningRays, int const& maxDepth) const override {
		Vector faceDirection(1., 0., 0.);
		return material->color(world, point, reflected, faceDirection*reflected < 0 ? faceDirection : -1*faceDirection, remaningRays, maxDepth);
	}

};


#endif
