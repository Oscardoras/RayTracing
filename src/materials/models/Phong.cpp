#include "Phong.hpp"

#include "../../OpticsUtils.hpp"


Phong::Phong(Texture* const ambiant, Texture* const diffuse, Texture* const specular, Texture* const alpha, std::vector<LightSource> const& light_sources):
	ambiant(ambiant), diffuse(diffuse), specular(specular), alpha(alpha), light_sources(light_sources) {}

Light Phong::color(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const {
	Spectrum spectrum = ambiant->getSpectrum(position);

	for (auto const& light_source : light_sources) {
		Vector light_direction = light_source.getDirection(in.p);
		Spectrum light = world.trace(Ray(in.p, light_direction), true, 1, depth).compute();

		spectrum += light * diffuse->getSpectrum(position) * std::max(0.f, position.normal * light_direction);
		spectrum += light * specular->getSpectrum(position) * std::pow(std::max(0.f, in.v * specularReflection(position.normal, light_direction)), alpha->getFloat(position));
	}
	
	return Light(spectrum);
}