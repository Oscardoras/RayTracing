#include "Dielectric.hpp"

#include "../../OpticsUtils.hpp"


Dielectric::Dielectric(float const ior, Texture* const roughness): ior(ior), roughness(roughness) {}

Light Dielectric::color(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const {
	bool out = in.v*position.normal < 0.;
	Vector const& n = (out ? 1 : -1)*position.normal;
	float n1;
	float n2;
	if (out) {
		n1 = 1.f;
		n2 = ior;
	} else {
		n1 = ior;
		n2 = 1.f;
	}
	std::tuple<float, float> fd = fresnelDescartes(n1, n2, n, in.v);
	float f = std::get<0>(fd);
	float sin_theta = std::get<1>(fd);
	float r = roughness->getFloat(position);
	bool tr = (n1/n2)*sin_theta <= 1.;
	bool rf = n1 < n2;
	int s = tr && rf ? std::max(samples/2, 1) : samples;

	Spectrum spectrum;
	int radius = 0;
	if (tr) {
		auto sp = specular(world, in, specularTransimission(n1, n2, n, in.v), r, !out, s, depth);
		spectrum += (1.f-f) * sp.compute();
		radius = std::max(radius, sp.radius);
	}
	if (rf) {
		auto sp = specular(world, in, specularReflection(n, in.v), r, true, s, depth);
		spectrum += f * sp.compute();
		radius = std::max(radius, sp.radius);
	}
	
	return Light(this, radius, Spectrum::white(), spectrum);
}