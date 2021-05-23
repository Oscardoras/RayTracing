#ifndef MATERIALS_MATTER_WOOD_H_
#define MATERIALS_MATTER_WOOD_H_

#include "../models/Glossy.hpp"
#include "../../images/Perlin.hpp"
#include "../../textures/Plain.hpp"


class WoodTexture: public Texture {

public:

	Perlin perlin;
	Vector origin;
	Vector direction;
	Spectrum spectrum;
	
	WoodTexture(Vector const& origin, Vector const& direction, Spectrum const& spectrum, int const& seed = 1):
		origin(origin), direction(direction.unit()), spectrum(spectrum), perlin(seed) {}

	virtual Spectrum getSpectrum(RelativePosition const& relative) const override {
		return spectrum * getFloat(relative);
	}

	virtual Vector getVector(RelativePosition const& relative) const override {
		return Vector(1,1,1) * getFloat(relative);
	}

	virtual float getFloat(RelativePosition const& relative) const override {
		Vector pos = relative.position - origin;
		float turb_b = perlin.turb(pos/10);
		float turb_l = perlin.turb(pos);
		float radius = 0.1+0.2*turb_b;
		float sin = std::sin((2*Pi*(pos - (pos*direction)*direction).length() / radius) + 5*turb_b + 10*turb_l);
		return 0.5*( 0.5*(1 + sin*sin) + 0.5*(1 + turb_b));
	}

};


class Wood: public Glossy {

public:

	Wood(Vector const& origin, Vector const& direction, Spectrum const& spectrum, float const& roughness, int const& seed = 1, std::vector<std::shared_ptr<Priority>> priorities = std::vector<std::shared_ptr<Priority>>(), std::shared_ptr<ImageTexture> const& lightMap = nullptr):
		Glossy(std::make_shared<WoodTexture>(origin, direction, spectrum, seed), std::make_shared<Plain>(1.3), std::make_shared<Plain>(roughness), priorities, lightMap) {}

};


#endif
