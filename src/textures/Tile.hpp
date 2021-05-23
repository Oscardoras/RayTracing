#ifndef TEXTURES_TILE_H_
#define TEXTURES_TILE_H_

#include <memory>
#include <algorithm>

#include "Texture.hpp"


class Tile: public Texture {

public:

	Spectrum spectrum1;
	Spectrum spectrum2;
	float size;

	Tile(Spectrum const& spectrum1, Spectrum const& spectrum2, float const& size): Texture(), spectrum1(spectrum1), spectrum2(spectrum2), size(size) {}

	virtual Spectrum getSpectrum(RelativePosition const& relative) const override {
		if (std::isnan(relative.u) || std::isnan(relative.v)) return Spectrum();
		int a = int(relative.u / size);
		int b = int(relative.v / size);
		return a%2 == b%2 ? spectrum1 : spectrum2;
	}

	virtual Vector getVector(RelativePosition const& relative) const override {
		return getSpectrum(relative).toColor().toVector();
	}

	virtual float getFloat(RelativePosition const& relative) const override {
		return getSpectrum(relative).getIntensity();
	}

};


#endif
