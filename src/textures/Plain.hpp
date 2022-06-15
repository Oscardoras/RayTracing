#ifndef __TEXTURES_PLAIN_H__
#define __TEXTURES_PLAIN_H__

#include "Texture.hpp"


struct Plain: public Texture {

	Spectrum spectrum;

	Plain(Spectrum const& spectrum);
	Plain(float const r, float const g, float const b);
	Plain(float const f);

	virtual Spectrum getSpectrum(RelativePosition const& relative) const override;
	virtual Vector getVector(RelativePosition const& relative) const override;
	virtual float getFloat(RelativePosition const& relative) const override;

};


#endif
