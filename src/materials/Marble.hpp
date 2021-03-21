#ifndef MATERIALS_MARBLE_H_
#define MATERIALS_MARBLE_H_

#include "Lambertian.hpp"
#include "../images/Perlin.hpp"


class Marble: public Material {

public:

	Perlin perlin;
	Point p;
	Vector direction;
	std::vector<std::shared_ptr<Priority>> priorities;

	Marble(Point const& origin, Vector const& direction, int const& seed = 1, std::vector<std::shared_ptr<Priority>> priorities = std::vector<std::shared_ptr<Priority>>()):
		Material(), origin(origin), direction(direction), perlin(seed), priorities(priorities) {}

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		Spectrum scattered = Lambertian::getScattered(priorities, faceDirection, in, world, samples, maxDepth);

		Vector pos = (origin + relative.position) - Point();
		Spectrum albedo = Spectrum(1,1,1) * 0.5 * (1 + sin(4*(pos*direction) + 10*perlin.turb(relative.position)));
		
		return Light(long(this), albedo, scattered);
	}

};


#endif
