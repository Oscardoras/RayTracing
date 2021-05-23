#ifndef MATERIALS_SPECULARMETAL_H_
#define MATERIALS_SPECULARMETAL_H_

#include <algorithm>

#include "../../OpticsUtils.cpp"
#include "../Material.hpp"
#include "../../World.hpp"
#include "../../textures/Texture.hpp"


class SpecularMetal: public Material {

public:

	std::shared_ptr<Texture> reflection;
	std::shared_ptr<Texture> roughness;

	SpecularMetal(std::shared_ptr<Texture> reflection, std::shared_ptr<Texture> roughness) : Material(), reflection(reflection), roughness(roughness) {}

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		Spectrum r = reflection->getSpectrum(relative);
		float f = roughness->getFloat(relative);
		return r*specularReflection(f, faceDirection, in, world, samples, maxDepth).addId(long(this));
	}

	static Light specularReflection(float const& fuzziness, Vector const& normal, Ray const& in, World const& world, int const& samples, int const& maxDepth) {
		Light light;

		Vector R = spRef(normal, in.v);
		int n = std::max(1, std::min(int(fuzziness*10 * samples), samples));
		if (n == 1) {
			light = world.trace(Ray(in.p, R), true, samples, maxDepth);
		} else {
			Spectrum spectrum = Spectrum();
			int remaining = std::max(1, int(samples / n));
			for (int i = 0; i < n; i++) {
				Vector d = (R + fuzziness*Vector::randomUnit()).unit();
				spectrum += world.trace(Ray(in.p, d), true, remaining, maxDepth).compute();
			}
			light = Light(0, std::min(int(fuzziness*10), 3), Spectrum(1,1,1), spectrum);
		}

		return light/n;
	}

};


#endif
