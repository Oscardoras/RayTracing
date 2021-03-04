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

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& ray, World const& world, int const& remainingRays, int const& maxDepth) const override {
		if (ray.direction*faceDirection <= 0) {
			Hit hit = world.hit(ray, false);

			if (std::isfinite(hit.t)) {
				if (remainingRays > 1) {
					int scatterRays = int(remainingRays*density);
					Spectrum scattered;
					for (int i = 0; i < scatterRays; i++) {
						Vector vec;
						float pb;
						if (!world.priorities.empty() && i < remainingRays*world.priority) {
							std::shared_ptr<Priority> priority = world.priorities[i%world.priorities.size()];
							float radius  = priority->radius;
							vec = (priority->center + Vector::random()*radius) - ray.origin;
							pb = ( (pi*radius*radius) / vec.lengthSquared() ) / (4*pi*1*1);
							if (vec*faceDirection < 0) vec *= -1;

							vec = vec.unit();
						} else {
							vec = Vector::randomUnit();
							if (vec*faceDirection < 0) vec *= -1;
							pb = 1;
						}
						Light light = world.trace(Ray(ray.at(random_double(0, hit.t)), vec), false, 1, maxDepth);
						scattered += light.compute()*pb;
					}

					Light transmitted;
					if (maxDepth > 1) transmitted = hit.object->color(Ray(ray.at(hit.t), ray.direction), world, remainingRays-scatterRays, maxDepth-1);
					else transmitted = Light();

					float scatter = std::min(double(density*hit.t), 1.);
					return Light(Spectrum(), transmitted*(1-scatter), Light(), albedo, scatter*scattered/std::max(1,scatterRays), long(this));
				} else {
					if (maxDepth > 1) return hit.object->color(Ray(ray.at(hit.t), ray.direction), world, remainingRays, maxDepth-1);
					else return Light();
				}
			} else {
				return Light(world.infiniteColor(ray), Light(), Light(), Spectrum(), Spectrum(), long(this));
			}
		} else {
			return world.trace(ray, true, remainingRays, maxDepth);
		}
	}

};


#endif
