#include "Texture.hpp"


Spectrum NullTexture::getSpectrum(RelativePosition const& relative) const {
	return Spectrum();
}

Vector NullTexture::getVector(RelativePosition const& relative) const {
	return Vector();
}

float NullTexture::getFloat(RelativePosition const& relative) const {
	return 0;
}