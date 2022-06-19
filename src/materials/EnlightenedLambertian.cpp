#include "EnlightenedLambertian.hpp"


Light EnlightenedLambertian::color(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const {
	Spectrum accurate = diffuse->getSpectrum(position);
	Spectrum scattered = accurate.isNull() ? Spectrum() : scatter(world, in, position, samples, depth);
	return Light(this, 1, scattered, accurate);
}

Spectrum EnlightenedLambertian::scatterLights(World const& world, Ray const& in, RelativePosition const& position, int const samples, int const depth) const {
	float importance = 1;
	float probability = 1;
	std::vector<Area> areas;

	for (std::shared_ptr<Priority> priority : priorities) {
		Area area(priority, in.p);
		importance -= priority->importance;
		probability -= area.probability; //Suppose qu'il n'y a pas de chevauchement.
		int s = int(priority->importance*samples);
		for (int i = 0; i < s; i++) {
			Vector v = ((priority->center + Vector::randomUnit()*priority->radius) - in.p).unit();
			if (v*normal >= 0) {
				area.spectrum += world.trace(Ray(in.p, v), true, 1, maxDepth).compute();
				area.rays++;
			}
		}
		areas.push_back(area);
	}


	Spectrum spectrum;
	if (lightMap == nullptr) {
		int rays = 0;
		int s = int(importance*float(samples));
		for (int i = 0; i < s; i++) {
			bool hit = false;
			Vector vec = Vector::randomUnit();
			if (vec*normal < 0) vec *= -1;
			Ray r = Ray(in.p, vec);
			Spectrum s = world.trace(r, true, 1, maxDepth).compute();
			for (Area area : areas) {
				if (area.priority->hit(r)) {
					hit = true;
					area.spectrum += s;
					area.rays++;
					break;
				}
			}
			if (!hit) {
				spectrum += s;
				rays++;
			}
		}

		if (rays > 0) spectrum /= rays;
	} else spectrum = lightMap->get(relative.u, relative.v).toSpectrum();
	spectrum *= probability;
	for (Area area : areas) if (area.rays > 0) spectrum += area.probability*area.spectrum/area.rays;

	return spectrum;
}