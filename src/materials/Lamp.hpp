#ifndef MATERIALS_LAMP_H_
#define MATERIALS_LAMP_H_

#include "Material.hpp"
#include "../World.hpp"


class Lamp: public Material {

public:

	Spectrum emitted;

	Lamp(Spectrum emitted) : Material(), emitted(emitted) {}

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& ray, World const& world, int const& samples, int const& maxDepth) const override {
		return Light(emitted);
	}

};


#endif
