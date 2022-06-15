#include "ImageTexture.hpp"

#include "../MathUtils.hpp"


ImageTexture::ImageTexture(Image* const image, float const width, float const height):
	image(image), width(width), height(height) {}

Color ImageTexture::get(float const u, float const v) const {
	if (std::isnan(u) || std::isnan(v)) return Color();
	int i = int((u/width) * image->width) % image->width;
	int j = int((v/height) * image->height) % image->height;
	if (i < 0) i += image->width;
	if (j < 0) j += image->height;
	return image->pixels[image->height-1-j][image->width-1-i];
}

Spectrum ImageTexture::getSpectrum(RelativePosition const& relative) const {
	return get(relative.u, relative.v).toSpectrum();
}

Vector ImageTexture::getVector(RelativePosition const& relative) const {
	return get(relative.u, relative.v).toVector();
}

float ImageTexture::getFloat(RelativePosition const& relative) const {
	return get(relative.u, relative.v).toBlackAndWhite();
}