#ifndef MATERIALS_LAMBERTIAN_H_
#define MATERIALS_LAMBERTIAN_H_

#include "Material.hpp"
#include "../textures/Texture.hpp"
#include "../Priority.hpp"


class Lambertian: public Material {

public:

	std::shared_ptr<Texture> texture;
	std::vector<std::shared_ptr<Priority>> priorities;

	Lambertian(std::shared_ptr<Texture> texture, std::vector<std::shared_ptr<Priority>> priorities = std::vector<std::shared_ptr<Priority>>()):
		Material(), texture(texture), priorities(priorities) {}
	
	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& ray, World const& world, int const& samples, int const& maxDepth) const override {
		Spectrum scattered = Lambertian::getScattered(priorities, faceDirection, ray, world, samples, maxDepth);
		return Light(long(this), texture->get(relative.u, relative.v).toSpectrum(), scattered);
	}

	static Spectrum getScattered(std::vector<std::shared_ptr<Priority>> priorities, Vector const& faceDirection, Ray const& ray, World const& world, int const& samples, int const& maxDepth) {
	float remaind = 1;
		float probability = 1;
		std::vector<Area> areas;

		for (std::shared_ptr<Priority> priority : priorities) {
			Area area(priority, ray.origin);
			remaind -= priority->portion;
			probability -= area.probability;
			int s = int(priority->portion*float(samples));
			for (int i = 0; i < s; i++) {
				Vector vec = (priority->center + Vector::random()*priority->radius) - ray.origin;
				if (vec*faceDirection < 0) vec *= -1;
				area.spectrum += world.trace(Ray(ray.origin, vec), true, 1, maxDepth).compute();
				area.rays++;
			}
			areas.push_back(area);
		}


		Spectrum spectrum;
		int rays = 0;
		int s = int(remaind*float(samples));
		for (int i = 0; i < s; i++) {
			bool hit = false;
			Vector vec = Vector::randomUnit();
			if (vec*faceDirection < 0) vec *= -1;
			Ray r = Ray(ray.origin, vec);
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
		spectrum *= probability;
		for (Area area : areas) if (area.rays > 0) spectrum += area.probability*area.spectrum/area.rays;

		return spectrum;
	}

};


#endif
