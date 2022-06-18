#include "Phong.hpp"

#include "../../OpticsUtils.hpp"


Phong::Phong(Texture* const ambiant, Texture* const diffuse, Texture* const specular, Texture* const alpha, std::vector<LightSource> const& light_sources):
	ambiant(ambiant), diffuse(diffuse), specular(specular), alpha(alpha), light_sources(light_sources) {}

Light Phong::color(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const {
	Light result;

	for (auto const& light_source : light_sources) {
		Vector light_direction = light_source.getDirection(in.p);
		Light light = world.trace(Ray(in.p, light_direction), true, 1, depth);

		result += (light.addId((int) long(this)+2) *= (diffuse->getSpectrum(position) * std::pow(std::max(float(0.), in.v * specularReflection(position.normal, light_direction)), alpha->getFloat(position))));
		result += Light((int) long(this)+1, 0, light.compute() * std::max(float(0.), position.normal * light_direction), diffuse->getSpectrum(position));
	}

	result += Light((int) long(this), 0, Spectrum::white(), ambiant->getSpectrum(position));
	
	return result;
}