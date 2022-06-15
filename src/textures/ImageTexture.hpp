#ifndef __TEXTURES_IMAGETEXTURE_H__
#define __TEXTURES_IMAGETEXTURE_H__

#include "Texture.hpp"
#include "../images/Image.hpp"


struct ImageTexture: public Texture {

    Image* image;
	float width;
	float height;

	ImageTexture(Image* const image, float const width, float const height);

	Color get(float const u, float const v) const;

	virtual Spectrum getSpectrum(RelativePosition const& relative) const override;
	virtual Vector getVector(RelativePosition const& relative) const override;
	virtual float getFloat(RelativePosition const& relative) const override;

};

#endif
