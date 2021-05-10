#ifndef MATERIALS_MARBLE_H_
#define MATERIALS_MARBLE_H_

#include "Lambertian.hpp"
#include "../images/Perlin.hpp"


class Marble: public Diffuse {

public:

	Perlin perlin;
	Vector origin;
	Vector direction;

	Marble(Vector const& origin, Vector const& direction, int const& seed = 1, std::vector<std::shared_ptr<Priority>> priorities = std::vector<std::shared_ptr<Priority>>()):
		Diffuse(priorities), origin(origin), direction(direction), perlin(seed) {}

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		Spectrum scattered = scatter(relative, faceDirection, in, world, samples, maxDepth);

		Vector pos = origin + relative.position;
		Spectrum albedo = Spectrum(1,1,1) * 0.5 * (1 + sin(4*(pos*direction) + 10*perlin.turb(pos)));
		
		return Light(long(this), albedo, scattered);
	}

	virtual Spectrum sample(Vector const& faceDirection, Ray const& in, Ray const& out, World const& world, int const& maxDepth) const override {
		return world.trace(out, true, 1, maxDepth).compute();
	}

};


#endif
