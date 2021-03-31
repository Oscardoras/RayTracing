#ifndef MATERIALS_SPECULARMETAL_H_
#define MATERIALS_SPECULARMETAL_H_

#include <algorithm>

#include "Material.hpp"
#include "../World.hpp"
#include "../textures/Texture.hpp"


class SpecularMetal: public Material {

public:

	std::shared_ptr<Texture> albedo;
	std::shared_ptr<Texture> fuzziness;

	SpecularMetal(std::shared_ptr<Texture> albedo, std::shared_ptr<Texture> fuzziness) : Material(), albedo(albedo), fuzziness(fuzziness) {}

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		Spectrum a = albedo->get(relative.u, relative.v).toSpectrum();
		float f = fuzziness->get(relative.u, relative.v).toBlackAndWhite();
		return specularReflection(a, f, faceDirection, in, world, samples, maxDepth).addId(long(this));
	}

	static Light specularReflection(Spectrum const& albedo, float const& fuzziness, Vector const& normal, Ray const& in, World const& world, int const& samples, int const& maxDepth) {
		Light light = Light();

		int n = std::max(1, std::min(int(fuzziness*10 * samples), samples));
		int remaining = std::max(1, int(samples / n));
		for (int i = 0; i < n; i++) {
			Vector d = (in.v - 2*(in.v*normal)*normal + fuzziness*Vector::randomUnit()).unit();
			light += world.trace(Ray(in.p, d), true, remaining, maxDepth);
		}

		return light/n;
	}

};


#endif
