#ifndef MATERIALS_FOG_H_
#define MATERIALS_FOG_H_

#include <algorithm>

#include "Material.hpp"
#include "../World.hpp"

class Fog: public Material {

public:

	Spectrum albedo;
	float density;

	Fog(Spectrum albedo, float density) : Material(), albedo(albedo), density(density) {}

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& ray, World const& world, int const& samples, int const& maxDepth) const override {
		if (ray.direction*faceDirection <= 0) {
			Hit hit = world.hit(ray, false);

			if (std::isfinite(hit.t)) {
				int scatterRays = int(samples*density);
				Spectrum scattered;
				Light transmitted;
				if (samples > 1) {
					for (int i = 0; i < scatterRays; i++) {
						Vector vec = Vector::randomUnit();
						scattered += world.trace(Ray(ray.at(random_double(0, hit.t)), vec), false, 1, maxDepth).compute();
					}

					if (maxDepth > 1) transmitted = hit.object->color(Ray(ray.at(hit.t), ray.direction), world, samples-scatterRays, maxDepth-1);
					else transmitted = Light();
				} else {
					if (maxDepth > 1) transmitted = hit.object->color(Ray(ray.at(hit.t), ray.direction), world, samples, maxDepth-1);
					else transmitted = Light();
				}
				float scatter = std::min(double(density*hit.t), 1.);
				return Light(Spectrum(), transmitted*(1-scatter), Light(), albedo, scatter*scattered/std::max(1,scatterRays), long(this));
			} else {
				return Light(world.infiniteColor(ray), Light(), Light(), Spectrum(), Spectrum(), long(this));
			}
		} else {
			return world.trace(ray, true, samples, maxDepth);
		}
	}

};


#endif
