#ifndef TEXTURES_IMAGETEXTURE_H_
#define TEXTURES_IMAGETEXTURE_H_

#include <memory>
#include <algorithm>

#include "Texture.hpp"
#include "../Image.hpp"


class ImageTexture: public Texture {

public:

    Image image;
	float width;
	float height;

	ImageTexture(Image image, float const& width, float const& height): Texture(), image(image), width(width), height(height) {}

	virtual Color get(float const& u, float const& v) const override {
		int i = image.width - (int((u/width) * image.width) % image.width) -1;
		int j = image.height - (int((v/height) * image.height) % image.height) -1;
        return image.pixels[j][i];
	}

};

#endif
