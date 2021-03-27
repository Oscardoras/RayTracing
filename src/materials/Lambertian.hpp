#ifndef MATERIALS_LAMBERTIAN_H_
#define MATERIALS_LAMBERTIAN_H_

#include "Priorisable.hpp"
#include "../textures/Texture.hpp"


class Lambertian: public Priorisable {

public:

	std::shared_ptr<Texture> albedo;

	Lambertian(std::shared_ptr<Texture> albedo, std::vector<std::shared_ptr<Priority>> priorities = std::vector<std::shared_ptr<Priority>>()):
		Priorisable(priorities), albedo(albedo) {}
	
	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		Spectrum scattered = scatter(faceDirection, in, world, samples, maxDepth);
		return Light(long(this), albedo->get(relative.u, relative.v).toSpectrum(), scattered);
	}

	virtual Spectrum sample(Vector const& faceDirection, Ray const& in, Ray &out, World const& world, int const& maxDepth) const override {
		if (out.v*faceDirection < 0) out.v *= -1;
		return world.trace(out, true, 1, maxDepth).compute();
	}

};


#endif
