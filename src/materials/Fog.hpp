#ifndef MATERIALS_FOG_H_
#define MATERIALS_FOG_H_

#include "Material.hpp"
#include "../World.hpp"


class Fog: public Material {

public:

	Color albedo;
	float density;

	Fog(Color albedo, float density) : Material(), albedo(albedo), density(density) {}

	virtual LightData color(World const& world, Point const& point, Vector const& reflected, Vector const& faceDirection, Hittable const& hittable, float const& u, float const& v, int const& remaningRays, int const& maxDepth) const override {
		Ray r(point + reflected.unit()*0.001, reflected);
		float t = hittable.hit(r, std::numeric_limits<double>::infinity());
		if (!std::isnan(t)) {
			if (remaningRays <= 1) return world.trace(Ray(point, reflected), remaningRays, maxDepth-1);
			else {
				int n = density*t;
			}
		}
		return LightData(albedo, long(this));
	}

};


#endif
