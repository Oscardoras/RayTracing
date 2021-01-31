#ifndef MATERIALS_LAMBERTIAN_H_
#define MATERIALS_LAMBERTIAN_H_

#include "Material.hpp"
#include "../World.hpp"


class Lambertian: public Material {

public:

	Color albedo;

	Lambertian(Color albedo) : Material(), albedo(albedo) {}
	
	virtual LightData color(World const& world, Point const& point, Vector const& reflected, Vector const& faceDirection, int const& remaningRays, int const& maxDepth) const override {
		Color c;
		for (int i = 0; i < remaningRays; i++) {
			Vector v;
			float pb;
			if (!world.priorities.empty() && i < remaningRays*world.priority) {
				std::shared_ptr<Priority> priority = world.priorities[i%world.priorities.size()];
				float radius  = priority->radius;
				v = (priority->center + Vector::random()*radius) - point;
				pb = ( (pi*radius*radius) / v.lengthSquared() ) / (4*pi*1*1);
				if (v*faceDirection < 0) v *= -1;

				v = v.unit();
			} else {
				v = Vector::randomUnit();
				if (v*faceDirection < 0) v *= -1;
				pb = 1;
			}
			LightData lightData = world.trace(Ray(point, v), 1, maxDepth-1);
			c += lightData.light*lightData.albedo*pb;
		}

		return LightData(albedo, c/remaningRays, long(this));
	}

};


#endif
