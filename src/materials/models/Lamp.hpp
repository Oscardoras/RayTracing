#ifndef MATERIALS_LAMP_H_
#define MATERIALS_LAMP_H_

#include "../Material.hpp"
#include "../../World.hpp"


class Lamp: public virtual Material {

public:

	std::shared_ptr<Texture> emitted;

	Lamp(std::shared_ptr<Texture> emitted) : Material(), emitted(emitted) {}

	virtual Light color(RelativePosition const& relative, FaceDirection const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		return Light(emitted->getSpectrum(relative));
	}

};


#endif
