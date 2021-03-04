#ifndef MATERIALS_METAL_H_
#define MATERIALS_METAL_H_

#include <algorithm>

#include "Material.hpp"
#include "../World.hpp"


class Metal: public Material {

public:

	Color albedo;
	float fuzziness;

	Metal(Color albedo, float fuzziness) : Material(), albedo(albedo), fuzziness(fuzziness) {}

	virtual Light color(RelativePosition const& relative, Vector const& faceDirection, Ray const& ray, World const& world, int const& remainingRays, int const& maxDepth) const override {
		Light light = Light();
		long id = long(this);

		int n = std::max(1, std::min(int(fuzziness * remainingRays), remainingRays));
		int remaining = std::max(1, int(remainingRays / n));
		bool reflectIds = fuzziness < 0.1;
		for (int i = 0; i < n; i++) {
			Vector d = (ray.direction - 2*(ray.direction*faceDirection)*faceDirection + fuzziness*Vector::random()).unit();
			light += world.trace(Ray(ray.origin, d), true, remaining, maxDepth);
		}

		return light;
	}

};


#endif
