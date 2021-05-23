#ifndef TEXTURES_PLAIN_H_
#define TEXTURES_PLAIN_H_

#include <memory>
#include <algorithm>

#include "Texture.hpp"


class Plain: public Texture {

public:

	Spectrum spectrum;

	Plain(Spectrum spectrum): Texture(), spectrum(spectrum) {}
	Plain(float r, float g, float b): Texture(), spectrum(r, g, b) {}
	Plain(float f): Texture(), spectrum(f,f,f) {}

	virtual Spectrum getSpectrum(RelativePosition const& relative) const override {
		return spectrum;
	}

	virtual Vector getVector(RelativePosition const& relative) const override {
		return spectrum.toColor().toVector();
	}

	virtual float getFloat(RelativePosition const& relative) const override {
		return spectrum.getIntensity();
	}

};


#endif
