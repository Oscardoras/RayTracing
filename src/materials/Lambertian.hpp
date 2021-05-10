#ifndef MATERIALS_LAMBERTIAN_H_
#define MATERIALS_LAMBERTIAN_H_

#include "Diffuse.hpp"
#include "../textures/Texture.hpp"


class Lambertian: public Diffuse {

public:

	std::shared_ptr<Texture> albedo;

	Lambertian(std::shared_ptr<Texture> albedo, std::vector<std::shared_ptr<Priority>> priorities = std::vector<std::shared_ptr<Priority>>()):
		Diffuse(priorities), albedo(albedo) {}
	
	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		Spectrum scattered = scatter(relative, faceDirection, in, world, samples, maxDepth);
		return Light(long(this), albedo->get(relative.u, relative.v).toSpectrum(), scattered);
	}

	virtual Spectrum sample(Vector const& faceDirection, Ray const& in, Ray const& out, World const& world, int const& maxDepth) const override {
		return world.trace(out, true, 1, maxDepth).compute();
	}

};


#endif
