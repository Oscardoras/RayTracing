#include "Metal.hpp"

#include "../../OpticsUtils.hpp"


Metal::Metal(Texture* reflection, Texture* roughness) : Material(), reflection(reflection), roughness(roughness) {}

Light Metal::color(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const {
	Spectrum const f = fresnel(reflection->getSpectrum(position), position.normal, in.v);
	Vector const d = specularReflection(position.normal, in.v);
	float const r = roughness->getFloat(position);
	auto l = f * specular(world, in, d, r, true, samples, depth).addId((int) long(this));
	return l;
}