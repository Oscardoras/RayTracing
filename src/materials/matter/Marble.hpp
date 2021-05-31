#ifndef MATERIALS_MATTER_MARBLE_H_
#define MATERIALS_MATTER_MARBLE_H_

#include "../BDF.hpp"
#include "../../images/Perlin.hpp"
#include "../../textures/Plain.hpp"


class MarbleTexture: public Texture {

public:

	Perlin perlin;
	Vector direction;
	
	MarbleTexture(Vector const& direction, int const& seed = 1):
		direction(direction.unit()), perlin(seed) {}

	virtual Spectrum getSpectrum(RelativePosition const& relative) const override {
		return Spectrum(1,1,1) * getFloat(relative);
	}

	virtual Vector getVector(RelativePosition const& relative) const override {
		return Vector(1,1,1) * getFloat(relative);
	}

	virtual float getFloat(RelativePosition const& relative) const override {
		Vector pos = relative.position;
		return 0.5 * (1 + sin(4*(pos*direction) + 10*perlin.turb(pos)));
	}

};


class Marble: public BDF {

public:

	Marble(Vector const& direction, float const& roughness, int const& seed = 1, std::vector<std::shared_ptr<Priority>> priorities = std::vector<std::shared_ptr<Priority>>(), std::shared_ptr<ImageTexture> const& lightMap = nullptr):
		BDF(
			std::make_shared<MarbleTexture>(direction, seed),
			std::make_shared<Plain>(getReflectance(1, 1.5)),
			std::make_shared<Plain>(0.),
			std::make_shared<Plain>(roughness),
			nullptr,
			priorities,
			lightMap
		) {}

};


#endif
