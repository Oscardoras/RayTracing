#ifndef MATERIALS_MARBLE_H_
#define MATERIALS_MARBLE_H_

#include "Material.hpp"
#include "../World.hpp"
#include "../images/Perlin.hpp"


class Marble: public Material {

public:

	Perlin perlin;
	Point origin;
	Vector direction;

	Marble(Point const& origin, Vector const& direction, int const& seed = 1) : Material(), origin(origin), direction(direction), perlin(seed) {}

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& ray, World const& world, int const& samples, int const& maxDepth) const override {
		Spectrum sp;
		for (int i = 0; i < samples; i++) {
			Vector vec = Vector::randomUnit();
			if (vec*faceDirection < 0) vec *= -1;
			sp += world.trace(Ray(ray.origin, vec), true, 1, maxDepth).compute();
		}

		Vector pos = (origin + relative.position).toVector();
		Spectrum albedo = Spectrum(1,1,1) * 0.5 * (1 + sin(4*(pos*direction) + 10*perlin.turb(relative.position)));
		return Light(Spectrum(), Light(), Light(), albedo, sp/samples, long(this));
	}

};


#endif
