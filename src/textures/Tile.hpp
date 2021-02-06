#ifndef TEXTURES_TILE_H_
#define TEXTURES_TILE_H_

#include <memory>
#include <algorithm>

#include "Texture.hpp"


class Tile: public Texture {

public:

	Color color1;
	Color color2;
	float size;

	Tile(Color const& color1, Color const& color2, float const& size): Texture(), color1(color1), color2(color2), size(size) {}

	virtual Color get(float const& u, float const& v) const override {
		int a = int(u / size);
		int b = int(v / size);
		return a%2 == b%2 ? color1 : color2;
	}

};


#endif
