#ifndef MATERIALS_MATTER_MARBLE_H_
#define MATERIALS_MATTER_MARBLE_H_

#include "../models/Glossy.hpp"
#include "../../images/Perlin.hpp"
#include "../../textures/Plain.hpp"


class MarbleTexture: public Texture {

public:

	Perlin perlin;
	Vector origin;
	Vector direction;
	
	MarbleTexture(Vector const& origin, Vector const& direction, int const& seed = 1):
		origin(origin), direction(direction.unit()), perlin(seed) {}

	virtual Spectrum getSpectrum(RelativePosition const& relative) const override {
		return Spectrum(1,1,1) * getFloat(relative);
	}

	virtual Vector getVector(RelativePosition const& relative) const override {
		return Vector(1,1,1) * getFloat(relative);
	}

	virtual float getFloat(RelativePosition const& relative) const override {
		Vector pos = relative.position - origin;
		return 0.5 * (1 + sin(4*(pos*direction) + 10*perlin.turb(pos)));
	}

};


class Marble: public Glossy {

public:

	Marble(Vector const& origin, Vector const& direction, float const& roughness, int const& seed = 1, std::vector<std::shared_ptr<Priority>> priorities = std::vector<std::shared_ptr<Priority>>(), std::shared_ptr<ImageTexture> const& lightMap = nullptr):
		Glossy(std::make_shared<MarbleTexture>(origin, direction, seed), std::make_shared<Plain>(1.5), std::make_shared<Plain>(roughness), priorities, lightMap) {}

};


#endif
