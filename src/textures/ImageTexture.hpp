#ifndef TEXTURES_IMAGETEXTURE_H_
#define TEXTURES_IMAGETEXTURE_H_

#include <memory>
#include <algorithm>

#include "Texture.hpp"
#include "../images/Image.hpp"


class ImageTexture: public Texture {

public:

    Image image;
	float width;
	float height;

	ImageTexture(Image image, float const& width, float const& height): Texture(), image(image), width(width), height(height) {}

	Color get(float const& u, float const& v) const {
		if (std::isnan(u) || std::isnan(v)) return Color();
		int i = image.width - (int((u/width) * image.width) % image.width) -1;
		int j = image.height - (int((v/height) * image.height) % image.height) -1;
        return image.pixels[j][i];
	}

	virtual Spectrum getSpectrum(RelativePosition const& relative) const override {
		return get(relative.u, relative.v).toSpectrum();
	}

	virtual Vector getVector(RelativePosition const& relative) const override {
		return get(relative.u, relative.v).toVector();
	}

	virtual float getFloat(RelativePosition const& relative) const override {
		return getSpectrum(relative).getIntensity();
	}

};

#endif
