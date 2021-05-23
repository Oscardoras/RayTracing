#ifndef MATERIALS_SPECULARDIELECTRIC_H_
#define MATERIALS_SPECULARDIELECTRIC_H_

#include <algorithm>
#include <tuple>

#include "../../OpticsUtils.cpp"
#include "../Material.hpp"
#include "../../World.hpp"


class SpecularDielectric: public Material {

public:

	float ior;
	std::shared_ptr<Texture> roughness;

	SpecularDielectric(float ior, std::shared_ptr<Texture> roughness) : Material(), ior(ior), roughness(roughness) {}

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		bool out = in.v*faceDirection < 0.;
		float n1;
		float n2;
		if (out) {
			n1 = 1.;
			n2 = ior;
		} else {
			n1 = ior;
			n2 = 1.;
		}
		Vector const& n = (out ? 1 : -1)*faceDirection;
		std::tuple<float, float> fd = fresnelDescartes(n1, n2, n, in.v);
		float fresnel = std::get<0>(fd);
		float sin_theta = std::get<1>(fd);
		float f = roughness->getFloat(relative);
		int s = std::max(samples/2, 1);

		Light light = Light();
		if ((n1/n2)*sin_theta <= 1.) light += specularTransmission(out, n1, n2, f, n, in, world, s, maxDepth).addId(long(this) + 2) * (1-fresnel);
		if (n1 < n2) light += SpecularMetal::specularReflection(f, faceDirection, in, world, s, maxDepth).addId(long(this) + 4) * fresnel;
		return light;
	}

	static Light specularTransmission(bool const& out, float const& n1, float const& n2, float const& fuzziness, Vector const& normal, Ray const& in, World const& world, int const& samples, int const& maxDepth) {
		Light light = Light();

		Vector R = spTr(n1, n2, normal, in.v);
		int n = std::max(1, std::min(int(fuzziness*10 * samples), samples));
		if (n == 1) {
			light = world.trace(Ray(in.p, R), !out, samples, maxDepth);
		} else {
			Spectrum spectrum = Spectrum();
			int remaining = std::max(1, int(samples / n));
			for (int i = 0; i < n; i++) {
				Vector d = (R + fuzziness*Vector::randomUnit()).unit();
				spectrum += world.trace(Ray(in.p, d), !out, remaining, maxDepth).compute();
			}
			light = Light(0, std::min(int(fuzziness*10), 3), Spectrum(1,1,1), spectrum);
		}

		return light/n;
	}

};


#endif
