#ifndef TEXTURES_IMAGETEXTURE_H_
#define TEXTURES_IMAGETEXTURE_H_

#include <memory>
#include <algorithm>

#include "Texture.hpp"
#include "../images/Image.hpp"


class ImageTexture: public Texture {

public:

    std::shared_ptr<Image> image;
	float width;
	float height;

	ImageTexture(std::shared_ptr<Image> image, float width, float height):
		Texture(), image(image), width(width), height(height) {}

	Color get(float const& u, float const& v) const {
		if (std::isnan(u) || std::isnan(v)) return Color();
		int i = int((u/width) * image->width) % image->width;
		int j = int((v/height) * image->height) % image->height;
		if (i < 0) i += image->width;
		if (j < 0) j += image->height;
        return image->pixels[image->height-1-j][image->width-1-i];
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
