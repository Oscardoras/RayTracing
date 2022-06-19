#include "Lambertian.hpp"

#include "../../MathUtils.hpp"


Light Lambertian::color(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const {
	Spectrum accurate = diffuse->getSpectrum(position);
	Spectrum scattered = accurate.isNull() ? Spectrum() : scatter(world, in, position, samples, depth);
	return Light(this, 1, scattered, accurate);
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