#ifndef MATERIALS_METAL_H_
#define MATERIALS_METAL_H_

#include <algorithm>

#include "Material.hpp"
#include "../World.hpp"


class Metal: public Material {

public:

	float fuzziness;

	Metal(Color albedo, float fuzziness) : Material(), fuzziness(fuzziness) {}

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& in, World const& world, int const& samples, int const& maxDepth) const override {
		Light light = Light();
		long id = long(this);

		int n = std::max(1, std::min(int(fuzziness * samples), samples));
		int remaining = std::max(1, int(samples / n));
		bool reflectIds = fuzziness < 0.1;
		for (int i = 0; i < n; i++) {
			Vector d = (in.v - 2*(in.v*faceDirection)*faceDirection + fuzziness*Vector::random()).unit();
			light += world.trace(Ray(in.p, d), true, remaining, maxDepth).addId(long(this));
		}

		return light;
	}

};


#endif
