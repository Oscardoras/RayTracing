#ifndef __TEXTURES_TILE_H__
#define __TEXTURES_TILE_H__

#include "Texture.hpp"


struct Tile: public Texture {

	Spectrum spectrum1;
	Spectrum spectrum2;
	float size;

	Tile(Spectrum const& spectrum1, Spectrum const& spectrum2, float const size);

	virtual Spectrum getSpectrum(RelativePosition const& relative) const override;
	virtual Vector getVector(RelativePosition const& relative) const override;
	virtual float getFloat(RelativePosition const& relative) const override;

};


#endif
