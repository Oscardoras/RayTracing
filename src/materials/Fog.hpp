#ifndef MATERIALS_FOG_H_
#define MATERIALS_FOG_H_

#include <algorithm>

#include "../algebra/Spectrum.hpp"
#include "Material.hpp"
#include "../World.hpp"

class Fog: public Material {

public:

	Spectrum albedo;
	float density;

	Fog(Spectrum albedo, float density) : Material(), albedo(albedo), density(density) {}

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		if (in.v*faceDirection <= 0) {
			Hit hit = world.hit(in, false);

			if (std::isfinite(hit.t)) {
				int scatterRays = int(samples*density);
				Spectrum scattered;
				Light transmitted;
				if (samples > 1) {
					for (int i = 0; i < scatterRays; i++) {
						Vector vec = Vector::randomUnit();
						scattered += world.trace(Ray(in.at(random_double(0, hit.t)), vec), false, 1, maxDepth).compute();
					}

					if (maxDepth > 1) transmitted = hit.object->color(Ray(in.at(hit.t), in.v), world, samples-scatterRays, maxDepth-1);
					else transmitted = Light();
				} else {
					if (maxDepth > 1) transmitted = hit.object->color(Ray(in.at(hit.t), in.v), world, samples, maxDepth-1);
					else transmitted = Light();
				}
				float scatter = std::min(double(density*hit.t), 1.);
				Light light = Light();
				light += transmitted.addId(long(this) + 2)*(1-scatter);
				light += Light(long(this) + 4, albedo*scatter, scattered/std::max(1,scatterRays));
				return light;
			} else {
				return Light(world.infiniteColor(in));
			}
		} else {
			return world.trace(in, true, samples, maxDepth);
		}
	}

};


#endif
