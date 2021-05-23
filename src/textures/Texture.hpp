#ifndef TEXTURES_TEXTURE_H_
#define TEXTURES_TEXTURE_H_

#include "../algebra/Vector.hpp"
#include "../algebra/Spectrum.hpp"
#include "../objects/Object.hpp"


class Texture {

public:

	virtual Spectrum getSpectrum(RelativePosition const& relative) const = 0;

	virtual Vector getVector(RelativePosition const& relative) const = 0;

	virtual float getFloat(RelativePosition const& relative) const = 0;

};

class NullTexture: public Texture {

public:

	virtual Spectrum getSpectrum(RelativePosition const& relative) const override {
		return Spectrum();
	}

	virtual Vector getVector(RelativePosition const& relative) const override {
		return Vector();
	}

	virtual float getFloat(RelativePosition const& relative) const override {
		return 0;
	}

};


#endif
