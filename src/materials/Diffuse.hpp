#ifndef MATERIALS_DIFFUSE_H_
#define MATERIALS_DIFFUSE_H_

#include "Priorisable.hpp"
#include "../textures/Texture.hpp"


class Area {

public:

	std::shared_ptr<Priority> priority;
	float probability;
	Spectrum spectrum;
	int rays;

	Area(std::shared_ptr<Priority> const& priority, Point const& origin): priority(priority), spectrum(), rays(0) {
		probability = ( (Pi*priority->radius*priority->radius) / (priority->center - origin).lengthSquared() ) / (4*Pi*1*1);
	}
};


class Diffuse: public Priorisable {

public:

	std::shared_ptr<Texture> lightMap;

	Diffuse(std::vector<std::shared_ptr<Priority>> const& priorities = std::vector<std::shared_ptr<Priority>>()):
		Priorisable(priorities) {}
	
	virtual Spectrum sample(Vector const& faceDirection, Ray const& in, Ray &out, World const& world, int const& maxDepth) const = 0;

	Spectrum scatter(RelativePosition const& relative, Vector const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const {
		float remaind = 1;
		float probability = 1;
		std::vector<Area> areas;

		for (std::shared_ptr<Priority> priority : priorities) {
			Area area(priority, in.p);
			remaind -= priority->importance;
			probability -= area.probability;
			int s = int(priority->importance*float(samples));
			for (int i = 0; i < s; i++) {
				Vector v = (priority->center + Vector::random()*priority->radius) - in.p;
				Ray r = Ray(in.p, v);
				area.spectrum += sample(faceDirection, in, r, world, maxDepth);
				area.rays++;
			}
			areas.push_back(area);
		}


		Spectrum spectrum;
		if (lightMap == nullptr) {
			int rays = 0;
			int s = int(remaind*float(samples));
			for (int i = 0; i < s; i++) {
				bool hit = false;
				Vector vec = Vector::randomUnit();
				if (vec*faceDirection < 0) vec *= -1;
				Ray r = Ray(in.p, vec);
				for (Area area : areas) {
					if (area.priority->hit(r)) {
						hit = true;
						area.spectrum += world.trace(r, true, 1, maxDepth).compute();
						area.rays++;
					}
				}
				if (!hit) {
					spectrum += world.trace(r, true, 1, maxDepth).compute();
					rays++;
				}
			}

			if (rays > 0) spectrum /= rays;
		} else {
			spectrum += lightMap->get(relative.u, relative.v).toSpectrum();
		}
		spectrum *= probability;
		for (Area area : areas) if (area.rays > 0) spectrum += area.probability*area.spectrum/area.rays;

		return spectrum;
	}

};


#endif
