#include "Lambertian.hpp"


Lambertian::Lambertian(Texture* const diffuse): diffuse(diffuse) {}

Light Lambertian::color(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const {
	Spectrum accurate = diffuse->getSpectrum(position);
	Spectrum scattered = accurate.isNull() ? Spectrum() : scatter(world, in, position, samples, depth);
	return Light((int) long(this), 0, accurate, scattered);
}

Spectrum Lambertian::scatter(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const {
	Spectrum spectrum;
	
	for (int i = 0; i < samples; i++) {
		Vector vec = Vector::randomUnit();
		if (vec*position.normal < 0) vec *= -1;
		Ray r = Ray(in.p, vec);
		spectrum += world.trace(r, true, 1, depth).compute();
	}

	return spectrum/samples;
}