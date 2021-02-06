#ifndef TEXTURES_PLAIN_H_
#define TEXTURES_PLAIN_H_

#include <memory>
#include <algorithm>

#include "Texture.hpp"


class Plain: public Texture {

public:

	Color color;

	Plain(Color color): Texture(), color(color) {}
	Plain(float r, float g, float b): Texture(), color(r, g, b) {}

	virtual Color get(float const& u, float const& v) const override {
		return color;
	}

};


#endif
