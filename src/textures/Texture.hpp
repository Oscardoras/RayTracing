#ifndef __TEXTURES_TEXTURE_H__
#define __TEXTURES_TEXTURE_H__

#include "../objects/primitives/Primitive.hpp"


struct Texture {

	virtual Spectrum getSpectrum(RelativePosition const& relative) const = 0;
	virtual Vector getVector(RelativePosition const& relative) const = 0;
	virtual float getFloat(RelativePosition const& relative) const = 0;

};

struct NullTexture: public Texture {

	virtual Spectrum getSpectrum(RelativePosition const& relative) const override;
	virtual Vector getVector(RelativePosition const& relative) const override;
	virtual float getFloat(RelativePosition const& relative) const override;

};


#endif
