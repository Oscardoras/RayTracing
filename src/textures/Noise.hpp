#ifndef TEXTURES_NOISE_H_
#define TEXTURES_NOISE_H_

#include <memory>
#include <algorithm>

#include "../algebra/Color.hpp"
#include "../images/Perlin.hpp"
#include "Texture.hpp"


class Noise: public Texture {

public:

	Perlin noise;
	double scale;

	Noise(): Texture() {}
	Noise(double const& scale): Texture(), scale(scale) {}

	virtual Color get(float const& u, float const& v) const override {
		//return Color(1,1,1) * noise.turb(scale * Point(u*scale, v*scale, 0));
		Point p = Point(u*scale, v*scale, 0);
		return Color(1,1,1) * 0.5 * (1 + sin(scale*p.z + 10*noise.turb(p)));
	}

};


#endif
